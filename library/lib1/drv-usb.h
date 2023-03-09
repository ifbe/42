#include "const-def.h"




//usb class
#define class_reserve       0x00	//去看interface descriptor
#define class_audio         0x01	//音频类
#define class_cdccom        0x02	//通讯 + CDC控制类
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
//video collection: e,3,0
//video control: e,1,0
//video streaming: e,2,0
//audio collection: 1,2,0
//audio control: 1,1,0
//audio streaming: 1,2,0




#define _tohc_ 0x80000000
#define _tohc_addr_ (_tohc_|hex32('a','d','d','r'))	//prepare slotctx+ep0ctx
#define _tohc_eval_ (_tohc_|hex32('e','v','a','l'))	//modify epctx
#define _tohc_conf_ (_tohc_|hex32('c','o','n','f'))	//prepare ep*ctx
#define _tohc_hub_  (_tohc_|hex32('h','u','b', 0))	//notify it is hub
#define _tohc_new_  (_tohc_|hex32('n','e','w', 0))	//notify device under hub




//bit[0,4]: 0=device, 1=interface, 2=endpoint
#define bmreqdest_device 0x0
#define bmreqdest_interface 0x1
#define bmreqdest_endpoint 0x2
//bit[5,6]: 0=normal, 1=class, 2=vendor
#define bmreqtype_normal 0x0
#define bmreqtype_class 0x20
#define bmreqtype_vendor 0x40
#define bmreqtype_unknow 0x80
//bit7: 0=host to device, 1=device to host
#define bmreqdir_h2d 0x0
#define bmreqdir_d2h 0x80
//bRequest
#define GET_STATUS        0
#define CLEAR_FEATURE     1
#define SET_FEATURE       3
#define SET_ADDRESS       5
#define GET_DESCRIPTOR    6
#define SET_DESCRIPTOR    7
#define GET_CONFIGURATION 8
#define SET_CONFIGURATION 9
#define GET_INTERFACE     10
#define SET_INTERFACE     11
#define SYNCH_FRAME       12
struct UsbRequest{
	//0
	u8 bmRequestType;
	//1
	u8 bRequest;
	//[2,3]
	u16 wValue;		//if(GET_DESCRIPTOR)hi = type, lo = index
	//[4,5]
	u16 wIndex;		//if(GET_DESCRIPTOR_string)wIndex = LANGID
	//[6,7]
	u16 wLength;
}__attribute__((packed));




#define DESCTYPE_DEVICE 1
#define DESCTYPE_CONFIG 2
#define DESCTYPE_STRING 3
#define DESCTYPE_INTFERFACE 4
#define DESCTYPE_ENDPOINT 5
#define DESCTYPE_QUALIFIER 6
#define DESCTYPE_OTHERSPEED 7
#define DESCTYPE_INTFPOWER 8
#define DESCTYPE_BOS 0xf
#define DESCTYPE_HUB2 0x29
#define DESCTYPE_HUB3 0x2a
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
	union{
		u16 wLANGID[0];		//[2,3]: at least one, 0409=en-US
		u16 unicode[0];
	};
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
			//bit[0,1]: 0=control, 1=isochronous, 2=bulk, 3=interrupt
			//(onlyif isoch)bit[2,3]: 0=nosync..., 1=async, 2=adaptive, 3=sync
			//(onlyif isoch)bit[4,5]: 0=data, 1=feedback, 2=explicit feedback data
	u16  wMaxPacketSize;		//[4,5]
	u8        bInterval;		//6: interval between two access
}__attribute__((packed));

struct DeviceQualifier{
	u8            bLength;		//0
	u8    bDescriptorType;		//1: 0x06
	u16            bcdUSB;		//[2,3]
	u8       bDeviceClass;		//4
	u8    bDeviceSubclass;		//5
	u8    bDeviceProtocol;		//6
	u8    bMaxPacketSize0;		//7
	u8 bNumConfigurations;		//8
	u8           Reserved;		//9
}__attribute__((packed));

struct OtherSpeedConfiguration{
	u8             bLength;		//0: 0x09
	u8     bDescriptorType;		//1: 0x07
	u16       wTotalLength;		//[2,3]
	u8      bNumInterfaces;		//4: how many interface in this configuration
	u8 bConfigurationValue;		//5: this configuration
	u8      iConfiguration;		//6: index of configuration string
	u8        bmAttributes;		//7: 7=bus power, 6=self power, 5=remote wakeup
	u8           bMaxPower;		//8: 50=100mA, 250=500mA
}__attribute__((packed));

struct INTERFACE_POWER{
	u8             bLength;		//0:
	u8     bDescriptorType;		//1: 0x08
}__attribute__((packed));

struct USB_DT_OTG{
	u8             bLength;		//0:
	u8     bDescriptorType;		//1: 0x09
}__attribute__((packed));

struct USB_DT_DEBUG{
	u8             bLength;		//0:
	u8     bDescriptorType;		//1: 0x0a
}__attribute__((packed));

struct InterfaceAssociation{
	u8           bLength;		//0: 0x08
	u8   bDescriptorType;		//1: 0x0b
	u8   bFirstInterface;		//2
	u8   bInterfaceCount;		//3
	u8    bFunctionClass;		//4
	u8 bFunctionSubclass;		//5
	u8 bFunctionProtocol;		//6
	u8         iFunction;		//7
}__attribute__((packed));

struct USB_DT_SECURITY{
	u8           bLength;		//0: 0x08
	u8   bDescriptorType;		//1: 0x0c
}__attribute__((packed));

struct USB_DT_KEY{
	u8           bLength;		//0: 0x08
	u8   bDescriptorType;		//1: 0x0d
}__attribute__((packed));

struct USB_DT_ENCRYPTION_TYPE{
	u8           bLength;		//0: 0x08
	u8   bDescriptorType;		//1: 0x0e
}__attribute__((packed));

struct BOSDescriptor{
	u8         bLength;		//0: 0x08
	u8 bDescriptorType;		//1: 0x0f
	u16   wTotalLength;		//[2,3]
	u8  bNumDeviceCaps;		//4
}__attribute__((packed));

struct DeviceCapabilityDescriptor{
	u8            bLength;		//0: 0x08
	u8    bDescriptorType;		//1: 0x10
	u8 bDevCapabilityType;		//2
	u8             cap[0];		//3
}__attribute__((packed));

struct EndpointCompanionDescriptor{
	u8            bLength;		//0: 0x08
	u8    bDescriptorType;		//1: 0x30
	u8          bMaxBurst;		//2
	u8       bmAttributes;		//3
	u16 wBytesPerInterval;		//[4,5]
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

struct hubcharacter{
	u16 LogPwrSwitchMode : 2;
	u16 CompoundDevice : 1;
	u16 OverCurrentProtectMode : 2;
	u16 TTThinkTime : 2;
	u16 PortIndicatorsSupported : 1;
	u16 Reserved : 8;
}__attribute__((packed));
struct HubDescriptor{
	u8          bDescLength;
	u8      bDescriptorType;	//1: 0x29
	u8            bNbrPorts;

	union{
		u16 wHubCharacteristics;
		struct hubcharacter character;
	};

	u8       bPwrOn2PwrGood;
	u8     bHubContrCurrent;
}__attribute__((packed));

struct USB_DT_SS_ENDPOINT_COMP{
	u8            bLength;		//0: 0x09
	u8    bDescriptorType;		//1: 0x30
}__attribute__((packed));

struct USB_DT_SSP_ISOC_ENDPOINT_COMP{
	u8            bLength;		//0: 0x09
	u8    bDescriptorType;		//1: 0x31
}__attribute__((packed));




struct descnode{
	u32    type;	//dev,conf,intf,endp,str
	u32    real;	//value of this desc
	u32      id;	//offset to real desc
	u32   altid;	//setted as current
	u32 lfellow;	//prev brother node, if(0){first}
	u32 rfellow;	//next brother node, if(0){last}
	u32  lchild;	//first child node
	u32  rchild;	//last child node
}__attribute__((packed));


struct usbdesc_parsed{
	//device descriptor
	u8 iManufac;	//won't change
	u8 iProduct;	//won't change
	u8 iSerial; 	//won't change
	u8 numconf;

	//string descriptor
	u16 lang0;

	//config descriptor
	u32 nodecount;
	struct descnode node[0];
}__attribute__((packed));


struct usbdesc_origin{
	int byteused;
	struct DeviceDescriptor devdesc;
	struct StringDescriptor strdesc;
}__attribute__((packed));


struct perusb{
	//[0x0, 0xfff]
	union{
		struct usbdesc_parsed parsed;
		u8 padding0[0x1000];
	};

	//[0x1000, 0xffff]
	union{
		struct usbdesc_origin origin;
		u8 padding1[0xf000];
	};

	//[0x10000, 0xfffff]
	u8 perfunc[0];
}__attribute__((packed));
