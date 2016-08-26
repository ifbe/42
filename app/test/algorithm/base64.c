#include<stdio.h>
#include<string.h>
void base64_encode(char* out,char* in,int len);
void base64_decode(char* out,char* in);


void main(int argc, char* argv[])
{
	int i;
	unsigned char encrypt[256];	//存放于加密的信息  
	unsigned char decrypt[256];	//存放加密后的结果  
	if(argc<2)return;

	base64_encode( encrypt, argv[1], strlen(argv[1]) );
	base64_decode( decrypt, encrypt );

	printf("加密:%s\n",encrypt);
	printf("还原:%s\n",decrypt);

	return;
}
