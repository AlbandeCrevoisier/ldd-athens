/* ADXL345 3-axis accelerometer I2C driver
 *
 * Copyright (c) 2016, Alban de Crevoisier d'Hurbache
 *
 * Licensed under the GPLv2
 */

#include <linux/errno.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/miscdevice.h>
#include <linux/i2c.h>
#include <linux/fs.h>

static int adxl345_probe(struct i2c_client *c, const struct i2c_device_id *id);
static int adxl345_remove(struct i2c_client *c);
static ssize_t adxl345_read(struct file *f, char __user *buf, size_t count,
	loff_t *f_pos);
static long adxl345_unlocked_ioctl(struct file *f, unsigned int cmd,
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
};

static int adxl345_probe(struct i2c_client *c, const struct i2c_device_id *id)
{
	int ret = 0;
	struct adxl345_device *adxl345_dev;

	char devid = 0x00;
	char buf = 0;
	char bw_rate[2] = {0x2C, 0x0A};		/* disable low power, 100 Hz */
	char int_enable[2] = {0x2E, 0x00};	/* disable all interrupts */
	char data_format[2] = {0x31, 0x03 | 0x08}; /* 16g, full resolution */
	char fifo_ctl[2] = {0x38, 0x00};	/* bypass */
	char power_ctl[2] = {0x2D, 0x8};	/* measure mode */

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
	ret = misc_register(&adxl345_dev->misc_dev);
	if (!ret) {
		dev_err(&c->dev, "misc_register failed\n");
		return -EINVAL;
	}
	i2c_set_clientdata(c, adxl345_dev);

	return 0;
}

static int adxl345_remove(struct i2c_client *c)
{
	struct adxl345_device *adxl345_dev;
	char power_ctl_addr = 0;
	char power_ctl[2] = {power_ctl_addr, 0};

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

static ssize_t adxl345_read(struct file *f, char __user *buf, size_t count,
	loff_t *f_pos)
{
	return 0;
}

static long adxl345_unlocked_ioctl(struct file *f, unsigned int cmd,
	unsigned long arg)
{
	return 0;
}

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("ADXL345 I2C Driver");
MODULE_AUTHOR("ach");
