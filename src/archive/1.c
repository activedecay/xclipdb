#include <gtk/gtk.h>
#include <gdk/gdk.h>

static GtkClipboard *primary;
static GtkClipboard *secondary;
static GtkClipboard *clipboard;

static void
activate(GtkApplication *app,
    gpointer user_data) {
  GtkWidget *window;

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW (window), "Window");
  gtk_window_set_default_size(GTK_WINDOW (window), 200, 200);
  gtk_widget_show_all(window);

  primary = gtk_clipboard_get(GDK_SELECTION_PRIMARY);
  secondary = gtk_clipboard_get(GDK_SELECTION_SECONDARY);
  clipboard = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);

}

int
main(int argc,
    char **argv) {
  GtkApplication *app;
  int status;

  app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
  g_signal_connect (app, "activate", G_CALLBACK(activate), NULL);
  status = g_application_run(G_APPLICATION (app), argc, argv);
  g_object_unref(app);

  return status;
}
