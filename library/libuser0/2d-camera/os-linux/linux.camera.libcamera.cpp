/*
//usage example:
//https://github.com/kbingham/simple-cam/blob/master/simple-cam.cpp
*/

/*
//header define:
//https://www.libcamera.org/api-html/build_2include_2libcamera_2formats_8h_source.html
 constexpr uint32_t __fourcc(char a, char b, char c, char d)
 {
     return (static_cast<uint32_t>(a) <<  0) |
            (static_cast<uint32_t>(b) <<  8) |
            (static_cast<uint32_t>(c) << 16) |
            (static_cast<uint32_t>(d) << 24);
 }
 constexpr uint64_t __mod(unsigned int vendor, unsigned int mod)
 {
     return (static_cast<uint64_t>(vendor) << 56) |
            (static_cast<uint64_t>(mod) << 0);
 }
 constexpr PixelFormat R8{ __fourcc('R', '8', ' ', ' '), __mod(0, 0) };
 constexpr PixelFormat R10{ __fourcc('R', '1', '0', ' '), __mod(0, 0) };
 constexpr PixelFormat R12{ __fourcc('R', '1', '2', ' '), __mod(0, 0) };
 constexpr PixelFormat RGB565{ __fourcc('R', 'G', '1', '6'), __mod(0, 0) };
 constexpr PixelFormat RGB565_BE{ __fourcc('R', 'G', '1', '6'), __mod(0, 0) };
 constexpr PixelFormat RGB888{ __fourcc('R', 'G', '2', '4'), __mod(0, 0) };
 constexpr PixelFormat BGR888{ __fourcc('B', 'G', '2', '4'), __mod(0, 0) };
 constexpr PixelFormat XRGB8888{ __fourcc('X', 'R', '2', '4'), __mod(0, 0) };
 constexpr PixelFormat XBGR8888{ __fourcc('X', 'B', '2', '4'), __mod(0, 0) };
 constexpr PixelFormat RGBX8888{ __fourcc('R', 'X', '2', '4'), __mod(0, 0) };
 constexpr PixelFormat BGRX8888{ __fourcc('B', 'X', '2', '4'), __mod(0, 0) };
 constexpr PixelFormat ARGB8888{ __fourcc('A', 'R', '2', '4'), __mod(0, 0) };
 constexpr PixelFormat ABGR8888{ __fourcc('A', 'B', '2', '4'), __mod(0, 0) };
 constexpr PixelFormat RGBA8888{ __fourcc('R', 'A', '2', '4'), __mod(0, 0) };
 constexpr PixelFormat BGRA8888{ __fourcc('B', 'A', '2', '4'), __mod(0, 0) };
 constexpr PixelFormat YUYV{ __fourcc('Y', 'U', 'Y', 'V'), __mod(0, 0) };
 constexpr PixelFormat YVYU{ __fourcc('Y', 'V', 'Y', 'U'), __mod(0, 0) };
 constexpr PixelFormat UYVY{ __fourcc('U', 'Y', 'V', 'Y'), __mod(0, 0) };
 constexpr PixelFormat VYUY{ __fourcc('V', 'Y', 'U', 'Y'), __mod(0, 0) };
 constexpr PixelFormat AVUY8888{ __fourcc('A', 'V', 'U', 'Y'), __mod(0, 0) };
 constexpr PixelFormat XVUY8888{ __fourcc('X', 'V', 'U', 'Y'), __mod(0, 0) };
 constexpr PixelFormat NV12{ __fourcc('N', 'V', '1', '2'), __mod(0, 0) };
 constexpr PixelFormat NV21{ __fourcc('N', 'V', '2', '1'), __mod(0, 0) };
 constexpr PixelFormat NV16{ __fourcc('N', 'V', '1', '6'), __mod(0, 0) };
 constexpr PixelFormat NV61{ __fourcc('N', 'V', '6', '1'), __mod(0, 0) };
 constexpr PixelFormat NV24{ __fourcc('N', 'V', '2', '4'), __mod(0, 0) };
 constexpr PixelFormat NV42{ __fourcc('N', 'V', '4', '2'), __mod(0, 0) };
 constexpr PixelFormat YUV420{ __fourcc('Y', 'U', '1', '2'), __mod(0, 0) };
 constexpr PixelFormat YVU420{ __fourcc('Y', 'V', '1', '2'), __mod(0, 0) };
 constexpr PixelFormat YUV422{ __fourcc('Y', 'U', '1', '6'), __mod(0, 0) };
 constexpr PixelFormat YVU422{ __fourcc('Y', 'V', '1', '6'), __mod(0, 0) };
 constexpr PixelFormat YUV444{ __fourcc('Y', 'U', '2', '4'), __mod(0, 0) };
 constexpr PixelFormat YVU444{ __fourcc('Y', 'V', '2', '4'), __mod(0, 0) };
 constexpr PixelFormat MJPEG{ __fourcc('M', 'J', 'P', 'G'), __mod(0, 0) };
 constexpr PixelFormat SRGGB8{ __fourcc('R', 'G', 'G', 'B'), __mod(0, 0) };
 constexpr PixelFormat SGRBG8{ __fourcc('G', 'R', 'B', 'G'), __mod(0, 0) };
 constexpr PixelFormat SGBRG8{ __fourcc('G', 'B', 'R', 'G'), __mod(0, 0) };
 constexpr PixelFormat SBGGR8{ __fourcc('B', 'A', '8', '1'), __mod(0, 0) };
 constexpr PixelFormat SRGGB10{ __fourcc('R', 'G', '1', '0'), __mod(0, 0) };
 constexpr PixelFormat SGRBG10{ __fourcc('B', 'A', '1', '0'), __mod(0, 0) };
 constexpr PixelFormat SGBRG10{ __fourcc('G', 'B', '1', '0'), __mod(0, 0) };
 constexpr PixelFormat SBGGR10{ __fourcc('B', 'G', '1', '0'), __mod(0, 0) };
 constexpr PixelFormat SRGGB12{ __fourcc('R', 'G', '1', '2'), __mod(0, 0) };
 constexpr PixelFormat SGRBG12{ __fourcc('B', 'A', '1', '2'), __mod(0, 0) };
 constexpr PixelFormat SGBRG12{ __fourcc('G', 'B', '1', '2'), __mod(0, 0) };
 constexpr PixelFormat SBGGR12{ __fourcc('B', 'G', '1', '2'), __mod(0, 0) };
 constexpr PixelFormat SRGGB16{ __fourcc('R', 'G', 'B', '6'), __mod(0, 0) };
 constexpr PixelFormat SGRBG16{ __fourcc('G', 'R', '1', '6'), __mod(0, 0) };
 constexpr PixelFormat SGBRG16{ __fourcc('G', 'B', '1', '6'), __mod(0, 0) };
 constexpr PixelFormat SBGGR16{ __fourcc('B', 'Y', 'R', '2'), __mod(0, 0) };
 constexpr PixelFormat R10_CSI2P{ __fourcc('R', '1', '0', ' '), __mod(11, 1) };
 constexpr PixelFormat SRGGB10_CSI2P{ __fourcc('R', 'G', '1', '0'), __mod(11, 1) };
 constexpr PixelFormat SGRBG10_CSI2P{ __fourcc('B', 'A', '1', '0'), __mod(11, 1) };
 constexpr PixelFormat SGBRG10_CSI2P{ __fourcc('G', 'B', '1', '0'), __mod(11, 1) };
 constexpr PixelFormat SBGGR10_CSI2P{ __fourcc('B', 'G', '1', '0'), __mod(11, 1) };
 constexpr PixelFormat SRGGB12_CSI2P{ __fourcc('R', 'G', '1', '2'), __mod(11, 1) };
 constexpr PixelFormat SGRBG12_CSI2P{ __fourcc('B', 'A', '1', '2'), __mod(11, 1) };
 constexpr PixelFormat SGBRG12_CSI2P{ __fourcc('G', 'B', '1', '2'), __mod(11, 1) };
 constexpr PixelFormat SBGGR12_CSI2P{ __fourcc('B', 'G', '1', '2'), __mod(11, 1) };
 constexpr PixelFormat SRGGB10_IPU3{ __fourcc('R', 'G', '1', '0'), __mod(1, 13) };
 constexpr PixelFormat SGRBG10_IPU3{ __fourcc('B', 'A', '1', '0'), __mod(1, 13) };
 constexpr PixelFormat SGBRG10_IPU3{ __fourcc('G', 'B', '1', '0'), __mod(1, 13) };
 constexpr PixelFormat SBGGR10_IPU3{ __fourcc('B', 'G', '1', '0'), __mod(1, 13) };
*/
/*
AE_ENABLE = 1, AE_LOCKED = 2, AE_METERING_MODE = 3, AE_CONSTRAINT_MODE = 4,
AE_EXPOSURE_MODE = 5, EXPOSURE_VALUE = 6, EXPOSURE_TIME = 7, ANALOGUE_GAIN = 8,
BRIGHTNESS = 9, CONTRAST = 10, LUX = 11, AWB_ENABLE = 12,
AWB_MODE = 13, AWB_LOCKED = 14, COLOUR_GAINS = 15, COLOUR_TEMPERATURE = 16,
SATURATION = 17, SENSOR_BLACK_LEVELS = 18, SHARPNESS = 19, FOCUS_FO_M = 20,
COLOUR_CORRECTION_MATRIX = 21, SCALER_CROP = 22, DIGITAL_GAIN = 23, FRAME_DURATION = 24,
FRAME_DURATION_LIMITS = 25, SENSOR_TEMPERATURE = 26, SENSOR_TIMESTAMP = 27, AF_MODE = 28,
AF_RANGE = 29, AF_SPEED = 30, AF_METERING = 31, AF_WINDOWS = 32,
AF_TRIGGER = 33, AF_PAUSE = 34, LENS_POSITION = 35, AF_STATE = 36,
AF_PAUSE_STATE = 37, AE_PRECAPTURE_TRIGGER = 38, NOISE_REDUCTION_MODE = 39, COLOR_CORRECTION_ABERRATION_MODE = 40,
AE_STATE = 41, AWB_STATE = 42, SENSOR_ROLLING_SHUTTER_SKEW = 43, LENS_SHADING_MAP_MODE = 44,
SCENE_FLICKER = 45, PIPELINE_DEPTH = 46, MAX_LATENCY = 47, TEST_PATTERN_MODE = 48
7-ExposureTime = 66638
8-AnalogueGain = 8.000000
15-ColourCorrectionMatrix = [ 1.838451, -0.458022, -0.380436, -0.447965, 1.651210, -0.203245, 0.030786, -0.897781, 1.866994 ]
18-FrameDuration = 66773
b-Lux = 72.904442
2-AeLocked = true
f-ColourGains = [ 1.447118, 1.521400 ]
17-DigitalGain = 1.000415
10-ColourTemperature = 4264
12-SensorBlackLevels = [ 1024, 1024, 1024, 1024 ]
16-ScalerCrop = (16, 0)/2560x1920
1b-SensorTimestamp = 1725648992178000
*/
extern "C"{
#include "libuser.h"
#define _BG10_ hex32('B','G','1','0')
#define _GB10_ hex32('G','B','1','0')
u64 timeread_ns();
u64 timeread_from1970_ns();
}
#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <libcamera/libcamera.h>
using namespace libcamera;

struct mydata{
	_obj* myobj;
	int log;

	int choosecamera;
	u64 fmt;
	int fps;

	int w;
	int h;
	int stride;

	std::shared_ptr<Camera> cam;
	Stream* stream;

	FrameBufferAllocator* allocator;	//const std::vector<std::unique_ptr<FrameBuffer>> &buffers = my->allocator->buffers(my->stream);
	uint8_t* mem[8][3];	//void* mem = mmap(NULL, plane.length, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

	std::vector<std::unique_ptr<Request>> requests;
};

static void requestComplete(Request *request)
{
	struct kv88 kv[8] = {
		{'w', 0},
		{'h', 0},
		{_stride_, 0},
		{'f', 0},
		{'t', 0},
		{hex16('m','3'), 0},
		{hex16('p','l'), 0},		//plane
		{0,0}
	};
	if (request->status() == Request::RequestCancelled)return;

	uint64_t cookie = request->cookie();
	struct mydata* my = (struct mydata*)cookie;
	if(my->log){
		std::cout << "requestcookie: " << cookie << std::endl;
		std::cout << "requestcamera: " << my->cam << std::endl;
		uint64_t from1970 = timeread_from1970_ns();
		uint64_t fromboot = timeread_ns();
		std::cout << "from1970_ns: " << from1970 << std::endl;
		std::cout << "fromboot_ns: " << fromboot << std::endl;
	}

	u64 sensortime;
	void* ccm = 0;
	float m3[9];
	const ControlList &requestMetadata = request->metadata();
	for (const auto &ctrl : requestMetadata) {
		const ControlId *cid = controls::controls.at(ctrl.first);
		const ControlValue &value = ctrl.second;
		int id = cid->id();
		Span<const uint8_t> sp;
		Span<const float> sf;
		switch(id){
		case libcamera::controls::SENSOR_TIMESTAMP:
			sensortime = value.get<int64_t>();		//must use template int64_t
/*			sp = value.data();
			sensortime = sp[7];
			sensortime = (sensortime<<8) | sp[6];
			sensortime = (sensortime<<8) | sp[5];
			sensortime = (sensortime<<8) | sp[4];
			sensortime = (sensortime<<8) | sp[3];
			sensortime = (sensortime<<8) | sp[2];
			sensortime = (sensortime<<8) | sp[1];
			sensortime = (sensortime<<8) | sp[0];
*/
			if(my->log)std::cout << "\t" << id <<"-SENSOR_TIMESTAMP_ns = " << std::dec<< sensortime << std::endl;
			break;
		case libcamera::controls::COLOUR_CORRECTION_MATRIX:
			if(my->log)std::cout << "\t" << id <<"-COLOUR_CORRECTION_MATRIX = " << value.toString() << std::endl;
			sf = value.get<Span<const float>>();
            //std::cout << sf[0] << " " << sf[1] << " " << sf[2] << " " << sf[3] << std::endl;
			for(int k=0;k<9;k++)m3[k] = sf[k];
			ccm = m3;
			break;
		default:
			if(my->log)std::cout << "\t" << id <<"-"<< cid->name() << " = " << value.toString() << std::endl;
		}
	}

	const Request::BufferMap &buffers = request->buffers();
	for (auto bufferPair : buffers) {
		// (Unused) Stream *stream = bufferPair.first;
		FrameBuffer *buffer = bufferPair.second;
		uint64_t idx = buffer->cookie();
		if(my->log)std::cout << "\tbuffercookie: " << idx << std::endl;

		const FrameMetadata &metadata = buffer->metadata();
		if(my->log)std::cout << "\tbuffersequence: " << metadata.sequence << std::endl;
		if(my->log)std::cout << "\tbuffertime_ns: " << metadata.timestamp << std::endl;

		auto planedata = buffer->planes();
		auto planemeta = metadata.planes();
		for(int j=0;j<planedata.size();j++){
			SharedFD fd = planedata[j].fd;
			uint32_t off = planedata[j].offset;
			uint32_t len = planedata[j].length;
			int sz = planemeta[j].bytesused;
			if(my->log)std::cout << "\tplane: " << j << std::endl;
			if(my->log)std::cout << "\tfd: " << fd.get() << " off: " << off << std::endl;
			if(my->log)std::cout << "\tsz: " << sz << " in " << len << std::endl;

			uint8_t* p = my->mem[idx][j] + off;
			//printmemory(p, 0x20);
			kv[0].val = my->w;
			kv[1].val = my->h;
			kv[2].val = my->stride;
			kv[3].val = my->fmt;
			kv[4].val = sensortime;
			kv[5].val = (u64)ccm;
			kv[6].val = j;
			give_data_into_peer_temp_stack(my->myobj,_dst_, (p64)kv,_kv88_, p, sz);
		}
	}

	request->reuse(Request::ReuseBuffers);
	my->cam->queueRequest(request);
}



std::string cameraName(Camera *camera)
{
	const ControlList &props = camera->properties();
	std::string name;

	const auto &location = props.get(properties::Location);
	if (location) {
		switch (*location) {
		case properties::CameraLocationFront:
			name = "Internal front camera";
			break;
		case properties::CameraLocationBack:
			name = "Internal back camera";
			break;
		case properties::CameraLocationExternal:
			name = "External camera";
			const auto &model = props.get(properties::Model);
			if (model)name = *model;
			break;
		}
	}

	//name += " (" + camera->id() + ")";
	return name;
}

//one camera manager in one process !
std::unique_ptr<CameraManager> g_cammgr = 0;
int ensurecameramanager()
{
	if(g_cammgr)return 1;

	g_cammgr = std::make_unique<CameraManager>();
	//if()return 0;

	g_cammgr->start();
	//if()return 0;

	return 1;
}
int createcamera(struct mydata* my){
	int mgrok = ensurecameramanager();
	if(0 == mgrok){
		logtoall((void*)"camera manager not ok!\n");
		return 0;
	}

	auto camlist = g_cammgr->cameras();
	if(camlist.empty()){
		std::cout << "no camera" << std::endl;
		return 0;
	}

	int idx = 0;
	int chosen = my->choosecamera;
	for (auto const &camera : camlist){
		std::cout << idx;
		std::cout << ": cameraName=" << cameraName(camera.get());
		std::cout << ", id=" << camera->id();
		std::cout << (chosen==idx?", (chosen)":"") << std::endl;
		idx += 1;
	}

	std::string id = camlist[chosen]->id();
	my->cam = g_cammgr->get(id);
	my->cam->acquire();

	//camera config
	std::unique_ptr<CameraConfiguration> camcfg;
	if(0 == my->fmt)camcfg = my->cam->generateConfiguration({StreamRole::Raw});
	else camcfg = my->cam->generateConfiguration({StreamRole::Viewfinder});
	//std::unique_ptr<CameraConfiguration> camcfg = my->cam->generateConfiguration({StreamRole::VideoRecording});
	//std::unique_ptr<CameraConfiguration> camcfg = my->cam->generateConfiguration({StreamRole::StillCapture});

	//stream config
	std::cout << "streamconfig list:" << std::endl;
	for(auto cfg : *camcfg){
		std::cout << "config: " << cfg.toString() << std::endl;
	}

	StreamConfiguration &stmcfg = camcfg->at(0);
	std::cout << "default streamconfig = " << stmcfg.toString() << std::endl;

	//foramt
	std::cout << "supported format:" << std::endl;
	auto fmtlist = stmcfg.formats();
	auto enumfmt = fmtlist.pixelformats();
	for(auto& fmt: enumfmt){
		std::cout << "format: " << fmt.toString() <<std::hex<< " fourcc=" << fmt.fourcc() <<std::hex<< " modifier=" << fmt.modifier() << std::endl;
		if(my->log){
		auto enumsize = fmtlist.sizes(fmt);
		for(auto& sz: enumsize){
			std::cout << "size: " << sz.toString() << std::endl;
		}
		}
	}

	//if format wanted in config, try my format
	stmcfg.size.width = my->w;
	stmcfg.size.height = my->h;
	switch(my->fmt){
	case _yuyv_:
		stmcfg.pixelFormat = libcamera::formats::YUYV;
		stmcfg.stride = my->w * 2;
		break;
	case _uyvy_:
		stmcfg.pixelFormat = libcamera::formats::UYVY;
		stmcfg.stride = my->w * 2;
		break;
	case _y4_uv_:
		stmcfg.pixelFormat = libcamera::formats::NV12;
		stmcfg.stride = my->w;
		break;
	case _y4_vu_:
		stmcfg.pixelFormat = libcamera::formats::NV21;
		stmcfg.stride = my->w;
		break;
	case _y4_u_v_:
		stmcfg.pixelFormat = libcamera::formats::YUV420;
		stmcfg.stride = my->w;
		break;
	case _y4_v_u_:
		stmcfg.pixelFormat = libcamera::formats::YVU420;
		stmcfg.stride = my->w;
		break;
	}

	//check is it valid
	logtoall((void*)"calling validate\n");
	switch(camcfg->validate()){
	case libcamera::CameraConfiguration::Invalid:
		logtoall((void*)"Invalid\n");
		break;
	case libcamera::CameraConfiguration::Adjusted:
		logtoall((void*)"Adjusted\n");
		std::cout<<"adjusted streamconfig="<<stmcfg.toString()<<std::endl;
		break;
	case libcamera::CameraConfiguration::Valid:
		logtoall((void*)"Valid\n");
		break;
	default:
		logtoall((void*)"unknown\n");
		break;
	}

	//if format not set, convert libcam fmt to my fmt
	u32 libcam_width = stmcfg.size.width;
	u32 libcam_height = stmcfg.size.height;
	u32 libcam_stride = stmcfg.stride;
	u32 libcam_framesize = stmcfg.frameSize;
	u32 libcam_buffercount = stmcfg.bufferCount;
	u32 libcam_fourcc = stmcfg.pixelFormat.fourcc();
	u64 libcam_modifier = stmcfg.pixelFormat.modifier();
	switch(stmcfg.pixelFormat){
	case libcamera::formats::SBGGR10_CSI2P:
		my->fmt = _pBAA_;
		break;
	case libcamera::formats::SGBRG10_CSI2P:
		my->fmt = _pGAA_;
		break;
	case libcamera::formats::YUYV:
		my->fmt = _yuyv_;
		break;
	case libcamera::formats::UYVY:
		my->fmt = _uyvy_;
		break;
	case libcamera::formats::NV12:
		my->fmt = _y4_uv_;
		break;
	case libcamera::formats::NV21:
		my->fmt = _y4_vu_;
		break;
	case libcamera::formats::YUV420:
		my->fmt = _y4_u_v_;
		break;
	case libcamera::formats::YVU420:
		my->fmt = _y4_v_u_;
		break;
	case libcamera::formats::SBGGR16:
	default:
		my->fmt = libcam_fourcc;
		break;
	}
	my->stride = libcam_stride;
	logtoall((void*)"final param: width=%d,height=%d, stride=%d,framesize=%d,buffercount=%d, fourcc=%x,modifier=%llx\n",
		libcam_width, libcam_height,
		libcam_stride, libcam_framesize, libcam_buffercount,
		libcam_fourcc, libcam_modifier);

	//camera config
	int cfgret = my->cam->configure(camcfg.get());
	if(cfgret){
		logtoall((void*)"configure failed\n");
	}

	my->allocator = new FrameBufferAllocator(my->cam);
	for (StreamConfiguration &cfg : *camcfg) {
		int ret = my->allocator->allocate(cfg.stream());
		if (ret < 0) {
			std::cerr << "Can't allocate buffers" << std::endl;
			return EXIT_FAILURE;
		}

		size_t allocated = my->allocator->buffers(cfg.stream()).size();
		std::cout << "Allocated " << allocated << " buffers for stream" << std::endl;
	}

	//StreamConfiguration &streamConfig = camcfg->at(0);
	my->stream = stmcfg.stream();
	const std::vector<std::unique_ptr<FrameBuffer>> &buffers = my->allocator->buffers(my->stream);

	for (unsigned int i = 0; i < buffers.size(); ++i) {
		const std::unique_ptr<FrameBuffer> &buffer = buffers[i];
		for(unsigned int j = 0;j<buffer->planes().size();j++){
			const FrameBuffer::Plane &plane = buffer->planes()[j];
			int fd = plane.fd.get();
			int off = plane.offset;
			void* mem = mmap(NULL, off+plane.length, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
			std::cout << i << "." << j << ", fd=" << fd << ",mem=" << mem << std::endl;
			my->mem[i][j] = (uint8_t*)mem;
		}
		buffer->setCookie(i);

		std::unique_ptr<Request> request = my->cam->createRequest((uint64_t)my);
		if (!request)
		{
			std::cerr << "Can't create request" << std::endl;
			return EXIT_FAILURE;
		}

		int ret = request->addBuffer(my->stream, buffer.get());
		if (ret < 0)
		{
			std::cerr << "Can't set buffer for request"
				  << std::endl;
			return EXIT_FAILURE;
		}

		//Controls can be added to a request on a per frame basis.
		//ControlList &controls = request->controls();
		//controls.set(controls::Brightness, 0.5);

		my->requests.push_back(std::move(request));
	}

	std::cout << "wait 3s" << std::endl;
	usleep(1000*1000);
	std::cout << "wait 2s" << std::endl;
	usleep(1000*1000);
	std::cout << "wait 1s" << std::endl;
	usleep(1000*1000);
	std::cout << "start now" << std::endl;

	my->cam->requestCompleted.connect(requestComplete);
	my->cam->start();
	for (std::unique_ptr<Request> &request : my->requests)
		my->cam->queueRequest(request.get());
	return 0;
}
int deletecamera(struct mydata* my){
	my->cam->stop();

	my->allocator->free(my->stream);
	delete my->allocator;

	my->cam->release();

	//todo: delete camera manager
	//my->cm->stop();
	return 0;
}


extern "C" {


int libcam_take(_obj* cam,void* foot, _syn* stack,int sp, p64 arg,int idx, u8* buf,int len)
{
	return 0;
}
int libcam_give(_obj* cam,void* foot, _syn* stack,int sp, p64 arg,int idx, u8* buf,int len)
{
	return 0;
}
int libcam_attach()
{
	return 0;
}
int libcam_detach()
{
	return 0;
}


void copyfmt64(u8* src, u8* dst)
{
	int j;
	for(j=0;j<8;j++){
		if( (src[j]==',') |
		((src[j]>='a') && (src[j]<='z')) |
		((src[j]>='0') && (src[j]<='9')) ){
			dst[j] = src[j];
		}
		else break;
	}
	for(;j<8;j++)dst[j] = 0;
}


int libcam_reader(_obj* cam,void* foot, p64 arg,int idx, u8* buf,int len)
{
	return 0;
}
int libcam_writer(_obj* cam,void* foot, p64 arg,int idx, u8* buf,int len)
{
	return 0;
}
int libcam_delete(_obj* cam)
{
	if(cam->priv_ptr){
		struct mydata* my = (struct mydata*)cam->priv_ptr;
		deletecamera(my);
		sleep_us(1000);

		memoryfree(cam->priv_ptr);
		cam->priv_ptr = 0;
	}
	return 0;
}
int libcam_create(_obj* cam, void* arg, int argc, u8** argv)
{
static_assert(sizeof(struct mydata) < 0x1000, "struct mydata too big");
	struct mydata* my = (struct mydata*)memoryalloc(0x1000, 0);
	my->myobj = cam;
	my->log = 0;

	int j;
	u32 choosecamera = 0;
	u64 fmt = 0;
	u32 w = 640;
	u32 h = 480;
	for(j=1;j<argc;j++){
		if(0 == ncmp(argv[j], (void*)"camera:", 7)){
			decstr2u32(argv[j]+7, &choosecamera);
		}
		if(0 == ncmp(argv[j], (void*)"format:", 7)){
			copyfmt64(argv[j]+7, (u8*)&fmt);
			logtoall((void*)"libcam_create: fmtin=%.8s, fmtout=%.8s", argv[j]+7, &fmt);
		}
		if(0 == ncmp(argv[j], (void*)"width:", 6)){
			decstr2u32(argv[j]+6, &w);
		}
		if(0 == ncmp(argv[j], (void*)"height:", 7)){
			decstr2u32(argv[j]+7, &h);
		}
		if(0 == ncmp(argv[j], (void*)"log:", 4)){
			my->log = 1;
		}
	}
	logtoall((void*)"libcam_create fmt=%x,w=%d,h=%d\n", fmt, w, h);

	my->choosecamera = choosecamera;
	my->fmt = fmt;
	my->fps = 60;
	my->w = w;
	my->h = h;

	cam->priv_ptr = my;
	createcamera(my);
	return 0;
}


}	//extern "C"
