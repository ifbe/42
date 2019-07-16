#include "libuser.h"
void movsb(u8* rdi, u8* rsi, int rcx);




void bigint_print(struct str* src)
{
	int j;
	int len = src->len;
	u8* buf = src->buf;

	j = len-1;
	if(buf[j] < 0x80)
	{
		say("0x");
		while((j>0)&&(0==buf[j]))j--;
		for(;j>=0;j--)say("%02x", buf[j]);
		say("\n");
		return;
	}

	say("-0x");
	for(j=len-1;j>0;j--)say("%02x", 0xff-buf[j]);
	say("%02x", 0x100-buf[0]);
	say("\n");
}




//copy src into dst
int bigdup(struct str* src, struct str* dst)
{
	int j,len = src->len;
	u8* srcbuf = src->buf;
	u8* dstbuf = dst->buf;

	for(j=0;j<len;j++)dstbuf[j] = srcbuf[j];
	dst->len = len;
	return len;
}
int bigcmp(			//(a-b)
	u8* abuf, int alen,
	u8* bbuf, int blen)
{
	while( (alen>1) && (abuf[alen-1]==0) )alen--;
	while( (blen>1) && (bbuf[blen-1]==0) )blen--;

	if(alen < blen)return -1;
	else if(alen > blen)
	{
		//printf("%d>%d\n",alen,blen);
		return 1;
	}
	while(1)	//alen == blen
	{
		if(abuf[alen-1] < bbuf[blen-1])return -1;
		if(abuf[alen-1] > bbuf[blen-1])
		{
			//printf("%d>%d\n",abuf[alen-1], bbuf[blen-1]);
			return 1;
		}
		if(alen == 1)return 0;

		//cmp next
		alen--;
		blen--;
	}

	//never
	return 0;
}




int bigshl(u8* buf, int len, int sh)
{
	int j;
	int haha=0;
	for(j=0;j<len;j++)
	{
		haha += (u16)buf[j]<<8;
		buf[j] = ((haha<<sh)>>8) & 0xff;
		haha >>= 8;
	}

	if(haha != 0)
	{
		buf[len] = haha;
		len++;
	}
	return len;
}
int bigshr(u8* buf, int len, int sh)
{
	int j;
	u16 haha=0;
//printbigint(buf,len);
//printf("(before)\n");
	for(j=len-1;j>=0;j--)
	{
		haha |= buf[j];
		buf[j] = (haha>>sh)&0xff;
		haha <<= 8;
	}
	if(buf[len-1] == 0)len--;
//printbigint(buf,len);
//printf("(after)\n");
	return len;
}




int bigadd(				//a += b
	u8* abuf, int alen,
	u8* bbuf, int blen)
{
	int j;
	int temp;
	if(alen<blen)
	{
		for(j=alen;j<blen;j++)abuf[j]=0;
		alen = blen;
	}

	temp = 0;
	for(j=0;j<blen;j++)
	{
		temp += abuf[j] + bbuf[j];
		abuf[j] = temp & 0xff;
		temp >>= 8;
	}
	for(;j<alen;j++)
	{
		if(temp == 0)break;

		temp += abuf[j];
		abuf[j] = temp & 0xff;
		temp >>= 8;
	}
	if(temp != 0)
	{
		abuf[j] = 1;
		j++;
	}
	for(j=alen;j>0;j--)if(abuf[j-1] != 0)break;
	return j;
}
int bigadd_muled(		//a <= a+b*val
	u8* abuf, int alen,
	u8* bbuf, int blen,
	int val)
{
	int j;
	int remain=0;
	for(j=0;j<blen;j++)
	{
		remain += abuf[j] + bbuf[j]*val;
		abuf[j] = remain&0xff;
		remain >>= 8;
	}
	while(remain != 0)
	{
		remain += abuf[j];
		abuf[j] = remain&0xff;
		remain >>= 8;
		j++;
	}
	while( (alen>1) && (abuf[alen-1]==0) )alen--;
	return alen;
}




int bigsub(
	u8* abuf, int alen,
	u8* bbuf, int blen)
{
	int j;
	int temp;
	if(alen<blen)
	{
		for(j=alen;j<blen;j++)abuf[j]=0;
		alen = blen;
	}

	temp = 0;
	for(j=0;j<blen;j++)
	{
		temp += abuf[j] - bbuf[j];
		abuf[j] = temp&0xff;
		temp >>= 8;
	}
	for(;j<alen;j++)
	{
		if(temp == 0)break;

		temp = abuf[j] - temp;
		abuf[j] = temp & 0xff;
		temp >>= 8;
		
	}
	if(temp != 0)
	{
		abuf[j] = 0xff;
		j++;
	}

	for(j=alen;j>0;j--)if(abuf[j-1] != 0)break;
	return j;
}
int bigsub_muled(		//a <= a-b*val
	u8* abuf, int alen,
	u8* bbuf, int blen,
	int val)
{
	int j;
	int remain=0;
	for(j=0;j<blen;j++)
	{
		remain += abuf[j] - bbuf[j]*val;
		abuf[j] = remain&0xff;
		remain >>= 8;
	}
	while(remain != 0)
	{
		remain += abuf[j];
		abuf[j] = remain&0xff;
		remain >>= 8;
		j++;
	}
	while( (alen>1) && (abuf[alen-1]==0) )alen--;
	return alen;
}




int bigmul(			//ans = a * b
	u8* abuf, int alen,
	u8* bbuf, int blen,
	u8* ans, int x)
{
	int j,k;
	int temp,carry;

	x = 0;
	j = k = 0;
	temp = carry = 0;
	while(1)
	{
		temp = carry;
		if( (x<alen) && (x<blen) )
		{
			for(j=0;j<=x;j++)
			{
				temp += abuf[j] * bbuf[x-j];
				//printf("(%d,%d)", j, x-j);
			}
		}
		else if(x<blen)
		{
			for(j=0;j<alen;j++)
			{
				temp += abuf[j] * bbuf[x-j];
				//printf("(%d,%d)", j, x-j);
			}
		}
		else if(x<alen)
		{
			for(j=0;j<blen;j++)
			{
				temp += abuf[x-j] * bbuf[j];
				//printf("(%d,%d)", x-j, j);
			}
		}
		else
		{
			for(j=0;j<alen+blen-1-x;j++)
			{
				temp += abuf[alen-1-j] * bbuf[x-alen+j+1];
				//printf("(%d,%d)", alen-1-j, x-alen+j+1);
			}
		}
		//printf("\n");

		//
		ans[x] = temp & 0xff;
		carry = temp>>8;

		//
		x++;
		if(x >= alen+blen)break;
	}
	return x;
}




int bigmod(				//a << a%b
	u8* abuf, int alen,
	u8* bbuf, int blen)
{
	int j;
	u32 q;
//printf("in\n");
	//清理掉高位0
	while( (alen>1) && (abuf[alen-1]==0) )alen--;
	while( (blen>1) && (bbuf[blen-1]==0) )blen--;
	if(blen == 1)
	{
		if(bbuf[0] == 0)return 0;
		else
		{
			
		}
	}

	//被除数实际位数 <= 除数实际位数
	if(alen < blen)return alen;

	//每次尽量把那最高字节清零，清理不掉的交给下一次来打扫
	q = 0;
	for(j=alen-blen-1;j>=0;j--)
	{
		//printf("%x%02x%02x / %x\n", q, abuf[j+blen], abuf[j+blen-1], bbuf[blen-1]+1);
		q = ( (q<<16) + (abuf[j+blen]<<8) + abuf[j+blen-1] ) / (bbuf[blen-1]+1);
		//printf("%x\n",q);

		bigsub_muled(
			abuf + j, blen,
			bbuf, blen,
			q
		);
		if(q > 0xffff)
		{
			while(abuf[j+blen+1] >0)
			{
				bigsub_muled(
					abuf + j, alen,
					bbuf, blen,
					abuf[j+blen+1]*(256/bbuf[blen-1])
				);
			}
		}

		while( (alen>1) && (abuf[alen-1]==0) )alen--;
		q = abuf[j+blen];
		//printbigint(abuf,alen);
		//printf("\n");
	}

	//现在alen <= blen
	while(1)
	{
		//小于除数直接返回
		j = bigcmp(abuf, alen, bbuf, blen);
		//printf("j=%d\n",j);
		if(j < 0)break;

//printf("1\n");
		//确定要除以几，剪掉之后返回
		if(alen==blen)j = abuf[blen-1] / (bbuf[blen-1]+1);
		else j = ((abuf[blen]<<8) - abuf[blen-1]) / (bbuf[blen-1]+1);
		if(j==0)j=1;
		bigsub_muled(
			abuf, alen,
			bbuf, blen,
			j
		);
//printf("2\n");
		while( (alen>1) && (abuf[alen-1]==0) )alen--;
		//printbigint(abuf,alen);
		//printf("\n");
	}

	while( (alen>1) && (abuf[alen-1]==0) )alen--;
//printf("out\n");
	return alen;
}
int bigpow(			//ans = (base^exp)%mod
	u8* ans, int al,
	u8* base, int bl,
	u8* exp, int el,
	u8* mod, int ml)
{
	int j;
	u8 t1[0x1000];

	//base %= mod
	bl = bigmod(base, bl, mod, ml);

	//answ=1
	for(j=1;j<2*bl;j++)ans[j] = 0;
	ans[0] = 1;
	al = 1;

	//
	while(1)
	{
		//odd num
		if((exp[0]&1) == 1)
		{
			//ans *= base
			movsb(t1, ans, al);
			al = bigmul(
				t1, al,
				base, bl,
				ans, al
			);

			al = bigmod(ans, al, mod, ml);
		}

		//
		el = bigshr(exp, el, 1);
		if( (el <= 1)&&(exp[0] == 0) )break;

		//even num
		//base = base * base
		movsb(t1, base, bl);
		bl = bigmul(
			t1, bl,
			t1, bl,
			base, bl*2
		);

		bl = bigmod(base, bl, mod, ml);
	}

	return al;
}




//	/	%
int bigdiv_keeptry(
	u8* abuf, int alen,
	u8* bbuf, int blen)
{
	int j,k=0;
	while(1)
	{
//printf("%016llx\n",*(u64*)abuf);
		if(alen == blen)
		{
			//还能不能继续了
			for(j=blen-1;j>=0;j--)
			{
				if(abuf[j] < bbuf[j])goto nomore;
				if(abuf[j] > bbuf[j])break;
			}
			if(j<0)
			{
				if(abuf[0] < bbuf[0])goto nomore;
			}

			//还可以减一次
			bigsub(abuf, blen, bbuf, blen);
			k++;
		}
		else
		{
			bigsub(abuf, alen, bbuf, blen);
			k++;

			if(abuf[blen] == 0)
			{
				alen = blen;
				continue;
			}
		}//else

		//error
		if(k>0xff)break;
	}//while

nomore:
	return k;
}
int bigdiv(
	u8* abuf, int alen,
	u8* bbuf, int blen,
	u8* qbuf, int qlen,
	u8* rbuf, int rlen)
{
	int j,ret;

	while( (alen>1) && (abuf[alen-1]==0) )alen--;
	while( (blen>1) && (bbuf[blen-1]==0) )blen--;
	if( (blen == 1) && (bbuf[0] == 0) )return 0;

	//两种情况都要挪动
	for(j=0;j<alen;j++)
	{
		qbuf[j] = 0;
		rbuf[j] = abuf[j];
	}

	//除数比被除数位数多
	if(blen > alen)	return alen;

	//正常开始减
	for(j=alen-blen;j>=0;j--)
	{
		ret = blen;
		if(rbuf[j+blen] != 0)
		{
			//not first
			if(j != alen-blen)ret++;
		}

		qbuf[j] = bigdiv_keeptry(
			rbuf+j, ret,
			bbuf, blen
		);
	}

	for(j=alen-blen;j>=0;j--)
	{
		if(qbuf[j] != 0)break;
	}
	return j+1;
}
