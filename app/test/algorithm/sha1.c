#include<stdio.h>
#include<string.h>
void sha1_create();
void sha1_write(void*,int);
void sha1_read(char*);


int main(int argc, char* argv[])
{
	int i;
	sha1_create();

	unsigned char encrypt[1024];	//存放于加密的信息  
	unsigned char decrypt[21];	//存放加密后的结果  
	scanf("%s",encrypt);		//输入加密的字符  

	sha1_write(encrypt,strlen((char *)encrypt));
	sha1_read(decrypt);

	printf("加密前:%s\n加密后:",encrypt);
	for(i=0; i<16; i++)
	{
		printf("%02x",decrypt[i]);
	}
	printf("\n\n\n加密结束!\n");

	return 0;
}
