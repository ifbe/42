#define QWORD unsigned long long
//
void bt_init(void*,void*);
void i2c_init(void*,void*);
void ir_init(void*,void*);
void nfc_init(void*,void*);
void pci_init(void*,void*);
void spi_init(void*,void*);
void uart_init(void*,void*);
void usb_init(void*,void*);
void wifi_init(void*,void*);
void zigbee_init(void*,void*);
//
void bt_kill();
void i2c_kill();
void ir_kill();
void nfc_kill();
void pci_kill();
void spi_kill();
void usb_kill();
void uart_kill();
void wifi_kill();
void zigbee_kill();




static void interface_list()
{
}
static void interface_choose()
{
}
static void interface_read()
{
}
static void interface_write()
{
}
static void interface_start(QWORD type,char* p)
{
}
static void interface_stop()
{
}
int interface_init(char* world,QWORD* p)
{
	//
	p[0]=0;			//type
	p[1]=0x6563616669;	//id
	p[2]=(QWORD)interface_start;
	p[3]=(QWORD)interface_stop;
	p[4]=(QWORD)interface_list;
	p[5]=(QWORD)interface_choose;
	p[6]=(QWORD)interface_read;
	p[7]=(QWORD)interface_write;

	//
	char* q=(char*)p+0x40;

	bt_init(world,q);
	q+=0x40;

	i2c_init(world,q);
	q+=0x40;

	ir_init(world,q);
	q+=0x40;

	nfc_init(world,q);
	q+=0x40;

	pci_init(world,q);
	q+=0x40;

	spi_init(world,q);
	q+=0x40;

	uart_init(world,q);
	q+=0x40;

	usb_init(world,q);
	q+=0x40;

	wifi_init(world,q);
	q+=0x40;

	zigbee_init(world,q);
	q+=0x40;

	return q-(char*)p;
}
void interface_kill()
{
	zigbee_kill();
	wifi_kill();
	usb_kill();
	uart_kill();
	spi_kill();
	pci_kill();
	nfc_kill();
	ir_kill();
	i2c_kill();
	bt_kill();
}
