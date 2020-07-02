#include <string>
#include <Windows.h>
#include <windowsx.h>
#include <D3D11.h>
#include <d3dcompiler.h>
#include "libuser.h"
using namespace std;

//wnd thing
//HINSTANCE	g_hInstance(NULL);
HWND		g_hWnd(NULL);
UINT		g_winWidth(1024);
UINT		g_winHeight(768);
static int leftdown=0;
static int rightdown=0;
static POINT pt, pe;
static RECT rt, re;

//d3d thing
ID3D11Device*           g_dx11device(NULL);
ID3D11DeviceContext*    g_dx11context(NULL);
IDXGISwapChain*         g_dx11swapchain(NULL);
ID3D11DepthStencilView* g_depthStencilView(NULL);
ID3D11RenderTargetView* g_renderTargetView(NULL);
ID3D11RasterizerState*  g_rasterstate(NULL);

//input layout
D3D11_INPUT_ELEMENT_DESC inputlayout_p4c4[] = {
	{"PA", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"PB", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,16, D3D11_INPUT_PER_VERTEX_DATA, 0}
};
D3D11_INPUT_ELEMENT_DESC inputlayout_p4n4c4[] = {
	{"PA", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"PB", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,16, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"PC", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,32, D3D11_INPUT_PER_VERTEX_DATA, 0}
};
D3D11_INPUT_ELEMENT_DESC inputlayout_p3n3[] = {
	{"PA", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"PB", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,12, D3D11_INPUT_PER_VERTEX_DATA, 0}
};
D3D11_INPUT_ELEMENT_DESC inputlayout_p3n3c3[] = {
	{"PA", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"PB", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,12, D3D11_INPUT_PER_VERTEX_DATA, 0},
	{"PC", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,24, D3D11_INPUT_PER_VERTEX_DATA, 0}
};
/*
//shader thing
ID3D11VertexShader*     g_pVertexShader = NULL;
ID3D11PixelShader*      g_pPixelShader = NULL;
ID3D11InputLayout*      g_pVertexLayout = NULL;
//buffer thing
ID3D11Buffer*           g_pConstantBuffer = NULL;
ID3D11Buffer*           g_pVertexBuffer = NULL;
ID3D11Buffer*           g_pIndexBuffer = NULL;
//my own
#define VAL 500.0
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
*/



int Upload_shader(
	char* vshader, char* pshader, D3D11_INPUT_ELEMENT_DESC* desc, int count,
	ID3D11VertexShader** vs, ID3D11PixelShader** ps, ID3D11InputLayout** layout)
{
	ID3DBlob* VSBlob = NULL;
	ID3DBlob* VSError= NULL;
	ID3DBlob* PSBlob = NULL;
	ID3DBlob* PSError= NULL;
	if(0 != *vs)return 0;
	if(0 != *ps)return 0;
//printf("vs:%s\nps:%s\n",vshader,pshader);

	HRESULT hr = D3DCompile(
		vshader, strlen(vshader), "vs",
		0, 0,	//define, include
		"main", "vs_5_0",	//entry, target
		0, 0,	//flag1, flag2
		&VSBlob, &VSError
	);
	if(FAILED(hr)){
		MessageBox(NULL, (char*)VSError->GetBufferPointer(), "D3DCompile(vshader)", MB_OK);
		return 0;
	}

	hr = D3DCompile(
		pshader, strlen(pshader), "ps",
		0, 0,	//define, include
		"main", "ps_5_0",	//entry, target
		0, 0,	//flag1, flag2
		&PSBlob, &PSError
	);
	if(FAILED(hr)){
		MessageBox(NULL, (char*)PSError->GetBufferPointer(), "D3DCompile(pshader)", MB_OK);
		return 0;
	}

	//2. Create vshader and pshader
	hr = g_dx11device->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), NULL, vs);
	if(FAILED(hr)){
		VSBlob->Release();
		return hr;
	}

	hr = g_dx11device->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), NULL, ps);
	if(FAILED(hr)){
		PSBlob->Release();
		return hr;
	}

	//vertex layout
	g_dx11device->CreateInputLayout(desc, count, VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), layout);
	return 0;
}
int Upload_texture(ID3D11Texture2D** dst)
{
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));

	HRESULT hr = g_dx11device->CreateTexture2D(&desc, &data, dst);
	if(FAILED(hr)){
		MessageBox(NULL, "CreateTexture2D", "Error", MB_OK);
		return hr;
	}
	return 0;
}
int Upload_constant(void* buf, int len, ID3D11Buffer** dst)
{
	if(0 != *dst){
		g_dx11context->UpdateSubresource(*dst, 0, 0, buf, 0, 0);
		return 0;
	}

	D3D11_BUFFER_DESC Desc;
	ZeroMemory(&Desc, sizeof(Desc));
	Desc.ByteWidth           = len;
	Desc.Usage               = D3D11_USAGE_DEFAULT;
	Desc.BindFlags           = D3D11_BIND_CONSTANT_BUFFER;
	Desc.CPUAccessFlags      = 0;
	Desc.MiscFlags           = 0;
	Desc.StructureByteStride = 0;

	// Fill in the subresource data.
	D3D11_SUBRESOURCE_DATA Data;
	ZeroMemory(&Data, sizeof(Data));
	Data.pSysMem          = buf;
	Data.SysMemPitch      = 0;
	Data.SysMemSlicePitch = 0;

	// Create the buffer.
	HRESULT hr = g_dx11device->CreateBuffer(&Desc, &Data, dst);
	if(FAILED(hr)){
		MessageBox(NULL, "CreateBuffer(constant)", "Error", MB_OK);
		return hr;
	}

	return 0;
}
int Upload_vertex(void* buf, int len, ID3D11Buffer** dst)
{
	if(0 != *dst){
		g_dx11context->UpdateSubresource(*dst, 0, 0, buf, 0, 0);
		return 0;
	}

	//vertex desc
	D3D11_BUFFER_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.ByteWidth      = len;
	desc.Usage          = D3D11_USAGE_DEFAULT;
	desc.BindFlags      = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;

	//vertex buffer
	D3D11_SUBRESOURCE_DATA Data;
	ZeroMemory(&Data, sizeof(Data));
	Data.pSysMem = buf;

	//vertex fd
	HRESULT hr = g_dx11device->CreateBuffer(&desc, &Data, dst);
	if(FAILED(hr)){
		MessageBox(NULL, "CreateBuffer(vertex)", "Error", MB_OK);
		return hr;
	}

	return 0;
}
int Upload_index(void* buf, int len, ID3D11Buffer** dst)
{
	if(0 != *dst){
		g_dx11context->UpdateSubresource(*dst, 0, 0, buf, 0, 0);
		return 0;
	}

	// Fill in a buffer description.
	D3D11_BUFFER_DESC Desc;
	Desc.ByteWidth      = len;
	Desc.Usage          = D3D11_USAGE_DEFAULT;
	Desc.BindFlags      = D3D11_BIND_INDEX_BUFFER;
	Desc.CPUAccessFlags = 0;
	Desc.MiscFlags      = 0;

	// Define the resource data.
	D3D11_SUBRESOURCE_DATA Data;
	Data.pSysMem          = buf;
	Data.SysMemPitch      = 0;
	Data.SysMemSlicePitch = 0;

	// Create the buffer with the device.
	HRESULT hr = g_dx11device->CreateBuffer(&Desc, &Data, dst);
	if(FAILED(hr)){
		MessageBox(NULL, "CreateBuffer(index)", "Error", MB_OK);
		return hr;
	}
	return 0;
}
void Upload(struct dx11data** cam, struct dx11data** lit, struct dx11data** solid, struct dx11data** opaque)
{
	int j;
/*	float a = PI/(getrandom()%180);
	float c = getcos(a);
	float s = getsin(a);
	g_mat[0][0] = c;
	g_mat[0][1] =-s;
	g_mat[1][0] = s;
	g_mat[1][1] = c;
	g_dx11context->UpdateSubresource(g_pConstantBuffer, 0, 0, &g_mat, 0, 0);
*/
	Upload_constant((void*)&cam[0]->src.arg, 64+16, (ID3D11Buffer**)&cam[0]->dst.constant);

	struct vertex* vtx;
	for(j=0;j<64;j++){
		if(0 == solid[j])continue;
		vtx = &solid[j]->src.vtx[0];
		if(0 == vtx->vbuf)continue;
/*		printf("%d: %x,%x, (%llx,%x,%x,%x), (%llx,%x,%x,%x)\n",
			j, vtx->geometry, vtx->opaque,
			vtx->vbuf, vtx->vbuf_w, vtx->vbuf_h, vtx->vbuf_fmt,
			vtx->ibuf, vtx->ibuf_w, vtx->ibuf_h, vtx->ibuf_fmt);
*/
		if(vbuffmt_33 == vtx->vbuf_fmt){
			Upload_shader(
				(char*)solid[j]->src.vs,
				(char*)solid[j]->src.ps,
				inputlayout_p3n3,
				2,
				(ID3D11VertexShader**)&solid[j]->dst.vsprog,
				(ID3D11PixelShader**)&solid[j]->dst.psprog,
				(ID3D11InputLayout**)&solid[j]->dst.layout
			);
		}
		if(vtx->vbuf){
			Upload_vertex(vtx->vbuf, vtx->vbuf_len, (ID3D11Buffer**)&solid[j]->dst.vbuf);
			//if(vtx->vbuf_fmt == vbuffmt_33)
		}
		if(vtx->ibuf){
			Upload_index(vtx->ibuf, vtx->ibuf_len, (ID3D11Buffer**)&solid[j]->dst.ibuf);
		}
	}
}
void Render(struct dx11data** cam, struct dx11data** lit, struct dx11data** solid, struct dx11data** opaque)
{
	//viewport
	D3D11_VIEWPORT vp = {0};
	vp.TopLeftX = 0.f;
	vp.TopLeftY = 0.f;
	vp.Width	= static_cast<float>(g_winWidth);
	vp.Height	= static_cast<float>(g_winHeight);
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	g_dx11context->RSSetViewports(1,&vp);
	g_dx11context->RSSetState(g_rasterstate);

	//clear
	float color[4] = {0.1, 0.1, 0.1, 1.0};
	g_dx11context->ClearRenderTargetView(g_renderTargetView,reinterpret_cast<float*>(&color));
	g_dx11context->ClearDepthStencilView(g_depthStencilView,D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL,1.f,0);

/*
	//shader
	g_dx11context->VSSetShader(g_pVertexShader, nullptr, 0);
	g_dx11context->PSSetShader(g_pPixelShader, nullptr, 0);

	//constant
	//g_dx11context->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
	g_dx11context->VSSetConstantBuffers(0, 1, (ID3D11Buffer**)&cam[0]->dst.constant);

	//texture
	//g_dx11context->VSSetSamplers(0, 1, );
	//g_dx11context->PSSetSamplers(0, 1, );

	//ia = input assembler
	UINT stride = 4*9;
	UINT offset = 0;
	g_dx11context->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
	g_dx11context->IASetInputLayout(g_pVertexLayout);

	g_dx11context->IASetIndexBuffer(g_pIndexBuffer, DXGI_FORMAT_R16_UINT, 0 );

	g_dx11context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_dx11context->DrawIndexed(2*3, 0, 0);
	//g_dx11context->Draw(6, 0);
*/

	int j;
	struct vertex* vtx;
	for(j=0;j<64;j++){
		if(0 == solid[j])continue;
		vtx = &solid[j]->src.vtx[0];
		if(0 == vtx->vbuf)continue;

		if(vbuffmt_33 == vtx->vbuf_fmt){
			g_dx11context->VSSetShader((ID3D11VertexShader*)solid[j]->dst.vsprog, nullptr, 0);
			g_dx11context->PSSetShader((ID3D11PixelShader*)solid[j]->dst.psprog, nullptr, 0);

			//constant
			//g_dx11context->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);
			g_dx11context->VSSetConstantBuffers(0, 1, (ID3D11Buffer**)&cam[0]->dst.constant);

			UINT stride = 4*6;
			UINT offset = 0;
			g_dx11context->IASetVertexBuffers(0, 1, (ID3D11Buffer**)&solid[j]->dst.vbuf, &stride, &offset);
			g_dx11context->IASetInputLayout((ID3D11InputLayout*)solid[j]->dst.layout);

			g_dx11context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
			g_dx11context->Draw(vtx->vbuf_h, 0);
		}/*
		if( (0 != vtx->vbuf)&&
			(0 != vtx->ibuf)&&
			(vbuffmt_333 == vtx->vbuf_fmt))
		{
			UINT stride = 4*9;
			UINT offset = 0;
			g_dx11context->IASetVertexBuffers(0, 1, (ID3D11Buffer**)&solid[j]->dst.vbuf, &stride, &offset);
			g_dx11context->IASetInputLayout(g_pVertexLayout);

			g_dx11context->IASetIndexBuffer((ID3D11Buffer*)solid[j]->dst.ibuf, DXGI_FORMAT_R16_UINT, 0 );

			g_dx11context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			g_dx11context->DrawIndexed(3*vtx->ibuf_h, 0, 0);
		}*/
	}

	// 显示
	g_dx11swapchain->Present(0,0);
}/*
void Freemyctx()
{
}
int Initmyctx()
{
	Upload_shader(vshader, pshader, inputlayout_p3n3c3, 3, &g_pVertexShader, &g_pPixelShader, &g_pVertexLayout);

	Upload_constant(g_mat, sizeof(g_mat), &g_pConstantBuffer);

	Upload_vertex(vertices, sizeof(vertices), &g_pVertexBuffer);

	Upload_index(indices, sizeof(indices), &g_pIndexBuffer);

	return 1;
}*/




void FreeD3D11()
{
	g_depthStencilView->Release();
	g_renderTargetView->Release();

	g_dx11swapchain->Release();
	g_dx11context->Release();
	g_dx11device->Release();
}
BOOL InitD3D11()
{
	// a.创建设备和上下文
	D3D_FEATURE_LEVEL myFeatureLevel;

	UINT createDeviceFlags = 0;
#if defined(DEBUG) || defined(_DEBUG)  
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	HRESULT hr = D3D11CreateDevice(
		NULL,						// 默认显示适配器
		D3D_DRIVER_TYPE_HARDWARE,
		0,							// 不使用软件设备
		createDeviceFlags,
		NULL, 0,					// 默认的特征等级数组
		D3D11_SDK_VERSION,
		&g_dx11device,
		&myFeatureLevel,
		&g_dx11context
	);
	if(FAILED(hr))
	{
		MessageBox(NULL, "D3D11CreateDevice", "error",MB_OK);
		return FALSE;
	}

	// b.4X多重采样质量等级
	UINT m4xMsaaQuality(0);
	g_dx11device->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		4,
		&m4xMsaaQuality);

	// c.准备交换链属性
	DXGI_SWAP_CHAIN_DESC sd = {0};
	sd.BufferDesc.Width = g_winWidth;
	sd.BufferDesc.Height = g_winHeight;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	sd.SampleDesc.Count = 4;
	sd.SampleDesc.Quality = m4xMsaaQuality-1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = g_hWnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	// d.创建交换链
	IDXGIDevice *dxgiDevice(NULL);
	g_dx11device->QueryInterface(__uuidof(IDXGIDevice),(void**)(&dxgiDevice));
	IDXGIAdapter *dxgiAdapter(NULL);
	dxgiDevice->GetParent(__uuidof(IDXGIAdapter),(void**)(&dxgiAdapter));
	IDXGIFactory *dxgiFactory(NULL);
	dxgiAdapter->GetParent(__uuidof(IDXGIFactory),(void**)(&dxgiFactory));
	hr = dxgiFactory->CreateSwapChain(g_dx11device, &sd, &g_dx11swapchain);
	if(FAILED(hr))
	{
		MessageBox(NULL, "CreateSwapChain", "error",MB_OK);
		return FALSE;
	}
	dxgiFactory->Release();
	dxgiAdapter->Release();
	dxgiDevice->Release();

	// e.创建渲染目标视图
	ID3D11Texture2D *backBuffer(NULL);
	g_dx11swapchain->GetBuffer(0,__uuidof(ID3D11Texture2D),reinterpret_cast<void**>(&backBuffer));
	hr = g_dx11device->CreateRenderTargetView(backBuffer,NULL,&g_renderTargetView);
	if(FAILED(hr))
	{
		MessageBox(NULL, "CreateRenderTargetView", "error",MB_OK);
		return FALSE;
	}
	backBuffer->Release();

	// f.创建深度缓冲区和其视图
	D3D11_TEXTURE2D_DESC depthStencilDesc = {0};
	depthStencilDesc.Width				= g_winWidth;
	depthStencilDesc.Height				= g_winHeight;
	depthStencilDesc.MipLevels			= 1;
	depthStencilDesc.ArraySize			= 1;
	depthStencilDesc.Format				= DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilDesc.SampleDesc.Count	= 4;
	depthStencilDesc.SampleDesc.Quality	= m4xMsaaQuality-1;
	depthStencilDesc.Usage				= D3D11_USAGE_DEFAULT;
	depthStencilDesc.BindFlags			= D3D11_BIND_DEPTH_STENCIL;
	depthStencilDesc.CPUAccessFlags		= 0;
	depthStencilDesc.MiscFlags			= 0;

	ID3D11Texture2D *depthStencilBuffer(NULL);
	hr = g_dx11device->CreateTexture2D(&depthStencilDesc,NULL,&depthStencilBuffer);
	if(FAILED(hr))
	{
		MessageBox(NULL, "CreateTexture2D(depthstencil)", "error",MB_OK);
		return FALSE;
	}
	hr = g_dx11device->CreateDepthStencilView(depthStencilBuffer,NULL,&g_depthStencilView);
	if(FAILED(hr))
	{
		MessageBox(NULL, "CreateDepthStencilView", "error",MB_OK);
		return FALSE;
	}

	// g.将视图绑定到输出合并器阶段
	g_dx11context->OMSetRenderTargets(1,&g_renderTargetView,g_depthStencilView);
	depthStencilBuffer->Release();

	//no cull
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
	desc.FillMode = D3D11_FILL_SOLID;
	desc.CullMode = D3D11_CULL_NONE;
	g_dx11device->CreateRasterizerState(&desc, &g_rasterstate);
	return TRUE;
}




static void restorestackdeliverevent(struct supply* wnd, struct event* ev)
{
	u64* save = (u64*)wnd->spsave;
	if(0 == save){
		eventwrite(ev->why, ev->what, ev->where, 0);
		return;
	}

	struct halfrel* stack = (struct halfrel*)save[0];
	int sp = save[1];

	//get vertex
	struct relation* rel = (struct relation*)wnd->orel0;
	if(0 == rel)return;
	stack[sp+0].pchip = rel->psrcchip;
	stack[sp+0].pfoot = rel->psrcfoot;
	//stack[sp+0].type = rel->srctype;
	stack[sp+0].flag = rel->srcflag;
	stack[sp+1].pchip = rel->pdstchip;
	stack[sp+1].pfoot = rel->pdstfoot;
	//stack[sp+1].type = rel->dsttype;
	stack[sp+1].flag = rel->dstflag;
	entitywrite((struct entity*)rel->pdstchip, rel->dstflag, stack,sp+2, 0,0, ev, 0);
}
LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	struct event ev;
	u64 addr = GetWindowLongPtr(hwnd, GWLP_USERDATA);
	struct supply* win = (struct supply*)addr;

	switch(msg){
		case WM_MOUSEWHEEL:
		{
			u64 x,y,k;
			GetCursorPos(&pt);
			ScreenToClient(hwnd, &pt);

			short* t = (short*)&ev.why;
			t[0] = pt.x;
			t[1] = pt.y;
			t[2] = GET_WHEEL_DELTA_WPARAM(wparam)/60;
			printf("%d\n",t[2]);
			t[3] = (t[2]>0) ? 'f' : 'b';

			ev.what = 0x4070;
			ev.where = addr;
			restorestackdeliverevent(win, &ev);
			return 0;
		}

		case WM_MOUSEMOVE:
		{
			u64 x,y,k;
			if((leftdown>0)&&(rightdown>0))
			{
				GetCursorPos(&pe);		// 获取光标指针的新位置
				re.left=rt.left+(pe.x - pt.x);		// 窗口新的水平位置
				re.top =rt.top+(pe.y - pt.y);		// 窗口新的垂直位置
				MoveWindow(hwnd, re.left, re.top, re.right, re.bottom, 1);// 移动窗口
				GetCursorPos(&pt);			// 获取鼠标当前位置
				GetWindowRect(hwnd, &rt);	// 获取窗口位置与大小
				return 0;
			}
			else if(rightdown>0)k = 'r';
			else if(leftdown>0)k = 'l';
			else k = 'l';

			y = GET_Y_LPARAM(lparam);
			x = GET_X_LPARAM(lparam);
			//eventwrite(x + (y<<16) + (k<<48), 0x4070, addr, 0);

			ev.why = x + (y<<16) + (k<<48);
			ev.what = 0x4070;
			ev.where = addr;
			restorestackdeliverevent(win, &ev);
			return 0;
		}

		case WM_LBUTTONUP:
		{
			u64 x,y,k;
			leftdown=0;

			k = 'l';
			y = GET_Y_LPARAM(lparam);
			x = GET_X_LPARAM(lparam);
			//eventwrite(x + (y<<16) + (k<<48), 0x2d70, addr, 0);

			ev.why = x + (y<<16) + (k<<48);
			ev.what = 0x2d70;
			ev.where = addr;
			restorestackdeliverevent(win, &ev);
			return 0;
		}

		case WM_RBUTTONUP:
		{
			u64 x,y,k;
			rightdown=0;

			k = 'r';
			y = GET_Y_LPARAM(lparam);
			x = GET_X_LPARAM(lparam);
			//eventwrite(x + (y<<16) + (k<<48), 0x2d70, addr, 0);

			ev.why = x + (y<<16) + (k<<48);
			ev.what = 0x2d70;
			ev.where = addr;
			restorestackdeliverevent(win, &ev);
			return 0;
		}

		case WM_LBUTTONDOWN:
		{
			u64 x,y,k;
			leftdown=1;
			GetCursorPos(&pt);		// 获取鼠标光标指针当前位置

			if(rightdown>0)
			{
				GetWindowRect(hwnd, &rt);	// 获取窗口位置与大小
				re.right=rt.right-rt.left;	// 保存窗口宽度
				re.bottom=rt.bottom-rt.top;	// 保存窗口高度
			}
			
			k = 'l';
			y = GET_Y_LPARAM(lparam);
			x = GET_X_LPARAM(lparam);
			//eventwrite(x + (y<<16) + (k<<48), 0x2b70, addr, 0);

			ev.why = x + (y<<16) + (k<<48);
			ev.what = 0x2b70;
			ev.where = addr;
			restorestackdeliverevent(win, &ev);
			return 0;
		}

		case WM_RBUTTONDOWN:
		{
			u64 x,y,k;
			rightdown=1;
			GetCursorPos(&pt);

			if(leftdown>0)
			{
				GetWindowRect(hwnd, &rt);
				re.right=rt.right-rt.left;
				re.bottom=rt.bottom-rt.top;
			}

			k = 'r';
			y = GET_Y_LPARAM(lparam);
			x = GET_X_LPARAM(lparam);
			//eventwrite(x + (y<<16) + (k<<48), 0x2b70, addr, 0);

			ev.why = x + (y<<16) + (k<<48);
			ev.what = 0x2b70;
			ev.where = addr;
			restorestackdeliverevent(win, &ev);
			return 0;
		}

		case WM_CHAR:{
			//printf("%x\n",wparam);
			if(wparam==0x1b)
			{
				//eventwrite(0x1b, 0x64626b, addr, 0);
				ev.why = 0x1b;
				ev.what = 0x64626b;
				ev.where = addr;
				restorestackdeliverevent(win, &ev);
			}
			else
			{
				//eventwrite(wparam, 0x72616863, addr, 0);
				ev.why = wparam;
				ev.what = 0x72616863;
				ev.where = addr;
				restorestackdeliverevent(win, &ev);
			}
			return 0;
		}
		case WM_SIZE:
		{
			int w = lparam&0xffff;
			int h = (lparam>>16)&0xffff;
			//printf("wm_size:%d,%d\n", w, h);

			if(win != 0){
				win->fbwidth = win->width = w;
				win->fbheight= win->height= h;
			}

			//eventwrite(0x657a6973, 0x4077, addr, 0);
			return 0;
		}

		case WM_CLOSE:{
			eventwrite(0,0,0,0);
			PostQuitMessage(0);
			break;
		}
	}

	return DefWindowProc(hwnd,msg,wparam,lparam);
}
void FreeWin32()
{
}
BOOL InitWin32()
{
	g_hWnd = CreateWindow(
		"d3d11",
		"d3d11",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,CW_USEDEFAULT,
		g_winWidth,g_winHeight,
		NULL,
		NULL,
		0,
		NULL
	);
	if(!g_hWnd){
		MessageBox(NULL, "CreateWindow", "error",MB_OK);
		return FALSE;
	}

	ShowWindow(g_hWnd,SW_SHOW);
	UpdateWindow(g_hWnd);

	return TRUE;
}




extern "C" {
int windowread(struct supply* wnd,int foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	//take
	struct relation* rel = (struct relation*)wnd->orel0;
	while(1){
		if(0 == rel)break;

		struct fstyle* area = (struct fstyle*)rel->psrcfoot;
		if(area){
			//get vertex
			stack[sp+0].pchip = rel->psrcchip;
			stack[sp+0].pfoot = rel->psrcfoot;
			//stack[sp+0].type = rel->srctype;
			stack[sp+0].flag = rel->srcflag;
			stack[sp+1].pchip = rel->pdstchip;
			stack[sp+1].pfoot = rel->pdstfoot;
			//stack[sp+1].type = rel->dsttype;
			stack[sp+1].flag = rel->dstflag;
			entityread((struct entity*)rel->pdstchip, rel->dstflag, stack,sp+2, 0,'v', 0, 0);
		}

		rel = (struct relation*)samesrcnextdst(rel);
	}

	//give
	Upload(wnd->dx_camera, wnd->dx_light, wnd->dx_solid, wnd->dx_opaque);

	//draw
	Render(wnd->dx_camera, wnd->dx_light, wnd->dx_solid, wnd->dx_opaque);

	u64 save[2];
	save[0] = (u64)stack;
	save[1] = sp;
	wnd->spsave = save;

	//event
	MSG msg = {0};
	while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE)){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	wnd->spsave = 0;
	return 0;
}
int windowwrite(struct supply* wnd,int foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	return 0;
}
int windowdiscon(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int windowlinkup(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int windowsearch(struct supply* wnd)
{
	return 0;
}
int windowmodify(struct supply* wnd)
{
	return 0;
}
int windowdelete(struct supply* wnd)
{
	//Freemyctx();
	FreeD3D11();
	FreeWin32();
	return 0;
}
int windowcreate(struct supply* wnd)
{
	wnd->fmt = _dx11full_;
	wnd->vfmt= _dx11full_;

	wnd->width = wnd->fbwidth = 1024;
	wnd->height= wnd->fbheight= 768;
	wnd->dx_camera = (struct dx11data**)memorycreate(0x10000, 0);
	wnd->dx_light  = (struct dx11data**)memorycreate(0x10000, 0);
	wnd->dx_solid  = (struct dx11data**)memorycreate(0x10000, 0);
	wnd->dx_opaque = (struct dx11data**)memorycreate(0x10000, 0);

	if(!InitWin32())return -1;
	if(!InitD3D11())return -1;
	//if(!Initmyctx())return -1;
	SetWindowLongPtr(g_hWnd, GWLP_USERDATA, (u64)wnd);
	return 0;
}




void freewindow()
{
}
void initwindow()
{
	WNDCLASS wndcls;
	wndcls.cbClsExtra = 0;
	wndcls.cbWndExtra = 0;
	wndcls.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wndcls.hCursor = LoadCursor(NULL,IDC_ARROW);
	wndcls.hIcon = LoadIcon(NULL,IDI_APPLICATION);
	wndcls.hInstance = 0;
	wndcls.lpfnWndProc = WinProc;
	wndcls.lpszClassName = "d3d11";
	wndcls.lpszMenuName = NULL;
	wndcls.style = CS_HREDRAW | CS_VREDRAW;
	if(!RegisterClass(&wndcls)){
		MessageBox(NULL, "RegisterClass", "error",MB_OK);
	}
}

}//extern "C"
