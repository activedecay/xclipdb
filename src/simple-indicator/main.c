#include "libappindicator/app-indicator.h"

static void
item_clicked_cb(GtkWidget *_, gpointer data) {
  const gchar *text = (const gchar *) data;
  g_print("%s clicked!\n", text);
}

int main(int argc, char **argv) {
  GtkWidget *menu = NULL;
  AppIndicator *ci = NULL;
  GMainLoop *mainloop = NULL;

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
  GtkWidget *item = gtk_menu_item_new_with_label("MENU LABEL");
  g_signal_connect (item, "activate",
      G_CALLBACK(item_clicked_cb), "1");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu), item);
  gtk_widget_show(item);
  app_indicator_set_menu(ci, GTK_MENU (menu));

  mainloop = g_main_loop_new(NULL, FALSE);
  g_main_loop_run(mainloop);
  return 0;
}
