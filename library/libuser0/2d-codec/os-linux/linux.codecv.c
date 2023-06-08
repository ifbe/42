#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/videodev2.h>
#include "libuser.h"
//
#define _bs_ hex16('b','s')
//
#define _v4l2_ hex32('v','4','l','2')
#define _node_ hex32('n','o','d','e')
#define _read_ hex32('r','e','a','d')


struct privdata{
	int fd;

	struct v4l2_buffer bsbuf;
	struct v4l2_plane bsplane;
	void* bsmem;
	int bsown;

	struct v4l2_buffer imgbuf;
	struct v4l2_plane imgplane;
	void* imgmem;
	int imgown;
};


int codecv_take(_obj* cam,void* foot, _syn* stack,int sp, void* arg,int idx, u8* buf,int len)
{
    say("codecv_take\n");
	return 0;
}
int codecv_give(_obj* cam,void* foot, _syn* stack,int sp, void* arg,int idx, u8* buf,int len)
{
    say("codecv_give\n");
	printmemory(buf +640*4, 16);
	printmemory(buf+640*480 +640, 16);
	printmemory(buf+640*480*5/4 +640, 16);

	struct privdata* priv = (void*)cam->priv_256b;
	if(_read_ == priv->imgown){
		priv->imgplane.bytesused = priv->imgbuf.m.planes[0].length;
		ioctl(priv->fd, VIDIOC_QBUF, &priv->imgbuf);
		priv->imgown = _v4l2_;
	}
	return 0;
}
int codecv_attach()
{
	return 0;
}
int codecv_detach()
{
	return 0;
}


static void bsthread(_obj* obj)
{
	struct privdata* priv = (void*)obj->priv_256b;
	int fd = priv->fd;
	while(1){
		ioctl(fd, VIDIOC_QBUF, &priv->bsbuf);

		// Dequeue the capture buffer, write out the encoded frame and queue it back.
		priv->bsbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
		ioctl(priv->fd, VIDIOC_DQBUF, &priv->bsbuf);

		int len = priv->bsbuf.m.planes[0].bytesused;
		say("encoded bs:\n");
		printmemory(priv->bsmem, 16);
		//write(ff, bsmem, len);
		give_data_into_peer_temp_stack(obj,_bs_, 0,0, priv->bsmem,len);
	}
}


int codecv_reader(_obj* cam,void* foot, void* arg,int idx, u8* buf,int len)
{
	struct privdata* priv = (void*)cam->priv_256b;
    say("codecv_read:%p\n",priv->imgmem);

	// Dequeue the output buffer, read the next frame and queue it back.
	if(_v4l2_ == priv->imgown){
		priv->imgbuf.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
		ioctl(priv->fd, VIDIOC_DQBUF, &priv->imgbuf);
		priv->imgown = _node_;
	}
	*(void**)buf = priv->imgmem;
	priv->imgown = _read_;
	return 0;
}
int codecv_writer(_obj* cam,void* foot, void* arg,int idx, u8* buf,int len)
{
    say("codecv_write\n");
	printmemory(buf, 16);

	return 0;
}
int codecv_delete(_obj* cam)
{
	struct privdata* priv = (void*)cam->priv_256b;
	if(priv->fd){
		close(priv->fd);
		priv->fd = 0;
	}
	return 0;
}
int codecv_create(_obj* cam, void* arg, int argc, u8** argv)
{
	struct privdata* priv = (void*)cam->priv_256b;
	int fd = priv->fd = open("/dev/video11", O_RDWR);

	struct v4l2_format fm;
	struct v4l2_pix_format_mplane *mp = &fm.fmt.pix_mp;

	//1
	int ret;
	fm.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
	ret = ioctl(fd, VIDIOC_G_FMT, &fm);
	if(ret<0)printf("img.gfmt:ret=%d,errno=%d\n", ret, errno);
	else printf("img.gfmt:w=%d,h=%d,fmt=%.4s\n", mp->width, mp->height, &mp->pixelformat);

	mp->width = 640;
	mp->height = 480;
	mp->pixelformat = V4L2_PIX_FMT_YUV420;	//V4L2_PIX_FMT_YUYV;
	ret = ioctl(fd, VIDIOC_S_FMT, &fm);
	if(ret<0)printf("img.sfmt:ret=%d,errno=%d\n", ret, errno);
	else printf("img.sfmt\n");

	//2
	fm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	ret = ioctl(fd, VIDIOC_G_FMT, &fm);
	if(ret<0)printf("bs.gfmt:ret=%d,errno=%d\n", ret, errno);
	else printf("bs.gfmt:w=%d,h=%d\n", mp->width, mp->height);

	mp->width = 640;
	mp->height = 480;
	ret = ioctl(fd, VIDIOC_S_FMT, &fm);
	if(ret<0)printf("bs.sfmt:ret=%d,errno=%d\n", ret, errno);
	else printf("bs.sfmt\n");

	//3
	struct v4l2_streamparm stream;
	memset(&stream, 0, sizeof(stream));
	stream.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
	stream.parm.output.timeperframe.numerator = 1;
	stream.parm.output.timeperframe.denominator = 30;
	ret = ioctl(fd, VIDIOC_S_PARM, &stream);
	if(ret<0)printf("sparam:ret=%d,errno=%d\n",errno);
	else printf("sparam\n");

	struct v4l2_requestbuffers img;
	img.memory = V4L2_MEMORY_MMAP;
	img.count = 1;
	img.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
	ret = ioctl(fd, VIDIOC_REQBUFS, &img);
	if(ret<0)printf("img.reqbuf:ret=%d,errno=%d\n",errno);
	else printf("img.reqbuf\n");

	struct v4l2_requestbuffers bs;
	bs.memory = V4L2_MEMORY_MMAP;
	bs.count = 1;
	bs.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	ret = ioctl(fd, VIDIOC_REQBUFS, &bs);
	if(ret<0)printf("bs.reqbuf:ret=%d,errno=%d\n",errno);
	else printf("bs.reqbuf\n");

	//img
	memset(&priv->imgbuf, 0, sizeof(struct v4l2_buffer));
	priv->imgbuf.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
	priv->imgbuf.memory = V4L2_MEMORY_MMAP;
	priv->imgbuf.index = 0;
	priv->imgbuf.length = 1;
	priv->imgbuf.m.planes = &priv->imgplane;
	ret = ioctl(fd, VIDIOC_QUERYBUF, &priv->imgbuf);
	if(ret<0)printf("img.querybuf:ret=%d,errno=%d\n",errno);
	else printf("img.querybuf:len=%x,offs=%x\n", priv->imgbuf.m.planes[0].length, priv->imgbuf.m.planes[0].m.mem_offset);

	unsigned char* imgmem = mmap(NULL, priv->imgbuf.m.planes[0].length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, priv->imgbuf.m.planes[0].m.mem_offset);
	printf("imgmem=%p\n", imgmem);
	priv->imgmem = imgmem;
	priv->imgown = _node_;

	//bs
	memset(&priv->bsbuf, 0, sizeof(struct v4l2_buffer));
	priv->bsbuf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	priv->bsbuf.memory = V4L2_MEMORY_MMAP;
	priv->bsbuf.index = 0;
	priv->bsbuf.length = 1;
	priv->bsbuf.m.planes = &priv->bsplane;
	ret = ioctl(fd, VIDIOC_QUERYBUF, &priv->bsbuf);
	if(ret<0)printf("bs.querybuf:ret=%d,errno=%d\n",errno);
	else printf("bs.querybuf:len=%x,offs=%x\n", priv->bsbuf.m.planes[0].length, priv->bsbuf.m.planes[0].m.mem_offset);

	unsigned char* bsmem = mmap(NULL, priv->bsbuf.m.planes[0].length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, priv->bsbuf.m.planes[0].m.mem_offset);
	printf("bsmem=%p\n", bsmem);
	priv->bsmem = bsmem;
	priv->bsown = _node_;

	//ioctl(fd, VIDIOC_QBUF, &bsbuf);
	//ioctl(fd, VIDIOC_QBUF, &imgbuf);
	int type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
	ret = ioctl(fd, VIDIOC_STREAMON, &type);
	if(ret<0)printf("img.streamon:ret=%d,errno=%d\n",errno);
	else printf("img.streamon\n");

	type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	ret = ioctl(fd, VIDIOC_STREAMON, &type);
	if(ret<0)printf("bs.streamon:ret=%d,errno=%d\n",errno);
	else printf("bs.streamon\n");

	say("sizeof codecv=%x", sizeof(struct privdata));
	threadcreate(bsthread, cam);
	return 0;
}
