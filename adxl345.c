/* ADXL345 3-axis accelerometer I2C driver
 *
 * Copyright (c) 2016, Alban de Crevoisier d'Hurbache
 *
 * Licensed under the 3-clause BSD License
 */

#include <linux/errno.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <linux/i2c.h>
#include <linux/fs.h>
#include <asm/uaccess.h>

static int adxl345_probe(struct i2c_client *c, const struct i2c_device_id *id);
static int adxl345_remove(struct i2c_client *c);
static ssize_t adxl345_read(struct file *fp, char __user *buf, size_t count,
	loff_t *f_pos);
static long adxl345_unlocked_ioctl(struct file *fp, unsigned int cmd,
	unsigned long arg);

static struct i2c_device_id adxl345_idtable[] = {
	{"adxl345", (kernel_ulong_t) 0},
	{}
};

MODULE_DEVICE_TABLE(i2c, adxl345_idtable);

#ifdef CONFIG_OF
static const struct of_device_id adxl345_of_match[] = {
	{.compatible = "ad,adxl345", },
	{}
};

MODULE_DEVICE_TABLE(of, adxl345_of_match);
#endif /* CONFIG_OF */
static struct i2c_driver adxl345_driver = {
	.driver = {
		.name = "adxl345",
		.of_match_table = of_match_ptr(adxl345_of_match)
	},
	.id_table = adxl345_idtable,
	.probe = adxl345_probe,
	.remove = adxl345_remove
};

module_i2c_driver(adxl345_driver);

static const struct file_operations adxl345_fops = {
	.owner = THIS_MODULE,
	.read = adxl345_read,
	.unlocked_ioctl = adxl345_unlocked_ioctl
};

struct adxl345_device {
	struct miscdevice misc_dev;
	uint8_t data_addr;
};

static int adxl345_probe(struct i2c_client *c, const struct i2c_device_id *id)
{
	int ret = 0;
	struct adxl345_device *adxl345_dev;

	uint8_t devid = 0x00;
	uint8_t buf = 0;
	uint8_t bw_rate[2] = {0x2C, 0x0A};	/* disable low power, 100 Hz */
	uint8_t int_enable[2] = {0x2E, 0x00};	/* disable all interrupts */
	uint8_t data_format[2] = {0x31, 0x03 | 0x08}; /* 16g, full resolution */
	uint8_t fifo_ctl[2] = {0x38, 0x00};	/* bypass */
	uint8_t power_ctl[2] = {0x2D, 0x8};	/* measure mode */

	dev_info(&c->dev, "ADXL345 probe\n");

	/* read DEVID */
	i2c_master_send(c, &devid, 1);
	i2c_master_recv(c, &buf, 1);
	dev_info(&c->dev, "DEVID: %02x\n", buf);

	/* Configuration for the accelerometer */
	i2c_master_send(c, bw_rate, 2);
	i2c_master_send(c, int_enable, 2);
	i2c_master_send(c, data_format, 2);
	i2c_master_send(c, fifo_ctl, 2);
	i2c_master_send(c, power_ctl, 2);

	/* register to misc framework */
	adxl345_dev = devm_kzalloc(
		&c->dev, sizeof(struct adxl345_device), GFP_KERNEL);
	if (!adxl345_dev) {
		dev_err(&c->dev, "adxl345_dev alloc failed\n");
		return -ENOMEM;
	}
	adxl345_dev->misc_dev.minor = MISC_DYNAMIC_MINOR;
	adxl345_dev->misc_dev.name = "adxl345";
	adxl345_dev->misc_dev.fops = &adxl345_fops;
	adxl345_dev->misc_dev.parent = &c->dev;
	adxl345_dev->data_addr = 0x32;			/* DATAX0 by default */
	ret = misc_register(&adxl345_dev->misc_dev);
	if (ret) {
		dev_err(&c->dev, "misc_register failed\n");
		return ret;
	}
	i2c_set_clientdata(c, adxl345_dev);

	return 0;
}

static int adxl345_remove(struct i2c_client *c)
{
	struct adxl345_device *adxl345_dev;
	uint8_t power_ctl_addr = 0;
	uint8_t power_ctl[2] = {power_ctl_addr, 0};

	dev_info(&c->dev, "ADXL345 remove\n");

	/* standby mode */
	i2c_master_send(c, &power_ctl_addr, 1);
	i2c_master_recv(c, &power_ctl[1], 1);
	power_ctl[1] &= (~0x08);
	i2c_master_send(c, power_ctl, 1);

	/* deregister from misc framework */
	adxl345_dev = i2c_get_clientdata(c);
	misc_deregister(&adxl345_dev->misc_dev);

	return 0;
}

static ssize_t adxl345_read(struct file *fp, char __user *buf, size_t count,
	loff_t *f_pos)
{
	int ret = 0;
	char __user *tmp = buf;

	struct i2c_client *c;
	struct device *dev;
	struct adxl345_device *adxl345_dev;
	struct miscdevice *misc_dev;

	int8_t data_buf[2];

	misc_dev = fp->private_data;
	adxl345_dev = container_of(
		misc_dev, struct adxl345_device, misc_dev);
	dev = misc_dev->parent;
	c = to_i2c_client(dev);

	i2c_master_send(c, &adxl345_dev->data_addr, 1);
	i2c_master_recv(c, data_buf, 2);

	ret = put_user(data_buf[0], tmp);
	if (ret)
		return ret;

	if (count == 1)
		return 1;

	ret = put_user(data_buf[1], tmp + 1);
	if (ret)
		return ret;

	return 2;
}

static long adxl345_unlocked_ioctl(struct file *fp, unsigned int cmd,
	unsigned long arg)
{
	struct adxl345_device *adxl345_dev;
	struct miscdevice *misc_dev;

	misc_dev = fp->private_data;
	adxl345_dev = container_of(
		misc_dev, struct adxl345_device, misc_dev);

	if (cmd != 4242)
		return -EINVAL;
	/* select the axis */
	switch(arg) {
	case 'x':
		adxl345_dev->data_addr = 0x32;
		break;
	case 'y':
		adxl345_dev->data_addr = 0x34;
		break;
	case 'z':
		adxl345_dev->data_addr = 0x32;
		break;
	}
	return 0;
}

MODULE_LICENSE("Dual BSD/GPL");
MODULE_DESCRIPTION("ADXL345 I2C Driver");
MODULE_AUTHOR("ach");
