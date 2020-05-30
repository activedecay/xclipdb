#include     <stdlib.h>
#include     <X11/Xlib.h>

int main(int argc, char **argv) {
  int width = 600;
  int height = 600;
  int depth = 32; // works fine with depth = 24
  int bitmap_pad = 32; // 32 for 24 and 32 bpp, 16, for 15&16
  int bytes_per_line = 0; // number of bytes in the client image between the start of one scanline and the start of the next
  Display *display = XOpenDisplay(0);
  unsigned char *image32 = (unsigned char *) malloc(width * height * 4);
  XImage *img = XCreateImage(display, CopyFromParent, depth, ZPixmap, 0, image32, width, height, bitmap_pad,
      bytes_per_line);
  Pixmap p = XCreatePixmap(display, XDefaultRootWindow(display), width, height, depth);
  XGCValues gcvalues;
  GC gc = XCreateGC(display, p, 0, &gcvalues);
  XPutImage(display, p, gc, img, 0, 0, 0, 0, width, height); // 0, 0, 0, 0 are src x,y and dst x,y
  XEvent ev;
  while (1) {
    XNextEvent(display, &ev);
  }
}