/* first_params.c */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

static char *message = "Hello world!";
static int nb = 0;

module_param(message, charp, S_IRUGO);
module_param(nb, int, S_IRUGO);
MODULE_PARM_DESC(message, "The message and number to print");

static int __init first_init(void)
{
	pr_info("%s %d\n", message, nb);
	return 0;
}

static void __exit first_exit(void)
{
	pr_info("Goodbye, cruel world.\n");
}

module_init(first_init);
module_exit(first_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("My first module with parameters");
MODULE_AUTHOR("ach");

