#ifndef SDB_ARRAY_C
#define SDB_ARRAY_C

#include "../include/array.h"

using namespace std;

namespace StaticDB {
/*
    array::array(int size) : data(nullptr) , _size(0) , _memory(size)
    {
        if(!size)
        {
            _memory = 0;
            return ;
        }
        data = new db_tuple_var[size];
        //if(!data) throw memory_error();
        memset(data,0,sizeof(db_tuple_var)*size);
    }

    int array::size() const
	{
		return _size;
	}

	int array::memory() const
	{
	    return _memory;
	}

	db_TYPE array::get_type(int index) const
	{
	    if( this->is_null_array() )return db_NULL;
		if( index < 0 || index >= size() ) return db_NULL;
		return data[index].type;
	}

	bool array::is_null(int index) const
	{
		if(index < 0 || index >= size() ) return false;
		if( data[index].type != db_NULL) return true;
		return false;
	}

	bool array::append_null()
	{
	    if( this->is_null_array() )return false;
		if( size() == memory() ) return false;
		data[size()].type = db_NULL;
		_size++;
		return true;
	}

	bool array::append_bool(bool value)
	{
		if( size() == memory() )  return false;
		data[size()].type = db_BOOL;
		data[size()].b = value;
		_size++;
		return true;
	}

	bool array::append_int(int64_t value)
	{
		if( size() == memory() )  return false;
		data[size()].type = db_INT;
		data[size()].i = value;
		_size++;
		return true;
	}

	bool array::append_double(double value)
	{
		if( size() == memory() ) return false;
		data[ size()].type = db_DOUBLE;
		data[ size()].d = value;
		_size++;
		return true;
	}

	bool array::append_string(const string& value)
	{
		if( size() == memory() ) return false;
		data[size()].type = db_STRING;
		data[size()].s = new string(value);
		_size++;
		return true;
	}

	bool array::append_tuple(const tuple& value)
	{
		if( size() == memory() ) return false;
		data[size()].type = db_TUPLE;
		data[size()].t = new tuple(value);
		_size++;
		return true;
	}

	bool array::append_array(const array& value)
	{
		if( size() == memory() ) return false;
		data[size()].type = db_ARRAY;
		data[size()].a = new array(value);
		_size++;
		return true;
	}
*/
}

#endif // SDB_ARRAY_C
