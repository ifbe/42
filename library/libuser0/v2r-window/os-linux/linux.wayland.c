#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <errno.h>
#include <unistd.h>
#include <wayland-client.h>
#include <wayland-client-protocol.h>
#include <wayland-server-protocol.h>
#include <wayland-egl.h>
#include "libuser.h"




//display
static struct wl_display *display = NULL;
static struct wl_registry *registry = NULL;
static struct wl_compositor *compositor = NULL;
static struct wl_surface *surface;
static struct wl_shell *shell;
static struct wl_shell_surface *shell_surface;
static struct wl_shm *shm;
static struct wl_callback *frame_callback;
//input
struct wl_seat *seat;
struct wl_keyboard *keyboard;
//
static void* shm_data = 0;
static void* buffer = 0;








static int set_cloexec_or_close(int fd)
{
	long flags;

	if (fd == -1)return -1;

	flags = fcntl(fd, F_GETFD);
	if (flags == -1)goto err;

	if (fcntl(fd, F_SETFD, flags | FD_CLOEXEC) == -1)goto err;

	return fd;
err:
	close(fd);
	return -1;
}
static int create_tmpfile_cloexec(char *tmpname)
{
	int fd;

#ifdef HAVE_MKOSTEMP
	fd = mkostemp(tmpname, O_CLOEXEC);
	if (fd >= 0)unlink(tmpname);
#else
	fd = mkstemp(tmpname);
	if (fd >= 0) {
		fd = set_cloexec_or_close(fd);
		unlink(tmpname);
	}
#endif
	return fd;
}
int os_create_anonymous_file(off_t size)
{
	static const char template[] = "/weston-shared-XXXXXX";
	const char *path;
	char *name;
	int fd;

	path = getenv("XDG_RUNTIME_DIR");
	if (!path) {
		errno = ENOENT;
		return -1;
	}

	name = malloc(strlen(path) + sizeof(template));
	if (!name)return -1;

	strcpy(name, path);
	strcat(name, template);

	fd = create_tmpfile_cloexec(name);
	free(name);

	if (fd < 0)return -1;

	if (ftruncate(fd, size) < 0) {
		close(fd);
		return -1;
	}

	return fd;
}

static void keyboard_handle_keymap(void *data, struct wl_keyboard *keyboard, uint32_t format, int fd, uint32_t size)
{
}
static void keyboard_handle_enter(void *data, struct wl_keyboard *keyboard, uint32_t serial, struct wl_surface *surface, struct wl_array *keys)
{
	fprintf(stderr, "Keyboard gained focus\n");
}
static void keyboard_handle_leave(void *data, struct wl_keyboard *keyboard, uint32_t serial, struct wl_surface *surface)
{
	fprintf(stderr, "Keyboard lost focus\n");
}
static void keyboard_handle_key(void *data, struct wl_keyboard *keyboard, uint32_t serial, uint32_t time, uint32_t key, uint32_t state)
{
	if(state == 0)
	{
		if(key == 0x01)eventwrite(0x1b, 0x64626b, 0, 0);
		else if(key == 0x67)eventwrite(0x26, 0x64626b, 0, 0);
		else if(key == 0x6c)eventwrite(0x28, 0x64626b, 0, 0);
		else if(key == 0x69)eventwrite(0x25, 0x64626b, 0, 0);
		else if(key == 0x6a)eventwrite(0x27, 0x64626b, 0, 0);
		else printf("%x\n",key);
	}
	fprintf(stderr, "Key=%x, state=%x\n", key, state);
}
static void keyboard_handle_modifiers(void *data, struct wl_keyboard *keyboard, uint32_t serial, uint32_t mods_depressed, uint32_t mods_latched, uint32_t mods_locked, uint32_t group)
{
	fprintf(stderr, "Modifiers depressed %d, latched %d, locked %d, group %d\n", mods_depressed, mods_latched, mods_locked, group);
}
static const struct wl_keyboard_listener keyboard_listener = {
    keyboard_handle_keymap,
    keyboard_handle_enter,
    keyboard_handle_leave,
    keyboard_handle_key,
    keyboard_handle_modifiers,
};
static void seat_handle_capabilities(void *data, struct wl_seat *seat, enum wl_seat_capability caps)
{
	if(caps & WL_SEAT_CAPABILITY_KEYBOARD)
	{
		keyboard = wl_seat_get_keyboard(seat);
		wl_keyboard_add_listener(keyboard, &keyboard_listener, NULL);
	}
	else if(!(caps & WL_SEAT_CAPABILITY_KEYBOARD))
	{
		wl_keyboard_destroy(keyboard);
		keyboard = NULL;
	}
}
static const struct wl_seat_listener seat_listener = {
    seat_handle_capabilities,
};


static void shm_format(void *data, struct wl_shm *wl_shm, uint32_t format)
{
	char *s;
	switch (format) {
	case WL_SHM_FORMAT_ARGB8888: s = "ARGB8888"; break;
	case WL_SHM_FORMAT_XRGB8888: s = "XRGB8888"; break;
	case WL_SHM_FORMAT_RGB565: s = "RGB565"; break;
	default: s = "other format"; break;
	}
	fprintf(stderr, "Possible shmem format %s\n", s);
}
struct wl_shm_listener shm_listener = {
	shm_format
};


static void global_registry_handler(void *data, struct wl_registry *registry, uint32_t id, const char *interface, uint32_t version)
{
	if (strcmp(interface, "wl_compositor") == 0)
	{
		compositor = wl_registry_bind(registry, id, &wl_compositor_interface, 1);
	}
	else if (strcmp(interface, "wl_shell") == 0)
	{
		shell = wl_registry_bind(registry, id, &wl_shell_interface, 1);
	}
	else if (strcmp(interface, "wl_shm") == 0)
	{
		shm = wl_registry_bind(registry, id, &wl_shm_interface, 1);
		wl_shm_add_listener(shm, &shm_listener, NULL);
	}
	else if (strcmp(interface, "wl_seat") == 0)
	{
		seat = wl_registry_bind(registry, id, &wl_seat_interface, 1);
		wl_seat_add_listener(seat, &seat_listener, NULL);
	}
}
static void global_registry_remover(void *data, struct wl_registry *registry, uint32_t id)
{
	printf("Got a registry losing event for %d\n", id);
}
static const struct wl_registry_listener registry_listener = {
	global_registry_handler,
	global_registry_remover
};


static void handle_ping(void *data, struct wl_shell_surface *shell_surface, uint32_t serial)
{
	wl_shell_surface_pong(shell_surface, serial);
	fprintf(stderr, "Pinged and ponged\n");
}
static void handle_configure(void *data, struct wl_shell_surface *shell_surface, uint32_t edges, int32_t width, int32_t height)
{
}
static void handle_popup_done(void *data, struct wl_shell_surface *shell_surface)
{
}
static const struct wl_shell_surface_listener shell_surface_listener = {
	handle_ping,
	handle_configure,
	handle_popup_done
};








void* uievent(struct supply* p)
{
	struct wl_shm_pool* pool;
	int ret;
	int fd;

	//display
	display = wl_display_connect(NULL);
	if (display == NULL)
	{
		fprintf(stderr, "Can't connect to display\n");
		exit(-1);
	}
	printf("connected to display\n");

	//registry
	registry = wl_display_get_registry(display);
	wl_registry_add_listener(registry, &registry_listener, NULL);

	wl_display_dispatch(display);
	wl_display_roundtrip(display);

	if (compositor == NULL)
	{
		fprintf(stderr, "Can't find compositor\n");
		exit(-1);
	}
	fprintf(stderr, "Found compositor\n");

	//surface
	surface = wl_compositor_create_surface(compositor);
	if (surface == NULL)
	{
		fprintf(stderr, "Can't create surface\n");
		exit(-1);
	}
	else fprintf(stderr, "Created surface\n");

	//shell_surface
	shell_surface = wl_shell_get_shell_surface(shell, surface);
	if (shell_surface == NULL)
	{
		fprintf(stderr, "Can't create shell surface\n");
		exit(-1);
	}
	else fprintf(stderr, "Created shell surface\n");

	//?
	wl_shell_surface_set_toplevel(shell_surface);
	wl_shell_surface_add_listener(shell_surface, &shell_surface_listener, NULL);

	//
	fd = os_create_anonymous_file(2048*1024*4);
	if (fd < 0) {
		fprintf(stderr, "creating a buffer file failed: %m\n");
		exit(1);
	}

	//
	shm_data = mmap(p->buf, 2048*1024*4,
		PROT_READ | PROT_WRITE, MAP_FIXED|MAP_SHARED, fd, 0);
	if (shm_data == MAP_FAILED) {
		fprintf(stderr, "mmap failed: %m\n");
		close(fd);
		exit(1);
	}

	//
	pool = wl_shm_create_pool(shm, fd, 2048*1024*4);
	buffer = wl_shm_pool_create_buffer(
		pool, 0,
		p->width, p->height,
		(p->width)*4,
		WL_SHM_FORMAT_XRGB8888
	);
	wl_shm_pool_destroy(pool);

	//loop
	while(1)
	{
		ret = wl_display_roundtrip(display);
	}

	wl_display_disconnect(display);
}
void windowread()
{
}
void windowwrite(struct supply* p)
{
	wl_display_dispatch(display);
	wl_surface_damage(surface, 0, 0, p->width, p->height);
	wl_surface_attach(surface, p->buf, 0, 0);
	wl_surface_commit(surface);
}
void windowlist()
{
}
void windowchange()
{
}
void windowstart()
{
}
void windowstop()
{
}
void windowdelete(struct supply* w)
{
}
void windowcreate(struct supply* w)
{
	u64 m = (u64)malloc(2048*1024*4 + 0x100000);
	w->buf = m - (m&0xfffff) + 0x100000;

	w->width = w->stride = 512;
	w->height = 512;
	w->thread = startthread(uievent, w);
}




void initwindow()
{
}
void freewindow()
{
}
