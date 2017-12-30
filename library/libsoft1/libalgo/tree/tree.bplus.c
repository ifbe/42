#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void printmemory(void*, int);
void say(void*, ...);




struct indexdata
{
	u64 hash;
	u64 buf;
};
struct leafdata
{
	u64 hash;
	u64 buf;
	u64 irel;
	u64 orel;
};
struct bplushead
{
	u64 left:56;
	u8 type;
	u64 right:56;
	u8 len;
	u64 parent:56;
	u8 flag;
	u64 child:56;
	u8 haha;
};
struct bplusindex
{
	struct bplushead head;
	struct leafdata node[6];
};
struct bplusleaf
{
	struct bplushead head;
	struct leafdata node[3];
};




void* bplus_grow(struct bplusleaf* info)
{
	int j;
	if(info == 0)return 0;

	j = info->head.child;
	info->head.child += sizeof(struct bplusleaf);
say("grow:%x\n",j);
	return (void*)info + j;
}
void* bplus_recycle(struct bplusleaf* info)
{
	if(info == 0)return 0;
}
void* bplus_getleft(struct bplusleaf* info, struct bplusleaf* this)
{
	if(info == 0)return 0;
	if(this == 0)return 0;
	if(this->head.left == 0)return 0;
	return (void*)info + (this->head.left);
}
void* bplus_getright(struct bplusleaf* info, struct bplusleaf* this)
{
	if(info == 0)return 0;
	if(this == 0)return 0;
	if(this->head.right == 0)return 0;
	return (void*)info + (this->head.right);
}
void* bplus_getparent(struct bplusleaf* info, struct bplusleaf* this)
{
	if(info == 0)return 0;
	if(this == 0)return 0;
	if(this->head.parent == 0)return 0;
	return (void*)info + (this->head.parent);
}
void* bplus_getchild(struct bplusleaf* info, struct bplusleaf* this, int j)
{
	if(info == 0)return 0;
	if(this == 0)return 0;
	if('!' == this->head.type)return 0;

	if( (j < 0) | (j >= this->head.len) )
	{
		if(this->head.child == 0)return 0;
		return (void*)info + (this->head.child);
	}
	else
	{
		if(this->node[j].buf == 0)return 0;
		return (void*)info + (this->node[j].buf);
	}
}




void* bplus_search(struct bplusleaf* info, struct bplusleaf* this, u64 hash)
{
	int j,k;
	struct bplusleaf* child;
	if(info == 0)return 0;
	if(this->head.type == '!')return this;

	k = this->head.len;
	for(j=k-1;j>=0;j--)
	{
		if(hash >= (this->node[j].hash))break;
	}

	child = bplus_getchild(info, this, j);
	if(child == 0)return 0;

	say("j=%d,child=%llx\n",j,child);
	return bplus_search(info, child, hash);
}
void bplus_nodecopy(struct leafdata* a, struct leafdata* b)
{
	int j;
	u8* src;
	u8* dst;
	if(a == 0)return;
	if(b == 0)return;
	if(a == b)return;

	src = (void*)a;
	dst = (void*)b;
	for(j=0;j<sizeof(struct leafdata);j++)
	{
		dst[j] = src[j];
	}
}
void bplus_noderepair(struct bplusleaf* info, struct bplusleaf* this)
{
	int j;
	struct bplusleaf* child;
	if(info == 0)return;
	if(this == 0)return;

	for(j=0;j<this->head.len;j++)
	{
		child = bplus_getchild(info, this, j);
		if(child == 0)break;

		child->head.parent = (void*)this - (void*)info;
	}
}




void* bplus_nodeadd(struct bplusleaf* this, struct leafdata* data)
{
	int j,k;
	u64 hash;

	//vars
	k = this->head.len;
	hash = data->hash;

	//where
	for(j=0;j<k;j++)
	{
		if(hash < this->node[j].hash)break;
	}

	//move
	for(;k>j;k--)
	{
		bplus_nodecopy(&this->node[k-1], &this->node[k]);
	}

	//put
	bplus_nodecopy(data, &this->node[j]);
	this->head.len += 1;
}
void* bplus_splitby(struct bplusleaf* info,
	struct bplusleaf* this, struct leafdata* data)
{
	int j,k;
	u64 hash;
	struct bplusleaf* right;
	struct bplusleaf* parent;
	struct leafdata haha;

	right = bplus_grow(info);
	right->head.left = (void*)this - (void*)info;
	right->head.right = this->head.right;
	right->head.type = this->head.type;
	right->head.len = 2;

	this->head.len = 2;
	this->head.right = (void*)right - (void*)info;

	hash = data->hash;
	for(j=0;j<3;j++)
	{
		if(hash < this->node[j].hash)break;
	}

	if(j == 0)
	{
		bplus_nodecopy(&this->node[1], &right->node[0]);
		bplus_nodecopy(&this->node[2], &right->node[1]);

		bplus_nodecopy(&this->node[0], &this->node[1]);
		bplus_nodecopy(data, &this->node[0]);
	}
	else if(j == 1)
	{
		bplus_nodecopy(&this->node[1], &right->node[0]);
		bplus_nodecopy(&this->node[2], &right->node[1]);

		bplus_nodecopy(data, &this->node[1]);
	}
	else if(j == 2)
	{
		bplus_nodecopy(data, &right->node[0]);
		bplus_nodecopy(&this->node[2], &right->node[1]);
	}
	else if(j == 3)
	{
		bplus_nodecopy(&this->node[2], &right->node[0]);
		bplus_nodecopy(data, &right->node[1]);
	}

	//非叶节点分裂时，要进入儿子修改它的parent
	if(right->head.type == '?')bplus_noderepair(info, right);

	parent = bplus_getparent(info, this);
	if(parent == 0)
	{
		parent = bplus_grow(info);

		parent->head.type = '?';
		parent->head.len = 1;

		parent->head.left = 0;
		parent->head.right = 0;
		parent->head.parent = 0;
		parent->head.child = (void*)this - (void*)info;

		parent->node[0].hash = right->node[0].hash;
		parent->node[0].buf = (void*)right - (void*)info;

		right->head.parent = this->head.parent;
		this->head.parent = (void*)parent - (void*)info;
		info->head.right = this->head.parent;
	}
	else
	{
		right->head.parent = this->head.parent;

		haha.hash = right->node[0].hash;
		haha.buf = (void*)right - (void*)info;
		haha.irel = 0;
		haha.orel = 0;

		if(parent->head.len < 3)bplus_nodeadd(parent, &haha);
		else bplus_splitby(info, parent, &haha);
	}
}
void* bplus_insert(struct bplusleaf* info, u64 hash)
{
	struct bplusleaf* this;
	struct bplusleaf* right;
	struct bplusleaf* parent;
	struct leafdata data;
	if(info == 0)return 0;

	this = bplus_getright(info, info);
	if(this == 0)
	{
		//empry
		this = bplus_grow(info);
		info->head.right = (void*)this - (void*)info;

		//infomation
		this->head.type = '!';	//leaf
		this->head.len = 0;
		this->head.left = 0;
		this->head.right = 0;
		this->head.parent = 0;
		this->head.child = 0;

		//first
		data.hash = hash;
		data.buf = 0;
		data.irel = 0;
		data.orel = 0;
		bplus_nodeadd(this, &data);
		return this;
	}

	//find leaf
	this = bplus_search(info, this, hash);
	if(this == 0)return 0;

	//normal insert
	if(this->head.len < 3)
	{
		data.hash = hash;
		data.buf = 0;
		data.irel = 0;
		data.orel = 0;
		bplus_nodeadd(this, &data);
		return this;
	}

	//split insert
	data.hash = hash;
	data.buf = 0;
	data.irel = 0;
	data.orel = 0;
	return bplus_splitby(info, this, &data);
}
void* bplus_destory(struct bplusleaf* info, u64 hash)
{
	if(info == 0)return 0;
}




void bplus_prepare(struct bplusleaf* info, int len)
{
	int j;
	char* buf;
	if(info == 0)return;

	buf = (void*)info;
	for(j=0;j<len;j++)buf[j] = 0;

	info->head.type = '@';
	info->head.len = 0;
	info->head.left = 0;
	info->head.right = 0;
	info->head.parent = len;
	info->head.child = sizeof(struct bplusleaf);
}
void bplus_stop(struct bplusleaf* info)
{
}
