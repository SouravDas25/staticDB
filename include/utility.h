#ifndef SDB_UTILITY_H
#define SDB_UTILITY_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <cmath>
#include <cstdio>
#include <cstdbool>
#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <cstdarg>
#include <iostream>
#include <string>
#include <vector>
#include <stack>
#include <algorithm>
#include <exception>
#include <fstream>
#include <sstream>
#include <cerrno>
#include <cctype>
#include <cstdio>
#include <cstdint>
#include <memory>
#include <limits>


#include "../version.h"

using namespace std;

namespace StaticDB {

    class tuple ;

    typedef enum db_TYPE { db_NULL = 0, db_INT , db_STRING, db_DOUBLE, db_TUPLE , db_BOOL , db_ARRAY } db_TYPE;

	typedef struct db_tuple_var db_tuple_var;

	std::string string_vsprintf(const char* format, std::va_list args);

	std::string string_sprintf(const char* format, ...) __attribute__ ((format (printf, 1, 2)));

	struct sdb_error : public exception {
        const string _msg;
	    sdb_error(const string& msg):_msg(msg){};
        virtual const char * what () const throw () {
	      return _msg.c_str();
	   }
	   ~sdb_error() throw() {};
	};

    vector<string> Split(const string& str,const string& sep);

    bool isInteger(const string& str);

    bool endbrac(int c);

    bool startbrac(int c);

    char opbrac(char c);

    db_TYPE sdb_ParseType(const char * str,int len);

    int strnicmp(const string& a, const string& b,size_t len);

    int stricmp(const string& a, const string& b);

    db_TYPE* generate_TypeFormat(const char * str, int len,int * cnt1);

    int comma_count(const char * cmd,size_t size);

    string multi_line_input(void);

    const string& get_version();

    const string& get_StartMsg(void );

}
#endif // SDB_UTILITY_H
