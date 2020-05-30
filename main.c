#include "libappindicator/app-indicator.h"
#include <inttypes.h>
#include <xcb/xcb.h>
#include <X11/X.h>
#include <X11/Xlib.h>
#include <X11/Xatom.h>
#include <X11/Xmu/Atoms.h>
#include "handler.h"

GMainLoop *mainloop = NULL;
xcb_connection_t *connection;
xcb_window_t window;

static void
appindicator_menu_clicked(GtkWidget *_, gpointer data) {
  const gchar *text = (const gchar *) data;
  xcb_map_window(connection, window);
  xcb_flush(connection);
}

gboolean daemonize_appindicator(gpointer _) {
  g_main_loop_run(mainloop);
  return TRUE;
}

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
  GtkWidget *menu = NULL;
  AppIndicator *ci = NULL;

  /* *** app indicator setup *** */

  gtk_init(&argc, &argv);

  ci = app_indicator_new("x",
      "task-past-due" /* find /usr/share/icons -name \*.svg | grep status/16/ */,
      APP_INDICATOR_CATEGORY_APPLICATION_STATUS);

  g_assert (IS_APP_INDICATOR(ci));
  g_assert (G_IS_OBJECT(ci));

  app_indicator_set_status(ci, APP_INDICATOR_STATUS_ACTIVE); /* PASSIVE, ACTIVE, ATTENTION */
  app_indicator_set_attention_icon_full(ci, "task-due", "ICON_DESC");
  app_indicator_set_label(ci, "INDICATOR LABEL", "GUIDE");
  app_indicator_set_title(ci, "TITLE");

  menu = gtk_menu_new();
  GtkWidget *item = gtk_menu_item_new_with_label("Show");
  g_signal_connect (item, "activate",
      G_CALLBACK(appindicator_menu_clicked), "1");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
  gtk_widget_show(item);
  app_indicator_set_menu(ci, GTK_MENU (menu));

  mainloop = g_main_loop_new(NULL, FALSE);
  g_thread_new("main", (GThreadFunc) daemonize_appindicator, 0);

  /* *** xcb setup *** */

  /* Open the connection to the X server */
  connection = xcb_connect(NULL, NULL);
  /* Get the first screen */
  xcb_screen_t *screen = xcb_setup_roots_iterator(xcb_get_setup(connection)).data;
  /* Create the window */
  window = xcb_generate_id(connection);

  uint32_t mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
  uint32_t values[2] = {screen->white_pixel,
      XCB_EVENT_MASK_EXPOSURE | XCB_EVENT_MASK_BUTTON_PRESS |
          XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_POINTER_MOTION |
          XCB_EVENT_MASK_ENTER_WINDOW | XCB_EVENT_MASK_LEAVE_WINDOW |
          XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_KEY_RELEASE
      /* XCB_EVENT_MASK_NO_EVENT | XCB_EVENT_MASK_OWNER_GRAB_BUTTON |
         XCB_EVENT_MASK_KEY_PRESS | XCB_EVENT_MASK_BUTTON_MOTION |
         XCB_EVENT_MASK_KEY_RELEASE | XCB_EVENT_MASK_KEYMAP_STATE |
         XCB_EVENT_MASK_BUTTON_PRESS | XCB_EVENT_MASK_EXPOSURE |
         XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_VISIBILITY_CHANGE |
         XCB_EVENT_MASK_ENTER_WINDOW | XCB_EVENT_MASK_STRUCTURE_NOTIFY |
         XCB_EVENT_MASK_LEAVE_WINDOW | XCB_EVENT_MASK_RESIZE_REDIRECT |
         XCB_EVENT_MASK_POINTER_MOTION | XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY |
         XCB_EVENT_MASK_POINTER_MOTION_HINT | XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT |
         XCB_EVENT_MASK_BUTTON_1_MOTION | XCB_EVENT_MASK_FOCUS_CHANGE |
         XCB_EVENT_MASK_BUTTON_2_MOTION | XCB_EVENT_MASK_PROPERTY_CHANGE |
         XCB_EVENT_MASK_BUTTON_3_MOTION | XCB_EVENT_MASK_COLOR_MAP_CHANGE |
         XCB_EVENT_MASK_BUTTON_4_MOTION | XCB_EVENT_MASK_BUTTON_5_MOTION */
  };

  xcb_create_window(connection,
      0,                             /* depth               */
      window,
      screen->root,                  /* parent window       */
      0, 0,                          /* x, y                */
      150, 150,                      /* width, height       */
      10,                            /* border_width        */
      XCB_WINDOW_CLASS_INPUT_OUTPUT, /* class               */
      screen->root_visual,           /* visual              */
      mask, values);                 /* masks */

  /* Map the window on the screen */
  xcb_map_window(connection, window);
  xcb_flush(connection);

  /* *** raw xcb window draw functions and event handlers *** */

  xcb_generic_event_t *event;
  while ((event = xcb_wait_for_event(connection))) {
    handle(event, &window, connection);
  }
  return 0;
}
