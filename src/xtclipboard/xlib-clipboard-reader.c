/* "pure" x11 xlib xmu(x miscellaneous utils) to read the clipboard
 *
 * pretty much all we'd need to do is understand the message passing
 * niceties that xt is doing for us with XtOwnSelection and friends. */

#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xmu/Atoms.h>
#include <stdlib.h>
#include <stdio.h>

/* Returns the machine-specific number of bytes per data element
 * returned by XGetWindowProperty */
static size_t machine_arch_itemsize(int format) {
  if (format == 8)
    return sizeof(char);
  if (format == 16)
    return sizeof(short);
  if (format == 32)
    return sizeof(long);
  return 0;
}

int main(int argc, char **argv) {
  Atom primary = XA_PRIMARY;
  Display *dpy = XOpenDisplay(0);
  Atom clipboard = XA_CLIPBOARD(dpy);
  Window clipboard_manager;      /* Window */
  clipboard_manager = XCreateSimpleWindow(dpy, DefaultRootWindow(dpy), 0, 0, 1, 1, 0, 0, 0);
  XSelectInput(dpy, clipboard_manager, PropertyChangeMask);
  XEvent evt;
  // an atom unique to our app
  Atom pty = XInternAtom(dpy, "CLIPBOARD_MANAGER_PRIME", False);
  XConvertSelection(dpy, clipboard, XA_STRING, pty, clipboard_manager, CurrentTime);
  while (1) {
    XNextEvent(dpy, &evt);
    if (evt.type == SelectionNotify)
      break;
  }

  /* return failure when the current target failed */
  if (evt.xselection.property == None)
    return (7); // we don't have a string on the clipboard.
  else {

    unsigned char *buffer;
    int pty_format;
    unsigned long pty_size, pty_items, pty_arch_size;

    Atom sel_type = None;
    XGetWindowProperty(dpy,
        clipboard_manager,
        pty,
        0,
        0,
        False,
        AnyPropertyType, &sel_type, &pty_format, &pty_items, &pty_size, &buffer);
    XFree(buffer);
    // sel_type == XA_STRING or the X11 target, like text/html, image/png or utf8_string, etc.

    /* if we're over an x11 network, and the buffer is too large for x to handle in a single
     * pass, then INCR is used to transfer bytestreams according to ICCCM */
    Atom inc = XInternAtom(dpy, "INCR", False);
    if (sel_type == inc)
      return 8;

    XGetWindowProperty(dpy,
        clipboard_manager,
        pty,
        0,
        (long) pty_size,
        False,
        AnyPropertyType, &sel_type, &pty_format, &pty_items, &pty_size, &buffer);
    XDeleteProperty(dpy, clipboard_manager, pty);

    /* compute the size of the data buffer we received */
    pty_arch_size = pty_items * machine_arch_itemsize(pty_format);

    /* copy the buffer to the pointer for returned data */
    unsigned char *ltxt = (unsigned char *) calloc(pty_arch_size, sizeof(ltxt));
    memcpy(ltxt, buffer, pty_arch_size);

    printf("clipboard = '%s'", ltxt);

    /* free the buffer */
    XFree(buffer);
  }
  XCloseDisplay(dpy);

  return 0;
}
