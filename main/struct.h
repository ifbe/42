//这里每个struct占用0x40字节，目的是写代码的时候方便些(吧)
//表开始地址+0x40*第几个=某个项的位置，也许根本用不着结构体




//每0x40个字节是一个当前目录表
struct dirbuffer
{
	QWORD name;	//[+0,0x7]:起始lba
	QWORD unused;
	QWORD specialid;	//[+0x20,0x2f]:分区类型anscii
	QWORD unused1;
	QWORD type;	//[+0x10,0x1f]:末尾lba
	QWORD unused2;
	QWORD size;	//[0x30,0x3f]:分区名字
	QWORD unused3;
};


//每0x40字节存放分区的基本信息
struct mytable
{
	QWORD startlba;	//[+0,0x7]:起始lba
	QWORD endlba;	//[+0x8,0xf]:末尾lba
	QWORD type;	//[+0x10,0x17]:分区类型anscii
	QWORD name;	//[0x18,0x1f]:分区名字
	QWORD a[4];	//[0x20,0x3f]:unused
};


//每0x40字节存放分区的基本信息
struct diskinfo
{
	BYTE path[0x20];
	BYTE name[0x20];
};