#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sqlite3.h>
#include "libmysqlite3.h"

int doesExist(sqlite3 *conn,char* tableName, char* columnName,const void *value,int dataType)
{
  /* dataType : 1 - int, 2- char, 3 - float*/
  char SQL_STRING[1024];
  sqlite3_stmt *res;
  int returnVal;
  if (dataType != 1 && dataType != 2 && dataType != 3)
  {
    printf("$$INVALID DATATYPE$$\n");
    sqlite3_close(conn);
    exit(INVALID_INPUT);
  }
  if(dataType == 1)
  {
    sprintf(SQL_STRING, "SELECT COUNT (*) FROM %s WHERE %s = %d;", tableName, columnName, *((int*)value));
    sqlite3_prepare_v2(conn,SQL_STRING,-1,&res,0);
    if(sqlite3_step(res)== SQLITE_ROW)
    {
      returnVal = sqlite3_column_int(res,0);
      sqlite3_finalize(res);
      return returnVal;
    }
    printf("$$ERROR does exist() Function parameter$$\n");
    sqlite3_finalize(res);
    sqlite3_close(conn);
    exit(INVALID_INPUT);
  }
  else if (dataType == 3)
  {
    sprintf(SQL_STRING, "SELECT COUNT (*) FROM %s WHERE %s = %f;", tableName, columnName, *((float*)value));
    sqlite3_prepare_v2(conn,SQL_STRING,-1,&res,0);
    if(sqlite3_step(res)== SQLITE_ROW)
    {
      returnVal = sqlite3_column_int(res,0);
      sqlite3_finalize(res);
      return returnVal;
    }
    printf("$$ERROR does exist() Function parameter$$\n");
    sqlite3_finalize(res);
    sqlite3_close(conn);
    exit(INVALID_INPUT);
  }
  else
  {
    sprintf(SQL_STRING, "SELECT COUNT (*) FROM %s WHERE %s = '%s';", tableName, columnName, (char*)value);
    sqlite3_prepare_v2(conn,SQL_STRING,-1,&res,0);
    int rc =sqlite3_step(res);
    if(rc== SQLITE_ROW)
    {
      returnVal = sqlite3_column_int(res,0);
      sqlite3_finalize(res);
      return returnVal;
    }
    printf("$$ERROR: does exist() Function parameter$$\n");
    sqlite3_finalize(res);
    sqlite3_close(conn);
    exit(INVALID_INPUT);
  }
}

char *mygetline(char *str, int maxlength)
{
  int i=0;
  do{
    if(i>1)
    {
      printf("$Input cannot be empty$\n>");
    }
	  fgets(str,maxlength,stdin);
	  if(str[strlen(str)-1] == 10)
		{
			str[strlen(str)-1] = 0;
		}
    i++;
	}while(strlen(str)==0);
  return str;
}


int isLeap(int YYYY)
{
  if (YYYY%400 == 0)
    return 1;
  else if (YYYY%100 == 0)
    return false;
	else if (YYYY%4 == 0)
    return 0;
	else
    return 1;
}

int getDate(char **returnDate)
{
  int YYYY,MM,DD;
  printf("\tDate format is <YYYY-MM-DD> according to ISO-8601 standard!\n\tYear:\n\t>");
  scanf("%d",&YYYY);
  if(YYYY < 1)
  {
    puts("\t$$INVALID YEAR$$!\n");
    return INVALID_INPUT;
  }
  printf("\tMonth:\n\t>");
  scanf("%d",&MM);
  if( MM >12 || MM <1 )
  {
    puts("\t$$INVALID MONTH$$!\n");
    return INVALID_INPUT;
  }
  printf("\tDate:\n\t>");
  scanf("%d",&DD);
  if( !(DD >31 || DD <1) )
  {
    if( !((MM == 4 || MM == 6 || MM == 9 || MM == 11) && DD == 31))
    {
      if( !(MM == 2  && DD >29))
  		{
  			if( !(!isLeap(YYYY) && MM == 2 && DD >28))
  			{
          char *date = (char*)calloc(DATESTRINGBUFF,sizeof(char));
          *returnDate = date;
  				sprintf(*returnDate,"%04d-%02d-%02d",YYYY,MM,DD);
  				return SUCCEED;
  			}
  		}
    }
  }
  puts("\t$$INVALID DATE$$!\n");
  return INVALID_INPUT;
}

void libmysqlite3_addingRecord(sqlite3 *conn)
{
  int menu_Option = -1 ;
  printf("\tOption:\n\t1-clients\n\t2-deals\n\t3-prices\n\t4-stocks\n\t5-markets\n\t0-Cancel\n\t>");
  scanf("%d",&menu_Option);
  switch (menu_Option) {
    case 0:
      break;
    case 1:
      libmysqlite3_addingRecord_clients(conn);
      break;
    case 2:
      libmysqlite3_addingRecord_deals(conn);
      break;
    case 3:
      libmysqlite3_addingRecord_prices(conn);
      break;
    case 4:
      libmysqlite3_addingRecord_stocks(conn);
      break;
    case 5:
      libmysqlite3_addingRecord_markets(conn);
      break;
    default:
      puts("\t$$Invalid Option$$");
      break;
  }
}
int libmysqlite3_addingRecord_clients(sqlite3 *conn)
{
  int id_client_buff;
  char static_string_buff[STRINGBUFF];
  sqlite3_stmt *res;


  printf("\tEnter id_client:\n\t>");
  scanf("%d",&id_client_buff);
  if(doesExist(conn,"clients","id_client",&id_client_buff,1))
  {
    printf("$$Error! id_client is already existed.$$\n");
    return INVALID_INPUT;
  }

  sqlite3_prepare_v2(conn,"INSERT INTO clients VALUES(?,?,?);",-1,&res,0);
  sqlite3_bind_int(res,1,id_client_buff);

  printf("\tEnter first_name:\n\t>");
  mygetline(static_string_buff, 1024);
  sqlite3_bind_text(res,2,static_string_buff,strlen(static_string_buff),SQLITE_TRANSIENT);

  printf("\tEnter last_name:\n\t>");
  mygetline(static_string_buff, 1024);
  sqlite3_bind_text(res,3,static_string_buff,strlen(static_string_buff),SQLITE_STATIC);

  if(sqlite3_step(res) != SQLITE_DONE)
  {
    puts("\t$$SQL Statement fail!$$\n");
    sqlite3_finalize(res);
    return SQLSTMTERROR;
  }
  puts("\t#ADDING SUCCESSFULLY#");
  sqlite3_finalize(res);
  return SUCCEED;
}

int libmysqlite3_addingRecord_deals(sqlite3 *conn)
{
  int id_deal_buff,id_client_buff,id_price_buff,quantity_buff,is_buy_buff;
  sqlite3_stmt *res;


  printf("\tEnter id_deal:\n\t>");
  scanf("%d",&id_deal_buff);
  if(doesExist(conn,"deals","id_deal",&id_deal_buff,1))
  {
    printf("\t$$Error! id_deal already exists.$$\n");
    return INVALID_INPUT;
  }

  printf("\tEnter id_price:\n\t>");
  scanf("%d",&id_price_buff);
  if(!doesExist(conn,"prices","id_price",&id_price_buff,1))
  {
    printf("\t$$Error! id_deal does not exists.$$\n");
    return INVALID_INPUT;
  }


  printf("\tEnter id_client:\n\t>");
  scanf("%d",&id_client_buff);
  if(!doesExist(conn,"clients","id_client",&id_client_buff,1))
  {
    printf("\t$$Error! id_client does not exists.$$\n");
    return INVALID_INPUT;
  }

  printf("\tEnter quantity:\n\t>");
  scanf("%d",&quantity_buff);
  if(quantity_buff < 1)
  {
    printf("\t$$Error! quantity is negative.$$\n");
    return INVALID_INPUT;
  }

  printf("\tIs buy/sell ? (1:buy - 0:sell)):\n\t>");
  scanf("%d",&is_buy_buff);
  if( is_buy_buff != 0 && is_buy_buff != 1)
  {
    puts("\t&&Error! Invalid command! $$\n");
    return INVALID_INPUT;
  }

  sqlite3_prepare_v2(conn,"INSERT INTO deals VALUES(?,?,?,?,?);",-1,&res,0);
  sqlite3_bind_int(res,1,id_deal_buff);
  sqlite3_bind_int(res,2,id_client_buff);
  sqlite3_bind_int(res,3,id_price_buff);
  sqlite3_bind_int(res,4,quantity_buff);
  sqlite3_bind_int(res,5,is_buy_buff);
  if(sqlite3_step(res)!= SQLITE_DONE)
  {
    puts("\t$$SQL Statement fail!$$\n");
    printf("%s\n",sqlite3_errmsg(conn));
    sqlite3_finalize(res);
    return SQLSTMTERROR;
  }
  puts("\t#ADDING SUCCESSFULLY#");
  sqlite3_finalize(res);
  return SUCCEED;
}

int libmysqlite3_addingRecord_prices(sqlite3 *conn)
{
  int id_price_buff,id_stock_buff;
  float buy_price_buff,sell_price_buff;
  //char static_string_buff[STRINGBUFF];
  sqlite3_stmt *res;

  printf("\tEnter id_price:\n\t>");
  scanf("%d",&id_price_buff);
  if(doesExist(conn,"prices","id_price",&id_price_buff,1))
  {
    printf("\t$$Error! id_price already exists.$$\n");
    return INVALID_INPUT;
  }

  printf("\tEnter id_stock:\n\t>");
  scanf("%d",&id_stock_buff);
  if(! doesExist(conn,"stocks","id_stock",&id_stock_buff,1))
  {
    printf("\t$$Error! id_stock does not exists.$$\n");
    return INVALID_INPUT;
  }
  printf("\tEnter buy_price:\n\t>");
  scanf("%f",&buy_price_buff);
  printf("\tEnter sell_price:\n\t>");
  scanf("%f",&sell_price_buff);
  if( buy_price_buff < 0 || sell_price_buff < 0)
  {
    puts("\t&&Error! Price is negative! $$\n");
    return INVALID_INPUT;
  }

  char *date = NULL;
  printf("\tEnter date:\n\t>");
  if(getDate(&date) != SUCCEED)
  {
    free(date);
    return INVALID_INPUT;
  }

  sqlite3_prepare_v2(conn,"INSERT INTO prices VALUES(?,?,?,?,?);",-1,&res,0);
  sqlite3_bind_int(res,1,id_price_buff);
  sqlite3_bind_int(res,2,id_stock_buff);
  sqlite3_bind_double(res,3,buy_price_buff);
  sqlite3_bind_double(res,4,sell_price_buff);
  sqlite3_bind_text(res,5,date,DATESTRINGBUFF,SQLITE_STATIC);

  if(sqlite3_step(res) != SQLITE_DONE)
  {
    puts("\t$$SQL Statement fail!$$\n");
    sqlite3_finalize(res);
    return SQLSTMTERROR;
  }
  puts("\t#ADDING SUCCESSFULLY#");
  sqlite3_finalize(res);
  free(date);
  return SUCCEED;
}
int libmysqlite3_addingRecord_stocks(sqlite3 *conn)
{
  int id_market_buff,id_stock_buff;
  char static_string_buff[STRINGBUFF];
  sqlite3_stmt *res;

  printf("\tEnter id_stock:\n\t>");
  scanf("%d",&id_stock_buff);
  printf("\tEnter id_market:\n\t>");
  scanf("%d",&id_market_buff);

  if( doesExist(conn,"stocks","id_stock",&id_stock_buff,1))
  {
    printf("\t$$Error! id_stock already exists$$\n");
    return INVALID_INPUT;
  }

  if(! doesExist(conn,"markets","id_market",&id_market_buff,1))
  {
    printf("\t$$Error! id_market does not exists$$\n");
    return INVALID_INPUT;
  }

  /* Does not group all binding functions in order to use only one string buffer ( SQLITE_TRANSIENT) */
  sqlite3_prepare_v2(conn,"INSERT INTO stocks VALUES(?,?,?,?);",-1,&res,0);
  sqlite3_bind_int(res,1,id_stock_buff);
  sqlite3_bind_int(res,2,id_market_buff);

  printf("\tEnter stock_name:\n\t>");
  mygetline(static_string_buff, STRINGBUFF);
  sqlite3_bind_text(res,3,static_string_buff,strlen(static_string_buff),SQLITE_TRANSIENT);

  printf("\tEnter s_short_name:\n\t>");
  mygetline(static_string_buff, STRINGBUFF);
  sqlite3_bind_text(res,4,static_string_buff,strlen(static_string_buff),SQLITE_STATIC);

  if(sqlite3_step(res) != SQLITE_DONE)
  {
    puts("\t$$SQL Statement fail!$$\n");
    sqlite3_finalize(res);
    return SQLSTMTERROR;
  }
  puts("\t#ADDING SUCCESSFULLY#");
  sqlite3_finalize(res);
  return SUCCEED;
}
int libmysqlite3_addingRecord_markets(sqlite3 *conn)
{
  int id_market_buff;
  char static_string_buff[STRINGBUFF];
  sqlite3_stmt *res;
  printf("\tEnter id_market:\n\t>");
  scanf("%d",&id_market_buff);
  if( doesExist(conn,"markets","id_market",&id_market_buff,1))
  {
    printf("\t$$Error! id_market is already existed.$$\n");
    return INVALID_INPUT;
  }
  sqlite3_prepare_v2(conn,"INSERT INTO markets VALUES(?,?,?);",-1,&res,0);
  sqlite3_bind_int(res,1,id_market_buff);
  printf("\tEnter market_name:\n\t>");
  mygetline(static_string_buff, 1024);
  sqlite3_bind_text(res,2,static_string_buff,strlen(static_string_buff),SQLITE_TRANSIENT);
  printf("\tEnter m_short_name:\n\t>");
  mygetline(static_string_buff, 1024);
  sqlite3_bind_text(res,3,static_string_buff,strlen(static_string_buff),SQLITE_TRANSIENT);

  if(sqlite3_step(res) != SQLITE_DONE)
  {
    puts("\t$$SQL Statement fail!$$\n");
    sqlite3_finalize(res);
    return SQLSTMTERROR;
  }
  puts("\t#ADDING SUCCESSFULLY#");
  sqlite3_finalize(res);
  return SUCCEED;
}
void libmysqlite3_deleteRecord(sqlite3 *conn)
{
  int buff = -1 ;
  printf("\tWARNING: All the related information will be deleted\n");
  printf("\tOption:\n\t1-clients\n\t2-deals\n\t3-prices\n\t4-stocks\n\t5-markets\n\t0-Cancel\n\t>");
  scanf("%d",&buff);
  switch (buff) {
    case 0:
      break;
    case 1:
      printf("\tEnter id_client to delete:\n\t>");
      scanf("%d",&buff);
      libmysqlite3_deleteRecord_clients(conn,buff);
      break;
    case 2:
      printf("\tEnter id_deal to delete:\n\t>");
      scanf("%d",&buff);
      libmysqlite3_deleteRecord_deals(conn,buff);
      break;
    case 3:
      printf("\tEnter id_price to delete:\n\t>");
      scanf("%d",&buff);
      libmysqlite3_deleteRecord_prices(conn,buff);
      break;
    case 4:
      printf("\tEnter id_stock to delete:\n\t>");
      scanf("%d",&buff);
      libmysqlite3_deleteRecord_stocks(conn,buff);
      break;
    case 5:
      printf("\tEnter id_market to delete:\n\t>");
      scanf("%d",&buff);
      libmysqlite3_deleteRecord_markets(conn,buff);
      break;
    default:
      puts("$$Invalid Option$$");
      break;
  }
}

void libmysqlite3_deleteRecord_clients(sqlite3 *conn,int id_client)
{
  sqlite3_stmt *res;
  sqlite3_prepare_v2 (conn, "DELETE FROM clients WHERE id_client = ?;",-1,&res,0);
  sqlite3_bind_int(res,1,id_client);
  sqlite3_step(res);
  sqlite3_finalize(res);
  sqlite3_prepare_v2 (conn,"DELETE FROM deals WHERE id_client = ?;",-1,&res,0);
  sqlite3_bind_int(res,1,id_client);
  sqlite3_step(res);
  sqlite3_finalize(res);
}
void libmysqlite3_deleteRecord_deals(sqlite3 *conn,int id_deal)
{
  sqlite3_stmt *res;
  sqlite3_prepare_v2 (conn,"DELETE FROM deals WHERE id_deal = ?;",-1,&res,0);
  sqlite3_bind_int(res,1,id_deal);
  sqlite3_finalize(res);
}
void libmysqlite3_deleteRecord_prices(sqlite3 *conn,int id_price)
{
  sqlite3_stmt *res;
  sqlite3_prepare_v2 (conn,"DELETE FROM prices WHERE id_price = ?;",-1,&res,0);
  sqlite3_bind_int(res,1,id_price);
  sqlite3_step(res) ;
  sqlite3_finalize(res);
  sqlite3_prepare_v2 (conn,"DELETE FROM deals WHERE id_price = ?;",-1,&res,0);
  sqlite3_bind_int(res,1,id_price);
  sqlite3_step(res) ;
  sqlite3_finalize(res);
}
void libmysqlite3_deleteRecord_stocks(sqlite3 *conn,int id_stock)
{
  sqlite3_stmt *res;
  int *matched_id_price = NULL;
  int sizeOfMatch = 0, counter;
  sqlite3_prepare_v2 (conn,"SELECT id_price FROM prices WHERE id_stock = ?;",-1,&res,0);
  sqlite3_bind_int(res,1,id_stock);
  while(sqlite3_step(res) == SQLITE_ROW)
  {
    matched_id_price = (int*) realloc(matched_id_price,(1+sizeOfMatch)*sizeof(int));
    if(matched_id_price == NULL)
    {
      printf("$$HEAPOVEFLOW$$\n");
      sqlite3_finalize(res);
      sqlite3_close(conn);
      exit(HEAPOVEFLOW);
    }
    matched_id_price[sizeOfMatch] = sqlite3_column_int(res,0);
    sizeOfMatch ++;
  }
  sqlite3_finalize(res);
  for(counter = 0; counter < sizeOfMatch; counter ++)
  {
    libmysqlite3_deleteRecord_prices(conn,matched_id_price[counter]);
  }
  if(sizeOfMatch != 0)
  {
    free(matched_id_price);
  }
  sqlite3_prepare_v2 (conn,"DELETE FROM stocks WHERE id_stock = ?;",-1,&res,0);
  sqlite3_bind_int(res,1,id_stock);
  sqlite3_step(res) ;
  sqlite3_finalize(res);
}
void libmysqlite3_deleteRecord_markets(sqlite3 *conn, int id_market)
{
  sqlite3_stmt *res;
  int *matched_id_stock = NULL;
  int sizeOfMatch = 0, counter ;
  sqlite3_prepare_v2 (conn,"SELECT id_stock FROM stocks WHERE id_market = ?;",-1,&res,0);
  sqlite3_bind_int(res,1,id_market);
  while(sqlite3_step(res) == SQLITE_ROW)
  {
    matched_id_stock = (int*) realloc(matched_id_stock,(1+sizeOfMatch)*sizeof(int));
    if(matched_id_stock == NULL)
    {
      printf("$$HEAPOVEFLOW$$\n");
      sqlite3_finalize(res);
      sqlite3_close(conn);
      exit(HEAPOVEFLOW);
    }
    matched_id_stock[sizeOfMatch] = sqlite3_column_int(res,0);
    sizeOfMatch ++;
  }
  sqlite3_finalize(res);
  for(counter = 0; counter < sizeOfMatch; counter ++)
  {
    libmysqlite3_deleteRecord_stocks(conn,matched_id_stock[counter]);
  }
  if(sizeOfMatch != 0)
  {
    free(matched_id_stock);
  }
  sqlite3_prepare_v2 (conn,"DELETE FROM markets WHERE id_market = ?;",-1,&res,0);
  sqlite3_bind_int(res,1,id_market);
  sqlite3_step(res) ;
  sqlite3_finalize(res);
}

void libmysqlite3_updateRecord(sqlite3 *conn)
{
  int buff = -1 ;
  printf("\tWARNING: All the related information will be updated\n");
  sqlite3_exec(conn,"PRAGMA foreign_keys = OFF;",0,NULL,NULL);
  printf("\tOption:\n\t1-clients\n\t2-deals\n\t3-prices\n\t4-stocks\n\t5-markets\n\t0-Cancel\n\t>");
  scanf("%d",&buff);
  switch (buff) {
    case 0:
      break;
    case 1:
      printf("\tEnter id_client to update:\n\t>");
      scanf("%d",&buff);
      libmysqlite3_updateRecord_clients(conn,buff,false,0);
      break;
    case 2:
      printf("\tEnter id_deal to update:\n\t>");
      scanf("%d",&buff);
      libmysqlite3_updateRecord_deals(conn,buff);
      break;
    case 3:
      printf("\tEnter id_price to update:\n\t>");
      scanf("%d",&buff);
      libmysqlite3_updateRecord_prices(conn,buff,false,0);
      break;
    case 4:
      printf("\tEnter id_stock to update:\n\t>");
      scanf("%d",&buff);
      libmysqlite3_updateRecord_stocks(conn,buff,false,0);
      break;
    case 5:
      printf("\tEnter id_market to update:\n\t>");
      scanf("%d",&buff);
      libmysqlite3_updateRecord_markets(conn,buff,false,0);
      break;
    default:
      puts("$$Invalid Option$$");
      break;
  }
sqlite3_exec(conn,"PRAGMA foreign_keys = ON;",0,NULL,NULL);
}
void libmysqlite3_updateRecord_clients(sqlite3 *conn,int id_client,int forcedUpdate,int forcedUpdateTo)
{
  int option;
  int new_val;
  char strbuff[STRINGBUFF];
  sqlite3_stmt *res;
  if(!forcedUpdate)
  {
    printf("\t(1-id_client) (2-first_name) (3-last_name)\n\t>");
    scanf("%d",&option);
  }
  else
  {
    option = forcedUpdateTo;
  }

  switch (option) {
    case 1:
      printf("\tNew id_client:\n\t>");
      scanf("%d",&new_val);
      if(doesExist(conn,"clients","id_client",&new_val,1))
      {
        printf("\t$$New id_client = %d already exists$$\n",new_val);
      }
      else
      {
        sqlite3_prepare_v2(conn,"UPDATE clients SET id_client = ? WHERE id_client = ?;",-1,&res,0);
        sqlite3_bind_int(res,1,new_val);
        sqlite3_bind_int(res,2,id_client);
        if(sqlite3_step(res) != SQLITE_DONE)
        {
          printf("\t$$%s$$\n",sqlite3_errmsg(conn));
        }
        else
        {
          sqlite3_finalize(res);
          sqlite3_prepare_v2(conn,"UPDATE deals SET id_client = ? WHERE id_client = ?;",-1,&res,0);
          sqlite3_bind_int(res,1,new_val);
          sqlite3_bind_int(res,2,id_client);
          sqlite3_step(res) ;
        }
        sqlite3_finalize(res);
      }
      break;
    case 2:
      printf("\tNew first_name:\n\t>");
      mygetline(strbuff,STRINGBUFF);
      sqlite3_prepare_v2(conn,"UPDATE clients SET first_name = ? WHERE id_client = ?;",-1,&res,0);
      sqlite3_bind_text(res,1,strbuff,strlen(strbuff),SQLITE_STATIC);
      sqlite3_bind_int(res,2,id_client);
      sqlite3_step(res) ;
      sqlite3_finalize(res);
      break;
    case 3:
      printf("\tNew last_name:\n\t>");
      mygetline(strbuff,STRINGBUFF);
      sqlite3_prepare_v2(conn,"UPDATE clients SET last_name = ? WHERE id_client = ?;",-1,&res,0);
      sqlite3_bind_text(res,1,strbuff,strlen(strbuff),SQLITE_STATIC);
      sqlite3_bind_int(res,2,id_client);
      sqlite3_step(res) ;
      sqlite3_finalize(res);
      break;
    default :
      puts("\t$$Invalid Option$$");
      break;
  }
}
void libmysqlite3_updateRecord_deals(sqlite3 *conn,int id_deal)
{
  int option;
  int new_val;
  //char strbuff[STRINGBUFF];
  sqlite3_stmt *res;
  printf("\t(1-id_deal) (2-id_client) (3-id_price) (4-quantity) (5-is_buy)\n\t\t>");
  scanf("%d",&option);
  switch (option) {
    case 1:
      printf("\tNew id_deal:\n\t>");
      scanf("%d",&new_val);
      if( doesExist(conn,"deals","id_deal",&new_val,1))
      {
        printf("\t$$New id_deal = %d already exist$$\n",new_val);
      }
      else
      {
        sqlite3_prepare_v2(conn,"UPDATE deals SET id_deal = ? WHERE id_deal = ?;",-1,&res,0);
        sqlite3_bind_int(res,1,new_val);
        sqlite3_bind_int(res,2,id_deal);
        if(sqlite3_step(res) != SQLITE_DONE)
        {
          printf("\t$$%s$$\n",sqlite3_errmsg(conn));
        }
        sqlite3_finalize(res);
      }
      break;
    case 2:
      sqlite3_prepare_v2(conn,"SELECT id_client FROM deals WHERE id_deal = ?;",-1,&res,0);
      sqlite3_bind_int(res,1,id_deal);
      if (sqlite3_step(res) != SQLITE_ROW)
      {
        printf("\t$$%s$$",sqlite3_errmsg(conn));
      }
      else
      {
        libmysqlite3_updateRecord_clients(conn,sqlite3_column_int(res,0),true,1);
      }
      sqlite3_finalize(res);
      break;
    case 3:
      sqlite3_prepare_v2(conn,"SELECT id_price FROM deals WHERE id_deal = ?;",-1,&res,0);
      sqlite3_bind_int(res,1,id_deal);
      if(sqlite3_step(res) != SQLITE_ROW)
      {
        printf("\t$$%s$$\n",sqlite3_errmsg(conn));
      }
      else
      {
        libmysqlite3_updateRecord_prices(conn,sqlite3_column_int(res,0),true,1);
      }
      sqlite3_finalize(res);
      break;
    case 4:
      printf("\tNew quantity:\n\t>");
      scanf("%d",&new_val);
      if(new_val < 1)
      {
        printf("\t$$UPDATE FAIL: NEGATIVE VALUE\n$$");
      }
      else
      {
        sqlite3_prepare_v2(conn,"UPDATE deals SET quantity = ? WHERE id_deal = ?;",-1,&res,0);
        sqlite3_bind_int(res,1,new_val);
        sqlite3_bind_int(res,2,id_deal);
        if(sqlite3_step(res) != SQLITE_DONE)
        {
          printf("\t$$%s$$\n",sqlite3_errmsg(conn));
        }
        sqlite3_finalize(res);
      }
      break;
    case 5:
      printf("\tNew is_buy (0-Sell) (1-Buy):\n\t>");
      scanf("%d",&new_val);
      if(new_val != 0  && new_val != 1)
      {
        printf("\t$$UPDATE FAIL: INVALID VALUE\n$$");
      }
      else
      {
        sqlite3_prepare_v2(conn,"UPDATE deals SET is_buy = ? WHERE id_deal = ?;",-1,&res,0);
        sqlite3_bind_int(res,1,new_val);
        sqlite3_bind_int(res,2,id_deal);
        if(sqlite3_step(res) != SQLITE_DONE)
        {
          printf("\t$$%s$$\n",sqlite3_errmsg(conn));
        }
        sqlite3_finalize(res);
      }
      break;
    default :
      puts("\t$$Invalid Option$$");
      break;
  }
}
void libmysqlite3_updateRecord_prices(sqlite3 *conn,int id_price,int forcedUpdate,int forcedUpdateTo)
{
  int option, new_val;
  float new_price;
  sqlite3_stmt *res;
  if(!forcedUpdate)
  {
    printf("\t(1-id_price) (2-id_stock) (3-buy_price) (4-sell_price) (5-date)\n\t>");
    scanf("%d",&option);
  }
  else
  {
    option = forcedUpdateTo;
  }
  switch (option) {
    case 1:
      printf("\tNew id_price:\n\t>");
      scanf("%d",&new_val);
      if( doesExist(conn,"prices","id_price",&new_val,1))
      {
        printf("\t$$New id_price = %d already exists$$\n",new_val);
      }
      else
      {
        sqlite3_prepare_v2(conn,"UPDATE prices SET id_price = ? WHERE id_price = ?;",-1,&res,0);
        sqlite3_bind_int(res,1,new_val);
        sqlite3_bind_int(res,2,id_price);
        if(sqlite3_step(res) != SQLITE_DONE)
        {
          printf("\t$$%s$$\n",sqlite3_errmsg(conn));
        }
        else
        {
          sqlite3_finalize(res);
          sqlite3_prepare_v2(conn,"UPDATE deals SET id_price = ? WHERE id_price = ?;",-1,&res,0);
          sqlite3_bind_int(res,1,new_val);
          sqlite3_bind_int(res,2,id_price);
          sqlite3_step(res) ;
        }
        sqlite3_finalize(res);
      }
      break;
    case 2:
      sqlite3_prepare_v2(conn,"SELECT id_stock FROM prices WHERE id_price = ?;",-1,&res,0);
      sqlite3_bind_int(res,1,id_price);
      if(sqlite3_step(res) != SQLITE_ROW)
      {
        printf("\t$$%s$$\n",sqlite3_errmsg(conn));
      }
      else
      {
        libmysqlite3_updateRecord_stocks(conn,sqlite3_column_int(res,0),true,1);
      }
      sqlite3_finalize(res);
      break;
    case 3:
      printf("\tNew buy_price:\n\t>");
      scanf("%f",&new_price);
      if(new_price <= 0)
      {
        printf("\t$$UPDATE FAIL: NEGATIVE VALUE$$\n");
      }
      else
      {
        sqlite3_prepare_v2(conn,"UPDATE prices SET buy_price = ? WHERE id_price = ?;",-1,&res,0);
        sqlite3_bind_int(res,1,new_price);
        sqlite3_bind_int(res,2,id_price);
        sqlite3_step(res);
        sqlite3_finalize(res);
      }

      break;
    case 4:
      printf("\tNew sell_price:\n\t>");
      scanf("%f",&new_price);
      if(new_price <= 0)
      {
        printf("\t$$UPDATE FAIL: NEGATIVE VALUE$$\n");
      }
      else
      {
        sqlite3_prepare_v2(conn,"UPDATE prices SET sell_price = ? WHERE id_price = ?;",-1,&res,0);
        sqlite3_bind_int(res,1,new_price);
        sqlite3_bind_int(res,2,id_price);
        sqlite3_step(res);
        sqlite3_finalize(res);
      }
      break;
    case 5:
      printf("\tNew date:\n");
      char *dynamic_str = NULL;
      if(getDate(&dynamic_str) == SUCCEED)
      {
        sqlite3_prepare_v2(conn,"UPDATE prices SET date = ? WHERE id_price = ?;",-1,&res,0);
        sqlite3_bind_text(res,1,dynamic_str,-1,SQLITE_STATIC);
        sqlite3_bind_int(res,2,id_price);
        sqlite3_step(res);
        sqlite3_finalize(res);
        free(dynamic_str);
      }
      break;
    default:
      puts("\t$$Invalid Option$$\n");
      break;
  }
}
void libmysqlite3_updateRecord_stocks(sqlite3 *conn,int id_stock,int forcedUpdate,int forcedUpdateTo)
{
  int option, new_val;
  char strbuff[STRINGBUFF];
  sqlite3_stmt *res;
  if(!forcedUpdate)
  {
    printf("\t(1-id_stock) (2-id_market) (3-stock_name) (4-s_short_name)\n\t>");
    scanf("%d",&option);
  }
  else
  {
    option = forcedUpdateTo;
  }
  switch (option) {
    case 1:
      printf("\tNew id_stock:\n\t>");
      scanf("%d",&new_val);
      if(doesExist(conn,"stocks","id_stock",&new_val,1))
      {
        printf("\t$$New id_stock = %d already exists$$\n",new_val);
      }
      else
      {
        sqlite3_prepare_v2(conn,"UPDATE stocks SET id_stock = ? WHERE id_stock = ?;",-1,&res,0);
        sqlite3_bind_int(res,1,new_val);
        sqlite3_bind_int(res,2,id_stock);
        if(sqlite3_step(res) != SQLITE_DONE)
        {
          printf("\t$$%s$$\n",sqlite3_errmsg(conn));
        }
        else
        {
          sqlite3_finalize(res);
          sqlite3_prepare_v2(conn,"UPDATE prices SET id_stock = ? WHERE id_stock = ?;",-1,&res,0);
          sqlite3_bind_int(res,1,new_val);
          sqlite3_bind_int(res,2,id_stock);
          sqlite3_step(res) ;
        }
        sqlite3_finalize(res);
      }
      break;
    case 2:
      sqlite3_prepare_v2(conn,"SELECT id_market FROM stocks WHERE id_stock = ?;",-1,&res,0);
      sqlite3_bind_int(res,1,id_stock);
      if(sqlite3_step(res) != SQLITE_ROW)
      {
        printf("\t$$%s$$\n",sqlite3_errmsg(conn));
      }
      else
      {
        libmysqlite3_updateRecord_markets(conn,sqlite3_column_int(res,1),true,1);
      }
      sqlite3_finalize(res);
      break;
    case 3:
      printf("\tNew stock_name:\n\t>");
      mygetline(strbuff,STRINGBUFF);
      sqlite3_prepare_v2(conn,"UPDATE stocks SET stock_name = ? WHERE stock_id = ?;",-1,&res,0);
      sqlite3_bind_text(res,1,strbuff,-1,SQLITE_STATIC);
      sqlite3_bind_int(res,2,id_stock);
      sqlite3_step(res);
      sqlite3_finalize(res);
      break;
    case 4:
      printf("\tNew s_short_name:\n\t>");
      mygetline(strbuff,STRINGBUFF);
      sqlite3_prepare_v2(conn,"UPDATE stocks SET s_short_name = ? WHERE stock_id = ?;",-1,&res,0);
      sqlite3_bind_text(res,1,strbuff,-1,SQLITE_STATIC);
      sqlite3_bind_int(res,2,id_stock);
      sqlite3_step(res);
      sqlite3_finalize(res);
      break;
    default:
      puts("\t$$Invalid Option$$");
      break;
  }
}
void libmysqlite3_updateRecord_markets(sqlite3 *conn, int id_market,int forcedUpdate,int forcedUpdateTo)
{
  int option, new_val;
  char strbuff[STRINGBUFF];
  sqlite3_stmt *res;
  if(!forcedUpdate)
  {
    printf("\t(1-id_market) (2-market_name) (3-m_short_name)\n\t>");
    scanf("%d",&option);
  }
  else
  {
    option = forcedUpdateTo;
  }
  switch (option) {
    case 1:
      printf("\tNew id_market:\n\t>");
      scanf("%d",&new_val);
      if(doesExist(conn,"markets","id_market",&new_val,1))
      {
        printf("\t$$New id_market = %d already exists$$\n",new_val);
      }
      else
      {
        sqlite3_prepare_v2(conn,"UPDATE markets SET id_market = ? WHERE id_market = ?;",-1,&res,0);
        sqlite3_bind_int(res,1,new_val);
        sqlite3_bind_int(res,2,id_market);
        if(sqlite3_step(res) != SQLITE_DONE)
        {
          printf("\t$$%s$$\n",sqlite3_errmsg(conn));
        }
        else
        {
          sqlite3_finalize(res);
          sqlite3_prepare_v2(conn,"UPDATE stock SET id_market = ? WHERE id_market = ?;",-1,&res,0);
          sqlite3_bind_int(res,1,new_val);
          sqlite3_bind_int(res,2,id_market);
          sqlite3_step(res) ;
        }
        sqlite3_finalize(res);
      }
      break;
    case 2:
      printf("\tNew market_name:\n\t>");
      mygetline(strbuff,STRINGBUFF);
      sqlite3_prepare_v2(conn,"UPDATE markets SET market_name = ? WHERE id_market = ?;",-1,&res,0);
      sqlite3_bind_text(res,1,strbuff,-1,SQLITE_STATIC);
      sqlite3_bind_int(res,2,id_market);
      sqlite3_step(res);
      sqlite3_finalize(res);
      break;
    case 3:
      printf("\tNew m_short_name:\n\t>");
      mygetline(strbuff,STRINGBUFF);
      sqlite3_prepare_v2(conn,"UPDATE markets SET m_short_name = ? WHERE id_market = ?;",-1,&res,0);
      sqlite3_bind_text(res,1,strbuff,-1,SQLITE_STATIC);
      sqlite3_bind_int(res,2,id_market);
      sqlite3_step(res);
      sqlite3_finalize(res);
      break;
    default:
      puts("\t$$Invalid Option$$\n");
      break;
  }
}
int libmysqlite3_find_stock(sqlite3* conn)
{
  int option = -1;
  int id_stock = 0;
  char stock_name[STRINGBUFF];
  sqlite3_stmt *res;
  while (option != 1 && option != 2)
  {
    printf("\t(1- Using id_stock) (2- Using stock_name)\n\t>");
    scanf("%d",&option);
  }
  if(option == 2)
  {
    printf("\t\nEnter stock_name:\n\t>");
    mygetline(stock_name,STRINGBUFF);
    sqlite3_prepare_v2(conn,"SELECT COUNT(*) FROM stocks WHERE stock_name IS ? ;",-1,&res,0);
    sqlite3_bind_text(res,1,stock_name,strlen(stock_name),SQLITE_STATIC);
    sqlite3_step(res);
    if(sqlite3_column_int(res,0) == 0)
    {
      printf("\tNo such stock!\n\tOperation fail!\n");
      sqlite3_finalize(res);
      return 0;
    }
    else if(sqlite3_column_int(res,0) == 1)
    {
      sqlite3_finalize(res);
      sqlite3_prepare_v2(conn,"SELECT id_stock FROM stocks WHERE stock_name IS ?;",-1,&res,0);
      sqlite3_bind_text(res,1,stock_name,strlen(stock_name),SQLITE_STATIC);
      sqlite3_step(res);
      id_stock = sqlite3_column_int(res,0);
      sqlite3_finalize(res);
      return id_stock;

    }
    printf("\tFound %d results! Please search by id_stock\n",sqlite3_column_int(res,0));
    sqlite3_finalize(res);
  }
  printf("\tEnter id_stock:\n\t>");
  scanf("%d",&id_stock);
  if(doesExist(conn,"stocks","id_stock",&id_stock,1))
  {
    return id_stock;
  }
  printf("\tNo such stock!\n\tOperation fail!\n");
  return 0;
}
int libmysqlite3_find_clientAction(sqlite3* conn,int withSpecifiedStock )
{
  if(withSpecifiedStock != 0 && withSpecifiedStock != 1)
  {
    printf("\tPassing wrong argument!\n");
    sqlite3_close(conn);
    exit(INVALID_INPUT);
  }

  int id_client = libmysqlite3_find_client(conn);
  int id_stock = 0;
  if (withSpecifiedStock == 0 )
  {
    if(libmysqlite3_print_clientAction(conn,id_client,id_stock) == SUCCEED)
    {
      return SUCCEED;
    }
    sqlite3_close(conn);
    exit(UNDEFINEDERROR);
  }
  else
  {
    id_stock = libmysqlite3_find_stock(conn);
    if(id_stock == 0)
    {
      return SUCCEED;
    }
    if(libmysqlite3_print_clientAction(conn,id_client,id_stock) == SUCCEED)
    {
      return SUCCEED;
    }
    sqlite3_close(conn);
    exit(UNDEFINEDERROR);
  }
}
int libmysqlite3_find_client(sqlite3* conn)
{
  int id_client = 0;
  char fname[STRINGBUFF];
  char lname[STRINGBUFF];
  int option = -1;
  sqlite3_stmt *res;
  while (option != 1 && option != 2)
  {
    printf("\t(1- Using id_client) (2- Using client first and last name)\n\t>");
    scanf("%d",&option);
  }
  if(option == 2)
  {
    printf("\tEnter first_name:\n\t>");
    mygetline(fname,STRINGBUFF);
    sqlite3_prepare_v2(conn,"SELECT COUNT(*) FROM clients WHERE first_name IS ? AND last_name IS ?;",-1,&res,0);
    sqlite3_bind_text(res,1,fname,strlen(fname),SQLITE_STATIC);
    printf("\tEnter last_name:\n\t>");
    mygetline(lname,STRINGBUFF);
    sqlite3_bind_text(res,2,lname,strlen(lname),SQLITE_STATIC);
    sqlite3_step(res);
    if(sqlite3_column_int(res,0) == 0)
    {
      printf("\tNo such client!\n\tOperation fail!\n");
      sqlite3_finalize(res);
      return 0;
    }
    else if(sqlite3_column_int(res,0) == 1)
    {
      sqlite3_finalize(res);
      sqlite3_prepare_v2(conn,"SELECT id_client FROM clients WHERE first_name IS ? AND last_name IS ?;",-1,&res,0);
      sqlite3_bind_text(res,1,fname,strlen(fname),SQLITE_STATIC);
      sqlite3_bind_text(res,2,lname,strlen(lname),SQLITE_STATIC);
      sqlite3_step(res);
      id_client = sqlite3_column_int(res,0);
      sqlite3_finalize(res);
      return id_client;
    }
    else
    {
      printf("\tFound %d results! Please search by client_id\n",sqlite3_column_int(res,0));
      sqlite3_finalize(res);
    }
  }


  printf("\tEnter id_client:\n\t>");
  scanf("%d",&id_client);
  if(doesExist(conn,"clients","id_client",&id_client,1))
  {
    return id_client;
  }
  printf("\tNo such client!\n\tOperation fail!\n");
  return 0;
}

int libmysqlite3_print_clientAction(sqlite3 *conn,int id_client,int id_stock)
{

  char buff[STRINGBUFF];
  sqlite3_stmt *res;
  sqlite3_prepare_v2(conn,"SELECT * FROM clients where id_client =?;",-1,&res,0);
  sqlite3_bind_int(res,1,id_client);
  if(sqlite3_step(res) != SQLITE_ROW)
  {
    printf("\t\t$$%s$$\n",sqlite3_errmsg(conn));
    return INVALID_INPUT;
  }
  printf("ClientID|First Name|Last Name \n");
  printf("%4d\t|%-10s|%-s\n",sqlite3_column_int(res,0),sqlite3_column_text(res,1),sqlite3_column_text(res,2));
  sqlite3_finalize(res);
  printf("==================================\n");
  if(!doesExist(conn,"deals","id_client",&id_client,1))
  {
    printf(" No deal made by client!\n");
    return SUCCEED;
  }
  if( id_stock != 0)
  {
    sprintf(buff,        "WITH "
                           "buff AS "
                         "( "
                             "SELECT * "
                             "FROM prices "
                             "LEFT JOIN stocks USING (id_stock) "
                            ") "
                         "SELECT 	buff.date, "
                                 "d.id_deal, "
                                 "buff.id_stock, "
                                 "buff.stock_name, "
                                 "buff.id_price, "
                                 "CASE d.is_buy "
                                    "WHEN 1 THEN 'Buy' "
                                    "ELSE 'Sell' "
                                 "END AS Status, "
                                 "CASE d.is_buy "
                                    "WHEN 1 THEN buff.buy_price "
                                    "ELSE buff.sell_price "
                                 "END AS price, "
                                 "d.quantity "
                         "FROM deals d "
                         "LEFT JOIN buff USING (id_price) "
                         "WHERE d.id_client = %d AND buff.id_stock = %d "
                         "ORDER BY buff.date ASC,buff.id_stock ASC ,d.is_buy ASC,d.id_deal ASC ;",id_client,id_stock);
  }
  else
  {
    sprintf(buff,        "WITH "
                           "buff AS "
                         "( "
                             "SELECT * "
                             "FROM prices "
                             "LEFT JOIN stocks USING (id_stock) "
                            ") "
                         "SELECT 	buff.date, "
                                 "d.id_deal, "
                                 "buff.id_stock, "
                                 "buff.stock_name, "
                                 "buff.id_price, "
                                 "CASE d.is_buy "
                                    "WHEN 1 THEN 'Buy' "
                                    "ELSE 'Sell' "
                                 "END AS Status, "
                                 "CASE d.is_buy "
                                    "WHEN 1 THEN buff.buy_price "
                                    "ELSE buff.sell_price "
                                 "END AS price, "
                                 "d.quantity "
                         "FROM deals d "
                         "LEFT JOIN buff USING (id_price) "
                         "WHERE d.id_client = %d "
                         "ORDER BY buff.date,buff.id_stock,d.is_buy,d.id_deal ASC ;",id_client);
  }
  printf("  date \t  |id_deal|id_stock|     stock name   |id_price|Status|price  |Quantity\n");
  sqlite3_prepare_v2(conn,buff,-1,&res,0);
  while(sqlite3_step(res) == SQLITE_ROW)
  {
    printf("%s|%7d|%8d|%18s|%8d|%6s|%7.3f|%d\n",  sqlite3_column_text(res,0),sqlite3_column_int(res,1),sqlite3_column_int(res,2),
                                                  sqlite3_column_text(res,3),sqlite3_column_int(res,4),sqlite3_column_text(res,5),
                                                  sqlite3_column_double(res,6),sqlite3_column_int(res,7));
  }
  sqlite3_finalize(res);
  return SUCCEED;
}

int libmysqlite3_print_clientPortfolio(sqlite3 *conn,int id_client)
{
  char buff[STRINGBUFF];
  sqlite3_stmt *res;
  sqlite3_prepare_v2(conn,"SELECT * FROM clients where id_client =?;",-1,&res,0);
  sqlite3_bind_int(res,1,id_client);
  if(sqlite3_step(res) != SQLITE_ROW)
  {
    printf("\t$$%s<$$\n",sqlite3_errmsg(conn));
    return INVALID_INPUT;
  }
  printf("ClientID|First Name|Last Name \n");
  printf("%4d\t|%-10s|%-s\n",sqlite3_column_int(res,0),sqlite3_column_text(res,1),sqlite3_column_text(res,2));
  sqlite3_finalize(res);
  printf("==================================\n");
  sprintf(buff, "WITH LTBL AS ( SELECT * FROM deals LEFT JOIN clients USING (id_client))"
                    ",RTBL AS ( SELECT * FROM prices LEFT JOIN stocks c USING (id_stock))"
                "SELECT r.stock_name,r.id_stock,"
                      "SUM ( CASE l.is_buy WHEN 1 	THEN l.quantity "
                                          "ELSE -l.quantity END) AS share_holding "
                "FROM LTBL l "
                "LEFT JOIN RTBL r USING (id_price) "
                "WHERE l.id_client = %d "
                "GROUP by r.id_stock "
                "HAVING sum ( CASE l.is_buy WHEN 1 	THEN l.quantity "
                                           "ELSE -l.quantity END) > 0 "
                "ORDER by r.stock_name ASC;",id_client);
  sqlite3_prepare_v2(conn,buff,-1,&res,0);
  int returnCode =sqlite3_step(res);
  if(returnCode == SQLITE_DONE)
  {
    printf("Client is not holding any stock share at the moment!\n");
    sqlite3_finalize (res);
    return SUCCEED;
  }
  else
  {
    if(returnCode == SQLITE_ROW)
    {
      do{
          printf("Holding %d share(s) of %s (id_stock:%d)\n",sqlite3_column_int(res,2),sqlite3_column_text(res,0),sqlite3_column_int(res,1));
      }while ( sqlite3_step(res) == SQLITE_ROW);
      sqlite3_finalize(res);

      return SUCCEED;
    }
    sqlite3_finalize(res);
    printf("\t$$UNDEFINEDERROR$$\n\t$$%s$$\n",sqlite3_errmsg(conn));
    exit( UNDEFINEDERROR);
  }
}

int libmysqlite3_print_allClientPortfolio(sqlite3 *conn)
{
  int *id_client = NULL;
  int id_client_length = 0,counter = 0;
  sqlite3_stmt *res;
  sqlite3_prepare_v2(conn,"SELECT COUNT (*) FROM clients ;",-1,&res,0);
  if(sqlite3_step(res) != SQLITE_ROW)
  {
    printf("\t$$UNDEFINEDERROR$$\n\t$$%s$$\n",sqlite3_errmsg(conn));
    return UNDEFINEDERROR;
  }
  id_client_length = sqlite3_column_int(res,0);
  sqlite3_finalize(res);
  if(id_client_length == 0)
  {
    printf("\tclients table is empty!\n");
    return SUCCEED;
  }
  id_client = (int*)calloc(id_client_length,sizeof(int));
  if(id_client == NULL)
  {
    printf("\tHEAP OVER FLOW!\n");
    sqlite3_close(conn);
    exit(HEAPOVEFLOW);
  }
  sqlite3_prepare_v2(conn,"SELECT id_client FROM clients ORDER BY last_name ASC, first_name ASC;",-1,&res,0);
  while(sqlite3_step(res) ==SQLITE_ROW)
  {
    id_client[counter] = sqlite3_column_int(res,0);
    counter ++;
  }
  sqlite3_finalize(res);
  for(counter = 0;counter < id_client_length;counter ++)
  {
    if(libmysqlite3_print_clientPortfolio(conn,id_client[counter]) != SUCCEED)
    {
      printf("Undefined error!\n");
      sqlite3_close(conn);
      exit(UNDEFINEDERROR);
    }
  }
  free(id_client);
  return SUCCEED;
}
int libmysqlite3_find_singleClientPortfolio(sqlite3 *conn)
{
    int id_client = libmysqlite3_find_client(conn);
    if(id_client != 0)
    {
      libmysqlite3_print_clientPortfolio(conn,id_client);
    }
    return SUCCEED;
}
int libmysqlite3_print_clientProfit(sqlite3 *conn,int id_client)
{
  char buff[STRINGBUFF];
  sqlite3_stmt *res;
  sqlite3_prepare_v2(conn,"SELECT * FROM clients where id_client =?;",-1,&res,0);
  sqlite3_bind_int(res,1,id_client);
  if(sqlite3_step(res) != SQLITE_ROW)
  {
    printf("\t$$%s<$$\n",sqlite3_errmsg(conn));
    return INVALID_INPUT;
  }
  printf("%s %s (ClientID: %d) ",sqlite3_column_text(res,1),sqlite3_column_text(res,2),sqlite3_column_int(res,0));
  sqlite3_finalize(res);
  sprintf(buff, "WITH tempTBL AS (  SELECT p.date,d.id_client,d.quantity,p.id_stock, "
                                          "CASE d.is_buy 	WHEN 1 	THEN -p.buy_price*d.quantity "
																                                 "ELSE p.sell_price*d.quantity END AS spend_earn_by_stock_by_date "
					                         "FROM deals d "
					                         "LEFT JOIN prices p USING (id_price) "
					                         "WHERE d.id_client = %d "
					                         "ORDER BY p.date ASC ) "
                "SELECT id_client,sum(spend_earn_by_stock_by_date) as total_spend_earn "
                "FROM tempTBL; ",id_client);
  sqlite3_prepare_v2(conn,buff,-1,&res,0);
  sqlite3_step(res);
  if(sqlite3_column_double(res,1) == 0)
  {
      printf("does not have any profit!\n");
  }
  else if (sqlite3_column_double(res,1) > 0)
  {
      printf("earns %.2f €!\n",sqlite3_column_double(res,1));
  }
  else
  {
    printf("lost %.2f €!\n",-sqlite3_column_double(res,1));
  }
  sqlite3_finalize(res);
  return SUCCEED;
}
int libmysqlite3_find_singleClientProfit(sqlite3 *conn)
{
  int id_client = 0;
  id_client = libmysqlite3_find_client(conn);
  if(id_client != 0)
  {
    libmysqlite3_print_clientProfit(conn,id_client);
  }
  return SUCCEED;
}
int libmysqlite3_find_allClientProfit(sqlite3 *conn)
{
  sqlite3_stmt *res;
  sqlite3_prepare_v2(conn,"SELECT id_client FROM clients ORDER BY last_name ASC, first_name ASC;",-1,&res,0);
  while (sqlite3_step(res) == SQLITE_ROW)
  {
    libmysqlite3_print_clientProfit(conn,sqlite3_column_int(res,0));
  }
  sqlite3_finalize(res);
  return SUCCEED;
}

int libmysqlite3_find_dailyChange_stock(sqlite3 *conn)
{
  float prev_sell_price = 1, prev_buy_price = 1;
  int id_stock = 0,rc;
  sqlite3_stmt *res;
  id_stock = libmysqlite3_find_stock(conn);
  if(id_stock != 0)
  {
    sqlite3_prepare_v2 (conn,"SELECT id_stock, stock_name,s_short_name FROM stocks WHERE id_stock = ? ;",-1,&res,0);
    sqlite3_bind_int(res,1,id_stock);
    sqlite3_step(res);
    printf("History of price change of: %s ,ABRV:%s (id_stock:%d)\n",sqlite3_column_text(res,1),sqlite3_column_text(res,2),sqlite3_column_int(res,0));
    sqlite3_finalize(res);
    printf("   Date   |       \t  Sell  \t      |        Buy  \n");
    sqlite3_prepare_v2 (conn,"SELECT date, sell_price,buy_price FROM prices WHERE id_stock = ? ORDER BY date ;",-1,&res,0);
    sqlite3_bind_int(res,1,id_stock);
    rc = sqlite3_step(res) ;
    if(rc == SQLITE_DONE)
    {
      printf("Stock is not on air\n");
      sqlite3_finalize(res);
    }
    else
    {
      printf("%s|         Starting: %5.2f€          | Starting:%5.2f€ \n",sqlite3_column_text(res,0),sqlite3_column_double(res,1),sqlite3_column_double(res,2));
      prev_sell_price = sqlite3_column_double(res,1);
      prev_buy_price = sqlite3_column_double(res,2);
      rc = sqlite3_step(res);
      if(rc != SQLITE_ROW)
      {
        printf("No more update!\n");
      }
      else
      {
        while (rc == SQLITE_ROW)
        {
          printf("%s",sqlite3_column_text(res,0));
          libmysqlite3_print_change(sqlite3_column_double(res,1),prev_sell_price);
          libmysqlite3_print_change(sqlite3_column_double(res,2),prev_buy_price);
          prev_sell_price = sqlite3_column_double(res,1);
          prev_buy_price = sqlite3_column_double(res,2);
          puts("");
          rc = sqlite3_step(res);
        }
      }
      sqlite3_finalize(res);
    }
  }
  return SUCCEED;
}

void libmysqlite3_print_change(float todayPrice,float yesterdayPrice)
{
  if(todayPrice == yesterdayPrice )
  {
    printf("|         remains at %5.2f€         ",yesterdayPrice);
  }
  else if (todayPrice >yesterdayPrice)
  {
    printf("|%5.2f€ increases by %5.2f€(%6.2f%%)",todayPrice,todayPrice-yesterdayPrice,(todayPrice-yesterdayPrice)*100/yesterdayPrice);
  }
  else
  {
    printf("|%5.2f€ decreases by %5.2f€(%6.2f%%)",todayPrice,yesterdayPrice-todayPrice,(yesterdayPrice-todayPrice)*100/yesterdayPrice);
  }
}
