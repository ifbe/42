#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/epoll.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include "libuser.h"
#define STRIDE whdf.fourth
#define HEIGHT whdf.height
#define FORMAT vfmt
#define BUFCNT 24




//
struct perbuf{
	struct v4l2_buffer v4l2buf;
	int len;
	union{
		u64 addr;
		void* buf;
	};
};
struct percam{
	u64 thread;
	int alive;

	int deq;
	struct perbuf datainfo[BUFCNT];
};
//static struct buffer info[24];
//static int cur = 0;
//static int alive = 1;




void* cameraworker(_obj* cam)
{
	struct halfrel stack[0x80];
	struct percam* pcam = cam->priv_ptr;
	if(0 == pcam){
		say("@cameraworker:0=pcam\n");
		return 0;
	}

	//v4l2_open
	int j;
	int fd = open("/dev/video0",O_RDWR);    //|O_NONBLOCK);
	if(fd <= 0)
	{
		printf("error@open /dev/video0\n");
		return 0;
	}

	//v4l2_capability
	struct v4l2_capability cap;
	if(-1 == ioctl(fd,VIDIOC_QUERYCAP,&cap))
	{
		printf("VIDIOC_QUERYCAP error\n");
		return 0;
	}
	printf("v4l2_capability:\n");
	printf("	driver: %s\n", cap.driver);
	printf("	card: %s\n", cap.card);
	printf("	bus_info: %s\n", cap.bus_info);
	printf("	version: %08X\n", cap.version);
	printf("	capabilities: %08X\n", cap.capabilities);

	//v4l2_fmtdesc
	int enumx,enumy;
	struct v4l2_fmtdesc desc;
	struct v4l2_frmsizeenum frmsize;
	struct v4l2_frmivalenum frmival;

	desc.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	desc.index = 0;
	say("v4l2_fmtdesc:\n");
	while(ioctl(fd, VIDIOC_ENUM_FMT, &desc) >= 0)
	{
		say("	4cc=%.4s,str=%s\n", &desc.pixelformat, desc.description);

		frmsize.pixel_format = desc.pixelformat;
		frmsize.index = 0;
		while (ioctl(fd, VIDIOC_ENUM_FRAMESIZES, &frmsize) >= 0) {
			if (frmsize.type == V4L2_FRMSIZE_TYPE_DISCRETE) {
				enumx = frmsize.discrete.width;
				enumy = frmsize.discrete.height;
			} else if (frmsize.type == V4L2_FRMSIZE_TYPE_STEPWISE) {
				enumx = frmsize.stepwise.max_width;
				enumy = frmsize.stepwise.max_height;
			}
			say("		w=%d,h=%d\n", enumx,enumy);

			frmival.index = 0;
			frmival.pixel_format = desc.pixelformat;
			frmival.width = enumx;
			frmival.height = enumy;
			while(ioctl(fd, VIDIOC_ENUM_FRAMEINTERVALS, &frmival) >= 0){
				say("			fps=%d/%d\n",
					frmival.discrete.numerator,
					frmival.discrete.denominator
				);
				frmival.index++;
			}
			if(0 == frmival.index){
				say("			VIDIOC_ENUM_FRAMEINTERVALS:errno=%d\n",errno);
				//return 0;
			}

			frmsize.index++;
		}
		if(0 == frmsize.index){
			say("		VIDIOC_ENUM_FRAMESIZES:errno=%d\n",errno);
			return 0;
		}

		desc.index++;
	}
	if(0 == desc.index){
		say("	VIDIOC_ENUM_FMT:errno=%d\n",errno);
		return 0;
	}

	//v4l2_format
	struct v4l2_format fmt;
	fmt.type		= V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
	fmt.fmt.pix.width       = cam->STRIDE;
	fmt.fmt.pix.height      = cam->HEIGHT;
	fmt.fmt.pix.pixelformat = desc.pixelformat;	//cam->FORMAT;
	if(-1 == ioctl(fd, VIDIOC_S_FMT, &fmt))
	{
		printf("VIDIOC_S_FMT error\n");
		return 0;
	}

	//v4l2_requestbuffers
	struct v4l2_requestbuffers req;
	req.count       = BUFCNT;
	req.type	= V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory      = V4L2_MEMORY_MMAP;     //V4L2_MEMORY_USERPTR;
	if(-1 == ioctl(fd,VIDIOC_REQBUFS,&req))
	{
		printf("VIDIOC_REQBUFS error\n");
		return 0;
	}

	//prepare
	struct perbuf* myinfo;
	struct v4l2_buffer* v4l2buf;
	for(j=0;j<BUFCNT;j++)
	{
		myinfo = &pcam->datainfo[j];
		v4l2buf = &myinfo->v4l2buf;

		v4l2buf->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		v4l2buf->memory = V4L2_MEMORY_MMAP;
		v4l2buf->index = j;
		if(-1 == ioctl(fd, VIDIOC_QUERYBUF, v4l2buf))
		{
			printf("VIDIOC_QUERYBUF\n");
			return 0;
		}

		myinfo->buf = mmap(
			NULL,
			v4l2buf->length,
			PROT_READ|PROT_WRITE,
			MAP_SHARED,
			fd,
			v4l2buf->m.offset
		);

		if(MAP_FAILED == pcam->datainfo[j].buf)
		{
			printf("fail@mmap\n");
			return 0;
		}
		if(-1 == ioctl(fd, VIDIOC_QBUF, v4l2buf))
		{
			printf("VIDEOC_QBUF\n");
			return 0;
		}
	}

	//start
	j = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if(-1 == ioctl(fd, VIDIOC_STREAMON, &j))
	{
		printf("error@ON\n");
		return 0;
	}

	//record
	int epfd;
	struct epoll_event ev;
	epfd = epoll_create(256);
	ev.data.fd=fd;
	ev.events = EPOLLIN | EPOLLET;
	epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);
	pcam->deq = 0;
	while(pcam->alive)
	{
		//!!!!!!!!!!!!!!must take out ontime!!!!!!!!!!!!!!
		j = epoll_wait(epfd, &ev, 1, -1);
		if(j == -1)
		{
			printf("epoll error\n");
			return 0;
		}

		//deq
		myinfo = &pcam->datainfo[pcam->deq];
		v4l2buf = &myinfo->v4l2buf;

		v4l2buf->type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		v4l2buf->memory = V4L2_MEMORY_MMAP;
		if(-1 == ioctl(fd, VIDIOC_DQBUF, v4l2buf)){
			say("error@DQBUF\n");
		}

		//send
		give_data_into_peer(cam,_dst_, stack,0, 0,0, myinfo->buf, v4l2buf->bytesused);
		pcam->deq = (pcam->deq+1)%BUFCNT;

		//enq
		if(-1 == ioctl(fd, VIDIOC_QBUF, v4l2buf)){
			say("error@DQBUF\n");
		}
	}

	//stop
	j = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if(-1 == ioctl(fd, VIDIOC_STREAMOFF, &j))
	{
		printf("error@OFF\n");
	}
	for(j=0;j<BUFCNT;j++)
	{
		myinfo = &pcam->datainfo[j];
		v4l2buf = &myinfo->v4l2buf;
		munmap(myinfo->buf, v4l2buf->length);
	}

	//v4l2_release
	close(fd);
	return 0;
}




int video_take(_obj* cam,void* foot, _syn* stack,int sp, void* arg,int idx, u8* buf,int len)
{
	struct percam* pcam = cam->priv_ptr;
	if(0 == pcam)return 0;

	u64 addr = pcam->datainfo[(pcam->deq+BUFCNT-1)%BUFCNT].addr;
	say("addr=%llx\n",addr);

	*(u64*)buf = addr;
	return 0;
}
int video_give(_obj* cam,void* foot, _syn* stack,int sp, void* arg,int idx, u8* buf,int len)
{
	return 0;
}
int videostop()
{
	return 0;
}
int videostart()
{
	return 0;
}
int videodelete(_obj* cam)
{
	struct percam* pcam = cam->priv_ptr;
	if(0 == pcam)return 0;

	//set signal
	pcam->alive = 0;

	//wait signal
	//while(-1 != pcam->alive)

	//free data
	memorydelete(cam->priv_ptr);
	cam->priv_ptr = 0;
	return 0;
}
int videocreate(_obj* cam, void* arg, int argc, u8** argv)
{
	//default value
	int stride = 640;
	int height = 480;
	int format = 
	//	V4L2_PIX_FMT_JPEG;
	//	V4L2_PIX_FMT_MJPEG;
	//	V4L2_PIX_FMT_RGB24;
	//	V4L2_PIX_FMT_SBGGR8;
	//	V4L2_PIX_FMT_SN9C10X;
	//	V4L2_PIX_FMT_UYVY;
	//	V4L2_PIX_FMT_YUV422P;
	//	V4L2_PIX_FMT_YUV420;
		V4L2_PIX_FMT_YUYV;

	//parse value
	int j;
	for(j=1;j<argc;j++){
		arg = argv[j];
		if(0 == arg)break;
		//say("%d->%.16s\n",j,arg;
		if(0 == ncmp(arg, "format:", 7)){
			arg = argv[j]+7;
			//say("format=%.5s\n",arg);
			if(0)return 0;
			else if(0 == ncmp(arg, "mjpeg", 5))format = V4L2_PIX_FMT_MJPEG;
			else if(0 == ncmp(arg, "h264", 4))format = V4L2_PIX_FMT_H264;
		}
		if(0 == ncmp(arg, "width:", 6)){
			arg = argv[j]+6;
			decstr2u32(arg, &stride);
		}
		if(0 == ncmp(arg, "height:", 7)){
			arg = argv[j]+7;
			decstr2u32(arg, &height);
		}
	}//for

	//remember value
	cam->STRIDE = stride;
	cam->HEIGHT = height;
	cam->FORMAT = format;

	//percam data
	struct percam* pcam = cam->priv_ptr = memorycreate(0x1000, 4);
	if(0 == pcam){
		say("oom@alloc pcam\n");
		return -1;
	}

	//percam thread
	pcam->alive = 1;
	pcam->thread = threadcreate(cameraworker, cam);
	return 0;
}




void initcam()
{
}
void freecam()
{
}
