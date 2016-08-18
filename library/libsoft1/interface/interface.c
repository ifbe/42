#define QWORD unsigned long long
//
void bt_create(void*,void*);
void i2c_create(void*,void*);
void ir_create(void*,void*);
void nfc_create(void*,void*);
void pci_create(void*,void*);
void spi_create(void*,void*);
void uart_create(void*,void*);
void usb_create(void*,void*);
void wifi_create(void*,void*);
void zigbee_create(void*,void*);
//
void bt_delete();
void i2c_delete();
void ir_delete();
void nfc_delete();
void pci_delete();
void spi_delete();
void usb_delete();
void uart_delete();
void wifi_delete();
void zigbee_delete();




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
int interface_create(char* world,QWORD* p)
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

	bt_create(world,q);
	q+=0x40;

	i2c_create(world,q);
	q+=0x40;

	ir_create(world,q);
	q+=0x40;

	nfc_create(world,q);
	q+=0x40;

	pci_create(world,q);
	q+=0x40;

	spi_create(world,q);
	q+=0x40;

	uart_create(world,q);
	q+=0x40;

	usb_create(world,q);
	q+=0x40;

	wifi_create(world,q);
	q+=0x40;

	zigbee_create(world,q);
	q+=0x40;

	return q-(char*)p;
}
void interface_delete()
{
	zigbee_delete();
	wifi_delete();
	usb_delete();
	uart_delete();
	spi_delete();
	pci_delete();
	nfc_delete();
	ir_delete();
	i2c_delete();
	bt_delete();
}
