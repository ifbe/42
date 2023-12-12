#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "libuser.h"
void* supply_alloc();
void* supply_recycle(void*);
void* vulkan_init(int cnt, const char** ext);
void vulkan_exit();
void vulkan_device_create(int, void*);
void vulkan_device_delete();
void vulkan_myctx_create();
void vulkan_myctx_delete();
void drawframe();




struct perobj{
	//glfw
	GLFWwindow* window;
	void* surface;
};
static void* instance;




int freewindow()
{
	vulkan_exit();

	glfwTerminate();

	return 0;
}
int initwindow()
{
	//glfw lib
	glfwInit();
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	//required extension
	uint32_t j,count = 0;
	const char** extension = glfwGetRequiredInstanceExtensions(&count);
	printf("glfwGetRequiredInstanceExtensions:\n");
	for(j=0;j<count;j++){
		printf("%4d:%s\n", j, extension[j]);
	}

	instance = vulkan_init(count, extension);
	return 0;
}




static void callback_keyboard(GLFWwindow* fw, int key, int scan, int action, int mods)
{
	_obj* ogl = glfwGetWindowUserPointer(fw);
	say("@callback_keyboard\n");
}
static void callback_scroll(GLFWwindow* fw, double x, double y)
{
	_obj* ogl = glfwGetWindowUserPointer(fw);
	say("@callback_scroll\n");
}
static void callback_mouse(GLFWwindow* fw, int button, int action, int mods)
{
	_obj* ogl = glfwGetWindowUserPointer(fw);
	say("@callback_mouse\n");
}
static void callback_move(GLFWwindow* fw, double xpos, double ypos)
{
	_obj* ogl = glfwGetWindowUserPointer(fw);
	say("@callback_move\n");
}
static void callback_drop(GLFWwindow* fw, int count, const char** paths)
{
	_obj* ogl = glfwGetWindowUserPointer(fw);
	say("@callback_drop\n");
}
static void callback_reshape(GLFWwindow* fw, int w, int h)
{
	_obj* ogl = glfwGetWindowUserPointer(fw);
	say("@callback_reshape\n");
}




int glfw_window_delete(_obj* ogl)
{
	struct perobj* per = (void*)ogl->priv_256b;

	glfwDestroyWindow(per->window);
	return 0;
}
void* glfw_window_create(_obj* ogl)
{
	GLFWwindow* fw = glfwCreateWindow(1024, 768, "Vulkan window", 0, 0);
	if(0 == fw){
		printf("error@glfwCreateWindow\n");
		return 0;
	}

	//2.setup
	int x,y;
	glfwSetWindowUserPointer(fw, ogl);
	ogl->gl41list.glwnd = fw;

	glfwGetFramebufferSize(fw, &x, &y);
	ogl->whdf.fbwidth = x;
	ogl->whdf.fbheight = y;

	//3.callback
	glfwSetDropCallback(fw, callback_drop);
	glfwSetKeyCallback(fw, callback_keyboard);
	glfwSetScrollCallback(fw, callback_scroll);
	glfwSetCursorPosCallback(fw, callback_move);
	glfwSetMouseButtonCallback(fw, callback_mouse);
	glfwSetFramebufferSizeCallback(fw, callback_reshape);

	return fw;
}




int glfw_surface_delete()
{
	return 0;
}
void* glfw_surface_create(void* window, void* instance)
{
	void* surface;
	if (glfwCreateWindowSurface(instance, window, 0, (void*)&surface) != VK_SUCCESS) {
		printf("error@glfwCreateWindowSurface\n");
		return 0;
	}
	return surface;
}




void window_read()
{
}
void window_write()
{
}
void window_create(_obj* ogl, void* arg, int argc, char** argv)
{
	struct perobj* per = (void*)ogl->priv_256b;

	//glfw: window
	per->window = glfw_window_create(ogl);

	//glfw: surface
	per->surface = glfw_surface_create(per->window, instance);

	//vulkan: device and swapchain
	vulkan_device_create(0, per->surface);

	//vulkan: things
	vulkan_myctx_create();
}
void window_delete(_obj* ogl)
{
	struct perobj* per = (void*)ogl->priv_256b;

	//vulkan: things
	vulkan_myctx_delete();

	//vulkan: device and swapchain
	vulkan_device_delete();

	//glfw
	glfw_surface_delete();

	//window
	glfw_window_delete(ogl);
}




void window_take(_obj* ogl,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf,int len)
{
	struct perobj* per = (void*)ogl->priv_256b;
	//drawframe();

	if(glfwWindowShouldClose(per->window)){
		eventwrite(0,0,(u64)ogl,0);
		return;
	}
	glfwPollEvents();
}
void window_give(_obj* ogl,void* foot, _syn* stack,int sp, p64 arg,int idx, void* buf,int len)
{
}
void window_attach()
{
}
void window_detach()
{
}





void* window_alloc()
{
	return supply_alloc();
}