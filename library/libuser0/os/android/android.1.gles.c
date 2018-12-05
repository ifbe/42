#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <jni.h>
#include <errno.h>
#include <EGL/egl.h>
#include <GLES/gl.h>
#include <GLES3/gl3.h>
#include <GLES3/gl3ext.h>
#include <android/log.h>
#include <android_native_app_glue.h>
#include "libuser.h"
#define LOG_TAG "finalanswer"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
void asset_create();
//
void initobject(void*);
void initshader(void*);
void inittexture(void*);
void initvertex(void*);
void callback_update(void*);
void callback_display(void*,void*);




//
static EGLDisplay display = EGL_NO_DISPLAY;
static EGLContext context = EGL_NO_CONTEXT;
static EGLSurface surface = EGL_NO_SURFACE;
static struct android_app* theapp = 0;
static struct arena* thewin = 0;
static int status = 0;
void setapp(void* addr)
{
	theapp = addr;
}



void windowprepare(struct arena* win)
{
	int x,y,z;
	display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	eglInitialize(display, 0, 0);

	const EGLint configAttribs[] = {
		//		EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
		//		EGL_CONFORMANT, EGL_OPENGL_ES2_BIT,
		EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,
		EGL_BLUE_SIZE, 8,
		EGL_GREEN_SIZE, 8,
		EGL_RED_SIZE, 8,
		EGL_ALPHA_SIZE, 8,
		EGL_DEPTH_SIZE, 24,
		EGL_NONE
	};
	EGLConfig config;
	EGLint numConfigs;
	eglChooseConfig(display, configAttribs, &config, 1, &numConfigs);

	EGLint format;
	eglGetConfigAttrib(display, config, EGL_NATIVE_VISUAL_ID, &format);

	x = ANativeWindow_getWidth(theapp->window);
	y = ANativeWindow_getHeight(theapp->window);
	z = (x+y)/2;
	win->width  = win->fbwidth  = x;
	win->stride = win->fbstride = x;
	win->height = win->fbheight = y;
	win->depth  = win->fbdepth  = z;
	ANativeWindow_setBuffersGeometry(theapp->window, 0, 0, format);

	surface = eglCreateWindowSurface(display, config, theapp->window, NULL);

	EGLint contextAttribs[] = {
		EGL_CONTEXT_CLIENT_VERSION, 3,
		EGL_NONE
	};
	context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
	if (context == EGL_NO_CONTEXT) {
		LOGI("eglCreateContext failed with error 0x%04x", eglGetError());
		return;
	}

	if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
		LOGI("eglMakeCurrent failed with error 0x%04x", eglGetError());
		return;
	}

	LOGI("GL Version = %s\n", glGetString(GL_VERSION));
	LOGI("GL Vendor = %s\n", glGetString(GL_VENDOR));
	LOGI("GL Renderer = %s\n", glGetString(GL_RENDERER));
	LOGI("GL Extensions = %s\n", glGetString(GL_EXTENSIONS));
}
void windowunload()
{
	if (EGL_NO_DISPLAY != display)
	{
		eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		if (context != EGL_NO_CONTEXT)eglDestroyContext(display, context);
		if (surface != EGL_NO_SURFACE)eglDestroySurface(display, surface);
		eglTerminate(display);
	}
	display = EGL_NO_DISPLAY;
	context = EGL_NO_CONTEXT;
	surface = EGL_NO_SURFACE;
}
static void handle_cmd(struct android_app* app, int32_t cmd)
{
	LOGI("app=%llx,cmd=%x\n", (u64)app, cmd);
	if(APP_CMD_START == cmd)
	{
		LOGI("APP_CMD_START");
	}
	else if(APP_CMD_RESUME == cmd)
	{
		LOGI("APP_CMD_RESUME");
	}
	else if(APP_CMD_PAUSE == cmd)
	{
		LOGI("APP_CMD_PAUSE");
	}
	else if(APP_CMD_STOP == cmd)
	{
		LOGI("APP_CMD_STOP");
	}
	else if(APP_CMD_DESTROY == cmd)
	{
		LOGI("APP_CMD_DESTROY");
	}
	else if(APP_CMD_GAINED_FOCUS == cmd)
	{
		LOGI("APP_CMD_GAINED_FOCUS");
	}
	else if(APP_CMD_LOST_FOCUS == cmd)
	{
		LOGI("APP_CMD_LOST_FOCUS");
	}
	else if(APP_CMD_INIT_WINDOW == cmd)
	{
		LOGI("APP_CMD_INIT_WINDOW");
		//initDisplay(appState);
		windowprepare(thewin);

		initobject(thewin);
		initshader(thewin);
		inittexture(thewin);
		initvertex(thewin);
		status = 1;
	}
	else if(APP_CMD_WINDOW_RESIZED == cmd)
	{
		LOGI("APP_CMD_WINDOW_RESIZED");
	}
	else if(APP_CMD_TERM_WINDOW == cmd)
	{
		LOGI("APP_CMD_TERM_WINDOW");
		//appState->windowInitialized = false;
		//termDisplay(appState);
		windowunload();
	}
	else if(APP_CMD_SAVE_STATE == cmd)
	{
		LOGI("APP_CMD_SAVE_STATE");
		//app->savedState = malloc(sizeof(SavedState));
		//app->savedStateSize = sizeof(SavedState);
		//app->savedState = appState->savedState;
	}
	else if(APP_CMD_CONFIG_CHANGED == cmd)
	{
		LOGI("APP_CMD_CONFIG_CHANGED");
	}
	else
	{
		LOGI("Unknown CMD: %d", cmd);
	}
	//appState->running = (appState->resumed && appState->windowInitialized && appState->focused);
}
static int32_t handle_input(struct android_app* app, AInputEvent* ev)
{
	u64 why[4];
	int x,y,a,c,j;
	int32_t type;
	int32_t source;
	//LOGI("app=%llx,ev=%llx\n", (u64)app, (u64)ev);

	type = AInputEvent_getType(ev);
	if(AINPUT_EVENT_TYPE_KEY == type)
	{
		LOGI("!!!!!!!\n");
		eventwrite(0,0,0,0);
		app->destroyRequested = 1;
	}
	else if(AINPUT_EVENT_TYPE_MOTION == type)
	{
		source = AInputEvent_getSource(ev);
		if(AINPUT_SOURCE_TOUCHSCREEN == source)
		{
			a = AMotionEvent_getAction(ev);
			c = AMotionEvent_getPointerCount(ev);
			LOGI("a=%x,c=%x\n",a,c);

			j = (a>>8)&0xf;
			a &= 0xf;
			if(2 == a)
			{
				for(j=0;j<c;j++)
				{
					x = AMotionEvent_getX(ev, j);
					y = AMotionEvent_getY(ev, j);
					why[0] = j;
					why[0] = x+(y<<16)+(why[0]<<48);
					//eventwrite(why, 0x4070, (u64)thewin, 0);

					why[1] = 0x4070;
					why[2] = (u64)thewin;
					actorevent((void*)why);
				}
			}
			else
			{
				if((0==a)|(5==a))a = 0x2b70;
				else if((1==a)|(6==a))a = 0x2d70;

				x = AMotionEvent_getX(ev, j);
				y = AMotionEvent_getY(ev, j);
				why[0] = AMotionEvent_getPointerId(ev, j);
				why[0] = x+(y<<16)+(why[0]<<48);
				why[1] = a;
				why[2] = (u64)thewin;
				actorevent((void*)why);
			}
		}
		else if(AINPUT_SOURCE_TRACKBALL == source)
		{
		}
	}
	return 0;
}




void windowread(struct arena* win)
{
	int ident;
	int events;
	struct android_poll_source* source;
	if(status)
	{
		actorread_all(win);
		callback_update(win);
		callback_display(win, 0);
		eglSwapBuffers(display, surface);
	}

	while((ident=ALooper_pollAll(0, NULL, &events, (void**)&source)) >= 0)
	{
		if(source)source->process(theapp, source);
		if(theapp->destroyRequested)return;
	}
}
void windowwrite(void* dc,void* df,void* sc,void* sf,void* buf,int len)
{
}
void windowstop()
{
}
void windowstart()
{
}
void windowdelete(struct arena* win)
{
}
void windowcreate(struct arena* win)
{
	win->fmt = _vbo_;
	win->width  = win->fbwidth  = 1024;
	win->stride = win->fbstride = 1024;
	win->height = win->fbheight = 1024;
	win->depth  = win->fbdepth  = 1024;

	thewin = win;
	theapp->onAppCmd = handle_cmd;
	theapp->onInputEvent = handle_input;
}




void initwindow()
{
}
void freewindow()
{
}
