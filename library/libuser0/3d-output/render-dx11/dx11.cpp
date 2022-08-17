#include <string>
#include <Windows.h>
#include <windowsx.h>
#include <D3D11.h>
#include <d3dcompiler.h>
#include "libuser.h"
using namespace std;
extern "C"{
void* supply_alloc();
void* supply_recycle(void*);
}
int dx11easy_create();
int dx11easy_delete();
int dx11easy_upload(ID3D11DeviceContext* devctx);
int dx11easy_render(ID3D11DeviceContext* devctx);

//wnd thing
static int leftdown=0;
static int rightdown=0;
static POINT pt, pe;
static RECT rt, re;

//d3d thing
ID3D11Device*            g_dx11device(NULL);
ID3D11DeviceContext*     g_dx11context(NULL);
IDXGISwapChain*          g_dx11swapchain(NULL);

ID3D11DepthStencilView*  g_depthStencilView(NULL);
ID3D11RenderTargetView*  g_renderTargetView(NULL);

ID3D11RasterizerState*   g_rasterstate(NULL);
ID3D11BlendState*        g_blendstate(NULL);
ID3D11DepthStencilState* g_depthstate(NULL);

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
//printf("vs::\n%s\nps::\n%s\n",vshader,pshader);

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
int Upload_texture(struct texture* tex, ID3D11Texture2D** texture, ID3D11ShaderResourceView** resource, ID3D11SamplerState** sampler)
{
	HRESULT hr;
	if(0 == tex->data)return 0;

	//
	DXGI_FORMAT fmt;
	int per = 4;
	if(hex32('o','n','e',0) == tex->fmt){
		fmt = DXGI_FORMAT_R8_UNORM;
		per = 1;
	}
	else{
		fmt = DXGI_FORMAT_B8G8R8A8_UNORM;
		per = 4;
	}

	if(0 != *resource){
/*		D3D11_BOX box;
		box.left = 0;
		box.right = 1;
		box.top = 0;
		box.bottom = 1;
		box.front = 0;
		box.back = 1;
*/
		g_dx11context->UpdateSubresource(
			texture[0], 0, 0, 
			tex->data, per*tex->w, per*tex->w*tex->h
		);
		return 0;
	}
	printf("%x,%x,%x\n", tex->w, tex->h, tex->fmt);

	//texture
	D3D11_TEXTURE2D_DESC desc;
	ZeroMemory(&desc, sizeof(desc));
	desc.Width = tex->w;
	desc.Height= tex->h;
	desc.MipLevels = 1;
	desc.ArraySize = 1;
	desc.Format = fmt;
	desc.SampleDesc.Count = 1;
	desc.SampleDesc.Quality = 0;
	desc.Usage = D3D11_USAGE_DEFAULT;
	desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA data;
	ZeroMemory(&data, sizeof(data));
	data.pSysMem = tex->data;
	data.SysMemPitch = per * tex->w;
	data.SysMemSlicePitch = per * tex->w * tex->h;

	hr = g_dx11device->CreateTexture2D(&desc, &data, texture);
	if(FAILED(hr)){
		MessageBox(NULL, "CreateTexture2D", "Error", MB_OK);
		return hr;
	}


	//resource
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	srvDesc.Format = fmt;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;
	srvDesc.Texture2D.MostDetailedMip = 0;

	hr = g_dx11device->CreateShaderResourceView(texture[0], &srvDesc, resource);
	if(FAILED(hr)){
		MessageBox(NULL, "CreateShaderResourceView", "Error", MB_OK);
		return hr;
	}


	//sampler
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	hr = g_dx11device->CreateSamplerState(&sampDesc, sampler);
	if(FAILED(hr)){
		MessageBox(NULL, "CreateSamplerState", "Error", MB_OK);
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
void Upload_one(struct dx11data* one)
{
	struct vertex* vtx = &one->src.vtx[0];
	if(0 == vtx->vbuf)return;
/*
	//debug
	printf("%d: %x,%x, (%llx,%x,%x,%x), (%llx,%x,%x,%x)\n",
		j, vtx->geometry, vtx->opaque,
		vtx->vbuf, vtx->vbuf_w, vtx->vbuf_h, vtx->vbuf_fmt,
		vtx->ibuf, vtx->ibuf_w, vtx->ibuf_h, vtx->ibuf_fmt
	);
*/
	//layout
	D3D11_INPUT_ELEMENT_DESC* desc;
	int size;
	switch(vtx->vbuf_fmt){
	case vbuffmt_33:{
		desc = inputlayout_p3n3;
		size = 2;
		break;
	}
	case vbuffmt_333:{
		desc = inputlayout_p3n3c3;
		size = 3;
		break;
	}
	case vbuffmt_444:{
		desc = inputlayout_p4n4c4;
		size = 3;
		break;
	}
	default:return;
	}

	//shader
	Upload_shader(
		(char*)one->src.vs,
		(char*)one->src.fs,
		desc,
		size,
		(ID3D11VertexShader**)&one->dst.vsprog,
		(ID3D11PixelShader**)&one->dst.psprog,
		(ID3D11InputLayout**)&one->dst.layout
	);

	//texture
	if(one->dst.tex_deq[0] != one->src.tex_enq[0]){
		Upload_texture(
			&one->src.tex[0],
			(ID3D11Texture2D**)&one->dst.texture[0],
			(ID3D11ShaderResourceView**)&one->dst.resource[0],
			(ID3D11SamplerState**)&one->dst.sampler[0]
		);
		one->dst.tex_deq[0] = one->src.tex_enq[0];
	}

	//constant
	struct uniform* uni = &one->src.uni[0];
	if(uni->buf && uni->len){
		Upload_constant(uni->buf, uni->len, (ID3D11Buffer**)&one->dst.constant);
	}

	//vertices
	if(vtx->vbuf && vtx->vbuf_len){
		Upload_vertex(vtx->vbuf, vtx->vbuf_len, (ID3D11Buffer**)&one->dst.vbuf);
	}

	//indices
	if(vtx->ibuf && vtx->ibuf_len){
		Upload_index(vtx->ibuf, vtx->ibuf_len, (ID3D11Buffer**)&one->dst.ibuf);
	}
}
void Upload_all(struct dx11data** cam, struct dx11data** lit, struct dx11data** solid, struct dx11data** opaque)
{
/*
	//debug
	dx11easy_upload(g_dx11context);
	return;
*/
	int j;
	struct dx11data* data = cam[0];
	struct uniform* uni = &data->src.uni[0];
	if(uni->buf && uni->len){
		Upload_constant(data->src.uni[0].buf, data->src.uni[0].len, (ID3D11Buffer**)&data->dst.constant);
	}

	for(j=0;j<64;j++){
		if(0 == solid[j])continue;
		Upload_one(solid[j]);
	}

	for(j=0;j<64;j++){
		if(0 == opaque[j])continue;
		Upload_one(opaque[j]);
	}
}




void Render_one(struct dx11data* cam, struct dx11data* lit, struct dx11data* one)
{
	struct vertex* vtx = &one->src.vtx[0];
	if(0 == vtx->vbuf)return;

	g_dx11context->VSSetShader((ID3D11VertexShader*)one->dst.vsprog, nullptr, 0);
	g_dx11context->VSSetConstantBuffers(0, 1, (ID3D11Buffer**)&cam->dst.constant);
	g_dx11context->VSSetConstantBuffers(1, 1, (ID3D11Buffer**)&one->dst.constant);

	g_dx11context->PSSetShader((ID3D11PixelShader*)one->dst.psprog, nullptr, 0);
	g_dx11context->PSSetShaderResources(0, 1, (ID3D11ShaderResourceView**)&one->dst.resource);
	g_dx11context->PSSetSamplers(0, 1, (ID3D11SamplerState**)&one->dst.sampler);

	g_dx11context->IASetInputLayout((ID3D11InputLayout*)one->dst.layout);
	switch(vtx->vbuf_fmt){
	case vbuffmt_33:{
		UINT stride = 4*3*2;
		UINT offset = 0;
		g_dx11context->IASetVertexBuffers(0, 1, (ID3D11Buffer**)&one->dst.vbuf, &stride, &offset);
		break;
	}
	case vbuffmt_333:{
		UINT stride = 4*3*3;
		UINT offset = 0;
		g_dx11context->IASetVertexBuffers(0, 1, (ID3D11Buffer**)&one->dst.vbuf, &stride, &offset);
		break;
	}
	case vbuffmt_444:{
		UINT stride = 4*4*3;
		UINT offset = 0;
		g_dx11context->IASetVertexBuffers(0, 1, (ID3D11Buffer**)&one->dst.vbuf, &stride, &offset);
		break;
	}
	}//switch

	if(vtx->ibuf){
		g_dx11context->IASetIndexBuffer((ID3D11Buffer*)one->dst.ibuf, DXGI_FORMAT_R16_UINT, 0);
		switch(vtx->geometry){
		case 1:
			g_dx11context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
			g_dx11context->DrawIndexed(vtx->ibuf_h*1, 0, 0);
			break;
		case 2:
			g_dx11context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
			g_dx11context->DrawIndexed(vtx->ibuf_h*2, 0, 0);
			break;
		case 3:
			g_dx11context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			g_dx11context->DrawIndexed(vtx->ibuf_h*3, 0, 0);
			break;
		}
	}//drawindex

	else{
		switch(vtx->geometry){
		case 1:
			g_dx11context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
			g_dx11context->Draw(vtx->vbuf_h*1, 0);
			break;
		case 2:
			g_dx11context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);
			g_dx11context->Draw(vtx->vbuf_h*2, 0);
			break;
		case 3:
			g_dx11context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
			g_dx11context->Draw(vtx->vbuf_h*3, 0);
			break;
		}
	}//drawarray
}
void Render_all(struct dx11data** cam, struct dx11data** lit, struct dx11data** solid, struct dx11data** opaque, _obj* wnd, struct fstyle* area)
{
	//viewport
	float x0 = area->vc[0] * wnd->whdf.fbwidth;
	float y0 = area->vc[1] * wnd->whdf.fbheight;
	float ww = area->vq[0] * wnd->whdf.fbwidth;
	float hh = area->vq[1] * wnd->whdf.fbheight;

	D3D11_VIEWPORT vp = {0};
	vp.TopLeftX = x0;
	vp.TopLeftY = wnd->whdf.fbheight - (y0+hh);
	vp.Width	= ww;
	vp.Height	= hh;
	vp.MinDepth = 0.0f;
	vp.MaxDepth = 1.0f;
	g_dx11context->RSSetViewports(1,&vp);
	g_dx11context->RSSetState(g_rasterstate);

	//clear
	g_dx11context->ClearDepthStencilView(g_depthStencilView,D3D11_CLEAR_DEPTH|D3D11_CLEAR_STENCIL,1.f,0);
/*
	//debug
	if(1){
		dx11easy_render(g_dx11context);
		return;
	}
*/
	int j;
	float blendFactor[] = { 0.0f, 0.0f, 0.0f, 0.0f };

	g_dx11context->OMSetBlendState(0, blendFactor, 0xffffffff);
	g_dx11context->OMSetDepthStencilState(0, 1);
	for(j=0;j<64;j++){
		if(0 == solid[j])continue;
		Render_one(cam[0], lit[0], solid[j]);
	}//solid

	g_dx11context->OMSetBlendState(g_blendstate, blendFactor, 0xffffffff);
	g_dx11context->OMSetDepthStencilState(g_depthstate, 1);
	for(j=0;j<64;j++){
		if(0 == opaque[j])continue;
		Render_one(cam[0], lit[0], opaque[j]);
	}//opaque
}




void FreeTEXcd()
{
	g_dx11context->OMSetRenderTargets(0, 0, 0);
	g_depthStencilView->Release();
	g_renderTargetView->Release();
	g_dx11context->Flush();
}
BOOL InitTEXcd(_obj* wnd)
{
	// e.创建渲染目标视图
	ID3D11Texture2D *backBuffer(NULL);
	g_dx11swapchain->GetBuffer(0,__uuidof(ID3D11Texture2D),reinterpret_cast<void**>(&backBuffer));

	HRESULT hr = g_dx11device->CreateRenderTargetView(backBuffer,NULL,&g_renderTargetView);
	if(FAILED(hr))
	{
		MessageBox(NULL, "CreateRenderTargetView", "error",MB_OK);
		return FALSE;
	}
	backBuffer->Release();

	// f.创建深度缓冲区和其视图
	UINT m4xMsaaQuality(0);
	g_dx11device->CheckMultisampleQualityLevels(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		4,
		&m4xMsaaQuality
	);

	D3D11_TEXTURE2D_DESC depthStencilDesc = {0};
	depthStencilDesc.Width				= wnd->whdf.width;
	depthStencilDesc.Height				= wnd->whdf.height;
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
	depthStencilBuffer->Release();

	return TRUE;
}




void FreeD3D11()
{
	g_dx11swapchain->Release();
	g_dx11context->Release();
	g_dx11device->Release();
}
BOOL InitD3D11(_obj* wnd)
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
		&m4xMsaaQuality
	);

	// c.准备交换链属性
	DXGI_SWAP_CHAIN_DESC sd = {0};
	sd.BufferDesc.Width = wnd->whdf.width;
	sd.BufferDesc.Height = wnd->whdf.height;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	sd.SampleDesc.Count = 4;
	sd.SampleDesc.Quality = m4xMsaaQuality-1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = (HWND)wnd->dx11list.hwnd;
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

	//cull
	D3D11_RASTERIZER_DESC culldesc;
	ZeroMemory(&culldesc, sizeof(D3D11_RASTERIZER_DESC));
	culldesc.FillMode = D3D11_FILL_SOLID;
	culldesc.CullMode = D3D11_CULL_NONE;
	hr = g_dx11device->CreateRasterizerState(&culldesc, &g_rasterstate);
	if(FAILED(hr)){
		MessageBox(NULL, "CreateRasterizerState", "Error", MB_OK);
	}

	//blend
	D3D11_BLEND_DESC blenddesc;
	blenddesc.AlphaToCoverageEnable = false;
	blenddesc.IndependentBlendEnable = false;
	blenddesc.RenderTarget[0].BlendEnable = true;
	blenddesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	blenddesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	blenddesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blenddesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blenddesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	blenddesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blenddesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	hr = g_dx11device->CreateBlendState(&blenddesc, &g_blendstate);
	if(FAILED(hr)){
		MessageBox(NULL, "CreateBlendState", "Error", MB_OK);
	}
 
	//depth
	D3D11_DEPTH_STENCIL_DESC depthdesc;
	depthdesc.DepthEnable      = true;
	depthdesc.DepthWriteMask   = D3D11_DEPTH_WRITE_MASK_ZERO;
	depthdesc.DepthFunc        = D3D11_COMPARISON_LESS;
	depthdesc.StencilEnable    = false;
	depthdesc.StencilReadMask  = D3D11_DEFAULT_STENCIL_READ_MASK;
	depthdesc.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	hr = g_dx11device->CreateDepthStencilState(&depthdesc, &g_depthstate);
	if(FAILED(hr)){
		MessageBox(NULL, "CreateDepthStencilState", "Error", MB_OK);
	}

	return TRUE;
}




static void restorestackdeliverevent(_obj* wnd, struct event* ev)
{
	u64* save = (u64*)wnd->dx11list.spsave;
	if(0 == save){
		eventwrite(ev->why, ev->what, ev->where, 0);
		return;
	}

	struct halfrel* stack = (struct halfrel*)save[0];
	int sp = save[1];

	//get vertex
	struct relation* rel = (struct relation*)wnd->oreln;
	if(0 == rel)return;
	stack[sp+0].pchip = rel->psrcchip;
	stack[sp+0].pfoot = rel->psrcfoot;
	//stack[sp+0].type = rel->srctype;
	stack[sp+0].foottype = rel->srcfoottype;
	stack[sp+1].pchip = rel->pdstchip;
	stack[sp+1].pfoot = rel->pdstfoot;
	//stack[sp+1].type = rel->dsttype;
	stack[sp+1].foottype = rel->dstfoottype;
	entity_giveby((_obj*)rel->pdstchip, rel->pdstfoot, stack,sp+2, 0,0, ev, 0);
}
LRESULT CALLBACK WinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	struct event ev;
	u64 addr = GetWindowLongPtr(hwnd, GWLP_USERDATA);
	_obj* win = (_obj*)addr;

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
			if(SIZE_MINIMIZED == wparam)break;

			int w = lparam&0xffff;
			int h = (lparam>>16)&0xffff;
			printf("wm_size:%d,%d\n", w, h);

			if(win){
				FreeTEXcd();

				win->whdf.fbwidth = win->whdf.width = w;
				win->whdf.fbheight= win->whdf.height= h;

				g_dx11swapchain->ResizeBuffers(0,0,0, DXGI_FORMAT_UNKNOWN, 0);

				InitTEXcd(win);
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
BOOL InitWin32(_obj* wnd)
{
	RECT tmp;
	tmp.left = 0;
	tmp.right = wnd->whdf.width;
	tmp.top = 0;
	tmp.bottom = wnd->whdf.height;
	AdjustWindowRect(&tmp, WS_OVERLAPPEDWINDOW, FALSE);

	HWND hwnd = CreateWindow(
		"d3d11",
		"d3d11",
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		tmp.right-tmp.left,
		tmp.bottom-tmp.top,
		NULL,
		NULL,
		0,
		NULL
	);
	if(!hwnd){
		MessageBox(NULL, "CreateWindow", "error",MB_OK);
		return FALSE;
	}

	ShowWindow(hwnd,SW_SHOW);
	UpdateWindow(hwnd);

	SetWindowLongPtr(hwnd, GWLP_USERDATA, (u64)wnd);
	wnd->dx11list.hwnd = hwnd;
	return TRUE;
}




extern "C" {
int fullwindow_taking(_obj* wnd,void* foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
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
			stack[sp+0].foottype = rel->srcfoottype;
			stack[sp+1].pchip = rel->pdstchip;
			stack[sp+1].pfoot = rel->pdstfoot;
			//stack[sp+1].type = rel->dsttype;
			stack[sp+1].foottype = rel->dstfoottype;
			entity_takeby((_obj*)rel->pdstchip, rel->pdstfoot, stack,sp+2, 0,'v', 0, 0);
		}

		//give
		Upload_all(wnd->dx11list.world[0].camera, wnd->dx11list.world[0].light, wnd->dx11list.world[0].solid, wnd->dx11list.world[0].opaque);

		//draw
		Render_all(wnd->dx11list.world[0].camera, wnd->dx11list.world[0].light, wnd->dx11list.world[0].solid, wnd->dx11list.world[0].opaque, wnd, area);

		//next
		rel = (struct relation*)samesrcnextdst(rel);
	}
	return 0;
}
int fullwindow_giving(_obj* wnd,void* foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	return 0;
}




int window_take(_obj* wnd,void* foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	//target
	g_dx11context->OMSetRenderTargets(1, &g_renderTargetView, g_depthStencilView);

	//clear
	float color[4] = {0.1, 0.1, 0.1, 1.0};
	g_dx11context->ClearRenderTargetView(g_renderTargetView,reinterpret_cast<float*>(&color));

	//read
	fullwindow_taking(wnd,foot, stack,sp, arg,key, buf,len);

	//show
	g_dx11swapchain->Present(0,0);




	//temp
	u64 save[2];
	save[0] = (u64)stack;
	save[1] = sp;
	wnd->dx11list.spsave = save;

	//event
	MSG msg = {0};
	while(PeekMessage(&msg, 0, 0, 0, PM_REMOVE)){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	wnd->dx11list.spsave = 0;
	return 0;
}
int window_give(_obj* wnd,void* foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	return 0;
}
int window_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int window_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}




int window_read(_obj* wnd)
{
	return 0;
}
int window_write(_obj* wnd)
{
	return 0;
}
int window_delete(_obj* wnd)
{
	dx11easy_delete();
	FreeTEXcd();
	FreeD3D11();
	FreeWin32();
	return 0;
}
int window_create(_obj* wnd)
{
	//wnd->tier
	//wnd->type
	wnd->hfmt = _dx11list_;
	wnd->vfmt= _dx11list_;

	wnd->whdf.width = wnd->whdf.fbwidth = 1024;
	wnd->whdf.height= wnd->whdf.fbheight= 768;
	wnd->dx11list.world[0].camera = (struct dx11data**)memorycreate(0x10000, 0);
	wnd->dx11list.world[0].light  = (struct dx11data**)memorycreate(0x10000, 0);
	wnd->dx11list.world[0].solid  = (struct dx11data**)memorycreate(0x10000, 0);
	wnd->dx11list.world[0].opaque = (struct dx11data**)memorycreate(0x10000, 0);

	if(!InitWin32(wnd))return -1;
	if(!InitD3D11(wnd))return -1;
	if(!InitTEXcd(wnd))return -1;
	dx11easy_create();

	return 0;
}




void* window_alloc()
{
	return supply_alloc();
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
