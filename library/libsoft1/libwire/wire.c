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




static int wire_list()
{
	return 0;
}
static int wire_choose()
{
	return 0;
}
static int wire_read()
{
	return 0;
}
static int wire_write()
{
	return 0;
}
static int wire_start(u64 type,char* p)
{
	return 0;
}
static int wire_stop()
{
	return 0;
}
int wire_create(char* world,u64* p)
{
	//
	p[0]=0;			//type
	p[1]=0x65726977;	//id

	p[10]=(u64)wire_start;
	p[11]=(u64)wire_stop;
	p[12]=(u64)wire_list;
	p[13]=(u64)wire_choose;
	p[14]=(u64)wire_read;
	p[15]=(u64)wire_write;

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
void wire_delete()
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
