#include<stdio.h>
#include<string.h>
void md5sum(unsigned char* out, const unsigned char* str, int len);
void sha1sum(unsigned char* out, const unsigned char* str, int len);
void base64_encode(unsigned char* out,const unsigned char* in,int len);




void main(int argc,char** argv)
{
	int j,len;
	unsigned char buf1[256];
	unsigned char buf2[256];
	if(argc<2)return;

	len = strlen(argv[1]);

	//md5
	md5sum(buf2, argv[1], len);
	printf("md5(%s)=\n	", argv[1] );
	for(j=0;j<16;j++)printf("%.2x",buf2[j]);
	printf("\n");

	//sha1
	sha1sum(buf2, argv[1], len);
	printf("sha1(%s)=\n	", argv[1] );
	for(j=0;j<20;j++)printf("%.2x",buf2[j]);
	printf("\n");

	//base64
	base64_encode(buf2 ,argv[1], len);
	printf("base64(%s)=\n	", argv[1]);
	printf("%s\n", buf2);

	//websocket
	snprintf(buf1,256,"%s%s",argv[1],"258EAFA5-E914-47DA-95CA-C5AB0DC85B11");
	printf("websocket(%s)=\n	",buf1);

	sha1sum( buf2, buf1, strlen(buf1) );
	for(j=0;j<20;j++)printf("%.2x",buf2[j]);
	printf("\n");

	base64_encode( buf1 ,buf2, 20 );
	printf("	%s\n", buf1);
}
