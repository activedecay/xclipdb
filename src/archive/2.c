#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <glib-unix.h>
#include <sqlite3.h>

struct clippy {
    GtkClipboard *clip;
    char *text;
};

#define NAMES C(PRIMARY)C(SECONDARY)C(CLIPBOARD)
#define C(x) x,
enum clipboards {
    NAMES clip_count
};
#undef C
#define C(x) #x,
const char *const clipboard_names[] = {NAMES};
#undef C
static struct clippy clips[clip_count] = {};

static gboolean running = TRUE;
static sqlite3 *db;
static sqlite3_stmt *insert_stmt = NULL;


static int sqlite_cb(void *_, int argc, char **argv, char **azColName) {
  int i;
  for (i = 0; i < argc; i++) {
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}

int init_sqlite_db() {
  char *zErrMsg = 0;
  int rc;

  rc = sqlite3_open("db", &db);
  if (rc) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }
  rc = sqlite3_exec(db, "CREATE TABLE IF NOT EXISTS t("
                        "id INTEGER PRIMARY KEY ASC AUTOINCREMENT,"
                        "val);", sqlite_cb, 0, &zErrMsg);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
    return 2;
  }

  char *sql = "insert into t (val) values (?);";
  sqlite3_prepare_v2(db, sql, (int) strlen(sql), &insert_stmt, 0);
  if (!insert_stmt) {
    fprintf(stderr, "failed to create prepared statement\n");
    return 3;
  }

  return 0;
}

static gboolean compact(gpointer _) {
  char *zErrMsg = 0;
  int rc;

  rc = sqlite3_exec(db, "DELETE FROM t\n"
                        "  WHERE id NOT IN (\n"
                        "    SELECT MIN(id) \n"
                        "    FROM t \n"
                        "    GROUP BY val\n"
                        "  )", sqlite_cb, 0, &zErrMsg);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }
  return TRUE;
}

static gboolean cb(gpointer _) {
  gchar *t;
  gboolean grc;
  gint count;
  GdkAtom *targets;
  gboolean exists;

  for (int i = 0; i < clip_count; ++i) {
    exists =
        gtk_clipboard_wait_for_targets(clips[i].clip, &targets, &count);
    gboolean has_text = FALSE;
    for (int j = 0; j < count; j++) {
      has_text = has_text || targets[j] == GDK_TARGET_STRING;
    }

    if (exists && has_text && gtk_clipboard_wait_is_text_available(clips[i].clip)) {
      t = gtk_clipboard_wait_for_text(clips[i].clip);
      const gchar *end;
      grc = t && g_utf8_validate(t, -1, &end);
      if (grc && g_strcmp0(t, clips[i].text) != 0) {
        g_print("%s %s \n", clipboard_names[i], t);
        if (clips[i].text) g_free(clips[i].text);
        clips[i].text = g_strdup(t);
        sqlite3_bind_text(insert_stmt, 1, clips[i].text, -1, 0);
        sqlite3_step(insert_stmt); // todo errors
        sqlite3_reset(insert_stmt);
      }
      g_free(t);
    }
    g_free(targets);
  }
  return running; /* return true to loop forever */
}

static void destroy(gpointer _) {
}

gboolean handle_int(gpointer _) {
  running = FALSE;
  gtk_main_quit();
  sqlite3_close(db);
  return TRUE;
}

int main(int argc, char **argv) {

  int initrc;
  if ((initrc = init_sqlite_db()) != 0) {
    return initrc;
  }

  gtk_init(&argc, &argv);

  clips[PRIMARY].clip = gtk_clipboard_get(GDK_SELECTION_PRIMARY);
  clips[SECONDARY].clip = gtk_clipboard_get(GDK_SELECTION_SECONDARY);
  clips[CLIPBOARD].clip = gtk_clipboard_get(GDK_SELECTION_CLIPBOARD);

  g_unix_signal_add(SIGINT, (GSourceFunc) handle_int, 0 /* data */);
  g_unix_signal_add(SIGTERM, (GSourceFunc) handle_int, 0 /* data */);

  g_timeout_add_full(G_PRIORITY_LOW,
      250 /* ms */,
      (GSourceFunc) cb,
      0 /* data */,
      (GDestroyNotify) destroy);
  g_timeout_add_seconds_full(G_PRIORITY_LOW,
      10,
      compact,
      0,
      destroy);

  gtk_main();

  return 0;
}
