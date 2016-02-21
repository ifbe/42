#define QWORD unsigned long long
#define DWORD unsigned int
#define WORD unsigned short
#define BYTE unsigned char
//检查
int iself(char*);	//可执行文件
int ismacho(char*);
int ispe(char*);
int is7z(char*);	//压缩包
int iscpio(char*);
int isgz(char*);
int istar(char*);
int iszip(char*);
int isfat(char*);	//文件系统
int isntfs(char*);
int isext(char*);
int ishfs(char*);
int isgpt(char*);	//分区表
int ismbr(char*);




//试毒员
QWORD prelibation(char* memaddr)
{
/*
	//是视频文件
	if( ismp4(memaddr) !=0 )return ;	//'mp4'
	if( isrmvb(memaddr) !=0 )return ;	//'rmvb'

	//是音乐文件
	if( ismp3(memaddr) !=0 )return ;	//'mp3'
	if( iswav(memaddr) !=0 )return ;	//'wav'

	//是图片
	if( isjpeg(memaddr) !=0 )return ;	//'jpeg'
	if( ispng(memaddr) !=0 )return ;	//'png'

	//办公文件
	if( isdoc(memaddr) !=0 )return ;	//'doc'
	if( ispdf(memaddr) !=0 )return ;	//'pdf'

	//3d模型

	//网络协议包
	if( isethernet(memaddr) !=0 )return ;	//'ethernet'
	if( isarp(memaddr) !=0 )return ;	//'arp'
	if( isudp(memaddr) !=0 )return ;	//'udp'
	if( istcp(memaddr) !=0 )return ;	//'tcp'
*/
	//是可执行文件
	if( iself(memaddr) !=0 )return 0x666c65;	//'elf'
	if( ismacho(memaddr) !=0 )return 0x6f6863616d;	//'macho'
	if( ispe(memaddr) !=0 )return 0x6570;		//'pe'

	//是压缩包
	if( is7z(memaddr) !=0 )return 0x7a37;		//'7z'
	if( iscpio(memaddr) !=0 )return 0x6f697063;	//'cpio'
	if( isgz(memaddr) !=0 )return 0x7a67;		//'gz'
	if( istar(memaddr) !=0 )return 0x726174;	//'tar'
	if( iszip(memaddr) !=0 )return 0x70697a;	//'zip'

	//是文件系统
	if( isfat(memaddr) !=0 )return 0x746166;	//'fat'
	if( isntfs(memaddr) !=0 )return 0x7366746e;	//'ntfs'
	if( isext(memaddr) !=0 )return 0x747865;	//'ext'
	if( ishfs(memaddr) !=0 )return 0x736668;	//'hfs'

	//是分区表头
	//if( isapm(memaddr) !=0)return ;	//'apm'	//apple partition map
	//if( isbsd(memaddr) !=0)return ;	//'bsd'	//bsd label
	if( isgpt(memaddr) !=0 )return 0x747067;	//'gpt'
	if( ismbr(memaddr) !=0 )return 0x72626d;	//'mbr',特殊,只能放最后

	//什么都不像，返回失败
	return 0;	//'unknown'
}
