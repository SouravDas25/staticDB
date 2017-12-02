from libcpp.string cimport string as string
from libcpp cimport bool
from libc.stdint cimport *

cdef extern from "db_tuple.h" namespace "StaticDB":
    const string empty_string

    enum db_TYPE :
        db_NULL = 0
        db_INT = 1
        db_STRING =2 
        db_DOUBLE =3
        db_TUPLE =4 
        db_BOOL = 5
        db_ARRAY =6

    enum nosqlRT :
        SUCCESS_MSG = 0 
        QUIT_MSG = 1
        ERROR_MSG = 2
        RETURN_TUPLE = 3

    cdef cppclass tuple:
        bool is_array() const
        int size() const 
        int memory() const 
        bool has_key(int index) const 
        const string get_key(int index) const 
        int get_index(const string& key) const 
        bool set_key(const string& key,int index)
        bool del_key(const string& key,int index)
        db_TYPE get_type(const string& key) const 
        db_TYPE get_type(int index) const 
        bool is_null(const string& key) const
        bool is_null(int index) const 

        bool append_null(const string& key = empty_string )
        bool append_bool(bool value,const string& key = empty_string)
        bool append_int(int64_t value,const string& key = empty_string)
        bool append_double(double value,const string& key = empty_string)
        bool append_string(const string& value,const string& key = empty_string)
        bool append_tuple(const tuple& value,const string& key = empty_string)
        bool append_array(const tuple& value,const string& key = empty_string)
        bool append_tuple_or_array(const tuple& value,const string& key = empty_string)

        bool get_bool(const string& key) const 
        bool get_bool(int index) const 
        int64_t get_int(const string& key) const 
        int64_t get_int(int index) const 
        double get_double(int index) const 
        double get_double(const string& key) const 
        const string get_string(int index) const 
        const string get_string(const string& key) const 
        const tuple& get_tuple(int index) const 
        const tuple& get_tuple(const string& key) const 
        const tuple& get_array(int index) const
        const tuple& get_array(const string& key) const 
        const tuple& get_tup_or_arr(int index) const
        const tuple& get_tup_or_arr(const string& key) const

        bool remove(int index)

        bool pretty_print(const string& emps ) const 
        const tuple& operator=(const tuple& t) 
    
    const tuple& get_NoSqlReturnTuple()
    const string& get_NoSqlMsg()
    const string& get_StartMsg()
