#include "libsoft.h"

struct wakeonlan
{
	u8 dstmac[6];
	u8 srcmac[6];
	u8 type_0842;

	u8 allff[6];
	u8 target[16][6];
	u8 password[4];
};
