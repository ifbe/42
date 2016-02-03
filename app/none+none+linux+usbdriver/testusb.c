#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/usb.h>




//plugged in,plugged out
static int testusb_probe(struct usb_interface* interface,const struct usb_device_id* id)
{
	//descriptors
	struct usb_device* device;
	unsigned char* p;
	int i;
	struct usb_device_descriptor* devicedesc;
	struct usb_config_descriptor* configdesc;
	struct usb_interface_descriptor* ifdesc;
	struct usb_endpoint_descriptor* epdesc;
	printk("(testusb_probe)vendor=%x,product=%x\n",id->idVendor,id->idProduct);

	//device descriptor
	device=interface_to_usbdev(interface);
	devicedesc = &(device->descriptor);
	p=(unsigned char*)devicedesc;
	if(p==NULL)
	{
		printk("(NULL)devicedesc\n");
		return 0;
	}

	printk("devicedesc=");
	for(i=0;i<p[0];i++)
	{
		printk("%.2x ",p[i]);
	}
	printk("\n");

	//configuration descriptor
	configdesc = &(device->config->desc);
	p=(unsigned char*)configdesc;
	if(p==NULL)
	{
		printk("(NULL)configdesc\n");
		return 0;
	}

	printk("configdesc=");
	for(i=0;i<p[0];i++)
	{
		printk("%.2x ",p[i]);
	}
	printk("\n");

	//interface descriptor
	ifdesc = &(interface->cur_altsetting->desc);
	p=(unsigned char*)ifdesc;
	if(p==NULL)
	{
		printk("(NULL)ifdesc\n");
		return 0;
	}

	printk("ifdesc=");
	for(i=0;i<p[0];i++)
	{
		printk("%.2x ",p[i]);
	}
	printk("\n");

	//endpoint0 descriptor
	epdesc = &(device->ep0.desc);
	p=(unsigned char*)epdesc;
	if(p==NULL)
	{
		printk("(NULL)epdesc\n");
		return 0;
	}

	printk("ep0desc=");
	for(i=0;i<p[0];i++)
	{
		printk("%.2x ",p[i]);
	}
	printk("\n");

	return 0;
}
static void testusb_disconnect(struct usb_interface* interface)
{
	printk("(testusb_disconnect)\n");
}




//which device
static struct usb_device_id testusb_table[] =
{
	{ USB_DEVICE(0x0925,0x3881) },
	{}
};
MODULE_DEVICE_TABLE(usb,testusb_table);




//operator address
static struct usb_driver testusb_driver =
{
	.name = "testusb",
	.id_table = testusb_table,
	.probe = testusb_probe,
	.disconnect = testusb_disconnect,
};




//the start,the end
static int __init testusb_init(void)
{
	int ret=-1;
	ret=usb_register(&testusb_driver);
	printk("(testusb_init)%x\n",ret);
	return ret;
}
static void __exit testusb_exit(void)
{
	usb_deregister(&testusb_driver);
	printk("(testusb_exit)\n");
}




module_init(testusb_init);
module_exit(testusb_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("42");
MODULE_DESCRIPTION("42");
