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
	//want
	u8 path[0x100];
	int wantstride;
	int wantheight;
	u32 wantformat;

	//cam itself
	int fd;
	int started;

	int realstride;
	int realheight;
	u32 realformat;

	//cam handler
	u64 thread;
	int alive;

	int deq;
	struct perbuf datainfo[BUFCNT];
};
//static struct buffer info[24];
//static int cur = 0;
//static int alive = 1;




int v4l2cam_destroy(_obj* cam)
{
	struct percam* pcam = cam->priv_ptr;
	if(0 == pcam){
		say("@v4l2cam_destroy:0=pcam\n");
		return 0;
	}

	int fd = pcam->fd;
	if(fd <= 0)return 0;

	int j;
	struct perbuf* myinfo;
	struct v4l2_buffer* v4l2buf;
	if(0 == pcam->started)goto doclose;

dostop:
	j = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if(-1 == ioctl(fd, VIDIOC_STREAMOFF, &j))
	{
		printf("@OFF:errno=%d\n",errno);
		goto doclose;
	}

dounmap:
	for(j=0;j<BUFCNT;j++)
	{
		myinfo = &pcam->datainfo[j];
		v4l2buf = &myinfo->v4l2buf;
		munmap(myinfo->buf, v4l2buf->length);
	}

doclose:
	close(fd);
	return 0;
}
int v4l2cam_prepare(_obj* cam)
{
	struct percam* pcam = cam->priv_ptr;
	if(0 == pcam){
		say("@v4l2cam_prepare:0=pcam\n");
		return 0;
	}
	pcam->fd = 0;
	pcam->started = 0;

	//v4l2_open
	int fd = open(pcam->path, O_RDWR);    //|O_NONBLOCK);
	if(fd <= 0)
	{
		printf("error@open /dev/video0\n");
		return 0;
	}
	pcam->fd = fd;


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
	if(cap.capabilities&V4L2_CAP_VIDEO_CAPTURE)       printf("		V4L2_CAP_VIDEO_CAPTURE\n");
	if(cap.capabilities&V4L2_CAP_VIDEO_CAPTURE_MPLANE)printf("		V4L2_CAP_VIDEO_CAPTURE_MPLANE\n");
	if(cap.capabilities&V4L2_CAP_VIDEO_OUTPUT)        printf("		V4L2_CAP_VIDEO_OUTPUT\n");
	if(cap.capabilities&V4L2_CAP_VIDEO_OUTPUT_MPLANE) printf("		V4L2_CAP_VIDEO_OUTPUT_MPLANE\n");
	if(cap.capabilities&V4L2_CAP_VIDEO_M2M)           printf("		V4L2_CAP_VIDEO_M2M\n");
	if(cap.capabilities&V4L2_CAP_VIDEO_M2M_MPLANE)    printf("		V4L2_CAP_VIDEO_M2M_MPLANE\n");
	if(cap.capabilities&V4L2_CAP_VIDEO_OVERLAY)       printf("		V4L2_CAP_VIDEO_OVERLAY\n");
	if(cap.capabilities&V4L2_CAP_VBI_CAPTURE)         printf("		V4L2_CAP_VBI_CAPTURE\n");
	if(cap.capabilities&V4L2_CAP_VBI_OUTPUT)          printf("		V4L2_CAP_VBI_OUTPUT\n");
	if(cap.capabilities&V4L2_CAP_SLICED_VBI_CAPTURE)  printf("		V4L2_CAP_SLICED_VBI_CAPTURE\n");
	if(cap.capabilities&V4L2_CAP_SLICED_VBI_OUTPUT)   printf("		V4L2_CAP_SLICED_VBI_OUTPUT\n");
	if(cap.capabilities&V4L2_CAP_RDS_CAPTURE)         printf("		V4L2_CAP_RDS_CAPTURE\n");
	if(cap.capabilities&V4L2_CAP_VIDEO_OUTPUT_OVERLAY)printf("		V4L2_CAP_VIDEO_OUTPUT_OVERLAY\n");
	if(cap.capabilities&V4L2_CAP_HW_FREQ_SEEK)        printf("		V4L2_CAP_HW_FREQ_SEEK\n");
	if(cap.capabilities&V4L2_CAP_RDS_OUTPUT)          printf("		V4L2_CAP_RDS_OUTPUT\n");
	if(cap.capabilities&V4L2_CAP_TUNER)               printf("		V4L2_CAP_TUNER\n");
	if(cap.capabilities&V4L2_CAP_AUDIO)               printf("		V4L2_CAP_AUDIO\n");
	if(cap.capabilities&V4L2_CAP_RADIO)               printf("		V4L2_CAP_RADIO\n");
	if(cap.capabilities&V4L2_CAP_MODULATOR)           printf("		V4L2_CAP_MODULATOR\n");
	if(cap.capabilities&V4L2_CAP_SDR_CAPTURE)         printf("		V4L2_CAP_SDR_CAPTURE\n");
	if(cap.capabilities&V4L2_CAP_EXT_PIX_FORMAT)      printf("		V4L2_CAP_EXT_PIX_FORMAT\n");
	if(cap.capabilities&V4L2_CAP_SDR_OUTPUT)          printf("		V4L2_CAP_SDR_OUTPUT\n");
	if(cap.capabilities&V4L2_CAP_READWRITE)           printf("		V4L2_CAP_READWRITE\n");
	if(cap.capabilities&V4L2_CAP_ASYNCIO)             printf("		V4L2_CAP_ASYNCIO\n");
	if(cap.capabilities&V4L2_CAP_STREAMING)           printf("		V4L2_CAP_STREAMING\n");
	//if(cap.capabilities&V4L2_CAP_TOUCH)               printf("		V4L2_CAP_TOUCH\n");
	if(cap.capabilities&V4L2_CAP_DEVICE_CAPS)         printf("		V4L2_CAP_DEVICE_CAPS\n");
	if(0 == cap.capabilities&V4L2_CAP_VIDEO_CAPTURE)return 0;


	//v4l2_fmtdesc
	int enumx,enumy;
	int minx,miny,maxx,maxy,stepx,stepy;
	struct v4l2_fmtdesc desc;
	struct v4l2_frmsizeenum frmsize;
	struct v4l2_frmivalenum frmival;

	pcam->realstride = 0;
	pcam->realheight = 0;
	pcam->realformat = 0;

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
				say("		DISCRETE:w=%d,h=%d\n", enumx,enumy);
			} else if (frmsize.type == V4L2_FRMSIZE_TYPE_STEPWISE) {
				minx = frmsize.stepwise.min_width;
				miny = frmsize.stepwise.min_height;
				maxx = frmsize.stepwise.max_width;
				maxy = frmsize.stepwise.max_height;
				stepx = frmsize.stepwise.step_width;
				stepy = frmsize.stepwise.step_height;
				say("		STEPWISE:min=(%d,%d),max=(%d,%d),step=(%d,%d)\n", minx,miny,maxx,maxy,stepx,stepy);

				if( (0==(pcam->wantstride-minx)%stepx)&&
				    (0==(pcam->wantheight-miny)%stepy) )
				{
					enumx = pcam->wantstride;
					enumy = pcam->wantheight;
				}
			}
			else{
				say("		frmsize.type=%x\n", frmsize.type);
			}

			if( (pcam->wantstride == enumx) &&
			    (pcam->wantheight == enumy) &&
			    (pcam->wantformat == desc.pixelformat) &&
			    (pcam->realformat == 0) )
			{
				pcam->realstride = enumx;
				pcam->realheight = enumy;
				pcam->realformat = desc.pixelformat;
				printf("find what i want!\n");
			}
/*
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
*/
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

	if(0 == pcam->realformat){
		say("not support: w=%d,h=%d,fmt=%.4s\n", pcam->wantstride, pcam->wantheight, &pcam->wantformat);
		goto failclose;
	}

	//v4l2_format
	struct v4l2_format fmt;
	fmt.type		= V4L2_BUF_TYPE_VIDEO_CAPTURE;
	fmt.fmt.pix.field       = V4L2_FIELD_INTERLACED;
	fmt.fmt.pix.width       = pcam->realstride;
	fmt.fmt.pix.height      = pcam->realheight;
	fmt.fmt.pix.pixelformat = pcam->realformat;
	if(-1 == ioctl(fd, VIDIOC_S_FMT, &fmt))
	{
		printf("@VIDIOC_S_FMT:errno=%d\n",errno);
		return 0;
	}

	//v4l2_requestbuffers
	struct v4l2_requestbuffers req;
	req.count       = BUFCNT;
	req.type	= V4L2_BUF_TYPE_VIDEO_CAPTURE;
	req.memory      = V4L2_MEMORY_MMAP;     //V4L2_MEMORY_USERPTR;
	if(-1 == ioctl(fd,VIDIOC_REQBUFS,&req))
	{
		printf("@VIDIOC_REQBUFS:error=%d\n",errno);
		return 0;
	}

	//prepare
	int j;
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
			printf("@VIDIOC_QUERYBUF:errno=%d\n",errno);
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
			printf("@mmap:errno=%d\n",errno);
			return 0;
		}
		if(-1 == ioctl(fd, VIDIOC_QBUF, v4l2buf))
		{
			printf("@VIDEOC_QBUF:errno=%d\n",errno);
			return 0;
		}
	}

	//start
	j = V4L2_BUF_TYPE_VIDEO_CAPTURE;
	if(-1 == ioctl(fd, VIDIOC_STREAMON, &j))
	{
		printf("@ON:errno=%d\n",errno);
		return 0;
	}
	pcam->started = 1;

	return fd;

failclose:
	v4l2cam_destroy(cam);
	return 0;
}
void* cameraworker(_obj* cam)
{
	struct percam* pcam = cam->priv_ptr;
	if(0 == pcam){
		say("@cameraworker:0=pcam\n");
		return 0;
	}
	pcam->deq = 0;

	int fd = pcam->fd;
	if(0 == fd)return 0;
	if(0 == pcam->started)return 0;

	//record
	int epfd;
	epfd = epoll_create(256);

	struct epoll_event ev;
	ev.data.fd=fd;
	ev.events = EPOLLIN | EPOLLET;
	epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev);

	int j;
	struct halfrel stack[0x80];
	struct perbuf* myinfo;
	struct v4l2_buffer* v4l2buf;
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

	v4l2cam_destroy(cam);
	return 0;
}




int v4l2cam_take(_obj* cam,void* foot, _syn* stack,int sp, p64 arg,int idx, u8* buf,int len)
{
	struct percam* pcam = cam->priv_ptr;
	if(0 == pcam)return 0;

	u64 addr = pcam->datainfo[(pcam->deq+BUFCNT-1)%BUFCNT].addr;
	say("addr=%llx\n",addr);

	*(u64*)buf = addr;
	return 0;
}
int v4l2cam_give(_obj* cam,void* foot, _syn* stack,int sp, p64 arg,int idx, u8* buf,int len)
{
	return 0;
}
int v4l2cam_attach()
{
	return 0;
}
int v4l2cam_detach()
{
	return 0;
}




int v4l2cam_reader(_obj* cam,void* foot, p64 arg,int idx, u8* buf,int len)
{
	return 0;
}
int v4l2cam_writer(_obj* cam,void* foot, p64 arg,int idx, u8* buf,int len)
{
	return 0;
}
int v4l2cam_delete(_obj* cam)
{
	struct percam* pcam = cam->priv_ptr;
	if(0 == pcam)return 0;

	//set signal
	pcam->alive = 0;

	//wait signal
	//while(-1 != pcam->alive)

	//free data
	memoryfree(cam->priv_ptr);
	cam->priv_ptr = 0;
	return 0;
}
int v4l2cam_create(_obj* cam, void* arg, int argc, u8** argv)
{
	//default value
	u8* path = "/dev/video0";
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
		//say("%d->%.16s\n",j,arg[j]);

		if(0 == ncmp(arg, "path:", 5)){
			path = argv[j]+5;
		}
		if(0 == ncmp(arg, "format:", 7)){
			arg = argv[j]+7;
			//say("format=%.5s\n",arg);
			if(0)return 0;
			else if(0 == ncmp(arg, "mjpeg", 5))format = V4L2_PIX_FMT_MJPEG;
			else if(0 == ncmp(arg, "h264", 4))format = V4L2_PIX_FMT_H264;
			else if(0 == ncmp(arg, "bggr", 4))format = V4L2_PIX_FMT_SBGGR8;
			else if(0 == ncmp(arg, "BG10", 4))format = V4L2_PIX_FMT_SBGGR10;
			else if(0 == ncmp(arg, "rggb", 4))format = V4L2_PIX_FMT_SRGGB8;
			else if(0 == ncmp(arg, "RG10", 4))format = V4L2_PIX_FMT_SRGGB10;
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

	//percam data
	struct percam* pcam = cam->priv_ptr = memoryalloc(0x1000, 4);
	if(0 == pcam){
		say("oom@alloc pcam\n");
		return -1;
	}

	//remember value
	for(j=0;j<0x100;j++){
		if(path[j] <= 0x20){pcam->path[j] = 0;break;}
		else pcam->path[j] = path[j];
	}
	pcam->wantstride = stride;
	pcam->wantheight = height;
	pcam->wantformat = format;

	//enum
	int ret = v4l2cam_prepare(cam);
	if(ret <= 0)return 0;

	//percam thread
	pcam->alive = 1;
	pcam->thread = threadcreate(cameraworker, cam);
	return 0;
}
