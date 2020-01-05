
#ifndef LIBMYSQLITE_H
#define LIBMYSQLITE_H

#include <stdbool.h>

#define DATASTRINGBUFF 10
#define STRINGBUFF 1024
#define FAILCONNECTION -1
#define SQLSTMTERROR -2
#define INVALID_INPUT -3
#define SUCCEED 0
#define UNDEFINEDERROR -4



static int mycallback(void *data, int argc, char **argv, char **azColName);
char *mygetline(char *str, int strlen);
//void flush_input();
bool isLeap(int YYYY);
int getDate(char **returnDate);

bool doesExist(sqlite3 *conn,char* tableName, char* columnName,char *value,int dataType);

void libmysqlite3_addingRecord(sqlite3 *conn);
int libmysqlite3_addingRecord_clients(sqlite3 *conn);
int libmysqlite3_addingRecord_deals(sqlite3 *conn);
int libmysqlite3_addingRecord_prices(sqlite3 *conn);
int libmysqlite3_addingRecord_stocks(sqlite3 *conn);
int libmysqlite3_addingRecord_markets(sqlite3 *conn);

void libmysqlite3_deleteRecord(sqlite3 *conn);
void libmysqlite3_deleteRecord_clients(sqlite3 *conn,int id_client);
void libmysqlite3_deleteRecord_deals(sqlite3 *conn,int id_deal);
void libmysqlite3_deleteRecord_prices(sqlite3 *conn,int id_price);
void libmysqlite3_deleteRecord_stocks(sqlite3 *conn,int id_stock);
void libmysqlite3_deleteRecord_markets(sqlite3 *conn, int id_market);

void libmysqlite3_updateRecord(sqlite3 *conn);
void libmysqlite3_updateRecord_clients(sqlite3 *conn,int id_client,bool restricted,int restrictedTo);
void libmysqlite3_updateRecord_deals(sqlite3 *conn,int id_deal,bool restricted,int restrictedTo);
void libmysqlite3_updateRecord_prices(sqlite3 *conn,int id_price,bool restricted,int restrictedTo);
void libmysqlite3_updateRecord_stocks(sqlite3 *conn,int id_stock,bool restricted,int restrictedTo);
void libmysqlite3_updateRecord_markets(sqlite3 *conn, int id_market,bool restricted,int restrictedTo);


int libmysqlite3_findDBTables(sqlite3 *conn,char ***stringArr);


#endif
