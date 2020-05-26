#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>



Display *display;
int screen_num;
static char *appname;

/* --------- XEMBED and systray */
#define SYSTEM_TRAY_REQUEST_DOCK   0
#define SYSTEM_TRAY_BEGIN_MESSAGE   1
#define SYSTEM_TRAY_CANCEL_MESSAGE  2

static int trapped_error_code = 0;
static int (*old_error_handler)(Display *, XErrorEvent *);

static int
error_handler(Display *display, XErrorEvent *error) {
  trapped_error_code = error->error_code;
  return 0;
}

void
trap_errors(void) {
  trapped_error_code = 0;
  old_error_handler = XSetErrorHandler(error_handler);
}

int
untrap_errors(void) {
  XSetErrorHandler(old_error_handler);
  return trapped_error_code;
}

void
send_systray_message(Display *dpy, long message, long data1, long data2, long data3) {
  XEvent ev = {};

  Atom selection_atom = XInternAtom(dpy, "_NET_SYSTEM_TRAY_S0", False);
  Window tray = XGetSelectionOwner(dpy, selection_atom);

  if (tray != None)
    XSelectInput(dpy, tray, StructureNotifyMask);

  ev.xclient.type = ClientMessage;
  ev.xclient.window = tray;
  ev.xclient.message_type = XInternAtom(dpy, "_NET_SYSTEM_TRAY_OPCODE", False);
  ev.xclient.format = 32;
  ev.xclient.data.l[0] = CurrentTime;
  ev.xclient.data.l[1] = message;
  ev.xclient.data.l[2] = data1; /* window */
  ev.xclient.data.l[3] = data2;
  ev.xclient.data.l[4] = data3;

  trap_errors();
  XSendEvent(dpy, tray, False, NoEventMask, &ev);
  XSync(dpy, False);
  if (untrap_errors()) {
    /* Handle errors */
    fprintf(stderr, "%s", appname);
  }
}

int main(int argc, char *argv[]) {
  Window window;
  int x, y;
  unsigned int width, height;
  unsigned int border_width = 1;
  char window_name[64];
  char *icon_name = "HelloX";
  char *display_name = NULL;
  unsigned int display_width, display_height;

  /*  Miscellaneous X variables  */
  XSizeHints *size_hints;
  XWMHints *wm_hints;
  XClassHint *class_hints;
  XTextProperty windowName, iconName;
  XEvent report;
  XFontStruct *font_info;
  XGCValues values;
  GC gc;

  appname = argv[0];

  /*  Allocate memory for our structures  */
  if (!(size_hints = XAllocSizeHints()) ||
      !(wm_hints = XAllocWMHints()) ||
      !(class_hints = XAllocClassHint())) {
    fprintf(stderr, "%s: couldn't allocate memory.\n", appname);
    exit(EXIT_FAILURE);
  }

  /*  Connect to X server  */
  if ((display = XOpenDisplay(display_name)) == NULL) {
    fprintf(stderr, "%s: couldn't connect to X server %s\n",
        appname, display_name);
    exit(EXIT_FAILURE);
  }

  /*  Get screen size from display structure macro  */
  screen_num = DefaultScreen(display);
  display_width = DisplayWidth(display, screen_num);
  display_height = DisplayHeight(display, screen_num);
  sprintf(window_name, "lol w%d h%d", display_width, display_height);

  /*  Set initial window size and position, and create it  */
  x = y = 0;
  width = display_width / 5;
  height = display_height / 3;

  window = XCreateSimpleWindow(display, RootWindow(display, screen_num),
      x, y, width, height, border_width,
      BlackPixel(display, screen_num),
      WhitePixel(display, screen_num));

  /*  Set hints for window manager before mapping window  */
  char *i = &window_name[0];
  if (XStringListToTextProperty(&i, 1, &windowName) == 0) {
    fprintf(stderr, "%s: structure allocation for windowName failed.\n",
        appname);
    exit(EXIT_FAILURE);
  }

  if (XStringListToTextProperty(&icon_name, 1, &iconName) == 0) {
    fprintf(stderr, "%s: structure allocation for iconName failed.\n",
        appname);
    exit(EXIT_FAILURE);
  }

  size_hints->flags = PPosition | PSize;

  wm_hints->flags = StateHint | InputHint;
  wm_hints->initial_state = NormalState;
  wm_hints->input = True;

  class_hints->res_name = appname;
  class_hints->res_class = "hellox";

  XSetWMProperties(display, window, &windowName, &iconName, argv, argc,
      size_hints, wm_hints, class_hints);

  /*  Choose which events we want to handle  */
  XSelectInput(display, window, ExposureMask | KeyPressMask |
      ButtonPressMask | StructureNotifyMask);

  /*  Load a font called "9x15"  */
  if ((font_info = XLoadQueryFont(display, "10x20")) == NULL) {
    fprintf(stderr, "%s: cannot open 10x20 font.\n", appname);
    exit(EXIT_FAILURE);
  }

  /*  Create graphics context  */
  gc = XCreateGC(display, window, 0, &values);
  XSetFont(display, gc, font_info->fid);
  XSetForeground(display, gc, BlackPixel(display, screen_num));

  /*  Display Window  */
  XMapWindow(display, window);

  /*  Enter event loop  */
  while (1) {
    static char *message = "cool nice job";
    static int length;
    static int font_height;
    static int msg_x, msg_y;

    XNextEvent(display, &report);
    KeySym keysym;
    switch (report.type) {
      case Expose:
        if (report.xexpose.count != 0)
          break;

        /*  Output message centrally in window  */
        length = XTextWidth(font_info, message, strlen(message));
        msg_x = (width - length) / 2;

        font_height = font_info->ascent + font_info->descent;
        msg_y = (height + font_height) / 2;

        XDrawString(display, window, gc, msg_x, msg_y,
            message, strlen(message));
        break;

      case ConfigureNotify:
        /*  Store new window width & height  */
        width = report.xconfigure.width;
        height = report.xconfigure.height;
        break;

      case ButtonPress:
        break;

      case KeyPress:
        /*  Clean up and exit  */
        keysym = XLookupKeysym(&report.xkey, 0);
        if (keysym == XK_Escape) {
          XUnloadFont(display, font_info->fid);
          XFreeGC(display, gc);
          XCloseDisplay(display);
          exit(EXIT_SUCCESS);
        } else if (keysym == XK_a) {
          XUnmapWindow(display, window);
          send_systray_message(display, SYSTEM_TRAY_REQUEST_DOCK, window, 0, 0);
        } else if (keysym == XK_s) {
          XMapWindow(display, window);
        } else {
          printf("key %lu\n", keysym);
        }
    }
  }
}
