#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>

/* quits when ctrl-shift-y is pressed in any X application */
int main() {
  Display *dpy = XOpenDisplay(0);
  Window root = DefaultRootWindow(dpy);
  XEvent ev;

  unsigned int modifiers = ControlMask | ShiftMask;
  int keycode = XKeysymToKeycode(dpy, XK_Y);

  XGrabKey(dpy, keycode, modifiers, root, False,
      GrabModeAsync, GrabModeAsync);
  XSelectInput(dpy, root, KeyPressMask);

  int shouldQuit = 0;
  while (1) {
    XNextEvent(dpy, &ev);
    switch (ev.type) {
      case KeyPress:
        XUngrabKey(dpy, keycode, modifiers, root);
        shouldQuit = 1;
      default:
        break;
    }

    if (shouldQuit)
      break;
  }

  XCloseDisplay(dpy);
  return 0;
}