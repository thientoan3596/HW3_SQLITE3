#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>
#include "libmysqlite3.h"
#include <unistd.h>
#define MYDATABASE "stockexchange_5A.db"
int main(void)
{
  sqlite3 *connection;
  int returnCode =  sqlite3_open(MYDATABASE,&connection);
  if(returnCode != SQLITE_OK)
  {
    printf("Failure connecting to <%s>!\nEnd Program!\n",MYDATABASE);
    exit(FAILCONNECTION);
  }
  puts("Set up connection successfully");
  char *zErrMsg;

  sqlite3_exec(connection,"PRAGMA foreign_keys = ON;",0,NULL,NULL);
  returnCode = sqlite3_exec(connection, "CREATE TABLE IF NOT EXISTS prices ("
                                        "id_price INTEGER PRIMARY KEY,"
                                        "id_stock INTEGER,"
                                        "buy_price REAL,"
                                        "sell_price REAL,"
                                        "date text NOT NULL,"
                                        "FOREIGN KEY(id_stock) REFERENCES stocks(id_stock)"
                                        ");"
                                        "CREATE TABLE IF NOT EXISTS deals ("
                                        "id_deal INTEGER PRIMARY KEY,"
                                        "id_client INTEGER,"
                                        "id_price INTEGER,"
                                        "quantity INTEGER,"
                                        "is_buy INTEGER NOT NULL,"
                                        "FOREIGN KEY(id_client) REFERENCES clients(id_client),"
                                        "FOREIGN KEY(id_price) REFERENCES prices(id_price));",0,NULL,&zErrMsg);
    if(returnCode != SQLITE_OK)
    {
      printf("SQL Statement Error: %s\n",zErrMsg);
      free(zErrMsg);
      sqlite3_close(connection);
      exit(SQLSTMTERROR);
    }


    int menu_Option = -1;
    while(menu_Option != 0 )
    {
      printf("Option: \n0-Quit\n1-Adding Record\n2-Delete Record\n3-Update"
             "\n4-Find client action.\n5-Find client action with for a specified stock."
             "\n6-Find all client portfolio.\n7-Find client portfolio.\n8-Find all clients profit."
             "\n9-Find single client profit.\n10-Find history of daily price change for a stock\n11-Clear terminal\n>");
      scanf("%d",&menu_Option);
      switch(menu_Option)
      {
        case 0:
          break;
        case 1:
          libmysqlite3_addingRecord(connection);
          break;
        case 2:
          libmysqlite3_deleteRecord(connection);
          break;
        case 3:
          libmysqlite3_updateRecord(connection);
          break;
        case 4:
          libmysqlite3_find_clientAction(connection,0);
          break;
        case 5:
          libmysqlite3_find_clientAction(connection,1);
          break;
        case 6:
          libmysqlite3_print_allClientPortfolio(connection);
          break;
        case 7:
          libmysqlite3_find_singleClientPortfolio(connection);
          break;
        case 8:
          libmysqlite3_find_allClientProfit(connection);
          break;
        case 9:
          libmysqlite3_find_singleClientProfit(connection);
          break;
        case 10:
          libmysqlite3_find_dailyChange_stock(connection);
          break;
        case 11:
          system("clear");
          break;
        default:
          printf("Invalid Option\n");
          break;
      }
    }
  sqlite3_close(connection);
  return 0;
}
