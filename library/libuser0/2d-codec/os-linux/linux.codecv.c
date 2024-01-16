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


struct v4l2bs{
	struct v4l2_buffer buf;
	struct v4l2_plane plane;
	void* mem;
	int own;
	int seq;
};
struct v4l2img{
	struct v4l2_buffer buf;
	struct v4l2_plane plane;
	void* mem;
	int own;
	int seq;
};
struct privdata{
	int log;

	int fd;
	int width;
	int height;

	struct v4l2bs bs[2];

	int imgenq;
	int imgdeq;
	struct v4l2img img[2];
};


static void bsthread(_obj* obj)
{
	struct kv88 kv[2] = {
		't', 0,
		 0 , 0
	};
	int j,ret;
	int enq = 0;
	int deq = 0;
	struct privdata* priv = (void*)obj->priv_ptr;
	int fd = priv->fd;
	while(1){
		for(j=0;j<2;j++){
			if(_v4l2_ == priv->bs[j].own)continue;

			if(priv->log)logtoall("bs.enq: id=%d,j=%d\n",enq,j);
			ret = ioctl(fd, VIDIOC_QBUF, &priv->bs[j].buf);
			if(priv->log)logtoall("bs.enq: id=%d,ret=%d\n",enq,ret);

			priv->bs[j].own = _v4l2_;
			priv->bs[j].seq = enq;
			enq++;
		}

		for(j=0;j<2;j++){
			if(_v4l2_ != priv->bs[j].own)continue;
			if(deq != priv->bs[j].seq)continue;

			if(priv->log)logtoall("bs.deq: id=%d,j=%d\n", deq, j);
			priv->bs[j].buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
			ret = ioctl(priv->fd, VIDIOC_DQBUF, &priv->bs[j].buf);

			u64 ts = (u64)priv->bs[j].buf.timestamp.tv_sec*1000*1000*1000 + priv->bs[j].buf.timestamp.tv_usec*1000;
			int len = priv->bs[j].buf.m.planes[0].bytesused;
			if(priv->log)logtoall("bs.deq: id=%d, ret=%d, len=%d, ts=%lld\n", deq, ret, len, ts);

			priv->bs[j].own = _node_;
			deq++;

			if(priv->log)printmemory(priv->bs[j].mem, 16);
			//write(ff, bsmem, len);

			kv[0].val = ts;
			give_data_into_peer_temp_stack(obj,_bs_, (p64)kv,_kv88_, priv->bs[j].mem,len);
		}
	}
}



int codecv_take(_obj* cam,void* foot, _syn* stack,int sp, p64 arg,int idx, u8* buf,int len)
{
	logtoall("codecv_take\n");
	return 0;
}
int codecv_give(_obj* cam,void* foot, _syn* stack,int sp, p64 arg,int idx, u8* buf,int len)
{
	struct privdata* priv = (void*)cam->priv_ptr;
	int w = priv->width;
	int h = priv->height;
	if(priv->log)logtoall("codecv_give\n");
	if(priv->log)printmemory(buf + w*4, 16);
	if(priv->log)printmemory(buf + w*h + w, 16);
	if(priv->log)printmemory(buf + w*h*5/4 + w, 16);

	u64 receivetime = timeread_ns();
	u64 capturetime = 0;
	if(_kv88_ == idx){
		int j;
		struct kv88* kv = (void*)arg;
		for(j=0;j<8;j++){
			if(0 == kv[j].key)break;
			if('t' == kv[j].key){
				capturetime = kv[j].val;
			}
		}
	}
	//printmemory(arg, 0x40);
	if(priv->log)logtoall("encoder: capturetime=%lld, receivetime=%lld\n", capturetime, receivetime);

	int j,ret;
	for(j=0;j<2;j++){
			if(_read_ != priv->img[j].own)continue;
			if(buf != priv->img[j].mem)continue;

			if(priv->log)logtoall("img.enq:j=%d\n",j);
			priv->img[j].plane.bytesused = priv->img[j].buf.m.planes[0].length;
			priv->img[j].buf.timestamp.tv_sec = capturetime/1000/1000/1000;
			priv->img[j].buf.timestamp.tv_usec = (capturetime/1000)%1000000;
			ret = ioctl(priv->fd, VIDIOC_QBUF, &priv->img[j].buf);
			if(-1 != ret){
				if(priv->log)logtoall("img.enq end\n");
			}
			else{
				if(priv->log)logtoall("img.enq:ret=%d,errno=%d\n",ret,errno);
			}

			priv->img[j].own = _v4l2_;
			priv->img[j].seq = priv->imgenq;
			priv->imgenq++;
			return 0;
	}
	if(priv->log)logtoall("codecv_give unknown:%p\n",buf);
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


int codecv_reader(_obj* cam,void* foot, p64 arg,int idx, u8* buf,int len)
{
	int j,ret;
	struct privdata* priv = (void*)cam->priv_ptr;

	for(j=0;j<2;j++){
		if(_node_ == priv->img[j].own)goto find;
	}
	for(j=0;j<2;j++){
		if(_v4l2_ != priv->img[j].own)continue;
		if(priv->imgdeq != priv->img[j].seq)continue;

		if(priv->log)logtoall("img.deq:j=%d\n",j);
		priv->img[j].buf.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
		ret = ioctl(priv->fd, VIDIOC_DQBUF, &priv->img[j].buf);
		if(-1 != ret){
			if(priv->log)logtoall("img.deq end\n");
		}
		else{
			if(priv->log)logtoall("img.deq:ret=%d,errno=%d\n",ret,errno);
		}

		priv->img[j].own = _node_;
		priv->imgdeq++;
		goto find;
	}
	if(priv->log)logtoall("codecv_read nobuf\n");
	return 0;

find:
	priv->img[j].own = _read_;
	*(void**)buf = priv->img[j].mem;
	if(priv->log)logtoall("codecv_read:%p,j=%d\n",priv->img[j].mem,j);
	return 0;
}
int codecv_writer(_obj* cam,void* foot, p64 arg,int idx, u8* buf,int len)
{
	logtoall("codecv_write\n");
	printmemory(buf, 16);

	return 0;
}
int codecv_delete(_obj* cam)
{
	int j;
	struct privdata* priv = (void*)cam->priv_ptr;
	for(j=0;j<2;j++){
		if(priv->img[j].mem){
		}
		if(priv->bs[j].mem){
		}
	}
	if(priv->fd){
		close(priv->fd);
		priv->fd = 0;
	}
	memoryfree(cam->priv_ptr);
	return 0;
}
int codecv_create(_obj* cam, void* arg, int argc, u8** argv)
{
	struct privdata* priv = cam->priv_ptr = memoryalloc(0x1000, 4);
	priv->log = 0;

	int j;
	int width = 640;
	int height = 480;
	for(j=0;j<argc;j++){
		if(0 == ncmp(argv[j], "width:", 6))decstr2u32(argv[j]+6, &width);
		if(0 == ncmp(argv[j], "height:", 7))decstr2u32(argv[j]+7, &height);
		if(0 == ncmp(argv[j], "log:", 4))priv->log = 1;
	}
	logtoall("video encoder:width=%d,height=%d\n",width,height);

	priv->width = width;
	priv->height = height;

	int fd = priv->fd = open("/dev/video11", O_RDWR);
	if(fd <= 0)printf("codec open failed\n");

	struct v4l2_format fm;
	struct v4l2_pix_format_mplane *mp = &fm.fmt.pix_mp;

	//1
	int ret;
	fm.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
	ret = ioctl(fd, VIDIOC_G_FMT, &fm);
	if(ret<0)printf("img.gfmt:ret=%d,errno=%d\n", ret, errno);
	else printf("img.gfmt:w=%d,h=%d,fmt=%.4s\n", mp->width, mp->height, &mp->pixelformat);

	mp->width = width;
	mp->height = height;
	mp->pixelformat = V4L2_PIX_FMT_YUV420;	//V4L2_PIX_FMT_YUYV;
	ret = ioctl(fd, VIDIOC_S_FMT, &fm);
	if(ret<0)printf("img.sfmt:ret=%d,errno=%d\n", ret, errno);
	else printf("img.sfmt\n");

	//2
	fm.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	ret = ioctl(fd, VIDIOC_G_FMT, &fm);
	if(ret<0)printf("bs.gfmt:ret=%d,errno=%d\n", ret, errno);
	else printf("bs.gfmt:w=%d,h=%d\n", mp->width, mp->height);

	mp->width = width;
	mp->height = height;
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
	if(ret<0)printf("sparam:ret=%d,errno=%d\n", ret, errno);
	else printf("sparam\n");

	//img
	struct v4l2_requestbuffers img;
	img.memory = V4L2_MEMORY_MMAP;
	img.count = 2;
	img.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
	ret = ioctl(fd, VIDIOC_REQBUFS, &img);
	if(ret<0)printf("img.reqbuf:ret=%d,errno=%d\n", ret, errno);
	else printf("img.reqbuf\n");

	for(j=0;j<2;j++){
		memset(&priv->img[j].buf, 0, sizeof(struct v4l2_buffer));
		priv->img[j].buf.type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
		priv->img[j].buf.memory = V4L2_MEMORY_MMAP;
		priv->img[j].buf.index = j;
		priv->img[j].buf.length = 1;
		priv->img[j].buf.m.planes = &priv->img[j].plane;
		ret = ioctl(fd, VIDIOC_QUERYBUF, &priv->img[j].buf);
		if(ret<0)printf("img.querybuf:ret=%d,errno=%d\n", ret, errno);
		else printf("img.querybuf:len=%x,offs=%x\n", priv->img[j].buf.m.planes[0].length, priv->img[j].buf.m.planes[0].m.mem_offset);

		unsigned char* imgmem = mmap(NULL, priv->img[j].buf.m.planes[0].length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, priv->img[j].buf.m.planes[0].m.mem_offset);
		printf("imgmem=%p\n", imgmem);
		priv->img[j].mem = imgmem;
		priv->img[j].own = _node_;
		priv->img[j].seq = -1;
	}
	priv->imgenq = 0;
	priv->imgdeq = 0;

	//bs
	struct v4l2_requestbuffers bs;
	bs.memory = V4L2_MEMORY_MMAP;
	bs.count = 2;
	bs.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	ret = ioctl(fd, VIDIOC_REQBUFS, &bs);
	if(ret<0)printf("bs.reqbuf:ret=%d,errno=%d\n", ret, errno);
	else printf("bs.reqbuf\n");

	for(j=0;j<2;j++){
		memset(&priv->bs[j].buf, 0, sizeof(struct v4l2_buffer));
		priv->bs[j].buf.type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
		priv->bs[j].buf.memory = V4L2_MEMORY_MMAP;
		priv->bs[j].buf.index = j;
		priv->bs[j].buf.length = 1;
		priv->bs[j].buf.m.planes = &priv->bs[j].plane;
		ret = ioctl(fd, VIDIOC_QUERYBUF, &priv->bs[j].buf);
		if(ret<0)printf("bs.querybuf:ret=%d,errno=%d\n", ret, errno);
		else printf("bs.querybuf:len=%x,offs=%x\n", priv->bs[j].buf.m.planes[0].length, priv->bs[j].buf.m.planes[0].m.mem_offset);

		unsigned char* bsmem = mmap(NULL, priv->bs[j].buf.m.planes[0].length, PROT_READ | PROT_WRITE, MAP_SHARED, fd, priv->bs[j].buf.m.planes[0].m.mem_offset);
		printf("bsmem=%p\n", bsmem);
		priv->bs[j].mem = bsmem;
		priv->bs[j].own = _node_;
		priv->bs[j].seq = -1;
	}

	//ioctl(fd, VIDIOC_QBUF, &bsbuf);
	//ioctl(fd, VIDIOC_QBUF, &imgbuf);
	int type = V4L2_BUF_TYPE_VIDEO_OUTPUT_MPLANE;
	ret = ioctl(fd, VIDIOC_STREAMON, &type);
	if(ret<0)printf("img.streamon:ret=%d,errno=%d\n", ret, errno);
	else printf("img.streamon\n");

	type = V4L2_BUF_TYPE_VIDEO_CAPTURE_MPLANE;
	ret = ioctl(fd, VIDIOC_STREAMON, &type);
	if(ret<0)printf("bs.streamon:ret=%d,errno=%d\n", ret, errno);
	else printf("bs.streamon\n");

	logtoall("sizeof codecv=%x\n", sizeof(struct privdata));
	threadcreate(bsthread, cam);
	return 0;
}
