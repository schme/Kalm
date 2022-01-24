#include <GL/glx.h>
#include <X11/X.h>
#include <X11/Xlib.h>

#include <cstdio>

#include "player/player.h"

namespace ks
{

}

typedef GLXContext (*glXCreateContextAttribsARBProc)
    (Display*, GLXFBConfig, GLXContext, Bool, const int*);

int main()
{
    Display* disp = 0;
    Window win = 0;

    disp = XOpenDisplay(0);

	int width = 1280;
	int height = 720;

	// TODO: Ask for display resolution
    win = XCreateSimpleWindow(disp, DefaultRootWindow(disp),
                              0, 0,   /* x, y */
                              width, height, /* width, height */
                              0, 0,     /* border_width, border */
                              0);       /* background */

    /* Create_the_modern_OpenGL_context
       -------------------------------- */
    static int visual_attribs[] = {
        GLX_RENDER_TYPE, GLX_RGBA_BIT,
        GLX_DRAWABLE_TYPE, GLX_WINDOW_BIT,
        GLX_DOUBLEBUFFER, true,
        GLX_RED_SIZE, 1,
        GLX_GREEN_SIZE, 1,
        GLX_BLUE_SIZE, 1,
        None
    };

    int num_fbc = 0;
    GLXFBConfig *fbc = glXChooseFBConfig(disp,
                                         DefaultScreen(disp),
                                         visual_attribs, &num_fbc);
    if (!fbc) {
		printf("glXChooseFBConfig() failed\n");
		return 1;
    }

    /* For details, refer to the spec
       (https://www.khronos.org/registry/OpenGL/extensions/ARB/GLX_ARB_get_proc_address.txt)
       which says:
           > Are function pointers context-independent? Yes. The pointer to an
           > extension function can be used with any context [...]

       This is in direct contrast to WGL's wglGetProcAddress. */

    glXCreateContextAttribsARBProc glXCreateContextAttribsARB = 0;
    glXCreateContextAttribsARB =
        (glXCreateContextAttribsARBProc)
        glXGetProcAddress((const GLubyte*)"glXCreateContextAttribsARB");

    if (!glXCreateContextAttribsARB) {
		printf("glXCreateContextAttribsARB() not found\n");
		return 1;
    }

    /* Set desired minimum OpenGL version */
    static int context_attribs[] = {
        GLX_CONTEXT_MAJOR_VERSION_ARB, 4,
        GLX_CONTEXT_MINOR_VERSION_ARB, 6,
        None
    };

    /* Create OpenGL context */
    GLXContext ctx = glXCreateContextAttribsARB(disp, fbc[0], NULL, true,
                                                context_attribs);
    if (!ctx) {
		printf("Failed to create OpenGL context. Exiting.\n");
		return 1;
    }

    /* Show_the_window
       --------------- */

	XSelectInput(disp, win, KeyPressMask);
    XMapWindow(disp, win);
    glXMakeCurrent(disp, win, ctx);

    int majorGl = 0, minorGl = 0;
    glGetIntegerv(GL_MAJOR_VERSION, &majorGl);
    glGetIntegerv(GL_MINOR_VERSION, &minorGl);

	printf("OpenGL context created.\nVersion %d.%d\nVendor %s\nRenderer %s\n",
		   majorGl, minorGl,
		   glGetString(GL_VENDOR),
		   glGetString(GL_RENDERER));


	XEvent ev;

    /* Demo init
       ---------------- */
	bool shouldQuit = false;

	ks::Player player;
	player.init(width, height);

	int status = 0;
	while (!shouldQuit) {

		bool hasEvent = XCheckWindowEvent(disp, win, KeyPressMask, &ev);
		if (hasEvent) {
			KeySym keysym = XLookupKeysym(&ev.xkey, 0);

			if (keysym == XK_Escape) {
				shouldQuit = true;
			}
		}

		status = player.play(&shouldQuit);
	}

	return status;
}
