/* ADXL345 3-axis accelerometer I2C driver
 *
 * Copyright (c) 2016, Alban de Crevoisier d'Hurbache
 *
 * Licensed under the GPLv2
 */

#include <linux/init.h>
#include <linux/module.h>
#include <linux/device.h>
#include <linux/i2c.h>

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

static int adxl345_probe(struct i2c_client *c, const struct i2c_device_id *id)
{
	char devid = 0x00;
	char buf = 0;
	char data_format_addr = 0x31;
	char bw_rate[2] = {0x2C, 0x0A};		/* disable low power, 100 Hz */
	char int_enable[2] = {0x2E, 0x00};	/* disable all interrupts */
	char data_format[2] = {0x31, 0x03 | 0x08}; /* full res, 16g */
	char fifo_ctl[2] = {0x38, 0x00};	/* bypass */
	char power_ctl[2] = {0x2D, 0x8};	/* measure mode */

	dev_info(&(c->dev), "ADXL345 probe\n");

	/* read DEVID */
	i2c_master_send(c, &devid, 1);
	i2c_master_recv(c, &buf, 1);
	dev_info(&(c->dev), "DEVID: %02x\n", buf);

	/* Configuration for the accelerometer */
	i2c_master_send(c, bw_rate, 2);
	i2c_master_send(c, int_enable, 2);
	i2c_master_send(c, data_format, 2);
	i2c_master_send(c, fifo_ctl, 2);
	i2c_master_send(c, power_ctl, 2);

	/* Check data_format */
	i2c_master_send(c, &data_format_addr, 1);
	i2c_master_recv(c, &buf, 1);
	dev_info(&(c->dev), "DATA_FORMAT: %02x\n", buf);

	return 0;
}

static int adxl345_remove(struct i2c_client *c)
{
	/* TODO */
	return 0;
}

static struct i2c_driver adxl345_driver = {
	.driver = {
		.name		= "adxl345",
		.of_match_table	= of_match_ptr(adxl345_of_match)
	},
	.id_table	= adxl345_idtable,
	.probe		= adxl345_probe,
	.remove		= adxl345_remove
};

module_i2c_driver(adxl345_driver);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("ADXL345 I2C Driver");
MODULE_AUTHOR("ach");
