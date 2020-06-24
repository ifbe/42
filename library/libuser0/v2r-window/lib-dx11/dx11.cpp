#include <Windows.h>
#include <string>
#include <D3D11.h>
#include <d3dcompiler.h>
#include "libuser.h"
using namespace std;

//wnd thing
//HINSTANCE	g_hInstance(NULL);
HWND		g_hWnd(NULL);
UINT		g_winWidth(640);
UINT		g_winHeight(480);

//d3d thing
ID3D11Device*           g_dx11device(NULL);
ID3D11DeviceContext*    g_dx11context(NULL);
IDXGISwapChain*         g_dx11swapchain(NULL);
ID3D11DepthStencilView* g_depthStencilView(NULL);
ID3D11RenderTargetView* g_renderTargetView(NULL);

//buf thing
ID3D11VertexShader*     g_pVertexShader = NULL;
ID3D11PixelShader*      g_pPixelShader = NULL;
ID3D11Buffer*           g_pConstantBuffer = NULL;
ID3D11InputLayout*      g_pVertexLayout = NULL;
ID3D11Buffer*           g_pVertexBuffer = NULL;

//my own
char vshader[] =
"cbuffer VSConstantBuffer : register(b0){\n"
	"matrix matmvp;\n"
"};\n"
"struct VSin{\n"
	"float4 where : POSITION;\n"
	"float4 color : COLOR;\n"
"};\n"
"struct VSout{\n"
	"float4 where : SV_POSITION;\n"
	"float4 color : COLOR;\n"
"};\n"
"VSout main(VSin input){\n"
	"VSout output;\n"
	"output.where = mul(input.where, matmvp);\n"
	"output.color = input.color;\n"
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




void Upload(struct gl41data** cam, struct gl41data** lit, struct gl41data** solid, struct gl41data** opaque)
{
	int j;
	printf("camera");
	for(j=0;j<16;j++){
		if(0 == cam[j])break;
		printf("	%llx", cam[j]);
	}
	printf("\n");

	printf("light");
	for(j=0;j<16;j++){
		if(0 == lit[j])break;
		printf("	%llx", lit[j]);
	}
	printf("\n");

	printf("solid");
	for(j=0;j<64;j++){
		if(0 == solid[j])continue;
		printf("	%llx", solid[j]);
	}
	printf("\n");

	printf("opaque");
	for(j=0;j<64;j++){
		if(0 == opaque[j])continue;
		printf("	%llx", opaque[j]);
	}
	printf("\n");
}
void Render()
{
	//viewport
	D3D11_VIEWPORT vp = {0};
	vp.TopLeftX = 0.f;
	vp.TopLeftY = 0.f;
	vp.Width	= static_cast<float>(g_winWidth);
	vp.Height	= static_cast<float>(g_winHeight);
	vp.MinDepth = 0.f;
	vp.MaxDepth = 1.f;
	g_dx11context->RSSetViewports(1,&vp);

	//background
	float color[4] = {0.f, 1.f, 1.f, 1.0f};
	g_dx11context->ClearRenderTargetView(g_renderTargetView,reinterpret_cast<float*>(&color));
	g_dx11context->ClearDepthStencilView(g_depthStencilView,D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL,1.f,0);

	//shader
	g_dx11context->VSSetShader(g_pVertexShader, nullptr, 0);
	g_dx11context->PSSetShader(g_pPixelShader, nullptr, 0);
	g_dx11context->VSSetConstantBuffers(0, 1, &g_pConstantBuffer);

	//ia = input assembler
	UINT stride = 32;
	UINT offset = 0;
	g_dx11context->IASetVertexBuffers(0, 1, &g_pVertexBuffer, &stride, &offset);
	g_dx11context->IASetInputLayout(g_pVertexLayout);
	g_dx11context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	g_dx11context->Draw(3, 0);

	// 显示
	g_dx11swapchain->Present(0,0);
}
void Freemyctx()
{
}
int Initmyctx()
{
	HRESULT hr;

	//1. Compile vshader and pshader
	ID3DBlob* VSBlob = NULL;
	ID3DBlob* VSError= NULL;
	ID3DBlob* PSBlob = NULL;
	ID3DBlob* PSError= NULL;

	hr = D3DCompile(
		vshader, sizeof(vshader), "vs",
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
		pshader, sizeof(pshader), "ps",
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
	hr = g_dx11device->CreateVertexShader(VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), NULL, &g_pVertexShader );
	if(FAILED(hr)){
		VSBlob->Release();
		return hr;
	}

	hr = g_dx11device->CreatePixelShader(PSBlob->GetBufferPointer(), PSBlob->GetBufferSize(), NULL, &g_pPixelShader );
	if(FAILED(hr)){
		PSBlob->Release();
		return hr;
	}



	// Fill in a buffer description.
	mat4 mat = {
		2.0, 0.0, 0.0, 0.0,
		0.0, 2.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	};

	D3D11_BUFFER_DESC cbDesc;
	cbDesc.ByteWidth = 4*16;
	cbDesc.Usage = D3D11_USAGE_DYNAMIC;
	cbDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbDesc.MiscFlags = 0;
	cbDesc.StructureByteStride = 0;

	// Fill in the subresource data.
	D3D11_SUBRESOURCE_DATA InitData;
	InitData.pSysMem = &mat;
	InitData.SysMemPitch = 0;
	InitData.SysMemSlicePitch = 0;

	// Create the buffer.
	hr = g_dx11device->CreateBuffer( &cbDesc, &InitData, &g_pConstantBuffer);
	if(FAILED(hr)){
		MessageBox(NULL, "CreateBuffer(constant)", "Error", MB_OK);
		return hr;
	}




	//vertex data
	float vertices[][8] = {
		{ 0.0f, 0.5f, 0.0f, 1.0,        1.0f, 0.0f, 0.0f, 1.0f},
		{ 0.5f,-0.5f, 0.0f, 1.0,        0.0f, 1.0f, 0.0f, 1.0f},
		{-0.5f,-0.5f, 0.0f, 1.0,        0.0f, 0.0f, 1.0f, 1.0f}
	};

	//vertex desc
	D3D11_BUFFER_DESC vbd;
	ZeroMemory(&vbd, sizeof(vbd));
	vbd.Usage = D3D11_USAGE_IMMUTABLE;
	vbd.ByteWidth = sizeof(vertices);
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;

	//vertex buffer
	D3D11_SUBRESOURCE_DATA Data;
	ZeroMemory(&Data, sizeof(Data));
	Data.pSysMem = vertices;
	hr = g_dx11device->CreateBuffer(&vbd, &Data, &g_pVertexBuffer);
	if(FAILED(hr)){
		MessageBox(NULL, "CreateBuffer(vertex)", "Error", MB_OK);
		return hr;
	}

	//vertex layout
	D3D11_INPUT_ELEMENT_DESC dies[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{   "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0,16, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};
	g_dx11device->CreateInputLayout(dies, 2, VSBlob->GetBufferPointer(), VSBlob->GetBufferSize(), &g_pVertexLayout);

	return 1;
}




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

	return TRUE;
}




LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch(msg){
	case WM_CLOSE:
		eventwrite(0,0,0,0);
		PostQuitMessage(0);
	}

	return DefWindowProc(hwnd,msg,wParam,lParam);
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
	Upload(wnd->gl_camera, wnd->gl_light, wnd->gl_solid, wnd->gl_opaque);

	//draw
	Render();

	//event
	MSG msg = {0};
	while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE)){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

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
	Freemyctx();
	FreeD3D11();
	FreeWin32();
	return 0;
}
int windowcreate(struct supply* wnd)
{
	wnd->fmt = _full_;
	wnd->gl_camera = (struct gl41data**)memorycreate(0x10000, 0);
	wnd->gl_light  = (struct gl41data**)memorycreate(0x10000, 0);
	wnd->gl_solid  = (struct gl41data**)memorycreate(0x10000, 0);
	wnd->gl_opaque = (struct gl41data**)memorycreate(0x10000, 0);

	if(!InitWin32())return -1;
	if(!InitD3D11())return -1;
	if(!Initmyctx())return -1;
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