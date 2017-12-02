#ifndef SDB_ARRAY_H
#define SDB_ARRAY_H

#include <iostream>
#include <vector>

#include "common.h"
#include "db_tuple.h"

using namespace std;

namespace StaticDB {

    class array {

        private :
            db_tuple_var * data;
            int _size;
            int _memory;

        public :
            array() : data(nullptr) ,_size(0) , _memory(0){};
            array(int size) ;
            int size() const ;
			int memory() const ;

			db_TYPE get_type(int index) const ;
			bool is_null(int index) const ;

			bool append_null();
			bool append_bool(bool value);
			bool append_int(int64_t value);
			bool append_double(double value);
			bool append_string(const string& value);
			bool append_tuple(const tuple& value);
			bool append_array(const array& value);

			bool is_null_array() const
            {
                if(!data && !_size && !_memory)return true;
                return false;
            }

    };

}

#endif // SDB_ARRAY_C
