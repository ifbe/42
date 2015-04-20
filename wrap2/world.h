//把当前文件夹翻译成能看懂的样子
struct dirbuffer
{
	QWORD name;		//[+0,0xf]:名字anscii
	QWORD unused;
	QWORD specialid;	//[+0x10,0x1f]:唯一标识符
	QWORD unused1;
	QWORD type;	//[+0x20,0x2f]:种类
	QWORD unused2;
	QWORD size;	//[0x30,0x3f]:大小
	QWORD unused3;
};


//把分区表翻译成能看懂的样子
struct mytable
{
	QWORD startlba;		//始
	QWORD endlba;		//尾
	QWORD type;		//类型anscii(ext，fat，ntfs，hfs，zip，rar，7z等)
	QWORD name;		//名
	QWORD cd;		//cd函数地址
	QWORD load;		//load函数地址
	QWORD explain;		//解释函数地址
	QWORD unused;		//未用
};


//每0x40字节存放分区的基本信息
struct diskinfo
{
	BYTE path[0x20];
	BYTE name[0x20];
};