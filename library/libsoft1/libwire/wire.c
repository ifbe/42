#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
int bt_create(void*);
int bt_delete();
int i2c_create(void*);
int i2c_delete();
int ir_create(void*);
int ir_delete();
int nfc_create(void*);
int nfc_delete();
int pci_create(void*);
int pci_delete();
int spi_create(void*);
int spi_delete();
int uart_create(void*);
int uart_delete();
int usb_create(void*);
int usb_delete();
int wifi_create(void*);
int wifi_delete();
int zigbee_create(void*);
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
void wire_create(char* w)
{
	bt_create(w);
	i2c_create(w);
	ir_create(w);
	nfc_create(w);
	pci_create(w);
	spi_create(w);
	uart_create(w);
	usb_create(w);
	wifi_create(w);
	zigbee_create(w);
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
