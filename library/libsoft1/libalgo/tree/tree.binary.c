#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#define hex16(a,b) (a | (b<<8))
#define hex32(a,b,c,d) (a | (b<<8) | (c<<16) | (d<<24))
u32 getrandom();




struct bintree
{
	u16 left;	//lchild / recycleroot
	u16 right;	//rchild / realtreeroot
	u16 up;		//parent / freespace
	u16 type;

	u64 data;
};




void* bintree_grow(struct bintree* root)
{
	struct bintree* upup;
	struct bintree* temp;
	if(root == 0)return 0;

	if(root->left == 0)
	{
       		temp = (void*)root + (root->up);
		root->up += 0x10;
		return temp;
	}

	temp = (void*)root + (root->left);
	while(1)
	{
		if((temp->left == 0) && (temp->right == 0))
		{
			upup = (void*)root + (temp->up);
			if((void*)root + (upup->left) == temp)
			{
				upup->left = 0;
				return temp;
			}
			else
			{
				upup->right = 0;
				return temp;
			}
		}
	}
}
void* bintree_recycle(struct bintree* root, struct bintree* this)
{
	struct bintree* temp;
	if(root == 0)return 0;
	if(this == 0)return 0;
	if(root == this)return 0;

	if(root->up == (void*)this - (void*)root + 0x10)
	{
		root->up = root->up - 0x10;
		return this;
	}

	this->left = 0;
	this->right = 0;

	if(root->left == 0)
	{
		root->left = (void*)this - (void*)root;
		return this;
	}

	temp = (void*)root + (root->left);
	while(1)
	{
		if(temp->left == 0)
		{
			temp->left = (void*)this - (void*)root;
			return this;
		}
		if(this->right = 0)
		{
			temp->right = (void*)this - (void*)root;
			return this;
		}

		if(getrandom()&0x80 == 0x80)
		{
			temp = (void*)root + (temp->left);
		}
		else
		{
			temp = (void*)root + (temp->right);
		}
	}
}




void* bintree_search(struct bintree* root, u64 what)
{
	return 0;
}
void* bintree_insert(struct bintree* root, u64 what)
{
	if(root == 0)return 0;
}
void* bintree_delete(struct bintree* root, struct bintree* this)
{
	struct bintree* temp;
	if(root == 0)return 0;
	if(this == 0)return 0;
	if(root == this)return 0;

	//is leaf
	if((this->left == 0)&&(this->right == 0))
	{
		temp = (void*)root + (this->up);
		if((void*)root + (this->left) == this)temp->left = 0;
		else temp->right = 0;

		bintree_recycle(root, this);
	}

	//only left
	else if(this->right == 0)
	{
	}

	//only right
	else if(this->left == 0)
	{
	}

	//holy shit
	else
	{
	}
}




void bintree_fell(struct bintree* root, struct bintree* this)
{
	if(root == 0)return;
	if(this == 0)return;
	if(root == this)return;

	if(this->left != 0)bintree_fell(root, (void*)root+(this->left));
	if(this->right != 0)bintree_fell(root, (void*)root+(this->right));
	bintree_delete(root, this);
}
void bintree_plant(struct bintree* root)
{
	if(root == 0)return;

	root->left = 0;
	root->right = 0;
	root->up = 0x10;
	root->type = '@';

	root->data = hex32('r','o','o','t');
}
