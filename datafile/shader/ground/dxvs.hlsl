cbuffer VSConstantBuffer : register(b0){
	matrix matmvp;
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
	output.where = mul(float4(input.v, 1.0), matmvp);
	output.color = float4(input.c, 1.0);
	return output;
}