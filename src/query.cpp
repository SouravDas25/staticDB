#ifndef staticDATABASE_QUERY_C
#define staticDATABASE_QUERY_C

#include "../include/query.h"

namespace StaticDB {

    /*struct query_intem {
        tuple projection;
        string key;
        tuple selection;
    };

    nosqlRT get_SLSQL(vector<string>& cmd)
    {
        string * fnd = find (cmd.begin(), cmd.end(), "FIND");
        if (fnd == cmd.end() ) return noSql_Msg(ERROR_MSG,"Not A Valid Query Command.");
        string * from = find (fnd , cmd.end(), "FROM");
        if (from == cmd.end() ) return noSql_Msg(ERROR_MSG,"Not A Valid Query Command.");
        tuple selection = tuple(from-fnd,true);
    }*/

    template<class T> bool are_almost_equal(T x, T y, int ulp = 0.00000001)
    {
        if( std::abs(x-y) <= std::numeric_limits<T>::epsilon() * std::max(std::abs(x), std::abs(y)) * ulp ){
            return true;
        }else{
            return false;
        }
    }

    static int db_tuple_cmp(const db_tuple_var& data1,const db_tuple_var& data2)
    {
        bool int_dubl = ( (data1.type() == db_INT && data2.type() == db_DOUBLE)  || (data1.type() == db_DOUBLE && data2.type() == db_INT));
        if( data2.type() != data1.type() && !int_dubl ) return 2;
        switch(data1.type() )
        {
            case db_BOOL : {
                if( data1.get_bool() == data2.get_bool() ) return 0;
                if( data1.get_bool() <  data2.get_bool() ) return -1;
                if( data1.get_bool() >  data2.get_bool() ) return 1;
            }
            case db_INT : {
                int64_t r;
                if( data2.type() == db_DOUBLE ) r = data2.get_double();
                else r = data2.get_int();
                if(data1.get_int() == r) return 0;
                if(data1.get_int() < r ) return -1;
                if(data1.get_int() > r ) return 1;
            }
            case db_DOUBLE : {
                double r;
                if( data2.type() == db_INT ) r = data2.get_int();
                else r = data2.get_double();
                if( are_almost_equal(data1.get_double(),r) ) return 0;
                if(data1.get_double() < r ) return -1;
                if(data1.get_double() > r ) return 1;
            }
            case db_STRING : {
                if( data1.get_string() == data2.get_string() ) return 0;
                if( data1.get_string() < data2.get_string() ) return -1;
                if( data1.get_string() > data2.get_string() ) return 1;
            }
            case db_NULL : {
                if( data2.is_null() && data1.is_null() ) return 0;
                return 1;
            }
            case db_ARRAY :
            case db_TUPLE : {
                return false;
            }
        }
        return 2;
    }

    static bool eliminate_condition(const db_tuple_var& s_srt,const tuple& con1)
    {
        int i;bool ret = true;
        const tuple * contmp;
        if( con1.has_key(0) && stricmp(con1.get_key(0),"$not") == 0 )
        {
            ret = false;
            if( con1.get_type(0) == db_ARRAY || con1.get_type(0) == db_TUPLE )contmp = &con1.get_tup_or_arr(0);
            else return db_tuple_cmp(s_srt,con1.get(0)) != 0;
        }
        else contmp = &con1;
        const tuple& con = *contmp;
        for(i = 0 ; i < con.size();i++)
        {
            if( con.has_key(i) )
            {
                if( stricmp(con.get_key(i),"$lt") == 0  )
                {
                    if( db_tuple_cmp(s_srt,con.get(i))  == -1 ) return ret;
                }
                else if( stricmp(con.get_key(i),"$gt") == 0  )
                {
                    if( db_tuple_cmp(s_srt,con.get(i))  == 1 ) return ret;
                }
                else if( stricmp(con.get_key(i),"$eq") == 0  )
                {
                    if( db_tuple_cmp(s_srt,con.get(i)) == 0 ) return ret;
                }
            }
            else
            {
                if( db_tuple_cmp(s_srt,con.get(i)) == 0 ) return ret;
            }
        }
        return !ret;
    }

    bool is_sel_match(staticDB * db,const db_tuple_var& srt,const tuple& sel,int match)
    {
        if( !srt.is_container() ) throw sdb_error("Data Given To FIND Command Is Not A Container Thus Cannot Be Queried.");
        if( srt.type() != db_ARRAY && srt.type() != db_TUPLE ) return false;
        if( sel.has_key(match) && stricmp(sel.get_key(match),"$or" ) == 0)
        {
            if( sel.get_type(match) == db_TUPLE || sel.get_type(match) == db_ARRAY )
            {
                return selection_OR(db,srt,sel.get_tup_or_arr(match));
            }
            return false;
        }
        else if( !sel.has_key(match) )
        {
            if( sel.get_type(match) == db_TUPLE || sel.get_type(match) == db_ARRAY )
            {
                return selection_AND(db,srt,sel.get_tup_or_arr(match));
            }
            return false;
        }
        tuple kt = splitKey(sel.get_key(match));
        const db_tuple_var& s_srt = srt.get_tup_arr().traverse(kt,0);
        if( ! s_srt.is_empty() )
        {
            if( sel.get_type(match) == db_ARRAY || sel.get_type(match) == db_TUPLE)
            {
                return eliminate_condition(s_srt,sel.get_tup_or_arr(match));
            }
            if( db_tuple_cmp(s_srt,sel.get(match)) == 0 ) return true;
            return false;
        }
        return false;
    }

    bool selection_OR(staticDB * db,const db_tuple_var& srt,const tuple& sel)
    {
        if( !srt.is_container() ) throw sdb_error("Data Given To FIND Command Is Not A Container Thus Cannot Be Queried.");
        if( sel.size() < 1) return true;
        for(int i=0;i < sel.size();i++)
        {
            if(is_sel_match(db,srt,sel,i))
            {
                return true;
            }
        }
        return false;
    }

    bool selection_AND(staticDB * db,const db_tuple_var& srt,const tuple& sel)
    {
        if( !srt.is_container() ) throw sdb_error("Data Given To FIND Command Is Not A Container Thus Cannot Be Queried.");
        if( sel.size() < 1) return true;
        bool ints = true;
        for(int i=0;i < sel.size();i++)
        {
            if(!is_sel_match(db,srt,sel,i))
            {
                ints = false;
                break;
            }
        }
        return ints;
    }

    static tuple projection(const tuple& pro,const db_tuple_var& psrt)
    {
        if( !pro.size()  )
        {
            if( psrt.type() == db_ARRAY || psrt.type() == db_TUPLE )
            {
                return psrt.get_tup_arr();
            }
            tuple t(1);
            t.append(psrt);
            return t;
        }
        //psrt.get_tup_arr().print();
        tuple t(pro.size());
        for( int i = 0;i < pro.size() ;++i)
        {
            if( pro.get_type(i) != db_STRING ) continue;
            tuple kt = splitKey(pro.get_string(i));
            const db_tuple_var& srt = psrt.get_tup_arr().traverse(kt,0);
            if( srt.is_empty() ) throw sdb_error("Projection Key '" + pro.get_string(i) + "' Not Found.");
            t.append(srt);
        }
        return t;
    }

    static nosqlRT selection(staticDB * db,const tuple& projt,const tuple& sel,const tuple& data)
    {
        int len = data.size();
        tuple t = tuple(len,true);//data.print(empty_string)
        for(int p=0;p<len;p++)
        {
            if( selection_AND(db,data.get(p),sel) )
            {
                tuple pie = projection(projt,data.get(p));
                if( !pie.empty() ) t.append_tuple( pie );
            }
        }
        return noSql_ReturnTuple(t);
    }

    nosqlRT cmd_find(staticDB * db,const tuple& projt,const string& identifier,const tuple& sel)
    {
        if( identifier.size() <= 0) return noSql_Msg(ERROR_MSG,"Not A Valid Query.");

        tuple data ;
        nosqlRT r;
        if ( identifier.find('(') != string::npos )
        {
            if ( identifier.rfind(')') != string::npos )
            {
                string key(identifier);
                key.erase(key.find('('),1);
                key.erase(key.rfind(')'),1);
                r = sdb_parse(db,key);
            }
            else throw sdb_error("Not A Valid Find Command Parenthesis Mismatch.");
        }
        else
        {
            r = pair_get(db,identifier);
        }
        if(r == RETURN_TUPLE)
        {
            data = get_NoSqlReturnTuple();
            if( data.empty() )
            {
                throw sdb_error("Inner Query : "+identifier+" Did Not Return Any Value.");
            }
        }
        else if (r == ERROR_MSG)
        {
            throw sdb_error(get_NoSqlMsg());
        }
        else
        {
            throw sdb_error("Inner Query : "+identifier+" Did Not Return Any Value.");
        }
        //cout << "data " << identifier ;
        //data.print();
        return selection(db,projt,sel,data);
    }

}

#endif
