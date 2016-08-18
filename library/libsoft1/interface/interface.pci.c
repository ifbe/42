#define QWORD unsigned long long




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
static void pci_start(QWORD type,char* p)
{
}
static void pci_stop()
{
}
void pci_create(char* world,QWORD* p)
{
	//
	p[0]=0x6563616669;	//type
	p[1]=0x696370;		//id
	p[2]=(QWORD)pci_start;
	p[3]=(QWORD)pci_stop;
	p[4]=(QWORD)pci_list;
	p[5]=(QWORD)pci_choose;
	p[6]=(QWORD)pci_read;
	p[7]=(QWORD)pci_write;
}
void pci_delete()
{
	pci_stop();
}
