/* ADXL345 3-axis accelerometer I2C driver
 *
 * Copyright (c) 2016, Alban de Crevoisier d'Hurbache
 *
 * Licensed under the GPLv2
 */

#include <linux/module.h>
#include <linux/i2c.h>

static struct i2c_device_id adxl345_idtable[] = {
	{"adxl345", 0}
};

MODULE_DEVICE_TABLE(i2c, adxl345_idtable);

#ifdef CONFIG_OF
static const struct of_device_id adxl345_of_match[] = {
	{.compatible = "vendor, adxl345", },
	{}
};


MODULE_DEVICE_TABLE(of, adxl345_of_match);
#endif /* CONFIG_OF */

static int adxl345_probe(struct i2c_client *c, const struct i2c_device_id *id)
{
	/* TODO */
}

static int adxl345_remove(struct i2c_client *c)
{
	/* TODO */
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
