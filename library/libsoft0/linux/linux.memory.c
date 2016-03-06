static int _dev_mem=-1;




void readmemory()
{
}
void writememory()
{
}




void initmemory()
{
	_dev_mem=open("/dev/mem", O_RDWR|O_SYNC);
}
void killmemory()
{
	close(_dev_mem);
}