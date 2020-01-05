#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sqlite3.h>
#include "libmysqlite3.h"

// int libmysqlite3_findDBTables(sqlite3 *conn,char ***stringArr)
// {
//   char **returnStrArr = NULL;
//   int stringArrSize = 0;
//   sqlite3_stmt *res;
//   int returnCode = sqlite3_prepare(conn,"SELECT name FROM sqlite_master "
//                                         "WHERE type = 'table' "
//                                         "ANd name NOT LIKE 'sqlite_%';",-1,&res,0);
//   if(returnCode != SQLITE_OK)
//   {
//     printf("SQL Statement Error! (Code: %d)\n",returnCode);
//     exit(SQLSTMTERROR);
//   }
//   while(sqlite3_step(res)==SQLITE_ROW)
//   {
//     returnStrArr = (char **)realloc(returnStrArr,(stringArrSize+1)*sizeof(char*));
//     *(returnStrArr+stringArrSize) = (char*) calloc(sizeof(sqlite3_column_text(res,0))+1);
//     strcpy(*(returnStrArr+stringArrSize),sqlite3_column_text(res,0));
//     stringArrSize ++;
//   }
//
//   return stringArrSize;
// }
bool doesExist(sqlite3 *conn,char* tableName, char* columnName,char *value,int dataType)
{
  /* dataType : 1 - int, 2- char, 3 - float*/
  char SQL_STRING[1024];
  sqlite3_stmt *res;
  int returnVal;
  if (dataType != 1 && dataType != 2 && dataType != 3)
  {
    printf("$$INVALID DATATYPE$$\n");
    exit(INVALID_INPUT);
  }
  if(dataType == 1)
  {
    int val = atoi(value);
    sprintf(SQL_STRING, "SELECT COUNT (*) FROM %s WHERE %s = %d;", tableName, columnName, val);
    sqlite3_prepare_v2(conn,SQL_STRING,-1,&res,0);
    if(sqlite3_step(res)== SQLITE_ROW)
    {
      returnVal = sqlite3_column_int(res,0);
      sqlite3_finalize(res);
      return returnVal;
    }
    printf("$$ERROR does exist() Function parameter$$\n");
    exit(INVALID_INPUT);
  }
  else if (dataType == 3)
  {
    float val = atof(value);
    sprintf(SQL_STRING, "SELECT COUNT (*) FROM %s WHERE %s = %f;", tableName, columnName, val);
    sqlite3_prepare_v2(conn,SQL_STRING,-1,&res,0);
    if(sqlite3_step(res)== SQLITE_ROW)
    {
      returnVal = sqlite3_column_int(res,0);
      sqlite3_finalize(res);
      return returnVal;
    }
    printf("$$ERROR does exist() Function parameter$$\n");
    exit(INVALID_INPUT);
  }
  else
  {
    sprintf(SQL_STRING, "SELECT COUNT (*) FROM %s WHERE %s = '%s';", tableName, columnName, value);
    sqlite3_prepare_v2(conn,SQL_STRING,-1,&res,0);
    int rc =sqlite3_step(res);
    if(rc== SQLITE_ROW)
    {
      returnVal = sqlite3_column_int(res,0);
      sqlite3_finalize(res);
      return returnVal;
    }
    printf("$$ERROR: does exist() Function parameter$$\n");
    exit(INVALID_INPUT);
  }
}

static int mycallback(void *data, int argc, char **argv, char **azColName){
   int i;
   fprintf(stderr, "%s: ", (const char*)data);
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}

char *mygetline(char *str, int strlength)
{
  int i=0;
  do{
    if(i>1)
    {
      printf("$Input cannot be empty$\n>");
    }
	  fgets(str,strlength,stdin);
	  if(str[strlen(str)-1] == 10)
		{
			str[strlen(str)-1] = 0;
		}
    i++;
	}while(strlen(str)==0);
  return str;
}

bool isLeap(int YYYY)
{
  if (YYYY%400 == 0)
    return true;
  else if (YYYY%100 == 0)
    return false;
	else if (YYYY%4 == 0)
    return true;
	else
    return false;
}

int getDate(char **returnDate)
{
  char *date = (char*)calloc(DATASTRINGBUFF,sizeof(char));
  *returnDate = date;
  int YYYY,MM,DD;


  printf("\tDate format is <YYYY-MM-DD> according to ISO-8601 standard!\n\t\tYear:");
  scanf("%d",&YYYY);
  if(YYYY < 1)
  {
    puts("\t\t$$INVALID YEAR$$!");
    return INVALID_INPUT;
  }


  printf("\t\tMonth:");
  scanf("%d",&MM);
  if( MM >12 || MM <1 )
  {
    puts("\t\t$$INVALID MONTH$$!");
    return INVALID_INPUT;
  }


  printf("\t\tDate:");
  scanf("%d",&DD);
  if( !(DD >31 || DD <1) )
  {
    if( !((MM == 4 || MM == 6 || MM == 9 || MM == 11) && DD == 31))
    {
      if( !(MM == 2  && DD >29))
  		{
  			if( !(!isLeap(YYYY) && MM == 2 && DD >28))
  			{
  				sprintf(*returnDate,"%04d-%02d-%02d",YYYY,MM,DD);
  				return SUCCEED;
  			}
  		}
    }
  }
  puts("\t\t$$INVALID DATE$$!");
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
      puts("$$Invalid Option$$");
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

  sprintf(static_string_buff,"%d",id_client_buff);
  if(doesExist(conn,"clients","id_client",static_string_buff,1))
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

int libmysqlite3_addingRecord_deals(sqlite3 *conn)
{
  int id_deal_buff,id_client_buff,id_price_buff,quantity_buff,is_buy_buff;
  char static_string_buff[STRINGBUFF];
  sqlite3_stmt *res;


  printf("\tEnter id_deal:\n\t>");
  scanf("%d",&id_deal_buff);

  sprintf(static_string_buff,"%d",id_deal_buff);
  if(doesExist(conn,"deals","id_deal",static_string_buff,1))
  {
    printf("\t$$Error! id_deal already exists.$$\n");
    return INVALID_INPUT;
  }

  printf("\tEnter id_price:\n\t>");
  scanf("%d",&id_price_buff);
  sprintf(static_string_buff,"%d",id_price_buff);
  if(!doesExist(conn,"prices","id_price",static_string_buff,1))
  {
    printf("\t$$Error! id_deal does not exists.$$\n");
    return INVALID_INPUT;
  }


  printf("\tEnter id_client:\n\t>");
  scanf("%d",&id_client_buff);
  sprintf(static_string_buff,"%d",id_client_buff);
  if(!doesExist(conn,"clients","id_client",static_string_buff,1))
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
  int id_price_buff,id_stock_buff,option;
  float buy_price_buff,sell_price_buff;
  char static_string_buff[STRINGBUFF];
  sqlite3_stmt *res;

  printf("\tEnter id_price:\n\t>");
  scanf("%d",&id_price_buff);
  sprintf(static_string_buff,"%d",id_price_buff);
  if(doesExist(conn,"prices","id_price",static_string_buff,1))
  {
    printf("\t$$Error! id_price already exists.$$\n");
    return INVALID_INPUT;
  }

  printf("\tEnter id_stock:\n\t>");
  scanf("%d",&id_stock_buff);
  sprintf(static_string_buff,"%d",id_stock_buff);
  if(! doesExist(conn,"stocks","id_stock",static_string_buff,1))
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
  sqlite3_bind_text(res,5,date,DATASTRINGBUFF,SQLITE_STATIC);

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

  sprintf(static_string_buff,"%d",id_stock_buff);
  if( doesExist(conn,"stocks","id_stock",static_string_buff,1))
  {
    printf("\t$$Error! id_stock already exists$$\n");
    return INVALID_INPUT;
  }
  sprintf(static_string_buff,"%d",id_market_buff);
  if(! doesExist(conn,"markets","id_market",static_string_buff,1))
  {
    printf("\t$$Error! id_market does not exists$$\n");
    return INVALID_INPUT;
  }

  /* Does not group all binding functions in order to use only one string buffer ( SQLITE_TRANSIENT) */
  sqlite3_prepare_v2(conn,"INSERT INTO stocks VALUES(?,?,?,?);",-1,&res,0);
  sqlite3_bind_int(res,1,id_stock_buff);
  sqlite3_bind_int(res,2,id_market_buff);

  printf("\tEnter stock_name:\n\t>");
  mygetline(static_string_buff, 1024);
  sqlite3_bind_text(res,3,static_string_buff,strlen(static_string_buff),SQLITE_TRANSIENT);

  printf("\tEnter s_short_name:\n\t>");
  mygetline(static_string_buff, 1024);
  sqlite3_bind_text(res,4,static_string_buff,strlen(static_string_buff),SQLITE_TRANSIENT);

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
  sprintf(static_string_buff,"%d",id_market_buff);
  if( doesExist(conn,"markets","id_market",static_string_buff,1))
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
      //libmysqlite3_updateRecord_deals(conn,buff);
      break;
    case 3:
      printf("\tEnter id_price to update:\n\t>");
      scanf("%d",&buff);
      //libmysqlite3_updateRecord_prices(conn,buff);
      break;
    case 4:
      printf("\tEnter id_stock to update:\n\t>");
      scanf("%d",&buff);
      //libmysqlite3_updateRecord_stocks(conn,buff);
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
}
void libmysqlite3_updateRecord_clients(sqlite3 *conn,int id_client,bool restricted, int restrictedTo)
{
  int option;
  int new_val;
  char strbuff[STRINGBUFF];
  sqlite3_stmt *res;
  if(!restricted)
  {
    printf("\t\t(1-id_client) (2-first_name) (3-last_name)\n\t\t>");
    scanf("%d",&option);
  }
  else
  {
    option = restrictedTo;
  }

  switch (option) {
    case 1:
      printf("\t\tNew id_client:\n\t\t>");
      scanf("%d",&new_val);
      sprintf(strbuff,"%d",new_val);
      if(doesExist(conn,"clients","id_client",strbuff,1))
      {
        printf("\t\t$$New id_client = %d already exists$$\n",new_val);
      }
      else
      {
        sqlite3_prepare_v2(conn,"UPDATE clients SET id_client = ? WHERE id_client = ?;",-1,&res,0);
        sqlite3_bind_int(res,1,new_val);
        sqlite3_bind_int(res,2,id_client);
        if(sqlite3_step(res) != SQLITE_DONE)
        {
          printf("\t\t$$%s$$\n",sqlite3_errmsg(conn));
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
      printf("\t\tNew first_name:\n\t\t>");
      mygetline(strbuff,1024);
      sqlite3_prepare_v2(conn,"UPDATE clients SET first_name = ? WHERE id_client = ?;",-1,&res,0);
      sqlite3_bind_text(res,1,strbuff,strlen(strbuff),SQLITE_STATIC);
      sqlite3_bind_int(res,2,id_client);
      sqlite3_step(res) ;
      sqlite3_finalize(res);
      break;
    case 3:
      printf("\t\tNew last_name:\n\t\t>");
      mygetline(strbuff,1024);
      sqlite3_prepare_v2(conn,"UPDATE clients SET last_name = ? WHERE id_client = ?;",-1,&res,0);
      sqlite3_bind_text(res,1,strbuff,strlen(strbuff),SQLITE_STATIC);
      sqlite3_bind_int(res,2,id_client);
      sqlite3_step(res) ;
      sqlite3_finalize(res);
      break;
    default :
      puts("\t\t$$Invalid Option$$");
      break;
  }
}
void libmysqlite3_updateRecord_deals(sqlite3 *conn,int id_deal,bool restricted,int restrictedTo)
{
  int option;
  int new_val;
  char strbuff[STRINGBUFF];
  sqlite3_stmt *res;
  printf("\t\t(1-id_deal) (2-id_client) (3-id_price) (4-quantity) (5-is_buy)\n\t\t>");
  scanf("%d",&option);
  switch (option) {
    case 1:
      printf("\t\tNew id_deal:\n\t\t>");
      scanf("%d",&new_val);
      sprintf(strbuff,"%d",new_val);
      if( doesExist(conn,"deals","id_deal",strbuff,1))
      {
        printf("\t\t$$New id_deal = %d already exist$$\n",new_val);
      }
      else
      {
        sqlite3_prepare_v2(conn,"UPDATE deals SET id_deal = ? WHERE id_deal = ?;",-1,&res,0);
        sqlite3_bind_int(res,1,new_val);
        sqlite3_bind_int(res,2,id_deal);
        if(sqlite3_step(res) != SQLITE_DONE)
        {
          printf("\t\t$$%s$$\n",sqlite3_errmsg(conn));
        }
        sqlite3_finalize(res);
      }
      break;
    case 2:
      sqlite3_prepare_v2(conn,"SELECT id_client FROM deals WHERE id_deal = ?;",-1,&res,0);
      sqlite3_bind_int(res,1,id_deal);
      if (sqlite3_step(res) != SQLITE_ROW)
      {
        printf("\t\t$$%s$$",sqlite3_errmsg(conn));
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
        printf("\t\t$$%s$$\n",sqlite3_errmsg(conn));
      }
      else
      {
        libmysqlite3_updateRecord_prices(conn,sqlite3_column_int(res,0),true,1);
      }
      sqlite3_finalize(res);
      break;
    case 4:
      printf("\t\tNew quantity:\n\t\t>");
      scanf("%d",&new_val);
      if(new_val < 1)
      {
        printf("\t\t$$UPDATE FAIL: NEGATIVE VALUE\n$$");
      }
      else
      {
        sqlite3_prepare_v2(conn,"UPDATE deals SET quantity = ? WHERE id_deal = ?;",-1,&res,0);
        sqlite3_bind_int(res,1,new_val);
        sqlite3_bind_int(res,2,id_deal);
        if(sqlite3_step(res) != SQLITE_DONE)
        {
          printf("\t\t$$%s$$\n",sqlite3_errmsg(conn));
        }
        sqlite3_finalize(res);
      }
      break;
    case 5:
      printf("\t\tNew is_buy (0-Sell) (1-Buy):\n\t\t>");
      scanf("%d",&new_val);
      if(new_val != 0  && new_val != 1)
      {
        printf("\t\t$$UPDATE FAIL: INVALID VALUE\n$$");
      }
      else
      {
        sqlite3_prepare_v2(conn,"UPDATE deals SET is_buy = ? WHERE id_deal = ?;",-1,&res,0);
        sqlite3_bind_int(res,1,new_val);
        sqlite3_bind_int(res,2,id_deal);
        if(sqlite3_step(res) != SQLITE_DONE)
        {
          printf("\t\t$$%s$$\n",sqlite3_errmsg(conn));
        }
        sqlite3_finalize(res);
      }
      break;
    default :
      puts("\t\t$$Invalid Option$$");
      break;
  }
}
void libmysqlite3_updateRecord_prices(sqlite3 *conn,int id_price,bool restricted,int restrictedTo)
{
  int option, new_val;
  char strbuff[STRINGBUFF];
  sqlite3_stmt *res;
  if(!restricted)
  {
    printf("\t\t(1-id_price) (2-id_stock) (3-buy_price) (4-sell_price) (5-date)\n\t\t>");
    scanf("%d",&option);
  }
  else
  {
    option = restrictedTo;
  }
  switch (option) {
    case 1:
      printf("\t\tNew id_price:\n\t\t>");
      scanf("%d",&new_val);
      sprintf(strbuff,"%d",new_val);
      if( doesExist(conn,"prices","id_price",strbuff,1))
      {
        printf("\t\t$$New id_price = %d already exists$$\n",new_val);
      }
      else
      {
        sqlite3_prepare_v2(conn,"UPDATE prices SET id_price = ? WHERE id_price = ?;",-1,&res,0);
        sqlite3_bind_int(res,1,new_val);
        sqlite3_bind_int(res,2,id_price);
        if(sqlite3_step(res) != SQLITE_DONE)
        {
          printf("\t\t$$%s$$\n",sqlite3_errmsg(conn));
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
        printf("\t\t$$%s$$\n",sqlite3_errmsg(conn));
      }
      else
      {
        libmysqlite3_updateRecord_stocks(conn,sqlite3_column_int(res,0),true,1);
      }
      sqlite3_finalize(res);
      break;
    case 3:
      printf("\t\tNew buy_price:\n\t\t>");
      scanf("%d",&new_val);
      sqlite3_prepare_v2(conn,"UPDATE prices SET buy_price = ? WHERE id_price = ?;",-1,&res,0);
      sqlite3_bind_int(res,1,new_val);
      sqlite3_bind_int(res,2,id_price);
      sqlite3_step(res);
      sqlite3_finalize(res);
      break;
    case 4:
      printf("\t\tNew sell_price:\n\t\t>");
      scanf("%d",&new_val);
      sqlite3_prepare_v2(conn,"UPDATE prices SET sell_price = ? WHERE id_price = ?;",-1,&res,0);
      sqlite3_bind_int(res,1,new_val);
      sqlite3_bind_int(res,2,id_price);
      sqlite3_step(res);
      sqlite3_finalize(res);
      break;
    case 5:
      printf("\t\tNew date:\n");
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
      puts("\t\t$$Invalid Option$$");
      break;
  }
}
void libmysqlite3_updateRecord_stocks(sqlite3 *conn,int id_stock,bool restricted,int restrictedTo)
{
  int option, new_val;
  char strbuff[STRINGBUFF];
  sqlite3_stmt *res;
  if(!restricted)
  {
    printf("\t\t(1-id_stock) (2-id_market) (3-stock_name) (4-s_short_name)\n\t\t>");
    scanf("%d",&option);
  }
  else
  {
    option = restrictedTo;
  }
  switch (option) {
    case 1:
      printf("\t\tNew id_stock:\n\t\t>");
      scanf("%d",&new_val);
      sprintf(strbuff,"%d",new_val);
      if(doesExist(conn,"stocks","id_stock",strbuff,1))
      {
        printf("\t\t$$New id_stock = %d already exists$$\n",new_val);
      }
      else
      {
        sqlite3_prepare_v2(conn,"UPDATE stocks SET id_stock = ? WHERE id_stock = ?;",-1,&res,0);
        sqlite3_bind_int(res,1,new_val);
        sqlite3_bind_int(res,2,id_stock);
        if(sqlite3_step(res) != SQLITE_DONE)
        {
          printf("\t\t$$%s$$\n",sqlite3_errmsg(conn));
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
        printf("\t\t$$%s$$",sqlite3_errmsg(conn));
      }
      else
      {
        libmysqlite3_updateRecord_markets(conn,sqlite3_column_int(res,1),true,1);
      }
      sqlite3_finalize(res);
      break;
    case 3:
      printf("\t\tNew stock_name:\n\t\t>");
      mygetline(strbuff,STRINGBUFF);
      sqlite3_prepare_v2(conn,"UPDATE stocks SET stock_name = ? WHERE stock_id = ?;",-1,&res,0);
      sqlite3_bind_text(res,1,strbuff,-1,SQLITE_STATIC);
      sqlite3_bind_int(res,2,id_stock);
      sqlite3_step(res);
      sqlite3_finalize(res);
      break;
    case 4:
      printf("\t\tNew s_short_name:\n\t\t>");
      mygetline(strbuff,STRINGBUFF);
      sqlite3_prepare_v2(conn,"UPDATE stocks SET s_short_name = ? WHERE stock_id = ?;",-1,&res,0);
      sqlite3_bind_text(res,1,strbuff,-1,SQLITE_STATIC);
      sqlite3_bind_int(res,2,id_stock);
      sqlite3_step(res);
      sqlite3_finalize(res);
      break;
    default:
      puts("\t\t$$Invalid Option$$");
      break;
  }
}
void libmysqlite3_updateRecord_markets(sqlite3 *conn, int id_market,bool restricted,int restrictedTo)
{
  int option, new_val;
  char strbuff[STRINGBUFF];
  sqlite3_stmt *res;
  if(!restricted)
  {
    printf("\t\t(1-id_market) (2-market_name) (3-m_short_name)\n\t\t>");
    scanf("%d",&option);
  }
  else
  {
    option = restrictedTo;
  }
  switch (option) {
    case 1:
      printf("\t\tNew id_market:\n\t\t>");
      scanf("%d",&new_val);
      sprintf(strbuff,"%d",new_val);
      if(doesExist(conn,"markets","id_market",strbuff,1))
      {
        printf("\t\t$$New id_market = %d already exists$$\n",new_val);
      }
      else
      {
        sqlite3_prepare_v2(conn,"UPDATE markets SET id_market = ? WHERE id_market = ?;",-1,&res,0);
        sqlite3_bind_int(res,1,new_val);
        sqlite3_bind_int(res,2,id_market);
        if(sqlite3_step(res) != SQLITE_DONE)
        {
          printf("\t\t$$%s$$\n",sqlite3_errmsg(conn));
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
      printf("\t\tNew market_name:\n\t\t>");
      mygetline(strbuff,1024);
      sqlite3_prepare_v2(conn,"UPDATE markets SET market_name = ? WHERE id_market = ?;",-1,&res,0);
      sqlite3_bind_text(res,1,strbuff,-1,SQLITE_STATIC);
      sqlite3_bind_int(res,2,id_market);
      sqlite3_step(res);
      sqlite3_finalize(res);
      break;
    case 3:
      printf("\t\tNew m_short_name:\n\t\t>");
      mygetline(strbuff,1024);
      sqlite3_prepare_v2(conn,"UPDATE markets SET m_short_name = ? WHERE id_market = ?;",-1,&res,0);
      sqlite3_bind_text(res,1,strbuff,-1,SQLITE_STATIC);
      sqlite3_bind_int(res,2,id_market);
      sqlite3_step(res);
      sqlite3_finalize(res);
      break;
    default:
      puts("\t\t$$Invalid Option$$");
      break;
  }
}
