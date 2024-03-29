#include "libsoft.h"
//
void generatePG(void*, int, void*, int);
int ncmp(void*, void*, int);
int cmp(void*, void*);
void bigint_print(struct str* src);




static u8 modulus_dh[] = {
0x00,0xd8,0x4b,0x8e,0x9b,0x72,0x59,0xf3,0x4a,0xa4,0x60,0x62,0x68,0x4a,0xd0,0x58,
0x73,0xfd,0x40,0x7c,0x5f,0xa8,0x04,0xff,0xbc,0x42,0x9d,0x0d,0xed,0x5b,0x49,0x72,
0x59,0xff,0xf7,0xf3,0xc2,0x6d,0x7a,0x4c,0x21,0x59,0x2f,0xcd,0x65,0x8b,0x60,0x7a,
0x5b,0xde,0x7c,0x4e,0xaa,0x7f,0x5a,0x3b,0x6b,0x71,0x3e,0xae,0xdd,0x5d,0x0d,0x47,
0x02,0x9c,0xf5,0x15,0x56,0xba,0xfb,0xbc,0x91,0x81,0x9d,0xda,0x50,0x03,0x32,0xf7,
0xce,0x67,0x77,0x56,0x5c,0x34,0xce,0x56,0xed,0x26,0xbf,0xe9,0x08,0x30,0x29,0x1c,
0x46,0x08,0x91,0xdd,0xfe,0xbf,0x4f,0xff,0xe5,0xf2,0x17,0x60,0x20,0x81,0x21,0x1e,
0x73,0xee,0xc9,0x76,0x12,0xac,0x2c,0x9f,0xe8,0x98,0x87,0x38,0xf0,0xdc,0x0f,0xd2,
0x24,0x1b,0x3c,0x54,0x35,0x71,0x09,0xd8,0x2f,0xaa,0xb5,0x32,0xdb,0x9c,0x18,0xb5,
0x3b,0x54,0x3d,0x43,0xb5,0x33,0xfd,0x67,0xe3,0x65,0x47,0x97,0x26,0x74,0x96,0x8e,
0xb8,0x3a,0xee,0x7e,0xfc,0x9f,0x2a,0xaf,0x2c,0x61,0x0c,0x45,0x2e,0xea,0x89,0xa4,
0xff,0x50,0x89,0xba,0x69,0x22,0xf3,0x51,0x28,0xa8,0x44,0xf6,0x35,0x0a,0x70,0x67,
0x1d,0xd5,0x3c,0x20,0x5a,0xc7,0xa7,0x5a,0x16,0x59,0xcd,0x3e,0x57,0xd3,0x85,0xab,
0x0b,0xd7,0x8a,0xf1,0x4d,0xc1,0x29,0xb1,0x17,0x28,0x84,0xf1,0xfc,0xf5,0xd7,0x95,
0xb6,0x03,0x1a,0x2b,0xeb,0x24,0x10,0x94,0x8a,0x42,0x2d,0x21,0xad,0x3a,0x21,0x5c,
0x85,0x72,0x0f,0x5e,0xe6,0x95,0xe2,0x35,0xe3,0x68,0x32,0xe0,0xa5,0x59,0xda,0xcd,
0xbd,0x2b,0x7c,0xa3,0xa1,0x31,0x7c,0xa0,0x05,0xa9,0x63,0xb4,0x9e,0x33,0x37,0x82,
0x9c,0x40,0x97,0x4c,0x70,0xf4,0x75,0x40,0xd8,0x9a,0xae,0x62,0x5a,0x1f,0xd1,0x10,
0x87,0x4d,0xc1,0x3d,0x6e,0x7b,0x57,0x11,0x02,0x1f,0x0c,0xfd,0x94,0xc4,0x6d,0x87,
0x58,0x2e,0xbf,0xf7,0x8e,0xb2,0x41,0x2b,0x6d,0x91,0xdc,0x02,0x50,0xa0,0xb8,0x05,
0xf5,0xe3,0xb0,0x96,0xac,0xb2,0x47,0xc2,0xa2,0x47,0xa3,0x91,0x64,0x33,0x7f,0xb3,
0xcb,0x81,0x8a,0xd4,0xa9,0x50,0xe6,0x34,0x7b,0x80,0x1d,0xa7,0x42,0xe5,0xb5,0x32,
0xf4,0xfc,0x48,0x81,0x5a,0xa6,0x2b,0x89,0x4f,0xdb,0x23,0xb6,0xe4,0xd4,0x48,0xdb,
0x62,0x71,0x3a,0xa4,0x6c,0xc3,0x3c,0x7d,0x83,0xd6,0x0a,0x44,0xb2,0xb7,0x0f,0xe8,
0xd8,0x41,0xa5,0x6a,0x41,0x50,0xa0,0x1c,0x82,0xb1,0x9c,0xb9,0x3a,0xe0,0xaa,0xa6,
0x5e,0x76,0x5e,0xff,0x95,0x93,0x3a,0xc5,0x57,0x42,0xe0,0x24,0x07,0x44,0x15,0x8b,
0x9c,0xef,0x9d,0xa8,0xe1,0x8c,0x37,0x36,0xbc,0x88,0x94,0x30,0x6b,0xf8,0x6d,0x9b,
0x92,0x7c,0xd6,0x22,0x52,0xab,0xfd,0x39,0xa3,0xb3,0x52,0x16,0x83,0x36,0x40,0x4c,
0x6b,0x5f,0x0d,0xb4,0xae,0xe5,0x18,0x06,0x92,0x74,0x26,0x6e,0x0c,0x42,0x4f,0x45,
0xd4,0x87,0xb5,0x00,0xbc,0x08,0x7f,0x49,0xb5,0x3b,0x0f,0xa8,0x22,0xd3,0x06,0xd8,
0xb2,0x72,0xe6,0x6c,0x03,0x55,0x69,0xff,0xf1,0xa1,0x91,0xd3,0x54,0x72,0x82,0x53,
0x41,0x9e,0x5b,0x49,0x11,0x5b,0x88,0xeb,0x94,0xe8,0x45,0x2a,0x28,0xa3,0xd6,0x99,
0xe7,0x37,0x09,0xff,0xd9,0x5d,0x30,0x43,0x0d,0x45,0xb7,0x30,0xaf,0x36,0xd9,0x0c,
0x4d,0xf4,0x64,0x24,0x1c,0x05,0x93,0x75,0xe8,0x21,0x5e,0x02,0x29,0x4d,0xfe,0xaf,
0x81,0x69,0xbf,0x1b,0x47,0xf7,0xee,0x6f,0x64,0x79,0xcb,0x77,0x2d,0x55,0x2d,0x60,
0x45,0x89,0xce,0x0a,0x80,0xbf,0x7c,0x38,0xaf,0xb2,0xd8,0x11,0x1d,0x02,0xa1,0x1d,
0x02,0x26,0x4e,0x2d,0x16,0x34,0x12,0x8e,0x53,0x83,0x5e,0x58,0x15,0xa9,0x2c,0x19,
0x02,0x7d,0xd0,0x84,0x93,0xc2,0x33,0x44,0x88,0x3f,0xa5,0xf2,0x80,0x2a,0x67,0xe5,
0x86,0x22,0x1e,0x2e,0x22,0xdf,0xb8,0xc6,0x7d,0x75,0x90,0x3f,0x72,0x11,0xd5,0x2d,
0x19,0xdc,0x92,0xda,0xc9,0xc2,0xe5,0x5d,0x5d,0x23,0xe4,0xb3,0x16,0xf4,0xf3,0x6a,
0xe2,0x45,0x26,0xc0,0xd7,0xcb,0x5d,0x43,0xa5,0xfa,0x64,0xcd,0x62,0x1e,0x9b,0xbe,
0xb4,0x4a,0xda,0xc0,0xfb,0x50,0x23,0x78,0x72,0xcf,0x13,0xe0,0x00,0x8d,0x4d,0x8f,
0x3b,0x53,0xb7,0xf9,0x7f,0xf8,0xfd,0x71,0xe7,0xb6,0xec,0xce,0x35,0x9f,0x5f,0xbf,
0x3f,0x61,0x28,0xdf,0xd0,0x62,0xc1,0xe1,0x17,0x7c,0x6d,0xdb,0x97,0xcb,0xb9,0xed,
0x8a,0xf0,0x82,0xbf,0x55,0xd2,0xdd,0xf7,0x69,0x20,0x02,0x0a,0x8a,0xfc,0xa0,0x82,
0x35,0x75,0x2d,0xe7,0x54,0x22,0x37,0xd2,0x73,0xf0,0x32,0x47,0x67,0xf2,0x97,0xbd,
0x98,0xf5,0x73,0x64,0x2b,0x54,0xd5,0x5a,0xae,0xdc,0xd9,0xa5,0xdb,0x65,0xdd,0xf2,
0x4f,0x1e,0x52,0x48,0x9e,0x22,0xa8,0xcd,0xb2,0x04,0x65,0x3f,0x03,0xa4,0x52,0x61,
0xe2,0x0c,0x96,0x57,0x72,0xcc,0x92,0xd7,0x1a,0xd8,0xa9,0xdc,0xa1,0x20,0x5c,0xa7,
0x2c,0x3a,0xe3,0x30,0xca,0x9a,0xbf,0xc9,0x3b,0xd4,0xc0,0x59,0x12,0x79,0x66,0x17,
0x36,0xb1,0x31,0xae,0x90,0x5d,0x5e,0xb8,0x99,0xbe,0xd4,0x56,0xfe,0x76,0xbe,0x26,
0xc4,0x70,0x0f,0xdf,0x32,0x38,0x99,0x9b,0x25,0x0d,0xef,0x64,0xf1,0x55,0xfe,0xd7,
0x24,0xd3,0xe3,0x8b,0xdc,0xaa,0xd3,0x1c,0xac,0x11,0x2b,0xf3,0x9f,0x0b,0xc0,0xcd,
0x3a,0x90,0x6b,0xea,0x43,0x67,0x6e,0x12,0x66,0x16,0xb5,0x15,0x46,0xc9,0xe2,0x82,
0x11,0x71,0x63,0xea,0x7b,0x38,0xf2,0x55,0x79,0x7a,0x80,0x3f,0x66,0x3e,0xa1,0x64,
0x85,0xfe,0x4a,0x8c,0x25,0x10,0x40,0x9d,0x3b,0x6e,0xf8,0x09,0xea,0xc2,0xbf,0x89,
0x72,0x77,0xa1,0x7a,0x9c,0x6e,0x58,0x70,0x9b,0x57,0x59,0x1f,0xda,0x62,0x32,0x1c,
0xec,0x2f,0x54,0x5f,0xcd,0xb0,0xae,0xea,0xf3,0x3e,0xd3,0x78,0x70,0x7c,0x81,0x71,
0x1e,0x00,0x3f,0x1e,0x77,0xee,0xee,0x29,0x2f,0x61,0xe3,0x7f,0xdc,0x2f,0xc4,0xf9,
0x7c,0xea,0x96,0xe1,0xa4,0xfb,0x86,0x43,0xa9,0xf1,0x8c,0x9d,0x67,0x80,0xbc,0x5b,
0x81,0x5e,0x43,0xaf,0x81,0xe3,0xb1,0xd4,0x7d,0x68,0xd5,0xa5,0x59,0xc1,0x94,0xf3,
0x5a,0x14,0x3b,0x9e,0xae,0x14,0x7d,0x97,0x5b,0xef,0x88,0xd5,0x5a,0xf1,0x40,0x8b,
0xc1,0x70,0x5c,0x02,0xf1,0x12,0x61,0x6d,0x08,0xbf,0xe6,0xa4,0x4c,0x0e,0xfa,0xc3,
0x73,0xcb,0xb5,0x57,0xe4,0x95,0x3c,0x79,0xac,0xaa,0x22,0x53,0x79,0x32,0x0a,0x3b,
0x27
};
static u8 modulus_rsa[] = {
0x00,0xf7,0x23,0xc1,0x1e,0x8f,0x9b,0x2d,0x23,0x81,0xdc,0xc2,0xe5,0xb4,0x26,0x1e,
0xda,0xbb,0x5b,0x04,0x64,0xaf,0x12,0x7f,0x1f,0x8c,0xbe,0x75,0xe0,0x12,0xb7,0xdb,
0x99,0x0a,0x02,0xe0,0xe5,0xb2,0x3e,0xde,0x69,0xb7,0x73,0x19,0x14,0x9b,0x4c,0xca,
0x72,0x66,0xf0,0xcf,0x89,0x7c,0xfd,0xcf,0xd5,0x3f,0x12,0x74,0x2a,0xcd,0x52,0x64,
0x4b,0x0f,0x73,0x6c,0xe8,0x8d,0xc0,0x35,0x70,0xbe,0xdd,0x21,0x3d,0x63,0x7d,0x5c,
0x57,0xa1,0xb1,0x16,0xb7,0x9f,0x22,0xd9,0xa0,0x69,0xe0,0xcd,0x98,0x33,0xdf,0x1d,
0xa9,0x0f,0x37,0x2b,0x2a,0x64,0xa8,0xf2,0x40,0x3e,0x1b,0x14,0x28,0x64,0x91,0xd4,
0x5f,0xb3,0x95,0x03,0xc2,0x3a,0x7d,0xb3,0x93,0xf3,0xf4,0x5b,0x9b,0xf4,0x01,0x11,
0x4c,0xda,0xa3,0xfc,0xb5,0x96,0x6d,0x0c,0x1e,0x6f,0x71,0x43,0x63,0xd1,0xd6,0xb9,
0x2b,0x3e,0x49,0xb6,0x4e,0x7f,0x80,0x28,0x3b,0x50,0x0e,0x30,0xdd,0x0c,0x33,0x3d,
0xc8,0x26,0x29,0x62,0xac,0x04,0x20,0x82,0x3c,0x1c,0x76,0x7c,0xc6,0xd1,0x0f,0x17,
0x4b,0x2e,0xf3,0xa7,0x61,0x6e,0x91,0xd5,0x95,0x4b,0x4b,0x27,0xd0,0xb2,0xa0,0x68,
0x9a,0x98,0xf1,0xe6,0x12,0x4d,0x49,0xba,0x76,0x5a,0x02,0x8f,0x00,0x06,0x8c,0xb4,
0x06,0x5c,0x13,0x75,0x16,0x98,0x39,0x02,0x97,0xde,0xf8,0x91,0x04,0xd2,0x9e,0x2b,
0x66,0x1f,0xcd,0x6b,0x74,0xe8,0xf6,0x30,0x49,0x0c,0x70,0x76,0x97,0xe5,0xaf,0xbc,
0xec,0x92,0x58,0xa5,0xde,0x34,0x82,0xb5,0x3d,0xc4,0x53,0x21,0x65,0x5b,0x8e,0xe5,
0xa7
};
void sshstr2bigint(struct str* src, struct str* dst)
{
	int j,len;
	u8* tmp;
	u8* buf;

	buf = (u8*)src;
	len = (buf[0]<<24)+(buf[1]<<16)+(buf[2]<<8)+buf[3];

	buf = src->buf;
	tmp = dst->buf;
	for(j=0;j<len;j++)tmp[len-1-j] = buf[j];
	dst->len = len;

	bigint_print(dst);
}




static u8 version[]="SSH-2.0-finalanswer_42\r\n";
int secureshell_read_head(u8* buf, int len)
{
	return 0;
}
int secureshell_write_head(u8* buf, int len)
{
	int j,temp;

	//padding
	if((len&7) < 4)temp = 8-(len&7);
	else temp = 16-(len&7);
	for(j=0;j<temp;j++)
	{
		buf[len+j] = 0;
	}
	len += temp;

	//head
	buf[0] = 0;
	buf[1] = 0;
	buf[2] = ((len-4)>>8)&0xff;
	buf[3] = (len-4)&0xff;
	buf[4] = temp;

	return len;
}




static void printalgorithm(u8* buf, int len)
{
	int j,k;
	u8 temp[64];

	j=4;
	k=0;
	while(1)
	{
		if( (buf[j] != ',') && (buf[j] != 0) )
		{
			temp[k] = buf[j];

			k++;
			if(k>60)
			{
				logtoall("error@algorithm\n");
				break;
			}
		}
		else
		{
			temp[k]=0;
			logtoall("	%s\n",temp);

			k=0;
		}

		//
		j++;
		if(j>len)break;
	}
}
int secureshell_read_0x14(u8* buf, int len, u8* dst, int cnt)
{
	u32 j, off;
	if(0x14 != buf[5])return 0;

	j = (buf[0]<<24) + (buf[1]<<16) + (buf[2]<<8) + buf[3];
	logtoall(
		"[0,5]SSH_MSG_KEXINIT\n"
		"	total=%x\n"
		"	plen=%x\n"
		"	type=%x\n",
		j, buf[4], buf[5]
	);

	//cookie
	logtoall("[6,15]cookie\n	");
	for(j=0;j<16;j++)logtoall("%02x ", buf[6+j]);
	logtoall("\n");
	off = 0x16;

	//
	j = buf[off+2];
	j = (j<<8) + buf[off+3];
	logtoall("[%x,%x]key_exchange_algorithm\n", off, off+j-1);
	printalgorithm(buf+off, j+4);
	off += 4 + j;

	//
	j = buf[off+2];
	j = (j<<8) + buf[off+3];
	logtoall("[%x,%x]server_host_key_algorithm\n", off, off+j-1);
	printalgorithm(buf+off, j+4);
	off += 4 + j;

	//
	j = buf[off+2];
	j = (j<<8) + buf[off+3];
	logtoall("[%x,%x]enc_alg_c2s\n", off, off+j-1);
	printalgorithm(buf+off, j+4);
	off += 4 + j;

	//
	j = buf[off+2];
	j = (j<<8) + buf[off+3];
	logtoall("[%x,%x]enc_alg_s2c\n", off, off+j-1);
	printalgorithm(buf+off, j+4);
	off += 4 + j;

	//
	j = buf[off+2];
	j = (j<<8) + buf[off+3];
	logtoall("[%x,%x]mac_alg_c2s\n", off, off+j-1);
	printalgorithm(buf+off, j+4);
	off += 4 + j;

	//
	j = buf[off+2];
	j = (j<<8) + buf[off+3];
	logtoall("[%x,%x]mac_alg_s2c\n", off, off+j-1);
	printalgorithm(buf+off, j+4);
	off += 4 + j;

	//
	j = buf[off+2];
	j = (j<<8) + buf[off+3];
	logtoall("[%x,%x]comp_alg_c2s\n", off, off+j-1);
	printalgorithm(buf+off, j+4);
	off += 4 + j;

	//
	j = buf[off+2];
	j = (j<<8) + buf[off+3];
	logtoall("[%x,%x]comp_alg_s2c\n", off, off+j-1);
	printalgorithm(buf+off, j+4);
	off += 4 + j;

	//
	j = buf[off+2];
	j = (j<<8) + buf[off+3];
	if(j <= 0)goto byebye;
	logtoall("[%x,%x]lang_c2s\n", off, off+j-1);
	printalgorithm(buf+off, j+4);
	off += 4 + j;

	//
	j = buf[off+2];
	j = (j<<8) + buf[off+3];
	if(j <= 0)goto byebye;
	logtoall("[%x,%x]lang_s2c\n", off, off+j-1);
	printalgorithm(buf+off, j+4);
	off += 4 + j;

	//
	j = buf[off+2];
	j = (j<<8) + buf[off+3];
	if(j <= 0)goto byebye;
	logtoall("[%x,%x]first_kex\n", off, off+j-1);
	printalgorithm(buf+off, j+4);

byebye:
	return 0x14;
}
int secureshell_write_0x14(u8* buf, int len, u8* dst, int cnt)
{
	int j;
	int off=0;

	//cookie unchanged
	for(j=0;j<16;j++)dst[off+j] = j;
	off = 0x16;

	//key_exchange_algorithm
	j = mysnprintf(dst+off+4, 999,
		"diffie-hellman-group-exchange-sha256,"
		"diffie-hellman-group-exchange-sha1,"
		"diffie-hellman-group14-sha1"
	);
	dst[off+0] = dst[off+1] = 0;
	dst[off+2] = (j>>8)&0xff;
	dst[off+3] = j&0xff;
	off += 4 + j;

	//server_host_key_algorithm
	j = mysnprintf(dst+off+4, 999,
		"ssh-rsa,"
		"rsa-sha2-256,"
		"rsa-sha2-512"
	);
	dst[off+0] = dst[off+1] = 0;
	dst[off+2] = (j>>8)&0xff;
	dst[off+3] = j&0xff;
	off += 4 + j;

	//encryption_algorithms_client_to_server
	j = mysnprintf(dst+off+4, 999,
		"aes128-cbc,"
		"aes256-cbc,"
		"aes128-ctr,"
		"aes256-ctr,"
		"3des-cbc"
	);
	dst[off+0] = dst[off+1] = 0;
	dst[off+2] = (j>>8)&0xff;
	dst[off+3] = j&0xff;
	off += 4 + j;

	//encryption_algorithms_server_to_client
	j = mysnprintf(dst+off+4, 999,
		"aes128-cbc,"
		"aes256-cbc,"
		"aes128-ctr,"
		"aes256-ctr,"
		"3des-cbc"
	);
	dst[off+0] = dst[off+1] = 0;
	dst[off+2] = (j>>8)&0xff;
	dst[off+3] = j&0xff;
	off += 4 + j;

	//mac_algorithms_client_to_server
	j = mysnprintf(dst+off+4, 999,
		"hmac-sha1,"
		"hmac-sha2-256,"
		"hmac-sha2-512"
	);
	dst[off+0] = dst[off+1] = 0;
	dst[off+2] = (j>>8)&0xff;
	dst[off+3] = j&0xff;
	off += 4 + j;

	//mac_algorithms_server_to_client
	j = mysnprintf(dst+off+4, 999,
		"hmac-sha1,"
		"hmac-sha2-256,"
		"hmac-sha2-512"
	);
	dst[off+0] = dst[off+1] = 0;
	dst[off+2] = (j>>8)&0xff;
	dst[off+3] = j&0xff;
	off += 4 + j;

	//compression_algorithms_client_to_server
	j = mysnprintf(dst+off+4, 999, "none");
	dst[off+0] = dst[off+1] = 0;
	dst[off+2] = (j>>8)&0xff;
	dst[off+3] = j&0xff;
	off += 4 + j;

	//compression_algorithms_server_to_client
	j = mysnprintf(dst+off+4, 999, "none");
	dst[off+0] = dst[off+1] = 0;
	dst[off+2] = (j>>8)&0xff;
	dst[off+3] = j&0xff;
	off += 4 + j;

	//other
	for(j=off;j<off+32;j++)dst[j] = 0;
	off += 4+4+5;

	//
	dst[5] = 0x14;
	return secureshell_write_head(dst, off);
}




int secureshell_clientread_handshake(u8* buf, int len, u8* dst, int cnt)
{
	if(ncmp(buf, "SSH-2.0-", 8) == 0)
	{
		return mysnprintf(dst, cnt, version);
	}
	return 0;
}
int secureshell_clientwrite_handshake(u8* dst, int cnt)
{
	return mysnprintf(dst, cnt, version);
}
int secureshell_clientread_handshake0x14(u8* buf, int len, u8* dst, int cnt)
{
	return secureshell_read_0x14(buf, len, dst, cnt);
}
int secureshell_clientwrite_handshake0x14(u8* buf, int len, u8* dst, int cnt)
{
	return secureshell_write_0x14(buf, len, dst, cnt);
}
int secureshell_clientread_handshake0x1f(u8* buf, int len, u8* dst, int cnt)
{
	int j;
	u8 t[0x1000];
	u8* p = buf+6;

	//P
	j = (p[2]<<8)+p[3];
	logtoall("(%04x)dh.P=\n", j);
	sshstr2bigint((void*)p, (void*)t);
	p += 4+j;

	//G
	j = (p[2]<<8)+p[3];
	logtoall("(%04x)dh.G=\n", j);
	sshstr2bigint((void*)p, (void*)t);
	p += 4+j;

	return (buf[0]<<8)+buf[1];
}
int secureshell_clientwrite_handshake0x20(u8* buf, int len, u8* dst, int cnt)
{
	int j, off=6;

	dst[off+0] = 0;
	dst[off+1] = 0;
	dst[off+2] = 0x4;
	dst[off+3] = 0;

	for(j=0;j<0x400;j++)dst[off+4+j] = j%255;

	off += 4+0x400;
	dst[5] = 0x20;
	return secureshell_write_head(dst, off);
}
int secureshell_clientwrite_handshake0x22(u8* buf, int len, u8* dst, int cnt)
{
	int j, off=6;

	//2048
	dst[off + 0] = 0;
	dst[off + 1] = 0;
	dst[off + 2] = 0x8;
	dst[off + 3] = 0;

	//8192 bits
	dst[off + 4] = 0;
	dst[off + 5] = 0;
	dst[off + 6] = 0x20;
	dst[off + 7] = 0;

	//8192 bits
	dst[off + 8] = 0;
	dst[off + 9] = 0;
	dst[off +10] = 0x20;
	dst[off +11] = 0;

	off += 12;
	dst[5] = 0x22;
	return secureshell_write_head(dst, off);
}
int secureshell_clientread_handshake0x21(u8* buf, int len, u8* dst, int cnt)
{
	int j;
	u8 t[0x1000];
	u8* p = buf+6;

	p += 4;

	//ssh-rsa
	j = (p[2]<<8)+p[3];
	p += 4;
	logtoall("(%04x)server.alg=\n", j);
	logtoall("%.*s\n", j, p);
	p += 7;

	//e
	j = (p[2]<<8)+p[3];
	logtoall("(%04x)server.rsa.E=\n", j);
	sshstr2bigint((void*)p, (void*)t);
	p += 4+j;

	//n
	j = (p[2]<<8)+p[3];
	logtoall("(%04x)server.rsa.N=\n", j);
	sshstr2bigint((void*)p, (void*)t);
	p += 4+j;

	//f
	j = (p[2]<<8)+p[3];
	logtoall("(%04x)server.dhval=\n", j);
	sshstr2bigint((void*)p, (void*)t);
	p += 4+j;


	//h
	j = (p[2]<<8)+p[3];
	logtoall("(%04x)signature=\n", j);
	sshstr2bigint((void*)p, (void*)t);
	p += 4+j;

	return 0;
}
int secureshell_clientwrite_handshake0x15(u8* buf, int len, u8* dst, int cnt)
{
	dst[5] = 0x15;
	return secureshell_write_head(dst, 6);
}
int secureshell_clientwrite_encryptdata(u8* buf, int len, u8* dst, int cnt)
{
	dst[0] = 0;
	dst[1] = 0;
	dst[2] = 0;
	dst[3] = 0x20;
	return 4+0x20;
}




int sshclient_read(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int sshclient_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	int ret;
	u8 tmp[0x1000];

	logtoall("@sshclient_write\n");
	printmemory(buf, len);

	if(0 == art->vfmt)
	{
		secureshell_clientread_handshake(buf, len, tmp, 0x1000);

		ret = secureshell_clientwrite_handshake0x14(buf, len, tmp, 0x1000);
		if(ret)give_data_into_peer(art,_src_, stack,sp, 0,0, tmp,ret);
	}
	else if(1 == art->vfmt)
	{
		ret = secureshell_clientread_handshake0x14(buf, len, tmp, 0x1000);

		ret = secureshell_clientwrite_handshake0x22(buf, len, tmp, 0x1000);
		if(ret)give_data_into_peer(art,_src_, stack,sp, 0,0, tmp,ret);
	}
	else if(2 == art->vfmt)
	{
		ret = secureshell_clientread_handshake0x1f(buf, len, tmp, 0x1000);

		ret = secureshell_clientwrite_handshake0x20(buf, len, tmp, 0x1000);
		if(ret)give_data_into_peer(art,_src_, stack,sp, 0,0, tmp,ret);
	}
	else if(3 == art->vfmt)
	{
		ret = secureshell_clientread_handshake0x21(buf, len, tmp, 0x1000);

		//new keys
		ret = secureshell_clientwrite_handshake0x15(buf, len, tmp, 0x1000);
		if(ret)give_data_into_peer(art,_src_, stack,sp, 0,0, tmp,ret);

		//encrypted data
		ret = secureshell_clientwrite_encryptdata(buf, len, tmp, 0x1000);
		if(ret)give_data_into_peer(art,_src_, stack,sp, 0,0, tmp,ret);
	}
	else printmemory(buf, len);

	art->vfmt += 1;
	return 0;
}
int sshclient_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int sshclient_attach(struct halfrel* self, struct halfrel* peer)
{
	int ret;
	u8 buf[0x100];
	logtoall("@sshclient_attach\n");

	ret = secureshell_clientwrite_handshake(buf, 0x100);
	ret = give_data_into_peer(self->pchip,_src_, 0,0, 0,0, buf,ret);
	return 0;
}
int sshclient_delete(_obj* ele)
{
	return 0;
}
int sshclient_create(_obj* ele, u8* url)
{
	ele->vfmt = 0;
	return 0;
}




int secureshell_serverread_handshake0x14(u8* buf, int len, u8* dst, int cnt)
{
	return secureshell_read_0x14(buf, len, dst, cnt);
}
int secureshell_serverwrite_handshake0x14(u8* buf, int len, u8* dst, int cnt)
{
	return secureshell_write_0x14(buf, len, dst, cnt);
}
int secureshell_serverread_handshake0x22(u8* buf, int len, u8* dst, int cnt)
{
	int min,max,prefer;
	u32 j, off;
	if(0x14 != buf[5])return 0;

	j = (buf[0]<<24) + (buf[1]<<16) + (buf[2]<<8) + buf[3];
	logtoall(
		"[0,5]SSH_MSG_KEXINIT\n"
		"	total=%x\n"
		"	plen=%x\n"
		"	type=%x\n",
		j, buf[4], buf[5]
	);

	min = (buf[8]<<8) + buf[9];
	max = (buf[0x10]<<8) + buf[0x11];
	prefer = (buf[0xc]<<8) + buf[0xd];

	logtoall("[6,11]DH GEX\n");
	logtoall("	min=%d\n", min);
	logtoall("	max=%d\n", max);
	logtoall("	prefer=%d\n", prefer);

	return 0;
}
int secureshell_serverwrite_handshake0x1f(u8* buf, int len, u8* dst, int cnt)
{
	int j;
	int Plen;
	int Glen;
	int off=6;

	//P.len, P.val
	for(j=0;j<0x401;j++)dst[off+4+j] = modulus_dh[j];
	Plen = 0x401;

	dst[off+0] = 0;
	dst[off+1] = 0;
	dst[off+2] = (Plen>>8)&0xff;
	dst[off+3] = Plen&0xff;
	off += 4 + Plen;


	//G.len, G.val
	dst[off+4] = 5;
	Glen = 1;

	dst[off+0] = 0;
	dst[off+1] = 0;
	dst[off+2] = (Glen>>8)&0xff;
	dst[off+3] = Glen&0xff;
	off += 4 + Glen;


	//
	//generatePG(Pbuf, Plen, Gbuf, Glen);

	//
	dst[5] = 0x1f;
	return secureshell_write_head(dst, off);
}
int secureshell_serverread_handshake0x20(u8* buf, int len, u8* dst, int cnt)
{
	int j;
	u8 t[0x1000];
	u8* p = buf+6;

	//E
	j = (p[0]<<24)+(p[1]<<16)+(p[2]<<8)+p[3];
	logtoall("(%04x)client.dhval=\n", j);
	sshstr2bigint((void*)p, (void*)t);
	p += j;

	return 0;
}
int secureshell_serverwrite_handshake0x21(u8* buf, int len, u8* dst, int cnt)
{
	int j;
	int off=6;

	//
	dst[off+0] = 0;
	dst[off+1] = 0;
	dst[off+2] = 0x01;
	dst[off+3] = 0x17;
	off += 4;


	//ssh-rsa
	dst[off+0] = 0;
	dst[off+1] = 0;
	dst[off+2] = 0;
	dst[off+3] = 7;
	off += 4;

	off += mysnprintf(dst+off, 1000, "ssh-rsa");

	//exponent
	dst[off+0] = 0;
	dst[off+1] = 0;
	dst[off+2] = 0;
	dst[off+3] = 3;
	off += 4;

	dst[off+4] = 1;
	dst[off+5] = 0;
	dst[off+6] = 1;
	off += 3;

	//modulus
	dst[off+0] = 0;
	dst[off+1] = 0;
	dst[off+2] = 1;
	dst[off+3] = 1;
	off += 4;

	for(j=0;j<0x101;j++)dst[off+j] = modulus_rsa[j];
	off += 0x101;


	//f
	dst[off+0] = 0;
	dst[off+1] = 0;
	dst[off+2] = 4;
	dst[off+3] = 0;
	off += 4;

	for(j=0;j<0x400;j++)dst[off+j] = j%255;
	off += 0x400;

	//h
	dst[off+0] = 0;
	dst[off+1] = 0;
	dst[off+2] = 0x1;
	dst[off+3] = 0xf;
	off += 4;

	for(j=0;j<0x10f;j++)dst[off+j] = j%255;
	off += 0x10f;


	dst[5] = 0x21;
	return secureshell_write_head(dst, off);
}
int secureshell_serverwrite_handshake0x15(u8* buf, int len, u8* dst, int cnt)
{
	int j;
	int off=6;

	//
	dst[5] = 0x15;
	return secureshell_write_head(dst, off);
}
int secureshell_serverwrite_encryptpacket(u8* buf, int len, u8* dst, int cnt)
{
	return 16;
}




int sshserver_read(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int sshserver_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	int ret;
	u8 tmp[0x1000];

	logtoall("@sshserver_write\n");
	printmemory(buf, len);

	if(0 == art->vfmt)
	{
		ret = secureshell_serverread_handshake0x14(buf, len, tmp, 0x100);

		//key exchange init
		ret = secureshell_serverwrite_handshake0x14(buf, len, tmp, 0x1000);
		if(ret)give_data_into_peer(art,_src_, stack,sp, 0,0, tmp,ret);
	}
	else if(1 == art->vfmt)
	{
		ret = secureshell_serverread_handshake0x22(buf, len, tmp, 0x100);

		//group exchange group
		ret = secureshell_serverwrite_handshake0x1f(buf, len, tmp, 0x1000);
		if(ret)give_data_into_peer(art,_src_, stack,sp, 0,0, tmp,ret);
	}
	else if(2 == art->vfmt)
	{
		ret = secureshell_serverread_handshake0x20(buf, len, tmp, 0x100);

		//group exchange reply, new keys, encrypted packet
		ret = secureshell_serverwrite_handshake0x21(buf, len, tmp, 0x1000);
		ret += secureshell_serverwrite_handshake0x15(buf, len, tmp+ret, 0x1000-ret);
		//ret += secureshell_serverwrite_encryptpacket(buf, len, tmp+ret, 0x1000-ret);
		if(ret)give_data_into_peer(art,_src_, stack,sp, 0,0, tmp,ret);
	}
	else printmemory(buf, len);

	art->vfmt += 1;
	return 0;
}
int sshserver_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int sshserver_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int sshserver_delete(_obj* ele)
{
	return 0;
}
int sshserver_create(_obj* ele, u8* url)
{
	ele->vfmt = 0;
	return 0;
}




int secureshell_serverread_handshake(u8* buf, int len, u8* dst, int cnt)
{
	if(ncmp(buf, "SSH-2.0-", 8) == 0)
	{
		return mysnprintf(dst, cnt, version);
	}
	return 0;
}
int secureshell_serverwrite_handshake(u8* buf, int len, u8* dst, int cnt)
{
	return 0;
}
int sshmaster_read(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	return 0;
}
int sshmaster_write(_obj* art,void* foot, _syn* stack,int sp, void* arg, int idx, u8* buf, int len)
{
	int ret;
	u8 tmp[0x100];

	logtoall("@sshmaster_write\n");
	printmemory(buf, len);

	//check if it's ssh
	ret = secureshell_serverread_handshake(buf, len, tmp, 0x100);
	if(0 == ret){
		//give_data_into_peer(art, _src_, 0, 0, "only ssh!\n", 9);
		//close(obj->tempobj);
		return 0;
	}

	//send back
	give_data_into_peer(art,_src_, stack,sp, 0,0, tmp,ret);

	//link temp to Ssh
	_obj* obj = stack[sp-2].pchip;
	if(0 == obj)return 0;
	_obj* child = obj->sockinfo.child;
	if(0 == child)return 0;

	art = artery_alloc_fromtype(_Ssh_);
	artery_create(art, 0, 0, 0);
	relationcreate(art, 0, _art_, _src_, child, 0, _sys_, _dst_);
	return 0;
}
int sshmaster_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int sshmaster_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int sshmaster_delete(_obj* ele)
{
	return 0;
}
int sshmaster_create(_obj* ele, u8* url)
{
	return 0;
}
