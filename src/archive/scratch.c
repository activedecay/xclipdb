Window window;
Display *display;
char *screendisplay = NULL;
#include <X11/Xatom.h>
#include <X11/Xmu/Atoms.h>
gboolean go(gpointer _) {
  XEvent evt;
  XSelectInput(display, window, PropertyChangeMask);
  XSetSelectionOwner(display, XA_CLIPBOARD(display), window, CurrentTime);
  XNextEvent(display, &evt);
  Window r;
  if (evt.type == SelectionRequest) {
    r = evt.xselectionrequest.requestor;
  } else if (evt.type == PropertyNotify) {
    r = evt.xproperty.window;
  } else if (evt.type == SelectionClear) {
  } else {
  }
  return TRUE;
}
void main0() {
  if (!(display = XOpenDisplay(screendisplay))) {
    exit(1);
  }
  static Atom ManagerAtom, ClipboardAtom;

  ManagerAtom = XInternAtom(display, "CLIPBOARD_MANAGER", False);
  if (XGetSelectionOwner(display, ManagerAtom))
    XtError("another clipboard is already running\n");
  ClipboardAtom = XA_CLIPBOARD(display);

  window = XCreateSimpleWindow(display, DefaultRootWindow(display), 0, 0, 1, 1, 0, 0, 0);
}
