#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#include <X11/Intrinsic.h>
#include <X11/Xatom.h>
#include <X11/Xmu/Atoms.h>

Display *display;
Widget top;
Atom primary = XA_PRIMARY;

static Boolean convert(Widget widget,
    Atom *selection, Atom *target, Atom *type_return,
    XtPointer *value_return, unsigned long *length_return,
    int *format_return
) {
  printf("convert %"PRIu64"\n", primary);
  Display *d = XtDisplay(widget);
  XSelectionRequestEvent *req =
      XtGetSelectionRequest(widget, *selection, (XtRequestId) NULL);
  if (*target == XA_TARGETS(XtDisplay(widget))) {
    printf("targets\n");
  } else {
    printf("not targets "
           "%"PRIu64" %s"
           "@%"PRIu64"\n",
        *target, XmuGetAtomName(XtDisplay(widget), *target),
        req->time);
  }

  *length_return = 8; // bytes
  *value_return = XtMalloc(sizeof(char) * (*length_return));
  *format_return = 8; // bits
  strcpy(*value_return, "hi");
  return TRUE;
}
static void lose(Widget widget, Atom *selection);
static void insert(Widget w, XtPointer closure, Atom *selection,
    Atom *type, XtPointer value, unsigned long *length, int *format) {
  printf("insert %"PRIu64"\n", primary);
  XtOwnSelection(top, primary, CurrentTime,
      convert, lose, 0);
}

static void lose(Widget w, Atom *selection) {
  printf("lose %"PRIu64"\n", primary);
  XtGetSelectionValue(top, primary, XA_UTF8_STRING(display), insert,
      (XtPointer) (XA_UTF8_STRING(display)), CurrentTime);
}

static void done(Widget widget, Atom *selection, Atom *target) {
  printf("done\n");
}

static void callback(Widget widget, XtPointer closure, Atom *selection,
    Atom *type, XtPointer value, unsigned long *length,
    int *format) {
  printf("callback %"PRIu64"\n", primary);
  XtOwnSelection(top, primary, CurrentTime,
      convert, lose, 0);
}

static void
WMProtocols(Widget w, XEvent *ev, String *params, Cardinal *n) {
  printf("protocols\n");
}

static XtActionsRec xclipboard_actions[] = {
    {"WMProtocols", WMProtocols}
};

int main(int argc, char **argv) {
  XtAppContext xtcontext = 0;
  XrmOptionDescList table = 0;
  String *resources = 0;

  top = XtAppInitialize(&xtcontext, "XClipboard", table, 0,
      &argc, argv, resources, 0, 0);
  XtAppAddActions(xtcontext,
      xclipboard_actions, XtNumber(xclipboard_actions));
  XtRealizeWidget(top);
  display = XtDisplay(top);
  primary = XA_CLIPBOARD(display);


  // caller of XtOwnSelection asserts that it wants the right to set the value
  // of an Xatom: primary, secondary, or clipboard. this ensures that only
  // one widget has the right to set the property at a time. the caller sets up
  // a callback to handle the conversion request for data.
//  XtOwnSelection(top, primary, CurrentTime,
//      convert, // resposible for converting selected data into the target format
//      // called as a result of the requestor calling XtGetSelectionValue
//      lose, // clears the highlighted area when this widget loses ownership of
//      // the selection
//      0);

//  if (XGetSelectionOwner(display, primary)) {
    lose(top, &primary);
//  } else {
//    printf("we don't own the selection\n");
//  }

  XtGetSelectionValue(top, primary, XA_UTF8_STRING(display), callback,
      (XtPointer) (XA_UTF8_STRING(display)), CurrentTime);
  XtMainLoop();
  exit(0);
}



//  Display *display = XOpenDisplay(0);
//  Window window = XCreateSimpleWindow(display, 0, 0, 0, 1, 1, 0, 0, 0);
