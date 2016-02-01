#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/usb.h>




static int testusb_probe(struct usb_interface* interface,const struct usb_device_id* id)
{
	printk("(usbdriver probe)vendor=%x,product=%x\n",id->idVendor,id->idProduct);
	return 0;
}
static void testusb_disconnect(struct usb_interface* interface)
{
	printk("(usbdriver disconnect)\n");
}




static struct usb_device_id testusb_table[] =
{
	{ USB_DEVICE(0x0925,0x3881) },
	{}
};
MODULE_DEVICE_TABLE(usb,testusb_table);




static struct usb_driver testusb_driver =
{
	.name = "i am testing",
	.id_table = testusb_table,
	.probe = testusb_probe,
	.disconnect = testusb_disconnect,
};




static int __init testusb_init(void)
{
	int ret=-1;
	ret=usb_register(&testusb_driver);
	printk("usbdriver init\n");
	return ret;
}
static void __exit testusb_exit(void)
{
	usb_deregister(&testusb_driver);
	printk("usbdriver exit\n");
}




module_init(testusb_init);
module_exit(testusb_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("42");
MODULE_DESCRIPTION("i am testing");
