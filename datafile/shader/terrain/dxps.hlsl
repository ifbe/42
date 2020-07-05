struct PSin{
	float4 where : SV_POSITION;
	float4 color : COLOR;
};
float4 main(PSin input) : SV_TARGET{
	return input.color;
}