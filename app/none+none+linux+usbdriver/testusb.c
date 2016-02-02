#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/usb.h>




//plugged in,plugged out
/*
static int testusb_probe(struct usb_interface* interface,const struct usb_device_id* id)
{
	//descriptors
	struct usb_host_interface* ifdesc;
	struct usb_endpoint_descriptor* epdesc;
	printk("(testusb_probe)vendor=%x,product=%x\n",id->idVendor,id->idProduct);

	//interface descriptor
	ifdesc = interface->cur_altsetting;
	printk("interface=%x\n",ifdesc->desc.bInterfaceNumber);
	printk("bNumEndpoints=%x\n",ifdesc->desc.bNumEndpoints);
	printk("bInterfaceClass=%x\n",ifdesc->desc.bInterfaceClass);

	//endpoint descriptor
	for(i=0;i<ifdesc->desc.bNumEndpoints;i++)
	{
		epdesc = &ifdesc->endpoint[0].desc;
		printk("bEndpointAddress=%x\n",epdesc->bEndpointAddress);
		printk("bmAttributes=%x\n",epdesc->bmAttributes);
		printk("wMaxPacketSize=%x\n",epdesc->wMaxPacketSize);
	}

	return 0;
}
*/
static int testusb_probe(struct usb_interface* interface,const struct usb_device_id* id)
{
	struct usb_device* device;
	char** descriptor;
	unsigned char* str;
	int i,j;
	printk("(testusb_probe)vendor=%x,product=%x\n",id->idVendor,id->idProduct);

	//device
	device=interface_to_usbdev(interface);
	descriptor=device->rawdescriptors;

	//descriptor
	for(i=0;i<8;i++)
	{
		str=descriptor[i];
		if(str < (unsigned char*)0x10000)break;

		for(j=0;j<str[0];j++)
		{
			printk("%.2x ",str[j]);
		}
		printk("\n");
	}

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
