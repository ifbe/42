#include "struct.h"




//xhci command
#define TRB_command_EnableSlot           9
#define TRB_command_DisableSlot         10
#define TRB_command_AddressDevice       11
#define TRB_command_ConfigureEndpoint   12
#define TRB_command_EvaluateContext     13
#define TRB_command_ResetEndpoint       14
#define TRB_command_StopEndpoint        15
#define TRB_command_SetTRDequeuePointer 16
#define TRB_command_ResetDevice         17
#define TRB_command_ForceEvent          18		//Optional, used with virtualization only
#define TRB_command_NegotiateBandwidth  19
#define TRB_command_SetLatencyTolerance 20
#define TRB_command_GetPortBandwidth    21
#define TRB_command_ForceHeader         22
#define TRB_command_NoOp                23
#define TRB_command_GetExtendedProperty 24
#define TRB_command_SetExtendedProperty 25
//usb bRequest
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




struct UsbRequest{
	//[0,3]
	u8 bmRequestType;
		//bit[0,4]: 0=device, 1=interface, 2=endpoint
		//bit[5,6]: 0=normal, 1=class, 2=vendor
		//bit7: 0=host to device, 1=device to host
	u8 bRequest;
		//0: GET_STATUS
		//1: CLEAR_FEATURE
		//3: SET_FEATURE
		//5: SET_ADDRESS
		//6: GET_DESCRIPTOR
		//7: SET_DESCRIPTOR
		//8: GET_CONFIGURATION
		//9: SET_CONFIGURATION
		//a: GET_INTERFACE
		//b: SET_INTERFACE
		//c: SYNCH_FRAME
	u16 wValue;
		//if(GET_DESCRIPTOR)hi = type, lo = index
	//[4,7]
	u16 wIndex;
		//if(GET_DESCRIPTOR_string)wIndex = LANGID
	u16 wLength;
}__attribute__((packed));




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




struct descnode{
	u32    type;	//dev,conf,intf,endp,str
	u32   index;	//value of this desc
	u32    real;	//offset to real desc
	u32  chosen;	//setted as current
	u32 lfellow;	//prev brother node, if(0){first}
	u32 rfellow;	//next brother node, if(0){last}
	u32  lchild;	//first child node
	u32  rchild;	//last child node
}__attribute__((packed));
struct bufhead{
	//[0x00, 0xff]
	u32  devnode;	//chosen node for device descriptor
	u32 confnode;	//chosen node for configure descriptor
	u32 intfnode;	//chosen node for interface descriptor
	u32  strnode;	//chosen node for string descriptor(chosen language)
	u32 nodelen;
	u32 desclen;

	u8 iManufac;	//won't change
	u8 iProduct;	//won't change
	u8 iSerial; 	//won't change
	u8 iConfigure;	//change when new conf
	u8 iInterface;	//change when new intf
}__attribute__((packed));
struct perusb{
	union{
		struct bufhead my;
		u8 padding0[0x100];
	};

	//[0x100, 0xfff]
	struct descnode node[0];	//0xf00/0x20=0x78, enough for node
	u8 padding1[0xf00];

	//[0x1000, 0xffff]
	u8 desc[0];					//0xf000=61440, enough for desc
	u8 padding2[0xf000];

	//[0x10000, 0xfffff]
	u8 freebuf[0];
}__attribute__((packed));