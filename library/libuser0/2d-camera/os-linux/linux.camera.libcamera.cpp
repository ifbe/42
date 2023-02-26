//https://github.com/kbingham/simple-cam/blob/master/simple-cam.cpp
extern "C"{
#include "libuser.h"
}
#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <libcamera/libcamera.h>
using namespace libcamera;

struct mydata{
	std::unique_ptr<CameraManager> cm;
	std::shared_ptr<Camera> cam;
	Stream* stream;
	FrameBufferAllocator* allocator;
	std::vector<std::unique_ptr<Request>> requests;

	_obj* myobj;
	uint8_t* mem[8];
};


static void requestComplete(Request *request)
{
	struct halfrel stack[0x80];
	if (request->status() == Request::RequestCancelled)return;

	uint64_t cookie = request->cookie();
	struct mydata* my = (struct mydata*)cookie;
	std::cout << "requestComplete: " << my->cam << std::endl;
	std::cout << "requestcookie: " << cookie << std::endl;

	const ControlList &requestMetadata = request->metadata();
	for (const auto &ctrl : requestMetadata) {
		const ControlId *id = controls::controls.at(ctrl.first);
		const ControlValue &value = ctrl.second;

		std::cout << "\t" << id->name() << " = " << value.toString() << std::endl;
	}

	const Request::BufferMap &buffers = request->buffers();
	for (auto bufferPair : buffers) {
		// (Unused) Stream *stream = bufferPair.first;
		FrameBuffer *buffer = bufferPair.second;
		uint64_t idx = buffer->cookie();
		std::cout << "\tbuffercookie: " << idx << std::endl;

		const FrameMetadata &metadata = buffer->metadata();
		std::cout << "\tbuffersequence: " << metadata.sequence << std::endl;
		std::cout << "\tbuffertimems: " << metadata.timestamp/1000/1000 << std::endl;

		auto planedata = buffer->planes();
		auto planemeta = metadata.planes();
		for(int j=0;j<planedata.size();j++){
			SharedFD fd = planedata[j].fd;
			uint32_t off = planedata[j].offset;
			uint32_t len = planedata[j].length;
			int sz = planemeta[j].bytesused;
			std::cout << "\tfd: " << fd.get() << " off: " << off << std::endl;
			std::cout << "\tsz: " << sz << " in " << len << std::endl;

			uint8_t* p = my->mem[idx];
			//printmemory(p, 0x20);
			give_data_into_peer(my->myobj,_dst_, stack,0, 0,0, p, sz);
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
	//std::unique_ptr<CameraConfiguration> camcfg = my->cam->generateConfiguration({StreamRole::Viewfinder});
	std::unique_ptr<CameraConfiguration> camcfg = my->cam->generateConfiguration({StreamRole::Raw});
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
		std::cout << "format: " << fmt.toString() << std::endl;

		auto pixsize= stcfmt.sizes(fmt);
		for(auto& sz: pixsize){
			std::cout << "size: " << sz.toString() << std::endl;
		}
	}

	//stream config
	stmcfg.size.width = 640;
	stmcfg.size.height = 480;
	//stmcfg.pixelFormat = PixelFormat(hex32('Y','U','Y','V'));

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
		const FrameBuffer::Plane &plane = buffer->planes()[0];
		int fd = plane.fd.get();
		void* mem = mmap(NULL, plane.length, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
		std::cout << i << ", fd=" << fd << ",mem=" << mem << std::endl;

		buffer->setCookie(i);
		my->mem[i] = (uint8_t*)mem;

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

		/*
		 * Controls can be added to a request on a per frame basis.
		 */
		ControlList &controls = request->controls();
		controls.set(controls::Brightness, 0.5);

		my->requests.push_back(std::move(request));
	}

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

	createcamera(my);
	return 0;
}


}	//extern "C"
