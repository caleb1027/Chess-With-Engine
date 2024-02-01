#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <unistd.h>
#include "window.h"

using namespace std;

Xwindow::Xwindow(int width, int height) {

  d = XOpenDisplay(NULL);
  if (d == NULL) {
    cerr << "Cannot open display" << endl;
    exit(1);
  }
  s = DefaultScreen(d);
  w = XCreateSimpleWindow(d, RootWindow(d, s), 10, 10, width, height, 1,
                          BlackPixel(d, s), WhitePixel(d, s));
  XSelectInput(d, w, ExposureMask | KeyPressMask);
  XMapRaised(d, w);

  Pixmap pix = XCreatePixmap(d,w,width,
        height,DefaultDepth(d,DefaultScreen(d)));
  gc = XCreateGC(d, pix, 0,(XGCValues *)0);

  XFlush(d);
  XFlush(d);

  // Set up colours.
  XColor xcolour;
  Colormap cmap;
  char color_vals[5][10]={"white", "black", "red", "green", "blue"};

  cmap=DefaultColormap(d,DefaultScreen(d));
  for(int i=0; i < 5; ++i) {
      XParseColor(d,cmap,color_vals[i],&xcolour);
      XAllocColor(d,cmap,&xcolour);
      colours[i]=xcolour.pixel;
  }

  XSetForeground(d,gc,colours[Black]);

  // Set up fonts
  string fontName = "-adobe-helvetica-bold-r-normal--34-240-100-100-p-182-iso8859-1";
  font = XLoadQueryFont(d, fontName.c_str());

  string smallFontName = "-adobe-new century schoolbook-bold-r-normal--25-180-100-100-p-149-iso8859-1";
  smallFont = XLoadQueryFont(d, smallFontName.c_str());

  // Make window non-resizeable.
  XSizeHints hints;
  hints.flags = (USPosition | PSize | PMinSize | PMaxSize );
  hints.height = hints.base_height = hints.min_height = hints.max_height = height;
  hints.width = hints.base_width = hints.min_width = hints.max_width = width;
  XSetNormalHints(d, w, &hints);

  XSynchronize(d,True);

  usleep(1000);
}

Xwindow::~Xwindow() {
  XFreeFont(d, font);
  XFreeFont(d, smallFont);
  XFreeGC(d, gc);
  XCloseDisplay(d);
}

void Xwindow::fillRectangle(int x, int y, int width, int height, int colour) {
  XSetForeground(d, gc, colours[colour]);
  XFillRectangle(d, w, gc, x, y, width, height);
  XSetForeground(d, gc, colours[Black]);
}

void Xwindow::fillGreenRectangle(int x, int y, int width, int height) {
  int redAmt = 119;
  int greenAmt = 153;
  int blueAmt = 84;
  unsigned long greenColour = blueAmt + (greenAmt<<8) + (redAmt<<16);

  XSetForeground(d, gc, greenColour);
  XFillRectangle(d, w, gc, x, y, width, height);
  XSetForeground(d, gc, colours[Black]);
}

void Xwindow::fillLightRectangle(int x, int y, int width, int height) {
  int redAmt = 233;
  int greenAmt = 237;
  int blueAmt = 204;
  unsigned long lightColour = blueAmt + (greenAmt<<8) + (redAmt<<16);

  XSetForeground(d, gc, lightColour);
  XFillRectangle(d, w, gc, x, y, width, height);
  XSetForeground(d, gc, colours[Black]);
}

void Xwindow::fillRedRectangle(int x, int y, int width, int height) {
  int redAmt = 255;
  int greenAmt = 102;
  int blueAmt = 102;
  unsigned long redColour = blueAmt + (greenAmt<<8) + (redAmt<<16);

  XSetForeground(d, gc, redColour);
  XFillRectangle(d, w, gc, x, y, width, height);
  XSetForeground(d, gc, colours[Black]);
}

void Xwindow::drawString(int x, int y, string msg) {
  XSetFont(d, gc, font->fid);
  XDrawString(d, w, gc, x, y, msg.c_str(), msg.length());
}

void Xwindow::drawSmallString(int x, int y, std::string msg) {
  XSetFont(d, gc, smallFont->fid);
  XDrawString(d, w, gc, x, y, msg.c_str(), msg.length());
}
