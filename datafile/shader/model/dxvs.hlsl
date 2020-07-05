cbuffer VSConstantBuffer : register(b0){
	matrix w2c;
};
cbuffer VSConstantBuffer : register(b1){
	matrix l2w;
};
struct VSin{
	float3 v : PA;
	float3 c : PB;
};
struct VSout{
	float4 where : SV_POSITION;
	float4 color : COLOR;
};
VSout main(VSin input){
	VSout output;
	float4 local = float4(input.v, 1.0);
	float4 world = mul(local, l2w);
	output.where = mul(world, w2c);
	output.color = float4(input.c, 1.0);
	return output;
}