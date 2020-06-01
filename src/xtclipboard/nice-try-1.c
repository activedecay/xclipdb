#include <stdio.h>
#include <X11/Intrinsic.h>
#include <X11/StringDefs.h>
#include <X11/Xatom.h>

#include <X11/Xmu/Atoms.h>
#include <X11/Xmu/StdSel.h>

#include <X11/Shell.h>
#include <X11/Xaw/Form.h>
#include <X11/Xaw/Label.h>
#include <X11/Xaw/Command.h>
#include <X11/Xaw/AsciiText.h>
#include <X11/Xaw/Dialog.h>
#include <X11/Xaw/Cardinals.h>
#include <X11/IntrinsicP.h>
#include <X11/Xaw/TextP.h>
#include <X11/Xfuncs.h>

#include <stdlib.h>

static Atom wm_delete_window;
static Atom wm_protocols;
static Atom ManagerAtom, ClipboardAtom;

static Widget top;
static Widget text, nextButton, prevButton, indexLabel;
static Widget fileDialog, fileDialogShell;
static Widget failDialog, failDialogShell;

typedef struct _Clip {
    struct _Clip *next, *prev;
    char *clip;
    char *filename;
    size_t avail;
} ClipRec, *ClipPtr;

static ClipPtr currentClip;

static XrmOptionDescRec table[] = {
    {"-w", "wrap", XrmoptionNoArg, "on"},
/*    {"-nw",	    "wrap",		XrmoptionNoArg,  "False"} */
};

static String fallback_resources[] = {
    "*international: true",
    NULL
};

typedef struct {
    Boolean wrap;
} ResourceData, *ResourceDataPtr;
static ResourceData userOptions;
#define Offset(field) XtOffsetOf(ResourceData, field)
static XtResource resources[] = {
    {"wrap", "Wrap", XtRBoolean, sizeof(Boolean),
        Offset(wrap), XtRImmediate, (XtPointer) False}
};
#undef Offset


/* ARGSUSED */
static void
CancelSaveFile(Widget w, XEvent *ev, String *parms, Cardinal *np) {
  XtPopdown(fileDialogShell);
}

/* ARGSUSED */
static void
FailContinue(Widget w, XEvent *ev, String *parms, Cardinal *np) {
  XtPopdown(failDialogShell);
}

static void _X_NORETURN
Quit(Widget w, XEvent *ev, String *parms, Cardinal *np) {
  XtCloseDisplay(XtDisplay(text));
  exit(0);
}

/*ARGSUSED*/
static void
WMProtocols(Widget w, XEvent *ev, String *params, Cardinal *n) {
  if (ev->type == ClientMessage &&
      ev->xclient.message_type == wm_protocols &&
      ev->xclient.data.l[0] == (long) wm_delete_window) {
    while (w && !XtIsShell(w))
      w = XtParent(w);
    if (w == top)
      Quit(w, ev, params, n);
    else if (w == fileDialogShell)
      CancelSaveFile(w, ev, params, n);
    else if (w == failDialogShell)
      FailContinue(w, ev, params, n);
  }
}

static XtActionsRec xtclipboard_actions[] = {
//    {"NewClip",      NewCurrentClip},
//    {"NextClip",     NextCurrentClip},
//    {"PrevClip",     PrevCurrentClip},
//    {"DeleteClip",   DeleteCurrentClip},
//    {"Save",         SaveToFile},
//    {"AcceptSave",   AcceptSaveFile},
    {"CancelSave",   CancelSaveFile},
    {"FailContinue", FailContinue},
    {"Quit",         Quit},
    {"WMProtocols",  WMProtocols}
};

static Boolean
RefuseSelection(Widget w, Atom *selection, Atom *target,
    Atom *type, XtPointer *value, unsigned long *length,
    int *format) {
  return False;
}

static void
LoseManager(Widget w, Atom *selection) {
  XtError("another clipboard has taken over control\n");
}

static long
TextLength(Widget w) {
  return XawTextSourceScan(XawTextGetSource(w),
      (XawTextPosition) 0,
      XawstAll, XawsdRight, 1, TRUE);
}

static Boolean
ConvertSelection(Widget w, Atom *selection, Atom *target,
    Atom *type, XtPointer *value, unsigned long *length,
    int *format) {
  Display *d = XtDisplay(w);
  XSelectionRequestEvent *req =
      XtGetSelectionRequest(w, *selection, (XtRequestId) NULL);

  if (*target == XA_TARGETS(d)) {
    Atom *targetP;
    Atom *std_targets;
    unsigned long std_length;
    XmuConvertStandardSelection(w, req->time, selection, target, type,
        (XPointer *) &std_targets, &std_length,
        format);
    *value = XtMalloc(sizeof(Atom) * (std_length + 7));
    targetP = *(Atom **) value;
    *targetP++ = XA_STRING;
    *targetP++ = XA_TEXT(d);
    *targetP++ = XA_UTF8_STRING(d);
    *targetP++ = XA_COMPOUND_TEXT(d);
    *targetP++ = XA_LENGTH(d);
    *targetP++ = XA_LIST_LENGTH(d);
    *targetP++ = XA_CHARACTER_POSITION(d);
    *length = std_length + (targetP - (*(Atom **) value));
    memmove((char *) targetP, (char *) std_targets, sizeof(Atom) * std_length);
    XtFree((char *) std_targets);
    *type = XA_ATOM;
    *format = 32;
    return True;
  }

  if (*target == XA_LIST_LENGTH(d) ||
      *target == XA_LENGTH(d)) {
    long *temp;

    temp = (long *) XtMalloc(sizeof(long));
    if (*target == XA_LIST_LENGTH(d))
      *temp = 1L;
    else      /* *target == XA_LENGTH(d) */
      *temp = (long) TextLength(text);

    *value = (XPointer) temp;
    *type = XA_INTEGER;
    *length = 1L;
    *format = 32;
    return True;
  }

  if (*target == XA_CHARACTER_POSITION(d)) {
    long *temp;

    temp = (long *) XtMalloc(2 * sizeof(long));
    temp[0] = (long) 0;
    temp[1] = TextLength(text);
    *value = (XPointer) temp;
    *type = XA_SPAN(d);
    *length = 2L;
    *format = 32;
    return True;
  }

  if (*target == XA_STRING ||
      *target == XA_TEXT(d) ||
      *target == XA_UTF8_STRING(d) ||
      *target == XA_COMPOUND_TEXT(d)) {
    Arg args[1];
    Widget source;
    XTextProperty prop;
    int ret, style = XStdICCTextStyle; /* a safe default for TEXT */
    char *data;

    source = XawTextGetSource(text);
    XtSetArg(args[0], XtNstring, &data);
    XtGetValues(source, args, 1);

    if (*target == XA_UTF8_STRING(d))
      style = XUTF8StringStyle;
    else if (*target == XA_COMPOUND_TEXT(d))
      style = XCompoundTextStyle;
    else if (*target == XA_STRING)
      style = XStringStyle;

    ret = XmbTextListToTextProperty(d, &data, 1, style, &prop);
    if (ret >= Success) {
      *length = prop.nitems;
      *value = prop.value;
      *type = prop.encoding;
      *format = prop.format;
      return True;
    } else
      return False;
  }

  if (XmuConvertStandardSelection(w, req->time, selection, target, type,
      (XPointer *) value, length, format))
    return True;

  return False;
}

static ClipPtr
NewClip(Widget w, ClipPtr old) {
  ClipPtr newClip;

  newClip = (ClipPtr) malloc(sizeof(ClipRec));
  if (!newClip)
    return newClip;
  newClip->clip = NULL;
  newClip->avail = 0;
  newClip->prev = old;
  newClip->next = NULL;
  newClip->filename = NULL;
  if (old) {
    newClip->next = old->next;
    old->next = newClip;
  }
  return newClip;
}

static void
SaveClip(Widget w, ClipPtr clip) {
  Arg args[1];
  char *data;
  size_t len;
  Widget source;

  if (!w) return;
  if (!clip) return;

  source = XawTextGetSource(w);
  XtSetArg(args[0], XtNstring, &data);
  XtGetValues(source, args, 1);
  len = strlen(data);
  if (len >= clip->avail) {
    if (clip->clip)
      free(clip->clip);
    clip->clip = malloc(len + 1);
    if (!clip->clip)
      clip->avail = 0;
    else
      clip->avail = len + 1;
  }
  if (clip->avail) {
    strcpy(clip->clip, data);
  }
}

static int
IndexCurrentClip(void) {
  int i = 0;
  ClipPtr clip;

  for (clip = currentClip; clip; clip = clip->prev)
    i++;
  return i;
}

static void
set_button_state(void) {
  Boolean prevvalid, nextvalid;
  Arg arg;
  char labelString[10];

  prevvalid = currentClip->prev != NULL;
  nextvalid = currentClip->next != NULL;
  XtSetArg(arg, XtNsensitive, prevvalid);
  XtSetValues(prevButton, &arg, ONE);
  XtSetArg(arg, XtNsensitive, nextvalid);
  XtSetValues(nextButton, &arg, ONE);
  snprintf(labelString, sizeof(labelString), "%d", IndexCurrentClip());
  XtSetArg(arg, XtNlabel, labelString);
  XtSetValues(indexLabel, &arg, ONE);
}

static void
NewCurrentClipContents(char *data, int len) {
  XawTextBlock textBlock;

  SaveClip(text, currentClip);

  /* append new clips at the end */
  while (currentClip && currentClip->next)
    currentClip = currentClip->next;
  /* any trailing clips with no text get overwritten */
  if (!currentClip) return;
  if (strlen(currentClip->clip) != 0)
    currentClip = NewClip(text, currentClip);

  textBlock.ptr = data;
  textBlock.firstPos = 0;
  textBlock.length = len;
  textBlock.format = FMT8BIT;
  if (XawTextReplace(text, 0, TextLength(text), &textBlock)) {
    XBell(XtDisplay(text), 0);
  }
  set_button_state();
}

static void
LoseSelection(Widget w, Atom *selection);

static void
InsertClipboard(Widget w, XtPointer client_data, Atom *selection,
    Atom *type, XtPointer value, unsigned long *length,
    int *format) {
  Display *d = XtDisplay(w);
  Atom target = (Atom) client_data;
  Boolean convert_failed = (*type == XT_CONVERT_FAIL);

  if (!convert_failed) {
    char **list;
    int i, ret, count;

    XTextProperty prop;
    prop.value = value;
    prop.nitems = *length;
    prop.format = *format;
    prop.encoding = *type;
    ret = XmbTextPropertyToTextList(d, &prop, &list, &count);
    if (ret >= Success) {
      /* manuals say something about multiple strings in a disjoint
      text selection (?), it should be harmless to get them all */
      for (i = 0; i < count; i++) {
        printf("%s %lu", list[i], strlen(list[i]));
        NewCurrentClipContents(list[i], strlen(list[i]));
      }
      XFreeStringList(list);
    } else
      convert_failed = True;
    XFree(value);
  }

  if (convert_failed) {
    /* if UTF8_STRING failed try COMPOUND_TEXT */
    if (target == XA_UTF8_STRING(d)) {
      XtGetSelectionValue(w, *selection, XA_COMPOUND_TEXT(d),
          InsertClipboard,
          (XtPointer) (XA_COMPOUND_TEXT(d)),
          CurrentTime);
      return;
    }
      /* if COMPOUND_TEXT failed try STRING */
    else if (target == XA_COMPOUND_TEXT(d)) {
      XtGetSelectionValue(w, *selection, XA_STRING,
          InsertClipboard,
          NULL,
          CurrentTime);
      return;
    }
      /* all conversions failed */
    else {
      Arg arg;
      XtSetArg(arg, XtNlabel, "CLIPBOARD selection conversion failed");
      XtSetValues(failDialog, &arg, 1);
      // CenterWidgetOnWidget(failDialogShell, text);
      XtPopup(failDialogShell, XtGrabNone);
      XBell(d, 0);
    }
  }

  XtOwnSelection(top, ClipboardAtom, CurrentTime,
      ConvertSelection, LoseSelection, NULL);
}
static void
LoseSelection(Widget w, Atom *selection) {
  Display *d = XtDisplay(w);
  XtGetSelectionValue(w, *selection, XA_UTF8_STRING(d), InsertClipboard,
      (XtPointer) (XA_UTF8_STRING(d)), CurrentTime);
}

int
main(int argc, char **argv) {
  Arg args[4];
  Cardinal n;
  XtAppContext xtcontext;
  Widget parent;

  top = XtAppInitialize(&xtcontext, "XClipboard", table, XtNumber(table),
      &argc, argv, fallback_resources, NULL, 0);
  XtGetApplicationResources(top, (XtPointer) &userOptions, resources,
      XtNumber(resources), NULL, 0);
  XtAppAddActions(xtcontext,
      xtclipboard_actions, XtNumber(xtclipboard_actions));
  ManagerAtom = XInternAtom(XtDisplay(top), "CLIPBOARD_MANAGER", False);
  ClipboardAtom = XA_CLIPBOARD(XtDisplay(top));
  if (XGetSelectionOwner(XtDisplay(top), ManagerAtom))
    XtError("another clipboard is already running\n");

  XtRealizeWidget(top);

  /* refuse to allow multiple instances of the clipboard manager */
  XtOwnSelection(top, ManagerAtom, CurrentTime,
      RefuseSelection, LoseManager, NULL);

  /* setup handlers for clipboard copy events */
  if (XGetSelectionOwner(XtDisplay(top), ClipboardAtom)) {
    LoseSelection(top, &ClipboardAtom);
  } else {
    XtOwnSelection(top, ClipboardAtom, CurrentTime,
        ConvertSelection, LoseSelection, NULL);
  }

  XtAppMainLoop(xtcontext);
  return 0;
}
