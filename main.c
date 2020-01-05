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
  sqlite3_stmt *myStmt;

  // sqlite3_stmt *res;
  // sqlite3_prepare_v2(connection,"SELECT market_name FROM markets WHERE ? = 1;",-1,&res,0);
  // sqlite3_bind_text(res,1,"id_market",-1,SQLITE_TRANSIENT);
  // int rc = sqlite3_step(res) ;
  // printf("\nrc %d\n",rc);
  // printf("\n%d\n",sqlite3_column_count(res));
  // printf("%s",sqlite3_column_text(res,0));
  // printf("\n%s",sqlite3_errmsg(connection));
  // sqlite3_step(res);
  // printf("\n%s",sqlite3_errmsg(connection));
  libmysqlite3_print_stock_buySell_Info(connection,6);





  return 0;

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
      printf("Option: \n0-Quit\n1-Adding Record\n2-Delete Record\n3-Update\n>");
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
        default:
          system("cls||clear");
          printf("Invalid Option\n");
          break;
      }
      //system("cls||clear");
    }




  sqlite3_close(connection);printf("TestOke!%d\n",returnCode);
  return 0;
}
