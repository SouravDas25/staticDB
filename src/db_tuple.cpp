
#include "../include/db_tuple.h"



namespace StaticDB {

	using namespace std;

	tuple::tuple(int s_size, bool as_a) : data() , as_array(as_a)
	{
	    if(!s_size) return;
		data.reserve(s_size);
	}

	bool tuple::is_array() const
	{
	    return as_array;
	}

	int tuple::size() const
	{
		return data.size();
	}

	int tuple::memory() const
	{
	    return data.capacity();
	}

	bool tuple::has_key(int index) const
	{
	    if( empty() )return false;
	    if(is_array())return false;
	    if(index < 0 || index > size()-1 ) return false;
	    if( !data[index].key().empty() ) return true;
	    return false;
	}

	bool tuple::has_key(const string& key) const
	{
	    int i;
	    for(i=0;i<this->size();i++)
        {
            if(this->has_key(i))
            {
                if( stricmp( this->get_key(i) , key ) == 0)return true;
            }
        }
        return false;
	}

	const string& tuple::get_key(int index) const
	{
	    if( is_array() ) return empty_string;
	    if(has_key(index) == false) return empty_string;
	    return data[index].key();
	}

	int tuple::get_index(const string& key) const
	{
	    if(is_array())return npos;
		int i;
		for(i=0 ;i < size();i++)
		{
            if(key == data[i].key())
            {
                return i;
            }
		}
		return tuple::npos;
	}
/*
	bool tuple::set_key(const string& key,int index)
	{
	    if( is_array() ) return false;
	    if( key.size() < 1)return false;
		if(index < 0 || index > size()-1 ) return false;
        if( identifierValidity(key,true) == false ) return false;
		int pos = get_index(key);
        if( pos!= npos )return false;
		if( data[index].key() ) delete data[index].key;
		data[index].key = new string(key);
		return true;
	}
*/

/*
	bool tuple::del_key(const string& key,int index)
	{
	    if(is_array())return false;
		if( key.size() > 0 )
		{
			int i;
			for(i=0;i<size();i++)
			{
				if( key == *keylist[i])
				{
					delete keylist[index];
					keylist[i] = nullptr;
					return true;
				}
			}
		}
		if(index > 0 || index <= size()-1 )
        {
            delete keylist[index];
            keylist[index] = nullptr;
            return true;
        }
		return false;
	}
*/
	bool tuple::remove(int index)
	{
	    if(index< 0 || index >= size()) return false;
	    data.erase(data.begin()+index);
        return true;
	}

	const db_tuple_var& tuple::get(int index) const
	{
	    if( this->empty() ) return empty_var;
		if( index < 0 || index >= size() ) return empty_var;
		return data[index];
	}

	const db_tuple_var& tuple::get(const string& key) const
	{
	    if( this->empty() )return empty_var;
	    int index = get_index(key);
		if( index < 0 || index >= size() ) return empty_var;
		return data[index];
	}

	db_TYPE tuple::get_type(const string& key) const
	{
	    int index = get_index(key);
		if(index< 0 || index >= size()) return db_NULL;
		return data[index].type();
	}

	db_TYPE tuple::get_type(int index) const
	{
	    if( this->empty() ) return db_NULL;
		if( index < 0 || index >= size() ) return db_NULL;
		return data[index].type();
	}

	bool tuple::append_null(const string& key )
	{
		db_tuple_var dtv(nullptr,key);
	    return append(dtv);
	}

	bool tuple::is_null(int index) const
	{
		if(index < 0 || index >= size() ) return false;
		return data[index].is_null();
	}

	bool tuple::is_null(const string& key) const
	{
		int index = get_index(key);
		return is_null(index);
	}

	bool tuple::append_bool(bool value,const string& key)
	{
		db_tuple_var dtv(value,key);
	    return append(dtv);
	}

	bool tuple::get_bool(int index) const
	{
		if( index < 0 || index >= size() ) return 0;
		return data[index].get_bool();
	}

	bool tuple::get_bool(const string& key) const
	{
		int index = get_index(key);
		return get_bool(index);
	}

	bool tuple::append_int(int64_t value,const string& key)
	{
		db_tuple_var dtv(value,key);
	    return append(dtv);
	}

	int64_t tuple::get_int(const string& key) const
	{
		int index = get_index(key);
		return get_int(index);
	}

	int64_t tuple::get_int(int index) const
	{
		if(index < 0 || index >= size() ) return 0;
		return data[index].get_int();
	}

	bool tuple::append_double(double value,const string& key)
	{
		db_tuple_var dtv(value,key);
	    return append(dtv);
	}

	double tuple::get_double(int index) const
	{
		if(index < 0 || index >= size() ) return 0;
		return data[index].get_double();
	}

	double tuple::get_double(const string& key) const
	{
		int index = get_index(key);
		return get_double(index);
	}

	bool tuple::append_string(const string& value,const string& key)
	{
		data.emplace_back(value,key);
	    return true;
	}

	bool tuple::append_array(const tuple& value,const string& key)
	{
		data.emplace_back(value,key);
	    return true;
	}

	bool tuple::append_tuple(const tuple& value,const string& key)
	{
		data.emplace_back(value,key);
	    return true;
	}

	bool tuple::append_tuple_or_array(const tuple& value,const string& key)
	{
		data.emplace_back(value,key);
	    return true;
	}

	bool tuple::append(const db_tuple_var& value)
	{
	    data.emplace_back(value);
		return true;
	}

	const string tuple::get_string(int index) const
	{
		if( index < 0 || index >= size() ) return string();
		return data[index].get_string();
	}

	const string tuple::get_string(const string& key) const
	{
		int index = get_index(key);
		return get_string(index);
	}

	const tuple& tuple::get_tuple(int index) const
	{
		if( index < 0 || index >= size() ) return null_tuple;
		return data[index].get_tup_arr();
	}

	const tuple& tuple::get_tuple(const string& key) const
	{
	    int index = get_index(key);
		return get_tuple(index);
	}

	const tuple& tuple::get_tup_or_arr(int index) const
	{
	    if( index < 0 || index >= size() ) return null_tuple;
		return data[index].get_tup_arr();
	}

	const tuple& tuple::get_tup_or_arr(const string& key) const
	{
	    int index = get_index(key);
        return get_tup_or_arr(index);
	}

	const tuple& tuple::get_array(int index) const
	{
		if( index < 0 || index >= size() ) return null_tuple;
		return data[index].get_tup_arr();
	}

	const tuple& tuple::get_array(const string& key) const
	{
	    int index = get_index(key);
		return get_array(index);
	}

	bool tuple::print(ofstream& out ,const string& emps) const
	{
	    string spc = "    ";
		int i;
		out << "\n" << emps << ((!is_array())?"{":"[");
		for(i=0;i<size();i++)
		{
		    out << "\n" << emps + spc;
			if(has_key(i) && !is_array() ) out << "'" << data[i].key() << "' : ";
			switch(data[i].type())
			{
                case db_INT : {
                    out << data[i].get_int();
                    break;
                }
                case db_BOOL : {
                    (data[i].get_bool())? out << "true" :out << "false";
                    break;
                }
                case db_DOUBLE : {
                    out << data[i].get_double();
                    break;
                }
                case db_STRING : {
                    out << '"' << data[i].get_string() << '"';
                    break;
                }
                case db_ARRAY :
                case db_TUPLE : {
                    data[i].get_tup_arr().print(out,emps + spc);
                    break;
                }
                case db_NULL : {
                    out << "Null";
                    break;
                }
			}
			if(i<size()-1)out << " ," ;
		}
		out << "\n" << emps  << ((!is_array())?"}":"]");
		return true;
	}

	bool tuple::print(const string& emps) const
	{
	    string spc = "    ";
		int i;
		cout << "\n" << emps << ((!is_array())?" {":" [");
		for(i=0;i<size();i++)
		{
		    cout << "\n" << emps + spc;
			if( has_key(i) && !is_array()  ) cout << "'" << data[i].key() << "' : ";
			switch(data[i].type())
			{
                case db_INT : {
                    cout << data[i].get_int();
                    break;
                }
                case db_BOOL : {
                    (data[i].get_bool())? cout << "true" :cout << "false";
                    break;
                }
                case db_DOUBLE : {
                    cout << data[i].get_double();
                    break;
                }
                case db_STRING : {
                    cout << '"' << data[i].get_string() << '"';
                    break;
                }
                case db_ARRAY :
                case db_TUPLE : {
                    data[i].get_tup_arr().print(emps + spc);
                    break;
                }
                case db_NULL : {
                    cout << "Null";
                    break;
                }
			}
			if(i<size()-1)cout << " ," ;
		}
		cout << "\n" << emps  << ((!is_array())?" }":" ]");
		return true;
	}
/*
	void tuple::free_index(int i)
	{
	    if(i<0 || i >size()-1)return ;
	    if(data[i].type == db_STRING)
        {
            delete data[i].s;
        }
        if(data[i].type == db_TUPLE || data[i].type == db_ARRAY)
        {
            delete data[i].t;
        }
        if(keylist[i]) del_key(string(),i);
	}

	void delete_tuple(tuple& t)
	{
	    if(t.empty()) return;
	    int i;
		for(i=0;i<t.size();i++)
		{
			t.free_index(i);
		}
		free(t.data);
		free(t.keylist);
		t.data = nullptr;
		t.keylist = nullptr;
        t._len = 0;
        t._memory = 0;
	}

	void copy_tuple(tuple& t1,const tuple& t2)
	{
	    if(t2.empty())
        {
            if(!t1.empty()) delete_tuple(t1);
            return;
        }
	    if(!t1.empty()) delete_tuple(t1);
	    t1._len = t2._len;
	    t1._memory = t2._memory;
	    t1.as_array = t2.as_array;

	    t1.data = (db_tuple_var*)calloc(t2.size(),sizeof(db_tuple_var));
		if(!t1.data) throw sdb_error("Out Of Memory");

		t1.keylist = (string**)calloc(t2.size(),sizeof(string*));
		if(!t1.keylist) free(t1.data),throw sdb_error("Out Of Memory");

		int i;
		for( i = 0 ; i < t2.size() ; i++ )
		{
			copy_db_var(t1.data[i],t2.data[i]);
			if(t2.keylist[i]) t1.keylist[i] = new string(*t2.keylist[i]);
		}
	}

	tuple::tuple(const tuple& t) : data() , as_array(t.as_array)
	{
        data = t.data;
	}
*/
	const tuple& tuple::operator=(const tuple& t)
	{
        data = t.data;
        as_array = t.as_array;
		return *this;
	}

	tuple tuple::enclose_in(int i) const
    {
        if(this->empty()) return null_tuple;
        if( i < 0 || i > size()-1 ) return null_tuple;
        tuple nt = tuple(1);
        switch(get_type(i))
        {
            case db_INT: {
                nt.append_int( get_int(i) , get_key(i) );
                break;
            }
            case db_STRING: {
                nt.append_string( get_string(i) , get_key(i));
                break;
            }
            case db_DOUBLE: {
                nt.append_double( get_double(i), get_key(i) );
                break;
            }
            case db_NULL : {
                nt.append_null(get_key(i));
                break;
            }
            case db_BOOL : {
                nt.append_bool( get_bool(i) , get_key(i) );
                break;
            }
            case db_ARRAY : {
                nt.append_array( get_array(i) , get_key(i) );
                break;
            }
            case db_TUPLE : {
                nt.append_tuple( get_tuple(i) , get_key(i) );
                break;
            }
        }
        return nt;
    }

    const db_tuple_var tuple::to_var(const string& key) const
    {
        db_tuple_var dtv(*this,key) ;
        return dtv;
    }

    const db_tuple_var tuple::traverse(const tuple& kt,int start) const
    {
        if(kt.empty() || this->empty() ) return this->to_var();
        if( start < 0 || start > kt.size() )
        {
            return this->to_var();
        }
        if( start == kt.size() )
        {
            return this->to_var();
        }
        const tuple * lt = this, * plt = this;
        int i,ind = -1;
        //kt.print();
        for(i = start ; i < kt.size() ; i++)
        {
            ind = -1;
            if(lt == nullptr)
            {
                string s;
                if( kt.get_type(i) == db_INT )
                    s = string_sprintf("Tuple Key '%I64d' is Not a Tuple Or Array Error_no - 1.",kt.get_int(i-1));
                else s = string_sprintf("Tuple Key '%s' is Not a Tuple Or Array Error_no - 1.",kt.get_string(i-1).c_str());
                throw sdb_error(s);
            }
            if( kt.get_type(i) == db_INT ) ind = kt.get_int(i);
            else if ( kt.get_string(i) == "*" ) //find ['replies[*].user_id'] in scp.comments
            {
                //lt->print();
                tuple t(lt->size(),(kt.get_type(i) == db_INT)?true:false);
                for(int p = 0;p<lt->size();++p)
                {
                    const db_tuple_var dvt =lt->get_tup_or_arr(p).traverse(kt,i+1);
                    t.append(dvt);
                }
                //t.print();
                return t.to_var(kt.get_string(i-1));
            }
            else ind = lt->get_index(kt.get_string(i));

            plt = lt;
            if(ind < 0 || ind > lt->size() )
            {
                string s;
                if( kt.get_type(i) == db_INT )
                    s = string_sprintf("Tuple Key '%I64d' Not Found.",kt.get_int(i));
                else s = string_sprintf("Tuple Key '%s' Not Found.",kt.get_string(i).c_str());
                throw sdb_error(s);
            }
            if( (lt->get_type(ind) != db_TUPLE && lt->get_type(ind) != db_ARRAY) && i < kt.size() - 1 )
            {
                string s;
                if( kt.get_type(i) == db_INT )
                    s = string_sprintf("Tuple Key '%I64d' is Not a Tuple Or Array Error_no - 2.",kt.get_int(i));
                else s = string_sprintf("Tuple Key '%s' is Not a Tuple Or Array Error_no - 2.",kt.get_string(i).c_str());
                throw sdb_error(s);
            }
            lt = (lt->get_type(ind) == db_TUPLE || lt->get_type(ind) == db_ARRAY)?&lt->data[ind].get_tup_arr():nullptr;//db_tuplePrint(plt,1)
        }
        //plt->print();
        return plt->get(ind);
    }

    tuple splitKey(const string& key);

    const db_tuple_var tuple::traverse(const string& str,int start) const
    {
        tuple kt = splitKey(str);
        return traverse(kt,start);
    }



}

/*
int main()
{
    using StaticDB::tuple;
	try
	{
		StaticDB::tuple t = StaticDB::tupleLiteral("{'lol':1,name:2, tuple:{1,array : ['sourav' ,'Das',1,2,true], bool : true,null} }");
		t.print();
		tuple t2;
		t2.print();
		return 0;
	}
	catch(StaticDB::sdb_error& e)
	{
		cout << e.what() ;
	}

}*/


