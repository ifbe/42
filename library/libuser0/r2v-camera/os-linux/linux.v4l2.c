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
#define STRIDE ix0
#define HEIGHT iy0
#define FORMAT iw0




//
struct buffer{
	union{
		u64 addr;
		void* buf;
	};
	u64 len;
};
static struct buffer info[24];
static int cur = 0;
static int alive = 1;




void* visionlistener(struct supply* win)
{
	struct halfrel stack[0x80];

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
	struct v4l2_fmtdesc desc;
	desc.type=V4L2_BUF_TYPE_VIDEO_CAPTURE;
	desc.index=0;
	printf("v4l2_fmtdesc:\n");
	while(-1 != ioctl(fd, VIDIOC_ENUM_FMT, &desc))
	{
		printf("	%s\n", desc.description);
		desc.index++;
	}
	if (desc.index == 0)
	{
		perror("VIDIOC_ENUM_FMT");
	}

	//v4l2_format
	struct v4l2_format fmt;
	fmt.type		= V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
	fmt.fmt.pix.width       = win->STRIDE;
	fmt.fmt.pix.height      = win->HEIGHT;
	fmt.fmt.pix.pixelformat = win->FORMAT;
	if(-1 == ioctl(fd,VIDIOC_S_FMT,&fmt))
	{
		printf("VIDIOC_S_FMT error\n");
		return 0;
	}

	//v4l2_requestbuffers
	struct v4l2_requestbuffers req;
	req.count       = 24;
	req.type	= V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory      = V4L2_MEMORY_MMAP;     //V4L2_MEMORY_USERPTR;
	if(-1 == ioctl(fd,VIDIOC_REQBUFS,&req))
	{
		printf("VIDIOC_REQBUFS error\n");
		return 0;
	}

	//prepare
	struct v4l2_buffer buf;
	for(j=0;j<24;j++)
	{
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		buf.index = j;
		if(-1 == ioctl(fd,VIDIOC_QUERYBUF,&buf))
		{
			printf("VIDIOC_QUERYBUF\n");
			return 0;
		}

		info[j].len = buf.length;
		info[j].buf = mmap(
			NULL,
			buf.length,
			PROT_READ|PROT_WRITE,
			MAP_SHARED,
			fd,
			buf.m.offset
		);

		if(MAP_FAILED == info[j].buf)
		{
			printf("fail@mmap\n");
			return 0;
		}
		if(-1 == ioctl(fd,VIDIOC_QBUF,&buf))
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
	cur = 0;
	while(alive)
	{
		//!!!!!!!!!!!!!!must take out ontime!!!!!!!!!!!!!!
		j = epoll_wait(epfd, &ev, 1, -1);
		if(j == -1)
		{
			printf("epoll error\n");
			return 0;
		}

		//deq
		buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE;
		buf.memory = V4L2_MEMORY_MMAP;
		if(-1 == ioctl(fd, VIDIOC_DQBUF, &buf)){
			say("error@DQBUF\n");
		}

		//send
		give_data_into_peer(win,_dst_, stack,0, 0,0, info[cur].buf,buf.bytesused);//info[cur].len);
		cur = (cur+1)%24;

		//enq
		if(-1 == ioctl(fd, VIDIOC_QBUF, &buf)){
			say("error@DQBUF\n");
		}
	}

	//stop
	j = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if(-1 == ioctl(fd, VIDIOC_STREAMOFF, &j))
	{
		printf("error@OFF\n");
	}
	for(j=0;j<24;j++)
	{
		munmap(info[j].buf, info[j].len);
	}

	//v4l2_release
	close(fd);
	return 0;
}




int videoread(_sup* sup,int foot, _syn* stack,int sp, void* arg,int idx, u8* buf,int len)
{
	u64 addr = info[(cur+23)%24].addr;
	say("addr=%llx\n",addr);

	*(u64*)buf = addr;
	return 0;
}
int videowrite(_sup* sup,int foot, _syn* stack,int sp, void* arg,int idx, u8* buf,int len)
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
int videodelete(struct supply* win)
{
	alive = 0;
	return 0;
}
int videocreate(struct supply* win, void* arg, int argc, u8** argv)
{
	int j;
	for(j=0;j<24;j++)info[j].addr = 0;

	win->STRIDE = 640;
	win->HEIGHT = 480;
	win->FORMAT = 
	//	V4L2_PIX_FMT_JPEG;
	//	V4L2_PIX_FMT_MJPEG;
	//	V4L2_PIX_FMT_RGB24;
	//	V4L2_PIX_FMT_SBGGR8;
	//	V4L2_PIX_FMT_SN9C10X;
	//	V4L2_PIX_FMT_UYVY;
	//	V4L2_PIX_FMT_YUV422P;
	//	V4L2_PIX_FMT_YUV420;
		V4L2_PIX_FMT_YUYV;

	for(j=1;j<argc;j++){
		arg = argv[j];
		//say("%d->%.16s\n",j,arg;
		if(0 == ncmp(arg, "format:", 7)){
			arg = argv[j]+7;
			//say("format=%.5s\n",arg);
			if(0 == ncmp(arg, "mjpeg", 5))win->FORMAT = V4L2_PIX_FMT_MJPEG;
		}
		if(0 == ncmp(arg, "width:", 6)){
			arg = argv[j]+6;
			decstr2u32(arg, &win->STRIDE);
		}
		if(0 == ncmp(arg, "height:", 7)){
			arg = argv[j]+7;
			decstr2u32(arg, &win->HEIGHT);
		}
	}

	alive = 1;
	threadcreate(visionlistener, win);
	return 0;
}




void initcam()
{
}
void freecam()
{
}
