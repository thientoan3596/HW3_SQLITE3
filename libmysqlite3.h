
#ifndef LIBMYSQLITE_H
#define LIBMYSQLITE_H

#include <stdbool.h>

#define DATASTRINGBUFF 10
#define STRINGBUFF 1024
#define FAILCONNECTION -1
#define SQLSTMTERROR -2
#define INVALID_INPUT -3
#define SUCCEED 0
char *mygetline(char *str, int strlen);
//void flush_input();
bool isLeap(int YYYY);
int getDate(char **returnDate);
void libmysqlite3_addingRecord(sqlite3 *conn);
int libmysqlite3_addingRecord_clients(sqlite3 *conn);
int libmysqlite3_addingRecord_deals(sqlite3 *conn);
int libmysqlite3_addingRecord_prices(sqlite3 *conn);
int libmysqlite3_addingRecord_stocks(sqlite3 *conn);
int libmysqlite3_addingRecord_markets(sqlite3 *conn);


int libmysqlite3_findDBTables(sqlite3 *conn,char ***stringArr);


#endif
