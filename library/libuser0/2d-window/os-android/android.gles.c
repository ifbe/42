#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
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
void* getapp();
void* pollenv();
void initobject(void*);
void initshader(void*);
void inittexture(void*);
void initvertex(void*);
void fullwindow_create(void*);
void fullwindow_delete(void*);
void fullwindow_take(void*,void*, void*,int, void*,int, void*,int);
void fullwindow_give(void*,void*, void*,int, void*,int, void*,int);




//
static EGLDisplay display = EGL_NO_DISPLAY;
static EGLContext context = EGL_NO_CONTEXT;
static EGLSurface surface = EGL_NO_SURFACE;
static int width = 0;
static int height = 0;
//
static struct android_app* theapp = 0;
static int candraw = 0;



void openwindow()
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

	width = ANativeWindow_getWidth(theapp->window);
	height = ANativeWindow_getHeight(theapp->window);
	ANativeWindow_setBuffersGeometry(theapp->window, 0, 0, format);

	surface = eglCreateWindowSurface(display, config, theapp->window, NULL);

	EGLint contextAttribs[] = {
		EGL_CONTEXT_CLIENT_VERSION, 3,
		EGL_NONE
	};
	context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttribs);
	if (context == EGL_NO_CONTEXT) {
		say("eglCreateContext failed with error 0x%04x\n", eglGetError());
		return;
	}

	if (eglMakeCurrent(display, surface, surface, context) == EGL_FALSE) {
		say("eglMakeCurrent failed with error 0x%04x\n", eglGetError());
		return;
	}

	say("GL Version = %s\n", glGetString(GL_VERSION));
	say("GL Vendor = %s\n", glGetString(GL_VENDOR));
	say("GL Renderer = %s\n", glGetString(GL_RENDERER));
	say("GL Extensions = %s\n", glGetString(GL_EXTENSIONS));
}
void closewindow()
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
void cleanwindow()
{
	int j;
	struct supply* wnd = theapp->userData;

	struct gl41data** cam = wnd->glfull_camera;
	for(j=0;j<64;j++){
		if(cam[j])bzero(&cam[j]->dst, sizeof(struct gldst));
	}

	struct gl41data** lit = wnd->glfull_light;
	for(j=0;j<64;j++){
		if(lit[j])bzero(&lit[j]->dst, sizeof(struct gldst));
	}

	struct gl41data** solid = wnd->glfull_solid;
	for(j=0;j<64;j++){
		if(solid[j])bzero(&solid[j]->dst, sizeof(struct gldst));
	}

	struct gl41data** opaque = wnd->glfull_opaque;
	for(j=0;j<64;j++){
		if(opaque[j])bzero(&opaque[j]->dst, sizeof(struct gldst));
	}
}




static void handle_cmd(struct android_app* app, int32_t cmd)
{
	say("app=%llx,cmd=%x\n", (u64)app, cmd);
	switch(cmd){
	case APP_CMD_DESTROY:
		say("APP_CMD_DESTROY\n");
		eventwrite(0,0,0,0);
		break;
	case APP_CMD_SAVE_STATE:
		say("APP_CMD_SAVE_STATE\n");
		//app->savedState = malloc(sizeof(SavedState));
		//app->savedStateSize = sizeof(SavedState);
		//app->savedState = appState->savedState;
		break;

	case APP_CMD_START:
		say("APP_CMD_START\n");
		break;
	case APP_CMD_STOP:
		say("APP_CMD_STOP\n");
		break;

	case APP_CMD_RESUME:
		say("APP_CMD_RESUME\n");
		break;
	case APP_CMD_PAUSE:
		say("APP_CMD_PAUSE\n");
		break;

	case APP_CMD_INIT_WINDOW:
		say("APP_CMD_INIT_WINDOW\n");
		openwindow();
		candraw = 1;
		break;
	case APP_CMD_TERM_WINDOW:
		say("APP_CMD_TERM_WINDOW\n");
		candraw = 0;
		closewindow();
		cleanwindow();
		break;

	case APP_CMD_GAINED_FOCUS:
		say("APP_CMD_GAINED_FOCUS\n");
		break;
	case APP_CMD_LOST_FOCUS:
		say("APP_CMD_LOST_FOCUS\n");
		break;

	case APP_CMD_WINDOW_RESIZED:
		say("APP_CMD_WINDOW_RESIZED\n");
		break;
	case APP_CMD_CONFIG_CHANGED:
		say("APP_CMD_CONFIG_CHANGED\n");
		break;
	case APP_CMD_INPUT_CHANGED:
		say("APP_CMD_INPUT_CHANGED\n");
		break;
	case APP_CMD_WINDOW_REDRAW_NEEDED:
		say("APP_CMD_WINDOW_REDRAW_NEEDED\n");
		break;
	case APP_CMD_CONTENT_RECT_CHANGED:
		say("APP_CMD_CONTENT_RECT_CHANGED\n");
		break;
	case APP_CMD_LOW_MEMORY:
		say("APP_CMD_LOW_MEMORY\n");
		break;

	default:
		say("Unknown CMD: %d\n", cmd);
	}
	//appState->running = (appState->resumed && appState->windowInitialized && appState->focused);
}
static int32_t handle_input(struct android_app* app, AInputEvent* ev)
{
	u64 why[4];
	int x,y,z;
	int act,cnt,j;
	int32_t type;
	int32_t source;
	//say("app=%llx,ev=%llx\n", (u64)app, (u64)ev);

	type = AInputEvent_getType(ev);
	switch(type){
	case AINPUT_EVENT_TYPE_KEY:
		act = AKeyEvent_getKeyCode(ev);
		say("AINPUT_EVENT_TYPE_KEY:%x\n", act);
		//app->destroyRequested = 1;
		break;
	case AINPUT_EVENT_TYPE_MOTION:
		source = AInputEvent_getSource(ev);
		if(AINPUT_SOURCE_TOUCHSCREEN == source){
			act = AMotionEvent_getAction(ev);
			cnt = AMotionEvent_getPointerCount(ev);
			say("act=%x,cnt=%x\n", act, cnt);

			j = (act>>8)&0xf;
			act &= 0xf;
			switch(act){
			case 2:
				for(j=0;j<cnt;j++){
					x = AMotionEvent_getX(ev, j);
					y = AMotionEvent_getY(ev, j);
					why[0] = j;
					why[0] = x+(y<<16)+(why[0]<<48);
					why[1] = touch_move;
					why[2] = (u64)(theapp->userData);
					//fullwindow_write(theapp->userData, (void*)why);
					eventwrite(why[0], why[1], why[2], why[3]);
				}
				return 0;
			case 0:
			case 5:
				act = touch_onto;
				break;
			case 1:
			case 6:
				act = touch_away;
				break;
			default:
				say("AMotionEvent_getAction=%x\n",act);
				return 0;
			}

			x = AMotionEvent_getX(ev, j);
			y = AMotionEvent_getY(ev, j);
			why[0] = AMotionEvent_getPointerId(ev, j);
			why[0] = x+(y<<16)+(why[0]<<48);
			why[1] = act;
			why[2] = (u64)(theapp->userData);
			//fullwindow_write(theapp->userData, (void*)why);
			eventwrite(why[0], why[1], why[2], why[3]);
		}//AINPUT_SOURCE_TOUCHSCREEN
		else if(AINPUT_SOURCE_TRACKBALL == source){
			say("AINPUT_SOURCE_TRACKBALL\n");
		}
		break;
	default:
		say("AInputEvent_getType=%x\n",type);
	}//switch
	return 0;
}
int checkevent()
{
	int ident;
	int events;
	struct android_poll_source* source;

	while((ident=ALooper_pollAll(0, NULL, &events, (void**)&source)) >= 0)
	{
		if(source)source->process(theapp, source);
		if(theapp->destroyRequested)return 0;
	}
	return 0;
}




int window_take(struct supply* wnd,void* foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	if(wnd != theapp->userData)return 0;
	if(candraw){
		fullwindow_take(wnd,foot, stack,sp, arg,key, buf,len);
		eglSwapBuffers(display, surface);
	}
	checkevent();
	return 0;
}
int window_give(struct supply* wnd,void* foot, struct halfrel* stack,int sp, void* arg,int key, void* buf,int len)
{
	switch(wnd->fmt){
	default:fullwindow_give(wnd,foot, stack,sp, arg,key, buf,len);
	}
	return 0;
}
void windowstop()
{
}
void windowstart()
{
}
void windowdelete(struct supply* wnd)
{
}
void windowcreate(struct supply* wnd, void* arg)
{
	say("@windowcreate\n");
	switch(wnd->fmt){
	default:{
		theapp->userData = wnd;
		wnd->fbwidth = wnd->width = width;
		wnd->fbheight= wnd->height= height;
		say("w=%d,h=%d\n", width, height);

		fullwindow_create(wnd);
		break;
	}//default
	}//switch
}




void initwindow()
{
	theapp = getapp();
	theapp->onAppCmd = handle_cmd;
	theapp->onInputEvent = handle_input;
	while(0==height)checkevent();
}
void freewindow()
{
}
