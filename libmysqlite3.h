
#ifndef LIBMYSQLITE_H
#define LIBMYSQLITE_H

#include <stdbool.h>

#define DATESTRINGBUFF 11
#define STRINGBUFF 1024
#define FAILCONNECTION -1
#define SQLSTMTERROR -2
#define INVALID_INPUT -3
#define SUCCEED 0
#define UNDEFINEDERROR -4
#define HEAPOVEFLOW -5


char *mygetline(char *str, int maxlength);
int isLeap(int YYYY);
int getDate(char **returnDate);

int doesExist(sqlite3 *conn,char* tableName, char* columnName,const void *value,int dataType);

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
void libmysqlite3_updateRecord_clients(sqlite3 *conn,int id_client,int forcedUpdate,int forcedUpdateTo);
void libmysqlite3_updateRecord_deals(sqlite3 *conn,int id_deal);
void libmysqlite3_updateRecord_prices(sqlite3 *conn,int id_price,int forcedUpdate,int forcedUpdateTo);
void libmysqlite3_updateRecord_stocks(sqlite3 *conn,int id_stock,int forcedUpdate,int forcedUpdateTo);
void libmysqlite3_updateRecord_markets(sqlite3 *conn, int id_market,int forcedUpdate,int forcedUpdateTo);

int libmysqlite3_find_clientAction(sqlite3* conn,int withSpecifiedStock );
int libmysqlite3_find_client(sqlite3* conn);
int libmysqlite3_print_clientAction(sqlite3 *conn,int id_client,int id_stock);
int libmysqlite3_find_stock(sqlite3* conn);


int libmysqlite3_find_singleClientPortfolio(sqlite3 *conn);
int libmysqlite3_print_allClientPortfolio(sqlite3 *conn);
int libmysqlite3_print_clientPortfolio(sqlite3 *conn,int id_client);

int libmysqlite3_print_clientProfit(sqlite3 *conn,int id_client);
int libmysqlite3_find_singleClientProfit(sqlite3 *conn);
int libmysqlite3_find_allClientProfit(sqlite3 *conn);

void libmysqlite3_print_change(float todayPrice,float yesterdayPrice);
int libmysqlite3_find_dailyChange_stock(sqlite3 *conn);



#endif
