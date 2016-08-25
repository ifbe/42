#define u64 unsigned long long




static void pci_list()
{
}
static void pci_choose()
{
}
static void pci_read()
{
}
static void pci_write()
{
}
static void pci_start(u64 type,char* p)
{
}
static void pci_stop()
{
}
void pci_create(char* world,u64* p)
{
	//
	p[0]=0x6563616669;	//type
	p[1]=0x696370;		//id
	p[2]=(u64)pci_start;
	p[3]=(u64)pci_stop;
	p[4]=(u64)pci_list;
	p[5]=(u64)pci_choose;
	p[6]=(u64)pci_read;
	p[7]=(u64)pci_write;
}
void pci_delete()
{
	pci_stop();
}
