#include "libhard.h"
#define class_reserve       0x00	//去看interface descriptor
#define class_audio         0x01	//音频类
#define class_cdccom        0x02	//CDC控制类
#define class_hid           0x03	//人机接口类（HID）
#define class_physical      0x05	//物理类
#define class_image         0x06	//图像类
#define class_printer       0x07	//打印机类
#define class_storage       0x08	//大数据存储类
#define class_hub           0x09	//集线器类
#define class_cdcdata       0x0A	//CDC数据类
#define class_smartcard     0x0B	//智能卡类
#define class_security      0x0D	//安全类
#define class_video         0x0E	//视频设备
#define class_healthcare    0x0F	//个人健康
#define class_audiovideo    0x10	//声音/视频
#define class_billboard     0x11	//广告牌
#define class_typecbridge   0x12	//typec bridge
#define class_diagnostic    0xDC	//诊断设备类
#define class_wireless      0xE0	//无线控制器类
#define class_miscellaneous 0xEF	//混杂
#define class_specific      0xFE	//特定应用类（包括红外的桥接器等）
#define class_vendor        0xFF	//厂商定义的设备




struct DeviceDescriptor{
	u8            bLength;		//0: 0x12
	u8    bDescriptorType;		//1: 0x01
	u16            bcdUSB;		//[2,3]: usb version
	u8       bDeviceClass;		//4: 3=hid, 9=hub
	u8    bDeviceSubClass;		//5
	u8    bDeviceProtocol;		//6
	u8    bMaxPacketSize0;		//7: max packet size on ep0
	u16          idVendor;		//[8,9]
	u16         idProduct;		//[a,b]
	u16         bcdDevice;		//[c,d]: device version
	u8      iManufacturer;		//0e: index of manufacturer string
	u8           iProduct;		//0f: index of product string
	u8      iSerialNumber;		//10: index of serialnumber string
	u8 bNumConfigurations;		//11: how many configuration in this device
}__attribute__((packed));

struct ConfigurationDescriptor{
	u8             bLength;		//0: 0x09
	u8     bDescriptorType;		//1: 0x02
	u16       wTotalLength;		//[2,3]: 
	u8      bNumInterfaces;		//4: how many interface in this configuration
	u8 bConfigurationValue;		//5: this configuration
	u8      iConfiguration;		//6: index of configuration string
	u8        bmAttributes;		//7: 7=bus power, 6=self power, 5=remote wakeup
	u8           bMaxPower;		//8: 50=100mA, 250=500mA
}__attribute__((packed));

struct StringDescriptor{
	u8            bLength;		//0
	u8    bDescriptorType;		//1: 0x03
	u16        wLANGID[0];		//[2,3]: at least one, 0409=en-US
}__attribute__((packed));

struct InterfaceDescriptor{
	u8            bLength;		//0: 0x09
	u8    bDescriptorType;		//1: 0x04
	u8   bInterfaceNumber;		//2: this interface
	u8  bAlternateSetting;		//3
	u8      bNumEndpoints;		//4: how many endpoint in this interface
	u8    bInterfaceClass;		//5: 3=HID
	u8 bInterfaceSubClass;		//6: if(HID)1=BootInterface
	u8 bInterfaceProtocol;		//7: if(HID)1=kbd,2=mouse
	u8         iInterface;		//8: index of interface string
}__attribute__((packed));

struct EndpointDescriptor{
	u8          bLength;		//0: 0x09
	u8  bDescriptorType;		//1: 0x05
	u8 bEndpointAddress;		//2: endpoint number and direction
	u8     bmAttributes;		//3: endpoint attribute
	u16  wMaxPacketSize;		//[4,5]
	u8        bInterval;		//6: interval between two access
}__attribute__((packed));

struct DeviceQualifier{
	u8            bLength;		//0
	u8    bDescriptorType;		//1: 0x03
}__attribute__((packed));

struct HIDDescriptor{
	u8            bLength;		//0: 0x09
	u8    bDescriptorType;		//1: 0x21
	u16            bcdHID;		//2: HID version
	u8       bCountryCode;
	u8    bNumDescriptors;		//how many HID class descriptors
	u8    bReportDescType;		//report descriptor type
	u16 wReportDescLength;		//report descriptor length
}__attribute__((packed));

struct CS_INTERFACE{
	u8            bLength;		//0: 0x09
	u8    bDescriptorType;		//1: 0x24, for audio?
}__attribute__((packed));

struct CS_ENDPOINT{
	u8            bLength;		//0: 0x09
	u8    bDescriptorType;		//1: 0x25, for audio?
}__attribute__((packed));




void explaindevdesc(struct DeviceDescriptor* desc)
{
	say(".bLength=%x\n",            desc->bLength);
	say(".bDescriptorType=DeviceDescriptor\n");
	say(".bcdUSB=%x\n",             desc->bcdUSB);
	say(".bDeviceClass=%x\n",       desc->bDeviceClass);
	say(".bDeviceSubClass=%x\n",    desc->bDeviceSubClass);
	say(".bDeviceProtocol=%x\n",    desc->bDeviceProtocol);
	say(".bMaxPacketSize0=%x\n",    desc->bMaxPacketSize0);
	say(".idVendor=%04x\n",         desc->idVendor);
	say(".idProduct=%04x\n",        desc->idProduct);
	say(".bcdDevice=%x\n",          desc->bcdDevice);
	say(".iManufacturer=%x\n",      desc->iManufacturer);
	say(".iProduct=%x\n",           desc->iProduct);
	say(".iSerialNumber=%x\n",      desc->iSerialNumber);
	say(".bNumConfigurations=%x\n", desc->bNumConfigurations);
}
void explainconfdesc(struct ConfigurationDescriptor* desc)
{
	say(".bLength=%x\n",             desc->bLength);
	say(".bDescriptorType=ConfigurationDescriptor\n");
	say(".wTotalLength=%x\n",        desc->wTotalLength);
	say(".bNumInterfaces=%x\n",      desc->bNumInterfaces);
	say(".bConfigurationValue=%x\n", desc->bConfigurationValue);
	say(".iConfiguration=%x\n",      desc->iConfiguration);
	say(".bmAttributes=%x\n",        desc->bmAttributes);
	say(".bMaxPower=%dmA\n",         desc->bMaxPower*2);
}
void explainintfdesc(struct InterfaceDescriptor* desc)
{
	say(".bLength=%x\n",            desc->bLength);
	say(".bDescriptorType=InterfaceDescriptor\n");
	say(".bInterfaceNumber=%x\n",   desc->bInterfaceNumber);
	say(".bAlternateSetting=%x\n",  desc->bAlternateSetting);
	say(".bNumEndpoints=%x\n",      desc->bNumEndpoints);
	say(".bInterfaceClass=%x\n",    desc->bInterfaceClass);
	say(".bInterfaceSubClass=%x\n", desc->bInterfaceSubClass);
	say(".bInterfaceProtocol=%x\n", desc->bInterfaceProtocol);
	say(".iInterface=%x\n",         desc->iInterface);
}
void explainendpdesc(struct EndpointDescriptor* desc)
{
	int tmp = desc->bEndpointAddress;
	say(".bLength=%x\n",          desc->bLength);
	say(".bDescriptorType=EndpointDescriptor\n");
	say(".bEndpointAddress=%x,%s\n", tmp&0x1f, (tmp>=0x80)?"in":"out");
	say(".bmAttributes=%x\n",     desc->bmAttributes);
	say(".wMaxPacketSize=%x\n",   desc->wMaxPacketSize);
	say(".bInterval=%dms\n",      desc->bInterval);
}
void explainHIDdesc(struct HIDDescriptor* desc)
{
	say(".bLength=%x\n",            desc->bLength);
	say(".bDescriptorType=HIDDescriptor\n");
	say(".bcdHID=%x\n",             desc->bcdHID);
	say(".bCountryCode=%x\n",       desc->bCountryCode);
	say(".bNumDescriptors=%x\n",    desc->bNumDescriptors);
	say(".bReportDescType=%x\n",    desc->bReportDescType);
	say(".wReportDescLength=%x\n",  desc->wReportDescLength);
}
void explainotherdesc(u8* buf)
{
	say(".bLength=%x\n",         buf[0]);
	say(".bDescriptorType=%x\n", buf[1]);
}
void explaineverydesc(u8* buf, int len)
{
	int j = 0;
	while(1){
		if(j >= len)break;

		say("@[%x,%x]:%x\n", j, j+buf[j]-1, buf[j+1]);
		switch(buf[j+1]){
		case 0x02:explainconfdesc((void*)(buf+j));break;
		case 0x04:explainintfdesc((void*)(buf+j));break;
		case 0x05:explainendpdesc((void*)(buf+j));break;
		case 0x21:explainHIDdesc((void*)(buf+j));break;
		default:explainotherdesc(buf+j);
		}

		if(buf[j] <= 2)break;
		j += buf[j];
	}
}




int usb_delete()
{
	return 0;
}
int usb_create()
{
	return 0;
}