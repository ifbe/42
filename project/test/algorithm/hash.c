#include<stdio.h>
#include<string.h>
void md5sum(char* dst, char* src, int len);
void sha1sum(char* dst, char* src, int len);
void sha256sum(char* dst, char* src, int len);
void sha512sum(char* dst, char* src, int len);




//
unsigned char src[1024];	//原始数据
unsigned char dst[0x100];	//hash结果




void main(int argc, char* argv[])
{
	int i,len,type;
	if(argc < 2)
	{
		printf("what?\n");
		return;
	}
	else if(strcmp(argv[1], "md5") == 0)type = 1;
	else if(strcmp(argv[1], "sha1") == 0)type = 2;
	else if(strcmp(argv[1], "sha256") == 0)type = 3;
	else if(strcmp(argv[1], "sha512") == 0)type = 4;
	else
	{
		printf("what?\n");
		return;
	}

	//输入加密的字符
	scanf("%s", src);
	if(type == 1)
	{
		md5sum(dst, src, strlen(src));
		len = 16;
	}
	if(type == 2)
	{
		sha1sum(dst, src, strlen(src));
		len = 20;
	}
	if(type == 3)
	{
		sha256sum(dst, src, strlen(src));
		len = 32;
	}
	if(type == 4)
	{
		sha512sum(dst, src, strlen(src));
		len = 64;
	}

	//
	for(i=0; i<len; i++)
	{
		printf("%02x", dst[i]);
	}
	printf("\n");
}
