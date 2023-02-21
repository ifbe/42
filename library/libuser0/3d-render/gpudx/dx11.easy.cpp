#include <string>
#include <Windows.h>
#include <windowsx.h>
#include <D3D11.h>
#include <d3dcompiler.h>
#include "libuser.h"
using namespace std;
int Upload_shader(char* vshader, char* pshader, D3D11_INPUT_ELEMENT_DESC* desc, int count, ID3D11VertexShader** vs, ID3D11PixelShader** ps, ID3D11InputLayout** layout);
int Upload_constant(void* buf, int len, ID3D11Buffer** dst);
int Upload_vertex(void* buf, int len, ID3D11Buffer** dst);
int Upload_index(void* buf, int len, ID3D11Buffer** dst);




//shader thing
ID3D11VertexShader*     g_pVertexShader = NULL;
ID3D11PixelShader*      g_pPixelShader = NULL;
ID3D11InputLayout*      g_pVertexLayout = NULL;
//buffer thing
ID3D11Buffer*           g_pConstantBuffer = NULL;
ID3D11Buffer*           g_pVertexBuffer = NULL;
ID3D11Buffer*           g_pIndexBuffer = NULL;
//my own
#define VAL 0.5
float vertices[][9] = {
	{-VAL,-VAL, 0.0,    0.0, 0.0, 1.0,    1.0, 0.0, 0.0},
	{ VAL,-VAL, 0.0,    0.0, 0.0, 1.0,    0.0, 1.0, 0.0},
	{-VAL, VAL, 0.0,    0.0, 0.0, 1.0,    0.0, 0.0, 1.0},
	{ VAL, VAL, 0.0,    0.0, 0.0, 1.0,    1.0, 0.0, 0.0}
};
unsigned short indices[] = {
	0, 1, 2,
	1, 2, 3
};
mat4 g_mat = {
	1.0, 0.0, 0.0, 0.0,
	0.0, 1.0, 0.0, 0.0,
	0.0, 0.0, 1.0, 0.0,
	0.0, 0.0, 0.0, 1.0
};
D3D11_INPUT_ELEMENT_DESC inputlayout[] = {
	{"PA", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"PB", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"PC", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,24, D3D11_INPUT_PER_VERTEX_DATA, 0}
};
char vshader[] =
"cbuffer VSConstantBuffer : register(b0){\n"
	"matrix matmvp;\n"
"};\n"
"struct VSin{\n"
	"float3 v : PA;\n"
	"float3 n : PB;\n"
	"float3 c : PC;\n"
"};\n"
"struct VSout{\n"
	"float4 where : SV_POSITION;\n"
	"float4 color : COLOR;\n"
"};\n"
"VSout main(VSin input){\n"
	"VSout output;\n"
	"output.where = mul(float4(input.v, 1.0), matmvp);\n"
	"output.color = float4(input.c, 1.0);\n"
	"return output;\n"
"}\n";
char pshader[] =
"struct PSin{\n"
"	float4 where : SV_POSITION;\n"
"	float4 color : COLOR;\n"
"};\n"
"float4 main(PSin input) : SV_TARGET{\n"
"	return input.color;\n"
"}";




void dx11easy_upload(ID3D11DeviceContext* devctx)
{
	float a = PI/(random_read()%180);
	float c = getcos(a);
	float s = getsin(a);
	g_mat[0][0] = c;
	g_mat[0][1] =-s;
	g_mat[1][0] = s;
	g_mat[1][1] = c;
	devctx->UpdateSubresource(g_pConstantBuffer, 0, 0, &g_mat, 0, 0);

	Upload_constant(g_mat, 64, &g_pConstantBuffer);
}
void dx11easy_render(ID3D11DeviceContext* devctx)
{
	//shader
	devctx->VSSetShader(g_pVertexShader, nullptr, 0);
	devctx->PSSetShader(g_pPixelShader, nullptr, 0);

	//constant
	devctx->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);

	//texture
	//devctx->VSSetSamplers(0, 1, );
	//devctx->PSSetSamplers(0, 1, );

	//ia = input assembler
	UINT stride = 4*9;
	UINT offset = 0;
	devctx->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
	devctx->IASetInputLayout(g_pVertexLayout);

	devctx->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0 );

	devctx->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	devctx->DrawIndexed(2*3, 0, 0);

	devctx->Draw(6, 0);
}
void dx11easy_delete()
{
}
void dx11easy_create()
{
	Upload_shader(vshader, pshader, inputlayout, 3, &g_pVertexShader, &g_pPixelShader, &g_pVertexLayout);

	Upload_constant(g_mat, sizeof(g_mat), &g_pConstantBuffer);

	Upload_vertex(vertices, sizeof(vertices), &g_pVertexBuffer);

	Upload_index(indices, sizeof(indices), &g_pIndexBuffer);
}