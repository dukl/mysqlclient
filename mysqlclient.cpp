#include <stdio.h>
#include <iostream>
#include <mysql.h>
#include <sstream>
#include <string>
using namespace std;
int main(int argc,char *argv[])
{
    MYSQL conn;
    int isOK;
    MYSQL_RES *res; 
    MYSQL_ROW column;
    mysql_init(&conn);
    //"root":数据库管理员 "123":root密码 "test":数据库的名字
    if(!mysql_real_connect(&conn, "localhost","witcomm","123456","witcomm",0,NULL,CLIENT_FOUND_ROWS)){
      cout << "connect error" << endl;
      return -1;
    }
    //- 查询数据库
    isOK = mysql_query(&conn, "select * from AuthenticationSubscription");
    if(isOK){
      printf("error\n");
      return -1;
    }
    printf("OK\n");
    res = mysql_store_result(&conn);
    int j = mysql_num_fields(res);
    char *str_field[32]; 
    for (int i = 0; i < j; i++){ 
      str_field[i] = mysql_fetch_field(res)->name;
    }
    for (int i = 0; i < j; i++){
      printf("%10s\t", str_field[i]);
      printf("\n");
    }
    while (column = mysql_fetch_row(res)){
      printf("%10s\t%10s\n", column[0], column[1]);
    }

    // - 插入数据
    string ueid = "460990000000001";
    istringstream istr (ueid);
    ostringstream ostr;
    long ueid_ = 0;
    for(int i=0; i<3; i++){
      string query = "select * from AuthenticationSubscription where ueid=";
      istr>>ueid_;
      ostr<<ueid_;
      //cout<<ostr.str()<<endl;
      ueid_++;
      res = NULL;
      query += "\'"+ostr.str()+"\'";
      cout<<"query: "<<query<<endl;
      isOK = mysql_query(&conn, query.c_str());
      if(isOK){
	cout<<"error  " << ostr.str()<<endl;
      }
      res = mysql_store_result(&conn);
      if(mysql_num_rows(res)<=0){
	cout<<"null  " << ostr.str()<<endl;
	string insert = "insert into AuthenticationSubscription (`ueid`,`authenticationMethod`,`encPermanentKey`,`authenticationManagementField`,`encOpcKey`,`supi`) values(\'"+ostr.str()+"\'"+",\'5G_AKA\',\'00112233445566778899AABBCCDDEEFF\',\'9001\',\'000102030405060708090A0B0C0D0E0F\',\'"+ostr.str()+"\'"+");";
        cout<<"query  "<<insert<<endl;
	if(!mysql_query(&conn, insert.c_str())){
          printf("insert successfully\n");
        }  
      }else{
        cout<<"exists, update ..."<<endl;
	string insert = "replace into AuthenticationSubscription (`ueid`,`authenticationMethod`,`encPermanentKey`,`authenticationManagementField`,`encOpcKey`,`supi`,`algorithmId`,`encTopcKey`,`sequenceNumber`) values(\'"+ostr.str()+"\'"+",\'5G_AKA\',\'00112233445566778899AABBCCDDEEFF\',\'9001\',\'000102030405060708090A0B0C0D0E0F\',\'"+ostr.str()+"\'"+",\'milenage\',\'Opc\',\'{\"sqn\": \"0000000017a0\", \"sqnScheme\": \"NON_TIME_BASED\", \"lastIndexes\": {\"ausf\": 0}}\');";
        cout<<"query  "<<insert<<endl;
	if(!mysql_query(&conn, insert.c_str())){
          printf("insert successfully\n");
        }  
      }
      ostr.str("");
    }
# if 0
    res = NULL;
    isOK = mysql_query(&conn, "select * from AuthenticationSubscription where ueid='460991111111111'");
    res = mysql_store_result(&conn);
    if(mysql_num_rows(res)<=0){
      printf("null\n");
      return -1;
    }
    if(mysql_query(&conn, "insert into AuthenticationSubscription (`ueid`,`authenticationMethod`,`encPermanentKey`) values ('460991111111111','5G_AKA','00112233445566778899AABBCCDDEEFF');")){
      printf("error\n");
      printf("Query failed (%s)\n", mysql_error(&conn));
      return -1;
    }
    printf("OK\n");
#endif
    mysql_close(&conn);
    return 0;
}
