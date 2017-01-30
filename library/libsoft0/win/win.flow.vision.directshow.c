#include <windows.h>
#include <dshow.h>
#include <rpc.h>
#include <rpcndr.h>
#define u8 unsigned char
#define u16 unsigned short
#define u32 unsigned int
#define u64 unsigned long long
#pragma comment(lib, "ole32")
#pragma comment(lib, "strmiids")
EXTERN_C const CLSID CLSID_NullRenderer;
EXTERN_C const CLSID CLSID_SampleGrabber;
EXTERN_C void eventwrite(u64,u64,u64,u64);

#ifndef SAMPLE_GRABBER_H
#define SAMPLE_GRABBER_H
DEFINE_GUID(IID_ISampleGrabberCB, 0x0579154A, 0x2B53, 0x4994, 0xB0, 0xD0, 0xE7, 0x73, 0x14, 0x8E, 0xFF, 0x85);
MIDL_INTERFACE("0579154A-2B53-4994-B0D0-E773148EFF85")
ISampleGrabberCB : public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE SampleCB(double SampleTime,IMediaSample* pSample) = 0;
    virtual HRESULT STDMETHODCALLTYPE BufferCB(double SampleTime,BYTE *pBuffer,long BufferLen) = 0;
};

DEFINE_GUID(IID_ISampleGrabber, 0x6B652FFF, 0x11FE, 0x4fce, 0x92, 0xAD, 0x02, 0x66, 0xB5, 0xD7, 0xC7, 0x8F);
MIDL_INTERFACE("6B652FFF-11FE-4fce-92AD-0266B5D7C78F")
ISampleGrabber : public IUnknown
{
public:
    virtual HRESULT STDMETHODCALLTYPE SetOneShot(BOOL OneShot) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetMediaType(const AM_MEDIA_TYPE *pType) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetConnectedMediaType(AM_MEDIA_TYPE *pType) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetBufferSamples(BOOL BufferThem) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetCurrentBuffer(long *pBufferSize,long *pBuffer) = 0;
    virtual HRESULT STDMETHODCALLTYPE GetCurrentSample(IMediaSample **ppSample) = 0;
    virtual HRESULT STDMETHODCALLTYPE SetCallback(ISampleGrabberCB *pCallback,long WhichMethodToCallback) = 0;
};
#endif




class CallbackObject : public ISampleGrabberCB
{
public:
	CallbackObject() {};
	STDMETHODIMP QueryInterface(REFIID riid, void **ppv)
	{
		if (NULL == ppv) return E_POINTER;
		printf("%llx\n",riid);
		if (riid == __uuidof(IUnknown))
		{
			*ppv = static_cast<IUnknown*>(this);
			return S_OK;
		}
		//if (riid == __uuidof(ISampleGrabberCB))   {
		else
		{
			*ppv = static_cast<ISampleGrabberCB*>(this);
			return S_OK;
		}
		return E_NOINTERFACE;
	}
	STDMETHODIMP_(ULONG) AddRef() {return S_OK;}
	STDMETHODIMP_(ULONG) Release(){return S_OK;}

	//ISampleGrabberCB
	STDMETHODIMP SampleCB(double SampleTime, IMediaSample *pSample);
	STDMETHODIMP BufferCB(double SampleTime, BYTE *pBuffer, long BufferLen){return S_OK;}
};
STDMETHODIMP CallbackObject::SampleCB(double SampleTime, IMediaSample *pSample)
{
	BYTE *p = NULL;
	pSample->GetPointer(&p);
	//printf("%x %x %x %x\n",p[0],p[1],p[2],p[3]);
	eventwrite((u64)p, 'v', 0, 0);
	return S_OK;

	if (!pSample)return E_POINTER;
	long sz = pSample->GetActualDataLength();
	pSample->GetPointer(&p);
	if (sz <= 0 || p==NULL) return E_UNEXPECTED;
	pSample->Release();
	return S_OK;
}




int enumdev(void* device, int which)
{
	//enumerator
	ICreateDevEnum *pDevEnum = NULL;
	IEnumMoniker *pEnum = NULL;
	IMoniker *pMoniker = NULL;
	IPropertyBag *pPropBag;
	VARIANT var;
	HRESULT hr;
	int flag = -1;

	//Create the System Device Enumerator.
	hr = CoCreateInstance(
		CLSID_SystemDeviceEnum, NULL,  
		CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pDevEnum)
	);
	if(FAILED(hr)){printf("error@ICreateDevEnum\n");return -1;}

	//Create an enumerator for the category.
	hr = pDevEnum->CreateClassEnumerator(CLSID_VideoInputDeviceCategory, &pEnum, 0);
	if (hr == S_FALSE){printf("error@IEnumMoniker\n");return -2;}

	while (pEnum->Next(1, &pMoniker, NULL) == S_OK)
	{
		hr = pMoniker->BindToStorage(0, 0, IID_PPV_ARGS(&pPropBag));
		if (FAILED(hr))
		{
			pMoniker->Release();
			continue;  
		} 

		var.vt = VT_BSTR;
/*
		hr = pPropBag->Read(L"DevicePath", &var, 0);
		if (SUCCEEDED(hr))
		{
			// The device path is not intended for display.
			printf("Device path: %S\n", var.bstrVal);
		}
*/

		// Get description or friendly name.
		hr = pPropBag->Read(L"Description", &var, 0);
		if (FAILED(hr))
		{
			hr = pPropBag->Read(L"FriendlyName", &var, 0);
		}
		if (SUCCEEDED(hr))
		{
			if(which == 0)
			{
				hr = pMoniker->BindToObject(0, 0, IID_IBaseFilter, (void**)device);
				if(FAILED(hr))printf("error@bindtoobject\n");
				else
				{
					printf("%S	**selected**\n", var.bstrVal);
					flag = 1;
				}
			}
			else printf("%S\n", var.bstrVal);

			which--;
		}

		pPropBag->Release();
		pMoniker->Release();
	}

	//
	pEnum->Release();
	pDevEnum->Release();
	return flag;
}
HRESULT enumpin(IBaseFilter * pFilter, PIN_DIRECTION dirrequired, int iNum, IPin **ppPin)
{
    IEnumPins* pEnum;
    *ppPin = NULL;

    HRESULT hr = pFilter->EnumPins(&pEnum);
    if(FAILED(hr)) return hr;

    ULONG ulFound;
    IPin *pPin;
    hr = E_FAIL;

    while(S_OK == pEnum->Next(1, &pPin, &ulFound))
    {
        PIN_DIRECTION pindir = (PIN_DIRECTION)3;

        pPin->QueryDirection(&pindir);
        if(pindir == dirrequired)
        {
            if(iNum == 0)
            {
                *ppPin = pPin;  // Return the pin's interface
                hr = S_OK;      // Found requested pin, so clear error
                break;
            }
            iNum--;
        } 

        pPin->Release();
    } 

    return hr;
}
IPin* GetInPin( IBaseFilter * pFilter, int nPin )
{
    IPin* pComPin=0;
    enumpin(pFilter, PINDIR_INPUT, nPin, &pComPin);
    return pComPin;
}
int enumfmt(IPin* pin, IAMStreamConfig* pConfig)
{
	IEnumMediaTypes *mtEnum=NULL;  
    AM_MEDIA_TYPE   *mt=NULL;  
    if( FAILED( pin->EnumMediaTypes( &mtEnum )) )return -1;
    mtEnum->Reset();

	VIDEOINFOHEADER* head;
	BITMAPINFOHEADER* bmp;
    ULONG mtFetched = 0;
    while (SUCCEEDED(mtEnum->Next(1, &mt, &mtFetched)) && mtFetched)
	{
		if(mt->majortype == MEDIATYPE_Video)
		{
			printf("video,");
			if(mt->subtype == MEDIASUBTYPE_YUY2)
			{
				printf("yuyv,");
				if(mt->formattype == FORMAT_VideoInfo)
				{
					head = (VIDEOINFOHEADER*)(mt->pbFormat);
					bmp = &(head->bmiHeader);
					printf("%dx%d", bmp->biWidth, bmp->biHeight);

					if( (bmp->biWidth==640) && (bmp->biHeight==480) )
					{
						if(pConfig != 0)
						{
							pConfig->SetFormat(mt);
							printf("	***selected***");
						}
					}
				}
				else printf("???");
			}
			else printf("???");
		}
		else printf("???");

		printf("\n");
	}

	return 1;
}



extern "C" {


void listvision()
{
}
void choosevision()
{
}
void readvision()
{
}
void writevision()
{
}
void startvision()
{
	//builder
	HRESULT hr;
	IGraphBuilder* m_pGraph;
	ICaptureGraphBuilder2* m_pBuild;
	IMediaControl * g_pMC = NULL;
	IMediaEventEx * g_pME = NULL;
	IVideoWindow  * g_pVW = NULL;

	//src
	IBaseFilter* device = NULL;
	IPin* deviceout = 0;

	//dst
	IBaseFilter* sample = NULL;
	IPin* samplein = 0;
	ISampleGrabber* pGrabber = NULL;
	CallbackObject cb;

	IAMStreamConfig *pConfig = NULL;
	AM_MEDIA_TYPE mt;
	VIDEOINFOHEADER* head;
	BITMAPINFOHEADER* bmp;




	//com init
	hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if(FAILED(hr))goto end;

	//builder
	hr = CoCreateInstance(
		CLSID_CaptureGraphBuilder2, NULL,
		CLSCTX_INPROC_SERVER, IID_ICaptureGraphBuilder2,
		(void**)&m_pBuild
	);
	if(FAILED(hr)){printf("error %x@CLSID_CaptureGraphBuilder2\n",hr);goto fail;}

	hr = CoCreateInstance(
		CLSID_FilterGraph, NULL,
		CLSCTX_INPROC_SERVER, IID_IGraphBuilder,
		(void**)&m_pGraph
	);
	if(FAILED(hr)){printf("error %x@CLSID_FilterGraph\n",hr);goto fail;}

	hr = m_pBuild->SetFiltergraph(m_pGraph);
	if(FAILED(hr)){printf("error %x@SetFiltergraph\n",hr);goto fail;}

	hr = m_pGraph->QueryInterface(IID_IMediaControl,(LPVOID *) &g_pMC);
    if (FAILED(hr)){printf("mediacontrol\n");return;}

    hr = m_pGraph->QueryInterface(IID_IVideoWindow, (LPVOID *) &g_pVW);
    if (FAILED(hr)){printf("videowindow\n");return;}

    hr = m_pGraph->QueryInterface(IID_IMediaEventEx, (LPVOID *) &g_pME);
    if (FAILED(hr)){printf("mediaevent\n");return;}




	//device filter
	hr = enumdev(&device, 0);
	if(hr < 0)goto fail;
/*
	hr = m_pBuild->FindInterface(
		&PIN_CATEGORY_CAPTURE, 0,
		device, IID_IAMStreamConfig,
		(void**)&pConfig
	);
	if(FAILED(hr))goto fail;
*/
	hr = m_pGraph->AddFilter(device, L"Capture Filter");
	if(FAILED(hr)){printf("error %x@add device\n",hr);goto fail;}

	hr = enumpin(device, PINDIR_OUTPUT, 0, &deviceout);
	if(hr < 0)goto fail;

	hr = enumfmt(deviceout, 0);
	if(hr < 0)goto fail;




	//sample filter
	hr = CoCreateInstance(
		CLSID_SampleGrabber, NULL,
		CLSCTX_INPROC_SERVER, IID_IBaseFilter,
		(void**)&sample
	);
	if(FAILED(hr)){printf("error %x@sample create\n",hr);goto fail;}

	hr = sample->QueryInterface(IID_ISampleGrabber, (void**)&pGrabber);
	if(FAILED(hr)){printf("error %x@sample query\n",hr);goto fail;}

	hr = pGrabber->SetOneShot(FALSE);
	hr = pGrabber->SetBufferSamples(TRUE);

	hr = pGrabber->SetCallback(&cb, 0);
	if(FAILED(hr)){printf("error %x@sample callback\n",hr);goto fail;}

	hr = m_pGraph->AddFilter(sample, L"Sample Grabber");
	if(FAILED(hr)){printf("error %x@add sample\n",hr);goto fail;}

	//enumpin(sample, PINDIR_INPUT, 0, &samplein);
	samplein = GetInPin(sample, 0);

	hr = enumfmt(samplein, 0);




	//connect, start
    hr = m_pGraph->Connect(deviceout, samplein);
	if(FAILED(hr)){printf("error %x@graph connect\n",hr);goto fail;}

	hr = pGrabber->GetConnectedMediaType(&mt);
	if(FAILED(hr)){printf("error %x@media type\n",hr);goto fail;}

	head = (VIDEOINFOHEADER*)(mt.pbFormat);
	bmp = &(head->bmiHeader);
	printf("[mediatype]%dx%d\n", bmp->biWidth, bmp->biHeight);

	hr = g_pMC->Run();
	return;




fail:
	//com uninit
	CoUninitialize();
end:
	return;
}
void stopvision()
{
	CoUninitialize();
}
void createvision()
{
}
void deletevision()
{
}

}