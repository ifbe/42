#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
void printmemory(void*, int);
void say(void*, ...);




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
struct indexdata
{
	u64 hash;
	u64 buf;
};
struct bplusindex
{
	struct bplushead head;
	struct indexdata node[6];
};
struct leafdata
{
	u64 hash;
	u64 buf;
	u64 irel;
	u64 orel;
};
struct bplusleaf
{
	struct bplushead head;
	struct leafdata node[3];
};




void bplus_prepare(struct bplushead* head, int len)
{
	int j;
	char* buf;
	if(head == 0)return;

	buf = (void*)head;
	for(j=0;j<len;j++)buf[j] = 0;

	head->type = '@';
	head->len = 0;
	head->left = 0;
	head->right = 0;
	head->parent = len;
	head->child = sizeof(struct bplusleaf);
}
void bplus_alldone(struct bplushead* head)
{
}
void* bplus_grow(struct bplushead* head)
{
	int j;
	if(head == 0)return 0;

	j = head->child;
	head->child += sizeof(struct bplusleaf);

	return (void*)head + j;
}
void* bplus_recycle(struct bplushead* head, struct bplusleaf* this)
{
	if(head == 0)return 0;
}




void* bplus_getleft(struct bplushead* head, struct bplushead* this)
{
	if(head == 0)return 0;
	if(this == 0)return 0;
	if(this->left == 0)return 0;
	return (void*)head + (this->left);
}
void* bplus_getright(struct bplushead* head, struct bplushead* this)
{
	if(head == 0)return 0;
	if(this == 0)return 0;
	if(this->right == 0)return 0;
	return (void*)head + (this->right);
}
void* bplus_getparent(struct bplushead* head, struct bplushead* this)
{
	if(head == 0)return 0;
	if(this == 0)return 0;
	if(this->parent == 0)return 0;
	return (void*)head + (this->parent);
}
void* bplus_getchild(struct bplushead* head, struct bplusindex* this, int j)
{
	if(head == 0)return 0;
	if(this == 0)return 0;
	if('!' == this->head.type)return 0;

	if( (j < 0) | (j >= this->head.len) )
	{
		if(this->head.child == 0)return 0;
		return (void*)head + (this->head.child);
	}
	else
	{
		if(this->node[j].buf == 0)return 0;
		return (void*)head + (this->node[j].buf);
	}
}




void bplus_indexcopy(struct indexdata* a, struct indexdata* b)
{
	int j;
	u8* src;
	u8* dst;
	if(a == 0)return;
	if(b == 0)return;
	if(a == b)return;

	src = (void*)a;
	dst = (void*)b;
	for(j=0;j<sizeof(struct indexdata);j++)
	{
		dst[j] = src[j];
	}
}
void* bplus_indexadd(struct bplusindex* this, struct indexdata* data)
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
	//say("j=%d,k=%d,hash=%c\n", j, k, hash);

	//move
	for(;k>j;k--)
	{
		bplus_indexcopy(&this->node[k-1], &this->node[k]);
	}

	//put
	bplus_indexcopy(data, &this->node[j]);
	this->head.len += 1;
}
void* bplus_indexsplit(struct bplushead* head,
	struct bplusindex* left, struct indexdata* data)
{
	int j,k;
	u64 hash;
	struct bplusindex* right;
	struct bplusindex* parent;
	struct bplusindex* child;
	struct indexdata haha;

	right = bplus_grow(head);
	right->head.type = '?';
	right->head.len = 1;
	right->head.left = (void*)left - (void*)head;
	right->head.right = left->head.right;

	left->head.len = 2;
	left->head.right = (void*)right - (void*)head;

	hash = data->hash;
	for(j=0;j<3;j++)
	{
		if(hash < left->node[j].hash)break;
	}

	if(j == 0)
	{
		bplus_indexcopy(&left->node[2], &right->node[0]);

		right->head.child = left->node[1].buf;
		haha.hash = left->node[1].hash;

		bplus_indexcopy(&left->node[0], &left->node[1]);
		bplus_indexcopy(data, &left->node[0]);
	}
	else if(j == 1)
	{
		bplus_indexcopy(&left->node[2], &right->node[0]);

		right->head.child = left->node[1].buf;
		haha.hash = left->node[1].hash;

		bplus_indexcopy(data, &left->node[1]);
	}
	else if(j == 2)
	{
		bplus_indexcopy(&left->node[2], &right->node[0]);

		right->head.child = data->buf;
		haha.hash = data->hash;
	}
	else if(j == 3)
	{
		bplus_indexcopy(data, &right->node[0]);

		right->head.child = left->node[2].buf;
		haha.hash = left->node[2].hash;
	}
	haha.buf = (void*)right - (void*)head;

	//非叶节点分裂时，要进入儿子修改它的parent
	if(right->head.type == '?')
	{
		for(j=0;j<right->head.len;j++)
		{
			child = bplus_getchild(head, right, j);
			if(child == 0)break;

			child->head.parent = (void*)right - (void*)head;
		}
	}

	parent = bplus_getparent(head, &left->head);
	if(parent == 0)
	{
		parent = bplus_grow(head);

		parent->head.type = '?';
		parent->head.len = 1;

		parent->head.left = 0;
		parent->head.right = 0;
		parent->head.parent = 0;
		parent->head.child = (void*)left - (void*)head;

		parent->node[0].hash = haha.hash;
		parent->node[0].buf = (void*)right - (void*)head;

		right->head.parent = (void*)parent - (void*)head;
		left->head.parent = right->head.parent;
		head->right = left->head.parent;
	}
	else
	{
		right->head.parent = left->head.parent;

		if(parent->head.len < 3)bplus_indexadd(parent, &haha);
		else bplus_indexsplit(head, parent, &haha);
	}
}




void bplus_leafcopy(struct leafdata* a, struct leafdata* b)
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
void* bplus_leafadd(struct bplusleaf* this, struct leafdata* data)
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
		bplus_leafcopy(&this->node[k-1], &this->node[k]);
	}

	//put
	bplus_leafcopy(data, &this->node[j]);
	this->head.len += 1;
}
void* bplus_leafsplit(struct bplushead* head,
	struct bplusleaf* left, struct leafdata* data)
{
	int j,k;
	u64 hash;
	struct bplusleaf* right;
	struct bplusindex* parent;	//not leaf
	struct indexdata haha;

	right = bplus_grow(head);
	right->head.type = '!';
	right->head.len = 2;
	right->head.left = (void*)left - (void*)head;
	right->head.right = left->head.right;

	left->head.len = 2;
	left->head.right = (void*)right - (void*)head;

	hash = data->hash;
	for(j=0;j<3;j++)
	{
		if(hash < left->node[j].hash)break;
	}

	if(j == 0)
	{
		bplus_leafcopy(&left->node[1], &right->node[0]);
		bplus_leafcopy(&left->node[2], &right->node[1]);

		bplus_leafcopy(&left->node[0], &left->node[1]);
		bplus_leafcopy(data, &left->node[0]);
	}
	else if(j == 1)
	{
		bplus_leafcopy(&left->node[1], &right->node[0]);
		bplus_leafcopy(&left->node[2], &right->node[1]);

		bplus_leafcopy(data, &left->node[1]);
	}
	else if(j == 2)
	{
		bplus_leafcopy(data, &right->node[0]);
		bplus_leafcopy(&left->node[2], &right->node[1]);
	}
	else if(j == 3)
	{
		bplus_leafcopy(&left->node[2], &right->node[0]);
		bplus_leafcopy(data, &right->node[1]);
	}

	parent = bplus_getparent(head, &left->head);
	if(parent == 0)
	{
		parent = bplus_grow(head);

		parent->head.type = '?';
		parent->head.len = 1;

		parent->head.left = 0;
		parent->head.right = 0;
		parent->head.parent = 0;
		parent->head.child = (void*)left - (void*)head;

		parent->node[0].hash = right->node[0].hash;
		parent->node[0].buf = (void*)right - (void*)head;

		right->head.parent = (void*)parent - (void*)head;
		left->head.parent = right->head.parent;
		head->right = left->head.parent;
	}
	else
	{
		right->head.parent = left->head.parent;

		haha.hash = right->node[0].hash;
		haha.buf = (void*)right - (void*)head;

		if(parent->head.len < 3)bplus_indexadd(parent, &haha);
		else bplus_indexsplit(head, parent, &haha);
	}
}




void* bplus_search(struct bplushead* head, struct bplusindex* this, u64 hash)
{
	int j,k;
	struct bplusindex* child;
	if(head == 0)return 0;
	if(this->head.type == '!')return this;

	k = this->head.len;
	for(j=k-1;j>=0;j--)
	{
		if(hash >= (this->node[j].hash))break;
	}

	child = bplus_getchild(head, this, j);
	if(child == 0)return 0;

	//say("j=%d,child=%llx\n",j,child);
	return bplus_search(head, child, hash);
}
void* bplus_change(struct bplushead* head)
{
	return 0;
}
void* bplus_insert(struct bplushead* head, u64 hash)
{
	struct bplusleaf* this;
	struct bplusleaf* right;
	struct bplusleaf* parent;
	struct leafdata data = {hash, 0, 0, 0};
	if(head == 0)return 0;

	//empty?
	this = bplus_getright(head, (void*)head);
	if(this == 0)
	{
		//root
		this = bplus_grow(head);
		head->right = (void*)this - (void*)head;

		//leaf
		this->head.type = '!';
		this->head.len = 0;
		this->head.left = 0;
		this->head.right = 0;
		this->head.parent = 0;
		this->head.child = 0;

		//first
		bplus_leafadd(this, &data);
		return this;
	}

	//find leaf
	this = bplus_search(head, (void*)this, hash);
	if(this == 0)return 0;

	//normal insert
	if(this->head.len < 3)
	{
		bplus_leafadd(this, &data);
		return this;
	}

	//split insert
	return bplus_leafsplit(head, this, &data);
}
void* bplus_destory(struct bplushead* head, u64 hash)
{
	if(head == 0)return 0;
}
