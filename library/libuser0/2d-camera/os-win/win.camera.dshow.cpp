#include <windows.h>
#include <dshow.h>
#include <rpc.h>
#include <rpcndr.h>
#include "libuser.h"
#define _yuyv_ hex32('y','u','y','v')
#define _mjpg_ hex32('m','j','p','g')
#define STRIDE whdf.ix0
#define HEIGHT whdf.iy0
#define FORMAT whdf.iw0

#pragma comment(lib, "ole32")
#pragma comment(lib, "strmiids")
EXTERN_C const CLSID CLSID_NullRenderer;
EXTERN_C const CLSID CLSID_SampleGrabber;

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




struct pictureobject
{
	union{
		u64 addr;
		BYTE* buf;
	};
	u64 len;
};
static struct pictureobject obj[60];
//
static _obj* working;
static int enq = 0;




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
		if (riid == IID_ISampleGrabberCB)
		{
			*ppv = static_cast<ISampleGrabberCB*>(this);
			return S_OK;
		}
		return E_NOINTERFACE;
	}
	STDMETHODIMP_(ULONG) AddRef() {return S_OK;}
	STDMETHODIMP_(ULONG) Release(){return S_OK;}

	//ISampleGrabberCB
	STDMETHODIMP BufferCB(double SampleTime, BYTE *pBuffer, long BufferLen){
		return S_OK;
	}
	STDMETHODIMP SampleCB(double SampleTime, IMediaSample *pSample)
	{
		BYTE** buf = &(obj[enq].buf);
		obj[enq].len = pSample->GetActualDataLength();
		pSample->GetPointer(buf);
		pSample->Release();

		struct halfrel stack[0x80];
		give_data_into_peer(working,_dst_, stack,0, 0,0, obj[enq].buf,obj[enq].len);

		enq = (enq+1)%60;
		return S_OK;
	}
};
static CallbackObject cb;
//
static IMediaControl* g_pMC = NULL;
static IMediaEventEx* g_pME = NULL;
static IVideoWindow*  g_pVW = NULL;




HRESULT enumdev(void* dev, int which)
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
		hr = pPropBag->Read(L"devPath", &var, 0);
		if (SUCCEEDED(hr))
		{
			// The dev path is not intended for display.
			printf("dev path: %S\n", var.bstrVal);
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
				hr = pMoniker->BindToObject(0, 0, IID_IBaseFilter, (void**)dev);
				if(FAILED(hr))printf("error@bindtoobject\n");
				else
				{
					printf("%S	***selected***\n", var.bstrVal);
					flag = 1;
				}
			}
			else printf("%S\n", var.bstrVal);

			which--;
		}

		pPropBag->Release();
		pMoniker->Release();
	}
	printf("\n");

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




HRESULT configgraph(_obj* win, IAMStreamConfig* devcfg)
{
	int iCount = 0;
	int iSize = 0;
	HRESULT hr;

	hr = devcfg->GetNumberOfCapabilities(&iCount, &iSize);
	if(FAILED(hr)){
		printf("%x@GetNumberOfCapabilities\n",hr);
		return -2;
	}
	if(iSize != sizeof(VIDEO_STREAM_CONFIG_CAPS)){
		printf("iSize != sizeof(VIDEO_STREAM_CONFIG_CAPS)\n");
		return -3;
	}

	// Use the video capabilities structure.
	VIDEO_STREAM_CONFIG_CAPS scc;
	AM_MEDIA_TYPE *pmtConfig;
	for (int iFormat = 0; iFormat < iCount; iFormat++)
	{
		hr = devcfg->GetStreamCaps(iFormat, &pmtConfig, (BYTE*)&scc);
		if (FAILED(hr))continue;

		u32 tmpfmt = 0;
		if(MEDIASUBTYPE_YUY2 == pmtConfig->subtype)tmpfmt = _yuyv_;
		if(MEDIASUBTYPE_MJPG == pmtConfig->subtype)tmpfmt = _mjpg_;
		printf("%03d: %.4s\n",iFormat, &tmpfmt);

		if(MEDIATYPE_Video != pmtConfig->majortype)continue;
		if(FORMAT_VideoInfo == pmtConfig->formattype){
			VIDEOINFOHEADER* head = (VIDEOINFOHEADER*)(pmtConfig->pbFormat);
			BITMAPINFOHEADER* bmp = &head->bmiHeader;
			printf("FORMAT_VideoInfo1: %dx%d@%d\n",bmp->biWidth, bmp->biHeight, 10*1000*1000/head->AvgTimePerFrame);

			if(	(win->STRIDE == bmp->biWidth) &&
				(win->HEIGHT == bmp->biHeight) &&
				(win->FORMAT == tmpfmt) )
			{
				hr = devcfg->SetFormat(pmtConfig);
				if(SUCCEEDED(hr))printf("success@SetFormat\n");
				else printf("errno=%d@SetFormat\n", hr);
			}
		}
		if(FORMAT_VideoInfo2 == pmtConfig->formattype){
			printf("FORMAT_VideoInfo2\n");
		}
	}//for

	printf("\n");
	return S_OK;
}
void shutupdie(_obj* win)
{
	if(g_pMC != 0)
	{
		g_pMC->Stop();
		g_pMC = 0;
	}
	CoUninitialize();
}
void letsgo(_obj* win)
{
	//builder
	HRESULT hr;
	IGraphBuilder* m_pGraph;
	ICaptureGraphBuilder2* m_pBuild;

	//src
	IBaseFilter* dev = NULL;
	IPin* devout = 0;
	IAMStreamConfig *devcfg = NULL;

	//dst
	IBaseFilter* sample = NULL;
	IPin* samplein = 0;
	ISampleGrabber* pGrabber = NULL;

	AM_MEDIA_TYPE mt;
	VIDEOINFOHEADER* head;
	BITMAPINFOHEADER* bmp;




	//com init
	hr = CoInitializeEx(NULL, COINIT_MULTITHREADED);
	if(FAILED(hr))goto fail;

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




	//dev filter
	hr = enumdev(&dev, 0);
	if(hr < 0)goto fail;

	hr = m_pGraph->AddFilter(dev, L"Capture Filter");
	if(FAILED(hr)){printf("error %x@add dev\n",hr);goto fail;}

	hr = enumpin(dev, PINDIR_OUTPUT, 0, &devout);
	if(hr < 0)goto fail;

	//hr = enumfmt(devout);
	//if(hr < 0)goto fail;




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

	hr = enumpin(sample, PINDIR_INPUT, 0, &samplein);

	//hr = enumfmt(samplein);




	//connect, connect, start
	hr = m_pGraph->Connect(devout, samplein);
	if(FAILED(hr)){printf("error %x@graph connect\n",hr);goto fail;}

	hr = m_pBuild->FindInterface(
		&PIN_CATEGORY_CAPTURE, &MEDIATYPE_Video,
		dev, IID_IAMStreamConfig,
		(void**)&devcfg
	);
	if(FAILED(hr)){printf("%x@findinterface\n",hr);goto fail;}

	hr = configgraph(win, devcfg);
/*
	hr = pGrabber->GetConnectedMediaType(&mt);
	if(FAILED(hr)){printf("error %x@media type\n",hr);goto fail;}

	head = (VIDEOINFOHEADER*)(mt.pbFormat);
	bmp = &head->bmiHeader;
	printf("GetConnectedMediaType: %dx%d~%d\n", bmp->biWidth, bmp->biHeight, 10*1000*1000/head->AvgTimePerFrame);
*/
	hr = g_pMC->Run();
	if(FAILED(hr)){printf("error %x@run\n",hr);goto fail;}

	return;




fail:
	return;
}




extern "C" {


int dshowcam_take(_obj* sup,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf,int len)
{
	u64 addr = obj[(enq+59)%60].addr;
	printf("addr=%llx\n",addr);

	*(u64*)buf = addr;
	return 0;
}
int dshowcam_give(_obj* sup,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf,int len)
{
	return 0;
}
int dshowcam_detach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int dshowcam_attach(struct halfrel* self, struct halfrel* peer)
{
	return 0;
}
int dshowcam_reader(_obj* sup,void* foot, p64 arg,int idx, void* buf,int len)
{
	return 0;
}
int dshowcam_writer(_obj* sup,void* foot, p64 arg,int idx, void* buf,int len)
{
	return 0;
}
int dshowcam_delete(_obj* win)
{
	//shutupdie(win);
	return 0;
}
int dshowcam_create(_obj* win, void* arg, int argc, u8** argv)
{
	int j;
	win->STRIDE = 640;
	win->HEIGHT = 480;
	win->FORMAT = _yuyv_;

	for(j=1;j<argc;j++){
		arg = argv[j];
		//logtoall("%d->%.16s\n",j,arg;
		if(0 == ncmp(arg, (void*)"format:", 7)){
			arg = argv[j]+7;
			//logtoall("format=%.5s\n",arg);
			if(0 == ncmp(arg, (void*)"mjpeg", 5))win->FORMAT = _mjpg_;
		}
		if(0 == ncmp(arg, (void*)"width:", 6)){
			arg = argv[j]+6;
			decstr2u32(arg, (u32*)&win->STRIDE);
		}
		if(0 == ncmp(arg, (void*)"height:", 7)){
			arg = argv[j]+7;
			decstr2u32(arg, (u32*)&win->HEIGHT);
		}
	}

	working = win;
	letsgo(win);
	//Sleep(5000);
	return 0;
}


}	//extern "C"
