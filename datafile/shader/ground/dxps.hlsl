Texture2D    albedotexture : register( t0 );
SamplerState albedosampler : register( s0 );
struct PSin{
	float4 where : SV_POSITION;
	float4 color : COLOR;
};
float4 main(PSin input) : SV_TARGET{
	//return input.color;
	float2 uv = input.color;
	float3 rgb = albedotexture.Sample(albedosampler, uv);
	return float4(rgb, 1.0);
}