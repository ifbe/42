#define u64 unsigned long long
//
int bt_create(void*,void*);
int i2c_create(void*,void*);
int ir_create(void*,void*);
int nfc_create(void*,void*);
int pci_create(void*,void*);
int spi_create(void*,void*);
int uart_create(void*,void*);
int usb_create(void*,void*);
int wifi_create(void*,void*);
int zigbee_create(void*,void*);
//
int bt_delete();
int i2c_delete();
int ir_delete();
int nfc_delete();
int pci_delete();
int spi_delete();
int usb_delete();
int uart_delete();
int wifi_delete();
int zigbee_delete();




static int interface_list()
{
}
static int interface_choose()
{
}
static int interface_read()
{
}
static int interface_write()
{
}
static int interface_start(u64 type,char* p)
{
}
static int interface_stop()
{
}
int interface_create(char* world,u64* p)
{
	//
	p[0]=0;			//type
	p[1]=0x6563616669;	//id

	p[10]=(u64)interface_start;
	p[11]=(u64)interface_stop;
	p[12]=(u64)interface_list;
	p[13]=(u64)interface_choose;
	p[14]=(u64)interface_read;
	p[15]=(u64)interface_write;

	//
	char* q=(char*)p+0x80;

	bt_create(world,q);
	q+=0x80;

	i2c_create(world,q);
	q+=0x80;

	ir_create(world,q);
	q+=0x80;

	nfc_create(world,q);
	q+=0x80;

	pci_create(world,q);
	q+=0x80;

	spi_create(world,q);
	q+=0x80;

	uart_create(world,q);
	q+=0x80;

	usb_create(world,q);
	q+=0x80;

	wifi_create(world,q);
	q+=0x80;

	zigbee_create(world,q);
	q+=0x80;

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
