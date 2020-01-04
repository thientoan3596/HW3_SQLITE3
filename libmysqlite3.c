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

  sqlite3_prepare_v2 (conn,"SELECT * FROM clients WHERE id_client = ?;",-1,&res,0);
  sqlite3_bind_int(res,1,id_client_buff);
  if(sqlite3_step(res) != SQLITE_DONE )
  {
    printf("$$Error! id_client is already existed.$$\n");
    sqlite3_finalize(res);
    return INVALID_INPUT;
  }
  sqlite3_finalize(res);


  sqlite3_prepare_v2(conn,"INSERT INTO clients VALUES(?,?,?);",-1,&res,0);
  sqlite3_bind_int(res,1,id_client_buff);

  printf("\tEnter first_name:\n\t>");
  mygetline(static_string_buff, 1024);
  //fgets(static_string_buff, 1024,stdin);
  sqlite3_bind_text(res,2,static_string_buff,strlen(static_string_buff),SQLITE_TRANSIENT);

  printf("\tEnter last_name:\n\t>");
  mygetline(static_string_buff, 1024);
  //fgets(static_string_buff, 1024,stdin);
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
  sqlite3_stmt *res;


  printf("\tEnter id_deal:\n\t>");
  scanf("%d",&id_deal_buff);

  sqlite3_prepare_v2 (conn,"SELECT *  FROM deals WHERE id_deal IS ?;",-1,&res,0);
  sqlite3_bind_int(res,1,id_deal_buff);
  if(sqlite3_step(res) != SQLITE_DONE)
  {
    printf("\t$$Error! id_price already exists.$$\n");
    sqlite3_finalize(res);
    return INVALID_INPUT;
  }
  sqlite3_finalize(res);

  printf("\tEnter id_price:\n\t>");
  scanf("%d",&id_price_buff);
  sqlite3_prepare_v2 (conn,"SELECT *  FROM prices WHERE id_price IS ?;",-1,&res,0);
  sqlite3_bind_int(res,1,id_price_buff);
  if(sqlite3_step(res) == SQLITE_DONE)
  {
    printf("\t$$Error! id_stock does not exist.$$\n");
    sqlite3_finalize(res);
    return INVALID_INPUT;
  }
  sqlite3_finalize(res);

  printf("\tEnter id_client:\n\t>");
  scanf("%d",&id_client_buff);
  sqlite3_prepare_v2 (conn,"SELECT *  FROM clients WHERE id_client IS ?;",-1,&res,0);
  sqlite3_bind_int(res,1,id_client_buff);
  if(sqlite3_step(res) == SQLITE_DONE)
  {
    printf("\t$$Error! id_client does not exist.$$\n");
    sqlite3_finalize(res);
    return INVALID_INPUT;
  }
  sqlite3_finalize(res);


  // sqlite3_prepare_v2 (conn,"SELECT"
  //                               "NOT EXITS(SELECT * FROM deals WHERE id_deal IS ?)"
  //                           "AND"
  //                               "EXISTS(SELECT * FROM prices WHERE id_price IS ?"
  //                           "AND"
  //                               "EXISTS(SELECT * FROM clients WHERE id_client IS ?;",-1,&res,0);
  //
  // sqlite3_bind_int(res,2,id_price_buff);
  // sqlite3_bind_int(res,3,id_client_buff);
  // sqlite3_step(res);
  // if(!(sqlite3_column_int(res,0)))
  // {
  //   printf("\t$$Error! id_price already exists or id_stock/id_client does not exist.$$\n");
  //   sqlite3_finalize(res);
  //   return INVALID_INPUT;
  // }
  // sqlite3_finalize(res);

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
  // int rc = sqlite3_step(res);
  // printf("<%d>,",rc);
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
  sqlite3_prepare_v2 (conn, "SELECT * FROM prices WHERE id_price IS ?",-1,&res,0);
  sqlite3_bind_int(res,1,id_price_buff);
  if(sqlite3_step(res) != SQLITE_DONE)
  {
    printf("\t$$Error! id_price already exists.$$\n");
    sqlite3_finalize(res);
    return INVALID_INPUT;
  }sqlite3_finalize(res);

  printf("\tEnter id_stock:\n\t>");
  scanf("%d",&id_stock_buff);
  sqlite3_prepare_v2 (conn, "SELECT * FROM stocks WHERE id_stock IS ?",-1,&res,0);
  sqlite3_bind_int(res,1,id_stock_buff);
  if(sqlite3_step(res) == SQLITE_DONE)
  {
    printf("\t$$Error! id_stock does not exists.$$\n");
    sqlite3_finalize(res);
    return INVALID_INPUT;
  }sqlite3_finalize(res);




  // printf("\tEnter id_stock:\n\t>");
  // scanf("%d",&id_stock_buff);
  // sqlite3_prepare_v2 (conn, "SELECT"
  //                               "NOT EXITS(SELECT * FROM prices WHERE id_price IS ?)"
  //                           "AND"
  //                               "EXISTS(SELECT * FROM stocks WHERE id_stock IS ?;",-1,&res,0);
  // sqlite3_bind_int(res,1,id_price_buff);
  // sqlite3_bind_int(res,2,id_stock_buff);
  // sqlite3_step(res);
  // if(!(sqlite3_column_int(res,0)))
  // {
  //   printf("\t$$Error! id_price already exists or id_stock does not exist.$$\n");
  //   sqlite3_finalize(res);
  //   return INVALID_INPUT;
  // }
  // sqlite3_finalize(res);

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

  sqlite3_prepare_v2 (conn, "SELECT * FROM stocks WHERE id_stock IS ?;",-1,&res,0);
  sqlite3_bind_int(res,1,id_stock_buff);
  if(sqlite3_step(res)!=SQLITE_DONE)
  {
    printf("\t$$Error! id_stock already exists$$\n");
    sqlite3_finalize(res);
    return INVALID_INPUT;
  }
  sqlite3_finalize(res);


  sqlite3_prepare_v2 (conn, "SELECT 1 FROM markets WHERE id_market IS ?;",-1,&res,0);
  sqlite3_bind_int(res,1,id_market_buff);
  if(sqlite3_step(res) == SQLITE_DONE)
  {
    printf("\t$$Error! id_market does not exists$$\n");
    sqlite3_finalize(res);
    return INVALID_INPUT;
  }



  // int test = sqlite3_step(res);printf(">%d<>%d<",sqlite3_column_count(res),test);
  // if(!(sqlite3_column_int(res,0)))
  // {
  //   printf("\t$$Error! id_stock already exists or id_market does not exist.$$\n");
  //   sqlite3_finalize(res);
  //   return INVALID_INPUT;
  //}
  //sqlite3_reset(res);
  sqlite3_finalize(res);
  /* Does not group all binding functions in order to use only one string buffer */
  sqlite3_prepare_v2(conn,"INSERT INTO stocks VALUES(?,?,?,?);",-1,&res,0);
  sqlite3_bind_int(res,1,id_stock_buff);
  sqlite3_bind_int(res,2,id_market_buff);

  printf("\tEnter stock_name:\n\t>");

  mygetline(static_string_buff, 1024);
  //fgets(static_string_buff, 1024,stdin);
  sqlite3_bind_text(res,3,static_string_buff,strlen(static_string_buff),SQLITE_TRANSIENT);

  printf("\tEnter s_short_name:\n\t>");
  mygetline(static_string_buff, 1024);
  //fgets(static_string_buff, 1024,stdin);
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
  sqlite3_prepare_v2 (conn,"SELECT * FROM markets WHERE id_market = ?;",-1,&res,0);
  sqlite3_bind_int(res,1,id_market_buff);
  if(sqlite3_step(res) != SQLITE_DONE )
  {
    printf("\t$$Error! id_market is already existed.$$\n");
    sqlite3_finalize(res);
    return INVALID_INPUT;
  }
  sqlite3_finalize(res);

  sqlite3_prepare_v2(conn,"INSERT INTO markets VALUES(?,?,?);",-1,&res,0);
  sqlite3_bind_int(res,1,id_market_buff);

  printf("\tEnter market_name:\n\t>");

  mygetline(static_string_buff, 1024);
  //fgets(static_string_buff, 1024,stdin);
  sqlite3_bind_text(res,2,static_string_buff,strlen(static_string_buff),SQLITE_TRANSIENT);

  printf("\tEnter m_short_name:\n\t>");

  //fgets(static_string_buff, 1024,stdin);
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
