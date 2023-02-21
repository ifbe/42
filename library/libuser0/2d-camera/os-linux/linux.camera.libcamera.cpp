//https://github.com/kbingham/simple-cam/blob/master/simple-cam.cpp
extern "C"{
int printmemory(void*, int);
}
#include <iomanip>
#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <libcamera/libcamera.h>
using namespace libcamera;

struct mydata{
	std::shared_ptr<Camera> cam;
	uint8_t* mem[8];
};


static void requestComplete(Request *request)
{
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
			printmemory(p, 0x20);
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
	std::unique_ptr<CameraManager> cm = std::make_unique<CameraManager>();

	cm->start();

	auto list = cm->cameras();
	if(list.empty()){
		std::cout << "no camera" << std::endl;
		return 0;
	}

	for (auto const &camera : list){
		std::cout << cameraName(camera.get())<< " @ " << camera->id() << std::endl;
	}

	std::string id = list[0]->id();
	my->cam = cm->get(id);
	my->cam->acquire();

	//std::unique_ptr<CameraConfiguration> cfglist = cam->generateConfiguration({StreamRole::Viewfinder});
	std::unique_ptr<CameraConfiguration> cfglist = my->cam->generateConfiguration({StreamRole::Raw});
	//std::unique_ptr<CameraConfiguration> cfglist = cam->generateConfiguration({StreamRole::VideoRecording});
	//std::unique_ptr<CameraConfiguration> cfglist = cam->generateConfiguration({StreamRole::StillCapture});
	//std::unique_ptr<CameraConfiguration> cfglist = cam->generateConfiguration({StreamRole::Raw, StreamRole::StillCapture,StreamRole::VideoRecording, StreamRole::Viewfinder});

	for(StreamConfiguration& cfg : *cfglist){
		std::cout << "config: " << cfg.toString() << std::endl;
	}

	cfglist->validate();
	my->cam->configure(cfglist.get());

	FrameBufferAllocator *allocator = new FrameBufferAllocator(my->cam);
	for (StreamConfiguration &cfg : *cfglist) {
		int ret = allocator->allocate(cfg.stream());
		if (ret < 0) {
			std::cerr << "Can't allocate buffers" << std::endl;
			return EXIT_FAILURE;
		}

		size_t allocated = allocator->buffers(cfg.stream()).size();
		std::cout << "Allocated " << allocated << " buffers for stream" << std::endl;
	}

	StreamConfiguration &streamConfig = cfglist->at(0);
	Stream *stream = streamConfig.stream();
	const std::vector<std::unique_ptr<FrameBuffer>> &buffers = allocator->buffers(stream);
	std::vector<std::unique_ptr<Request>> requests;
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

		int ret = request->addBuffer(stream, buffer.get());
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

		requests.push_back(std::move(request));
	}
	
	my->cam->requestCompleted.connect(requestComplete);
	my->cam->start();
	for (std::unique_ptr<Request> &request : requests)
		my->cam->queueRequest(request.get());
	usleep(10*1000*1000);
	my->cam->stop();

	allocator->free(stream);
	delete allocator;

	my->cam->release();
	cm->stop();
	return 0;
}


extern "C" {
#include "libuser.h"


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
	return 0;
}
int libcam_create(_obj* cam, void* arg, int argc, u8** argv)
{
	struct mydata* my = (struct mydata*)cam->priv_256b;
	createcamera(my);
	return 0;
}


}	//extern "C"
