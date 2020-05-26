#include <sqlite3.h>
#include <stdio.h>
#include <string.h>

#include "sql.h"

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
