




#include "include/marshal.h"
#include <iostream>


using namespace std;
using namespace StaticDB;

void printvoi(const vector<uint64_t>& v)
{
    std::cout << "[ ";
    for (size_t i=0; i<v.size();i++)
    {
        std::cout << v[i] << " , " ;
    }
    std::cout << " ]" << std::endl;
}

void printvos(const vector<string>& v)
{
    std::cout << "[ ";
    for (size_t i=0; i<v.size();i++)
    {
        std::cout << v[i] << " , " ;
    }
    std::cout << " ]" << std::endl;
}

void connect_DB(const string& DB_name, const string& pwd)
{
    Database db ;
    if( db.connect(DB_name,pwd) == SUCCESS_MSG )
    {
        string val;

        //cout << base_del_print(db.obj) << endl ;
        //cout << base_mem_detail(db.obj) ;

        //db.execute("import 'uim.txt' " );cout << get_NoSqlMsg() << endl ;
        //db.execute(" create kn = { lol : 1 , pol : [ 1,2,3 } }" );cout << endl<< get_NoSqlMsg() << endl ;

        while(1)
        {
            fflush(stdin);
            cout << "\n>>> ";
            val = multi_line_input();
            //cout << "\n str = '" << val << "'";
            if( val == "clear")
            {
                system ("CLS");
                cout << get_StartMsg();
                continue;
            }
            nosqlRT rt = db.execute(val);
            //printf("...\t");
            if( rt == SUCCESS_MSG || rt == ERROR_MSG )
            {
                cout << get_NoSqlMsg() ;
            }
            else if( rt == RETURN_TUPLE )
            {
                get_NoSqlReturnTuple().print();
            }
            else if(rt == QUIT_MSG)
            {
                 cout << get_NoSqlMsg() ;
                 break;
            }
            cout << "\n" ;
        }

    }
    else cout << get_NoSqlMsg() ;
    db.disconnect();
}

int main()
{
    cout << get_StartMsg();
    string dbname , pwd ;
    while(1)
    {
        cout << "\nEnter Database Name : ";
        dbname = multi_line_input();
        if(dbname == "quit") break;
        if(dbname.size() < 1)
        {
            dbname = "testdb.sdb";
            pwd = "passwrd";
            cout << "Connectiong To '" << dbname <<'\'' << endl;
            cout << "With password '" << pwd <<'\''<< endl;
        }
        else
        {
            cout << "Enter Password : ";
            pwd = multi_line_input();
            cout << "Connectiong To " << dbname << endl;
        }


        connect_DB(dbname,pwd);
    }
    return 0;
}
/*
int main()
{
    using StaticDB::tuple;
	try
	{
	    string s ;
	    //cout << "Enter A tuple : ";
	    //getline(cin ,s) ;
	    db_tuple_var d(12);
		StaticDB::tuple t ;
		t.append_int(10,"lol");
		//cout << t.size();
		t.print();
		tuple t2;
		t2.print();
		return 0;
	}
	catch(StaticDB::sdb_error& e)
	{
		cout << e.what() ;
	}

}
*/
