#include "libhard.h"
void* mmiobase();
int pinmgr_setfunc(int pin, int func);


#define ARM_UART0_BASE 0x201000
#define ARM_UART0_DR   0x00
#define ARM_UART0_FR   0x18
#define ARM_UART0_IBRD 0x24
#define ARM_UART0_FBRD 0x28
#define ARM_UART0_LCRH 0x2C
#define ARM_UART0_CR   0x30
#define ARM_UART0_IFLS 0x34
#define ARM_UART0_IMSC 0x38
#define ARM_UART0_RIS  0x3C
#define ARM_UART0_MIS  0x40
#define ARM_UART0_ICR  0x44
unsigned char bt_lo(unsigned int val) {
	return (unsigned char)(val & 0xff);
}
unsigned char bt_hi(unsigned int val) {
	return (unsigned char)((val & 0xff00) >> 8);
}
u32 mmio_read(void* addr)
{
	return *(u32*)(addr);
}
u32 mmio_write(void* addr, u32 data)
{
	*(u32*)(addr) = data;
}
unsigned int mmio_isReadByteReady(void* uart0) {
	return (!(mmio_read(uart0+ARM_UART0_FR) & 0x10));
}
unsigned char mmio_readByte(void* uart0)
{
	unsigned char ch = bt_lo(mmio_read(uart0+ARM_UART0_DR));
	return ch;
}
unsigned char mmio_waitReadByte(void* uart0)
{
	while (!mmio_isReadByteReady(uart0));
	return mmio_readByte(uart0);
}
void mmio_writeByte(void* uart0, char byte)
{
	while ((mmio_read(uart0+ARM_UART0_FR) & 0x20) != 0);
	mmio_write(uart0+ARM_UART0_DR, (unsigned int)byte);
}
void uart0_init(void* uart0)
{
	while( mmio_isReadByteReady(uart0) ){
		mmio_readByte(uart0);
	}

	mmio_write(uart0+ARM_UART0_IMSC, 0x00);
	mmio_write(uart0+ARM_UART0_ICR,  0x7ff);
	mmio_write(uart0+ARM_UART0_IBRD, 0x1a);
	mmio_write(uart0+ARM_UART0_FBRD, 0x03);
	mmio_write(uart0+ARM_UART0_IFLS, 0x08);
	mmio_write(uart0+ARM_UART0_LCRH, 0x70);
	mmio_write(uart0+ARM_UART0_CR,   0xB01);
	mmio_write(uart0+ARM_UART0_IMSC, 0x430);

	sleep_ms(1000);
}




enum {
	OGF_HOST_CONTROL          = 0x03,
	OGF_LE_CONTROL            = 0x08,
	OGF_VENDOR                = 0x3f,

	COMMAND_SET_BDADDR        = 0x01,
	COMMAND_RESET_CHIP        = 0x03,
	COMMAND_SET_BAUD          = 0x18,
	COMMAND_LOAD_FIRMWARE     = 0x2e,

	HCI_COMMAND_PKT           = 0x01,
	HCI_ACL_PKT               = 0x02,
	HCI_EVENT_PKT             = 0x04,
	COMMAND_COMPLETE_CODE     = 0x0e,
	CONNECT_COMPLETE_CODE     = 0x0f,

	LL_SCAN_ACTIVE            = 0x01,
	LL_ADV_NONCONN_IND        = 0x03
};
int bt_opcode(unsigned short ogf, unsigned short ocf)
{
	return ogf << 10 | ocf;
}
int bt_hciCommand(void* uart0, u16 opcode, volatile unsigned char *data, unsigned char length)
{
	unsigned char c=0;
	unsigned char opcodebytes[2] = { bt_lo(opcode), bt_hi(opcode) };

	mmio_writeByte(uart0, HCI_COMMAND_PKT);
	mmio_writeByte(uart0, opcodebytes[0]);
	mmio_writeByte(uart0, opcodebytes[1]);
	mmio_writeByte(uart0, length);

	while (c++<length) mmio_writeByte(uart0, *data++);

	if (mmio_waitReadByte(uart0) != HCI_EVENT_PKT) return 1;

	unsigned char code = mmio_waitReadByte(uart0);
	if (code == CONNECT_COMPLETE_CODE) {
	   if (mmio_waitReadByte(uart0) != 4) return 2;

		unsigned char err = mmio_waitReadByte(uart0);
		if (err != 0) {
			logtoall("Saw HCI COMMAND STATUS error:%x\n", err);
			return 12;
		}

		if (mmio_waitReadByte(uart0) == 0) return 3;
		if (mmio_waitReadByte(uart0) != opcodebytes[0]) return 4;
		if (mmio_waitReadByte(uart0) != opcodebytes[1]) return 5;
	}
	else if (code == COMMAND_COMPLETE_CODE) {
		if (mmio_waitReadByte(uart0) != 4) return 6;
		if (mmio_waitReadByte(uart0) == 0) return 7;
		if (mmio_waitReadByte(uart0) != opcodebytes[0]) return 8;
		if (mmio_waitReadByte(uart0) != opcodebytes[1]) return 9;
		if (mmio_waitReadByte(uart0) != 0) return 10;
	}
	else return 11;

	return 0;
}
void bt_reset(void* uart0) {
	volatile unsigned char empty[] = {};
	int ret = bt_hciCommand(uart0, bt_opcode(OGF_HOST_CONTROL, COMMAND_RESET_CHIP), empty, 0);
	if(ret)logtoall("bt_reset() failed=%d\n", ret);
}
/*
void bt_loadfirmware(void* uart0)
{
	volatile unsigned char empty[] = {};
	if (bt_hciCommand(uart0, bt_opcode(OGF_VENDOR, COMMAND_LOAD_FIRMWARE), empty, 0)) uart_writeText("loadFirmware() failed\n");

	extern unsigned char _binary_BCM4345C0_hcd_start[];
	extern unsigned char _binary_BCM4345C0_hcd_size[];

	unsigned int c=0;
	unsigned int size = (long)&_binary_BCM4345C0_hcd_size;

	unsigned char opcodebytes[2];
	unsigned char length;
	unsigned char *data = &(_binary_BCM4345C0_hcd_start[0]);

	while (c < size) {
		opcodebytes[0] = *data;
		opcodebytes[1] = *(data+1);
		length =         *(data+2);
		data += 3;

		if (bt_hciCommand(uart0, (opcodebytes[1]<<8)+opcodebytes[0], data, length)) {
			uart_writeText("Firmware data load failed\n");
			break;
		}

		data += length;
		c += 3 + length;
	}

	wait_msec(0x100000);
}
void bt_setbaud(void* uart0)
{
	volatile unsigned char command[6] = { 0, 0, 0x00, 0xc2, 0x01, 0x00 }; // little endian, 115200
	if (hciCommand(uart0, bt_opcode(OGF_VENDOR, COMMAND_SET_BAUD), command, 6)) uart_writeText("bt_setbaud() failed\n");
}
void bt_setbdaddr(void* uart0)
{
	volatile unsigned char command[6] = { 0xee, 0xff, 0xc0, 0xee, 0xff, 0xc0 }; // reversed
	if (hciCommand(uart0, bt_opcode(OGF_VENDOR, COMMAND_SET_BDADDR), command, 6)) uart_writeText("bt_setbdaddr() failed\n");
}
void bt_getbdaddr(void* uart0, unsigned char *bdaddr) {
	mmio_writeByte(uart0, HCI_COMMAND_PKT);
	mmio_writeByte(uart0, 0x09);
	mmio_writeByte(uart0, 0x10);
	mmio_writeByte(uart0, 0x00);

	if (mmio_waitReadByte(uart0) != HCI_EVENT_PKT) return;
	if (mmio_waitReadByte(uart0) != COMMAND_COMPLETE_CODE) return;
	if (mmio_waitReadByte(uart0) != 0x0a) return;
	if (mmio_waitReadByte(uart0) != 1) return;
	if (mmio_waitReadByte(uart0) != 0x09) return;
	if (mmio_waitReadByte(uart0) != 0x10) return;
	if (mmio_waitReadByte(uart0) != 0x00) return;

	for (int c=0;c<6;c++) bdaddr[c] = mmio_waitReadByte(uart0);
}
void sendACLsubscribe(void* uart0, unsigned int handle)
{
	mmio_writeByte(uart0, HCI_ACL_PKT);

	mmio_writeByte(uart0, bt_lo(handle));
	mmio_writeByte(uart0, bt_hi(handle));

	unsigned int length = 0x0009;
	mmio_writeByte(uart0, bt_lo(length));
	mmio_writeByte(uart0, bt_hi(length));

	unsigned int data_length = 0x0005;
	mmio_writeByte(uart0, bt_lo(data_length));
	mmio_writeByte(uart0, bt_hi(data_length));

	unsigned int channel = 0x0004;
	mmio_writeByte(uart0, bt_lo(channel));
	mmio_writeByte(uart0, bt_hi(channel));

	volatile unsigned char command[5] = { 0x12, 0x2b, 0x00, 0x01, 0x00 };

	unsigned int c=0;
	while (c++<data_length) mmio_writeByte(uart0, command[c-1]);
}
void setLEeventmask(void* uart0, unsigned char mask)
{
	volatile unsigned char command[8] = { 0 };
	command[0] = mask;

	if (hciCommand(uart0, bt_opcode(OGF_LE_CONTROL, 0x01), command, 8)) uart_writeText("setLEeventmask failed\n");
}
void setLEscanenable(void* uart0, unsigned char state, unsigned char duplicates) {
	volatile unsigned char command[2];
	command[0] = state;
	command[1] = duplicates;
	if (hciCommand(uart0, bt_opcode(OGF_LE_CONTROL, 0x0c), command, 2)) uart_writeText("setLEscanenable failed\n");
}
void setLEscanparameters(void* uart0, unsigned char type, unsigned char linterval, unsigned char hinterval, unsigned char lwindow, unsigned char hwindow, unsigned char own_address_type, unsigned char filter_policy) {
	volatile unsigned char command[7];
	command[0] = type;
	command[1] = linterval;
	command[2] = hinterval;
	command[3] = lwindow;
	command[4] = hwindow;
	command[5] = own_address_type;
	command[6] = filter_policy;
	if (hciCommand(uart0, bt_opcode(OGF_LE_CONTROL, 0x0b), command, 7)) uart_writeText("setLEscanparameters failed\n");
}
void setLEadvertenable(void* uart0, unsigned char state) {
	volatile unsigned char command[1];
	command[0] = state;
	if (hciCommand(uart0, bt_opcode(OGF_LE_CONTROL, 0x0a), command, 1)) uart_writeText("setLEadvertenable failed\n");
}
void setLEadvertparameters(void* uart0, unsigned char type, unsigned char linterval_min, unsigned char hinterval_min, unsigned char linterval_max, unsigned char hinterval_max, unsigned char own_address_type, unsigned char filter_policy) {
	volatile unsigned char command[15] = { 0 };

	command[0] = linterval_min;
	command[1] = hinterval_min;
	command[2] = linterval_max;
	command[3] = hinterval_max;
	command[4] = type;
	command[5] = own_address_type;
	command[13] = 0x07;
	command[14] = filter_policy;

	if (hciCommand(uart0, bt_opcode(OGF_LE_CONTROL, 0x06), command, 15)) uart_writeText("setLEadvertparameters failed\n");
}
void setLEadvertdata(void* uart0) {
	static unsigned char command[32] = {
	   0x19,
	   0x02, 0x01, 0x06,
	   0x03, 0x03, 0xAA, 0xFE,
	   0x11, 0x16, 0xAA, 0xFE, 0x10, 0x00, 0x03,
	   0x69, 0x73, 0x6f, 0x6d, 0x65, 0x74, 0x69, 0x6d,
	   0x2e, 0x65, 0x73,
	   0, 0, 0, 0, 0, 0
	};

	if (hciCommand(uart0, bt_opcode(OGF_LE_CONTROL, 0x08), command, 32)) uart_writeText("setLEadvertdata failed\n");
}
void stopScanning() {
	setLEscanenable(0, 0);
}
void stopAdvertising() {
	setLEadvertenable(0);
}
void startActiveScanning() {
	float BleScanInterval = 60; // every 60ms
	float BleScanWindow = 60;
	float BleScanDivisor = 0.625;

	unsigned int p = BleScanInterval / BleScanDivisor;
	unsigned int q = BleScanWindow / BleScanDivisor;

	setLEscanparameters(LL_SCAN_ACTIVE, bt_lo(p), bt_hi(p), bt_lo(q), bt_hi(q), 0, 0);
	setLEscanenable(1, 0);
}
void startActiveAdvertising() {
	float advertMinFreq = 100; // every 100ms
	float advertMaxFreq = 100; // every 100ms
	float bleGranularity = 0.625;

	unsigned int min_interval = advertMinFreq / bleGranularity;
	unsigned int max_interval = advertMaxFreq / bleGranularity;

	setLEadvertparameters(LL_ADV_NONCONN_IND, bt_lo(min_interval), bt_hi(min_interval), bt_lo(max_interval), bt_hi(max_interval), 0, 0);
	setLEadvertdata();
	setLEadvertenable(1);
}
void connect(void* uart0, unsigned char *addr)
{
	float BleScanInterval = 60; // every 60ms
	float BleScanWindow = 60;
	float BleScanDivisor = 0.625;

	float connMinFreq = 30; // every 30ms
	float connMaxFreq = 50; // every 50ms
	float BleGranularity = 1.25;

	unsigned int p = BleScanInterval / BleScanDivisor;
	unsigned int q = BleScanWindow / BleScanDivisor;

	unsigned int min_interval = connMinFreq / BleGranularity;
	unsigned int max_interval = connMaxFreq / BleGranularity;

	volatile unsigned char command[25] = { 0 };

	command[0] = bt_lo(p);
	command[2] = bt_lo(q);
	command[6] = *(addr+5);
	command[7] = *(addr+4);
	command[8] = *(addr+3);
	command[9] = *(addr+2);
	command[10] = *(addr+1);
	command[11] = *addr;
	command[13] = bt_lo(min_interval);
	command[15] = bt_lo(max_interval);
	command[19] = 0x2a;
	command[20] = 0x00;

	if (hciCommand(uart0, bt_opcode(OGF_LE_CONTROL, 0x0d), command, 25)) uart_writeText("createLEconnection failed\n");
}
*/




void initrpibt()
{
	logtoall("@initrpibt\n");

	//bt -> GPIO30-33 -> uart0
	pinmgr_setfunc(30, 7);
	pinmgr_setfunc(31, 7);
	pinmgr_setfunc(32, 7);
	pinmgr_setfunc(33, 7);

	logtoall(">>setup uart0\n");
	void* mmio = mmiobase();
	void* uart0 = mmio + ARM_UART0_BASE;
	uart0_init(uart0);

	logtoall(">>reset bt\n");
	bt_reset(uart0);
/*
	logtoall(">> firmware load: ");
	bt_loadfirmware();
	logtoall(">> set baud: ");
	bt_setbaud();
	logtoall(">> set bdaddr: ");
	bt_setbdaddr();

	// Print the BD_ADDR
	unsigned char local_addr[6];
	bt_getbdaddr(local_addr);
	printmemory(local_addr, 6);
*/
	logtoall("\n");
}