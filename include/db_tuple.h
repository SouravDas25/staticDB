#ifndef DB_TUPLE_H
#define DB_TUPLE_H

#include "common.h"

using namespace std;

namespace StaticDB {

    class db_tuple_var;

    const static string empty_string;

    class tuple {

    private :

        vector <db_tuple_var> data;
        bool as_array;

        void free_index(int i);

    public :
        tuple() : data(), as_array(false) {};

        tuple(int size, bool as_array = false);

        //tuple(const tuple& t);
        //~tuple() ;
        bool is_array() const;

        int size() const;

        int memory() const;

        bool has_key(int index) const;

        bool has_key(const string &key) const;

        const string &get_key(int index) const;

        int get_index(const string &key) const;

        //bool set_key(const string& key,int index);
        //bool del_key(const string& key,int index = -1);
        db_TYPE get_type(const string &key) const;

        db_TYPE get_type(int index) const;

        bool is_null(const string &key) const;

        bool is_null(int index) const;

        bool append(const db_tuple_var &value);

        bool append_null(const string &key = empty_string); //append Null
        bool append_bool(bool value, const string &key = empty_string);

        bool append_int(int64_t value, const string &key = empty_string);

        bool append_double(double value, const string &key = empty_string);

        bool append_string(const string &value, const string &key = empty_string);

        bool append_tuple(const tuple &value, const string &key = empty_string);

        bool append_array(const tuple &value, const string &key = empty_string);

        bool append_tuple_or_array(const tuple &value, const string &key = empty_string);

        const db_tuple_var &get(int index) const;

        const db_tuple_var &get(const string &key) const;

        bool get_bool(const string &key) const;

        bool get_bool(int index) const;

        int64_t get_int(const string &key) const;

        int64_t get_int(int index) const;

        double get_double(int index) const;

        double get_double(const string &key) const;

        const string get_string(int index) const;

        const string get_string(const string &key) const;

        const tuple &get_tuple(int index) const;

        const tuple &get_tuple(const string &key) const;

        const tuple &get_array(int index) const;

        const tuple &get_array(const string &key) const;

        const tuple &get_tup_or_arr(int index) const;

        const tuple &get_tup_or_arr(const string &key) const;

        bool remove(int index);

        bool print(const string &emps = "") const;

        bool print(ofstream &out, const string &emps = "") const;

        const tuple &operator=(const tuple &t);

        tuple enclose_in(int i) const;

        const db_tuple_var traverse(const tuple &kt, int start = 0) const;

        const db_tuple_var traverse(const string &str, int start = 0) const;

        const db_tuple_var to_var(const string &key = empty_string) const;

        void shrink(void) {
            data.shrink_to_fit();
        }


        friend void delete_tuple(tuple &t);

        friend void copy_tuple(tuple &t1, const tuple &t2);

        bool empty() const {
            return data.empty();
        }

        const static int npos = -1;

        const static db_tuple_var null_var();
    };

    const tuple null_tuple;

    class db_tuple_var {
        union {
            bool b;
            int64_t i;
            double d;
            string *s;
            tuple *t;
        };
        string *_key;
        db_TYPE _type;

        bool key(const string &k) {
            if (k.empty()) {
                _key = nullptr;
                return false;
            }
            if (identifierValidity(k, true) == false) return false;
            _key = new string(k);
            return true;
        }

    public:

        db_tuple_var() : i(-1), _key(nullptr), _type(db_NULL) {};

        explicit db_tuple_var(void *typ, const string &set_key = empty_string) : i(-1), _key(nullptr), _type(db_NULL) {
            this->key(set_key);
            this->type(db_NULL);
            i = 0;
            typ = typ;
        }

        explicit db_tuple_var(int32_t value, const string &set_key = empty_string) : i(-1), _key(nullptr),
                                                                                     _type(db_NULL) {
            this->key(set_key);
            this->type(db_INT);
            i = value;
        }

        explicit db_tuple_var(int64_t value, const string &set_key = empty_string) : i(-1), _key(nullptr),
                                                                                     _type(db_NULL) {
            this->key(set_key);
            this->type(db_INT);
            i = value;
        }

        explicit db_tuple_var(double value, const string &set_key = empty_string) : i(-1), _key(nullptr),
                                                                                    _type(db_NULL) {
            this->key(set_key);
            this->type(db_DOUBLE);
            d = value;
        }

        explicit db_tuple_var(bool value, const string &set_key = empty_string) : i(-1), _key(nullptr), _type(db_NULL) {
            this->key(set_key);
            this->type(db_BOOL);
            b = value;
        }

        explicit db_tuple_var(const string &value, const string &set_key = empty_string) : i(-1), _key(nullptr),
                                                                                           _type(db_NULL) {
            this->key(set_key);
            this->type(db_STRING);
            s = new string(value);
        }

        explicit db_tuple_var(const tuple &value, const string &set_key = empty_string) : i(-1), _key(nullptr),
                                                                                          _type(db_NULL) {
            this->key(set_key);
            (value.is_array()) ? this->type(db_ARRAY) : this->type(db_TUPLE);
            t = new tuple(value);
        }

        const db_tuple_var &operator=(const db_tuple_var &value) {
            this->~db_tuple_var();
            key(value.key());
            type(value.type());
            switch (value.type()) {
                case db_INT:
                    i = value.i;
                    break;
                case db_DOUBLE:
                    d = value.d;
                    break;
                case db_BOOL:
                    b = value.b;
                    break;
                case db_STRING :
                    s = new string(*value.s);
                    break;
                case db_ARRAY:
                case db_TUPLE:
                    t = new tuple(*value.t);
                    break;
                case db_NULL:
                    if (!value.is_empty()) i = value.i;
                    break;
            }
            return *this;
        }

        db_tuple_var(const db_tuple_var &value) : i(-1), _key(nullptr), _type(db_NULL) {
            *this = value;
        }

        ~db_tuple_var() {
            if (_key) {
                delete _key;
                _key = nullptr;
            }
            switch (type()) {
                case db_NULL:
                    break;
                case db_INT:
                    break;
                case db_DOUBLE:
                    break;
                case db_BOOL:
                    break;
                case db_STRING :
                    if (s)
                        delete s;
                    s = nullptr;
                    break;
                case db_ARRAY:
                case db_TUPLE:
                    if (t)
                        delete t;
                    t = nullptr;
                    break;
            }
        }

        bool get_bool() const {
            if (type() != db_BOOL) return 0;
            return b;
        }

        int64_t get_int() const {
            if (type() != db_INT) return 0;
            return i;
        }

        double get_double() const {
            if (type() != db_DOUBLE) return 0;
            return d;
        }

        const string &get_string() const {
            if (type() != db_STRING) return empty_string;
            return *s;
        }

        const tuple &get_tup_arr() const {
            if (type() != db_TUPLE && type() != db_ARRAY) return null_tuple;
            return *t;
        }

        db_TYPE type() const {
            return _type;
        }

        bool type(db_TYPE typ) {
            _type = typ;
            return true;
        }

        const string &key() const {
            if (_key) return *_key;
            return empty_string;
        }

        bool is_null() const {
            if ((!is_empty()) && type() == db_NULL) return true;
            return false;
        }

        bool is_empty() const {
            if (_type == db_NULL && _key == nullptr && i == -1)return true;
            return false;
        }

        bool is_container() const {
            if (type() == db_ARRAY || type() == db_TUPLE)return true;
            return false;
        }
    };

    const static db_tuple_var empty_var;

}


#endif
