//https://github.com/kbingham/simple-cam/blob/master/simple-cam.cpp
/*
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

	int fmt;
	int fps;
	int w;
	int h;

	std::unique_ptr<CameraManager> cm;
	std::shared_ptr<Camera> cam;
	Stream* stream;

	FrameBufferAllocator* allocator;	//const std::vector<std::unique_ptr<FrameBuffer>> &buffers = my->allocator->buffers(my->stream);
	uint8_t* mem[8][2];	//void* mem = mmap(NULL, plane.length, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);

	std::vector<std::unique_ptr<Request>> requests;
};


static void requestComplete(Request *request)
{
	struct halfrel stack[0x80];
	struct kv88 kv[5] = {
		{'w', 0},
		{'h', 0},
		{'f', 0},
		{'t', 0},
		{0,0}
	};
	if (request->status() == Request::RequestCancelled)return;

	uint64_t cookie = request->cookie();
	struct mydata* my = (struct mydata*)cookie;
	if(my->log)std::cout << "requestComplete: " << my->cam << std::endl;
	if(my->log)std::cout << "requestcookie: " << cookie << std::endl;

	u64 sensortime;
	const ControlList &requestMetadata = request->metadata();
	for (const auto &ctrl : requestMetadata) {
		const ControlId *cid = controls::controls.at(ctrl.first);
		const ControlValue &value = ctrl.second;
		int id = cid->id();
		Span<const uint8_t> sp = value.data();
		switch(id){
		case libcamera::controls::SENSOR_TIMESTAMP:
			//auto time = value.get<ControlTypeInteger64>();
			sensortime = sp[7];
			sensortime = (sensortime<<8) | sp[6];
			sensortime = (sensortime<<8) | sp[5];
			sensortime = (sensortime<<8) | sp[4];
			sensortime = (sensortime<<8) | sp[3];
			sensortime = (sensortime<<8) | sp[2];
			sensortime = (sensortime<<8) | sp[1];
			sensortime = (sensortime<<8) | sp[0];
			if(my->log)std::cout << "\t" << id <<"-SENSOR_TIMESTAMP = " <<std::dec<< sensortime << std::endl;
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
		if(my->log)std::cout << "\tbuffertimems: " << metadata.timestamp/1000/1000 << std::endl;

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
			kv[2].val = my->fmt;
			kv[4].val = sensortime;
			give_data_into_peer(my->myobj,_dst_, stack,0, kv,_kv88_, p, sz);
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

int createcamera(struct mydata* my){
	my->cm = std::make_unique<CameraManager>();

	my->cm->start();

	auto list = my->cm->cameras();
	if(list.empty()){
		std::cout << "no camera" << std::endl;
		return 0;
	}

	for (auto const &camera : list){
		std::cout << cameraName(camera.get())<< " @ " << camera->id() << std::endl;
	}

	std::string id = list[0]->id();
	my->cam = my->cm->get(id);
	my->cam->acquire();

	//camera config
	std::unique_ptr<CameraConfiguration> camcfg;
	if(0 == my->fmt)camcfg = my->cam->generateConfiguration({StreamRole::Raw});
	else camcfg = my->cam->generateConfiguration({StreamRole::Viewfinder});
	//std::unique_ptr<CameraConfiguration> camcfg = my->cam->generateConfiguration({StreamRole::VideoRecording});
	//std::unique_ptr<CameraConfiguration> camcfg = my->cam->generateConfiguration({StreamRole::StillCapture});

	//stream config
	for(auto cfg : *camcfg){
		std::cout << "config: " << cfg.toString() << std::endl;
	}
	StreamConfiguration &stmcfg = camcfg->at(0);

	//foramt
	auto stcfmt = stmcfg.formats();
	auto pixfmt = stcfmt.pixelformats();
	for(auto& fmt: pixfmt){
		std::cout << "format: " << fmt.toString() <<std::hex<< " fourcc=" << fmt.fourcc() << std::endl;

		auto pixsize= stcfmt.sizes(fmt);
		for(auto& sz: pixsize){
			std::cout << "size: " << sz.toString() << std::endl;
		}
	}

	//stream config
	stmcfg.size.width = my->w;
	stmcfg.size.height = my->h;
	if(my->fmt){
		stmcfg.pixelFormat = PixelFormat(my->fmt);
	}
	else{
		my->fmt = stmcfg.pixelFormat.fourcc();
		if(_BG10_ == my->fmt)my->fmt = _pBAA_;
		if(_GB10_ == my->fmt)my->fmt = _pGAA_;
	}

	//camera config
	camcfg->validate();
	my->cam->configure(camcfg.get());

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

	StreamConfiguration &streamConfig = camcfg->at(0);
	my->stream = streamConfig.stream();
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
	my->cm->stop();
	return 0;
}


extern "C" {


int libcam_take(_obj* cam,void* foot, _syn* stack,int sp, void* arg,int idx, u8* buf,int len)
{
	return 0;
}
int libcam_give(_obj* cam,void* foot, _syn* stack,int sp, void* arg,int idx, u8* buf,int len)
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




int libcam_reader(_obj* cam,void* foot, void* arg,int idx, u8* buf,int len)
{
	return 0;
}
int libcam_writer(_obj* cam,void* foot, void* arg,int idx, u8* buf,int len)
{
	return 0;
}
int libcam_delete(_obj* cam)
{
	struct mydata* my = (struct mydata*)cam->priv_256b;
	my->myobj = cam;

	deletecamera(my);
	return 0;
}
int libcam_create(_obj* cam, void* arg, int argc, u8** argv)
{
	struct mydata* my = (struct mydata*)cam->priv_256b;
	my->myobj = cam;
	my->log = 0;

	int j;
	u32 w = 640;
	u32 h = 480;
	u32 fmt = 0;
	for(j=1;j<argc;j++){
		if(0 == ncmp(argv[j], (void*)"format:", 7)){
			fmt = *(u32*)(argv[j]+7);
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
	say((void*)"libcam_create fmt=%x,w=%d,h=%d\n", fmt, w, h);

	my->fmt = fmt;
	my->fps = 60;
	my->w = w;
	my->h = h;

	createcamera(my);
	return 0;
}


}	//extern "C"
