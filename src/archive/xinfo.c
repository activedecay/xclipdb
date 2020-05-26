#include <stdio.h>
#include <Xlib.h>

Display *dpy;
Screen *s;

int main() {
  char *display_name = NULL; // protocol/hostname:server_number.screen_number
  int scr = 0;
  if ((dpy = XOpenDisplay(display_name)) == 0) {
    fprintf(stderr, "can't open display %s", display_name);
    return 1;
  }
  if ((s = XScreenOfDisplay(dpy, 0)) == 0) {
    fprintf(stderr, "can't open display %s", display_name);
    return 1;
  }
  printf("BitmapBitOrder %d\n",                 BitmapBitOrder(dpy));
  printf("BitmapPad %d\n",                      BitmapPad(dpy));
  printf("BitmapUnit %d\n",                     BitmapUnit(dpy));
  printf("BlackPixel %lu\n",                    BlackPixel(dpy, scr));
  printf("BlackPixelOfScreen %lu\n",            BlackPixelOfScreen(s));
  printf("CellsOfScreen %d\n",                  CellsOfScreen(s));
  printf("ConnectionNumber %d\n",               ConnectionNumber(dpy));
  printf("DefaultColormap %lu\n",               DefaultColormap(dpy, scr));
  printf("DefaultColormapOfScreen %lu\n",       DefaultColormapOfScreen(s));
  printf("DefaultDepth %d\n",                   DefaultDepth(dpy, scr));
  printf("DefaultDepthOfScreen %d\n",           DefaultDepthOfScreen(s));
  printf("DefaultGC %p\n",                      DefaultGC(dpy, scr));
  printf("DefaultGCOfScreen %p\n",              DefaultGCOfScreen(s));
  printf("DefaultRootWindow %lu\n",             DefaultRootWindow(dpy));
  printf("DefaultScreen %d\n",                  DefaultScreen(dpy));
  printf("DefaultScreenOfDisplay %p\n",         DefaultScreenOfDisplay(dpy));
  printf("DefaultVisual %p\n",                  DefaultVisual(dpy, scr));
  printf("DefaultVisualOfScreen %p\n",          DefaultVisualOfScreen(s));
  printf("DisplayCells %d\n",                   DisplayCells(dpy, scr));
  printf("DisplayHeight %d\n",                  DisplayHeight(dpy, scr));
  printf("DisplayHeightMM %d\n",                DisplayHeightMM(dpy, scr));
  printf("DisplayOfScreen %p\n",                DisplayOfScreen(s));
  printf("DisplayPlanes %d\n",                  DisplayPlanes(dpy, scr));
  printf("DisplayString %s\n",                  DisplayString(dpy));
  printf("DisplayWidth %d\n",                   DisplayWidth(dpy, scr));
  printf("DisplayWidthMM %d\n",                 DisplayWidthMM(dpy, scr));
  printf("EventMaskOfScreen %ld\n",             EventMaskOfScreen(s));
  printf("HeightMMOfScreen %d\n",               HeightMMOfScreen(s));
  printf("HeightOfScreen %d\n",                 HeightOfScreen(s));
  printf("ImageByteOrder %d\n",                 ImageByteOrder(dpy));
  printf("LastKnownRequestProcessed %lu\n",     LastKnownRequestProcessed(dpy));
  printf("MaxCmapsOfScreen %d\n",               MaxCmapsOfScreen(s));
  printf("MinCmapsOfScreen %d\n",               MinCmapsOfScreen(s));
  printf("NextRequest %lu\n",                   NextRequest(dpy));
  printf("PlanesOfScreen %d\n",                 PlanesOfScreen(s));
  printf("ProtocolRevision %d\n",               ProtocolRevision(dpy));
  printf("ProtocolVersion %d\n",                ProtocolVersion(dpy));
  printf("QLength %d\n",                        QLength(dpy));
  printf("RootWindow %lu\n",                    RootWindow(dpy, scr));
  printf("RootWindowOfScreen %lu\n",            RootWindowOfScreen(s));
  printf("ScreenCount %d\n",                    ScreenCount(dpy));
  printf("ScreenOfDisplay %p\n",                ScreenOfDisplay(dpy, scr));
  printf("ServerVendor %s\n",                   ServerVendor(dpy));
  printf("VendorRelease %d\n",                  VendorRelease(dpy));
  printf("WhitePixel %lu\n",                    WhitePixel(dpy, scr));
  printf("WhitePixelOfScreen %lu\n",            WhitePixelOfScreen(s));
  printf("WidthMMOfScreen %d\n",                WidthMMOfScreen(s));
  printf("WidthOfScreen %d\n",                  WidthOfScreen(s));
  printf("XAllocID %lu\n",                      XAllocID(dpy));
  XCloseDisplay(dpy);
  return 0;
}