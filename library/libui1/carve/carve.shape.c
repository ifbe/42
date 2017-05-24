void carvecubie(				//方块
	float* buf,
	float x, float y, float z,
	float w, float h, float d)
{
	//0:	x-, y-, z-
	buf[0] = x-(w/2);
	buf[1] = y-(h/2);
	buf[2] = z-(d/2);

	//1:	x+, y-, z-
	buf[3] = x+(w/2);
	buf[4] = y-(h/2);
	buf[5] = z-(d/2);

	//2:	x-, y+, z-
	buf[6] = x-(w/2);
	buf[7] = y+(h/2);
	buf[8] = z-(d/2);

	//3:	x+, y+, z-
	buf[9] = x+(w/2);
	buf[10] = y+(h/2);
	buf[11] = z-(d/2);

	//4:	x-, y-, z+
	buf[12] = x-(w/2);
	buf[13] = y-(h/2);
	buf[14] = z+(d/2);

	//5:	x+, y-, z+
	buf[15] = x+(w/2);
	buf[16] = y-(h/2);
	buf[17] = z+(d/2);

	//6:	x-, y+, z+
	buf[18] = x-(w/2);
	buf[19] = y+(h/2);
	buf[20] = z+(d/2);

	//7:	x+, y+, z+
	buf[21] = x+(w/2);
	buf[22] = y+(h/2);
	buf[23] = z+(d/2);
}




void carvecylinder(					//圆柱体
	float* buf,
	float x, float y, float z,		//底面圆心
	float r, float h)
{
	
}




void carvecone(						//圆锥
	float* buf,
	float x, float y, float z,		//底盘圆心
	float r, float h)
{
}




void carvesphere(					//球体
	float* buf,
	float x, float y, float z,		//球心
	float r)
{
	
}