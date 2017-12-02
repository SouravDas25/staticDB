
#ifndef _SATATICTABLE_DB_C
#define _SATATICTABLE_DB_C

#include "../include/table.h"

using namespace std;

namespace StaticDB {

    nosqlRT createTable(staticDB * db,const string& tablename,const tuple& t)
    {
        if(!db)
        {
            return noSql_Msg(ERROR_MSG,"Database Not Valid | Not Connected");
        }
        if( identifierValidity(tablename.c_str(),tablename.length()) == false)
        {
            return noSql_Msg(ERROR_MSG,"Table Name Not Valid.");
        }
        if( pairExists(db,tablename) == true )
        {
            return noSql_Msg(ERROR_MSG,"Table Name Already Exists.");
        }
        bool b = false;
        for(int i=0;i<t.size();i++)
        {
            if( t.get_type(i) == db_TUPLE )
            {
                b = true;
                break;
            }
        }
        tuple st(1,true);
        if(!b)
        {
            st.append_tuple(t);
        }
        else st = t;
        //st.print();
        nosqlRT r = pairCreate(db,tablename,st);
        if( r == SUCCESS_MSG) return noSql_Msg(SUCCESS_MSG,"New Table Created.");
        return r;
    }

    nosqlRT tableInsert(staticDB * db,const string& tablename,const tuple& t)
    {
        if(!db)
        {
            return noSql_Msg(ERROR_MSG,"Database Not Valid | Not Connected");
        }
        sdb_pair * p = getPairNode(db,tablename);
        if(!p)
        {
            return noSql_Msg(ERROR_MSG,"Table Not Present In Database.");
        }

        tuple kt = splitKey(tablename);
        tuple st = tuple(1,true);
        st.append_tuple(t);
        nosqlRT r = pairInsert(db,kt,st);
        if(r == SUCCESS_MSG) return noSql_Msg(SUCCESS_MSG,"New Table Row Inserted.");
        return r;
    }

}

#endif // _SATATICTABLE_DB_C
