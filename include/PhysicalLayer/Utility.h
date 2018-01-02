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
#include <map>

#include "SDB_Error.h"
#include "../../version.h"

using namespace std;

namespace StaticDB {

/** Declarations */
    typedef uint64_t L_ADDR;
    typedef uint64_t P_ADDR;
    typedef uint64_t MEM_SIZE;
    typedef uint8_t *MEM_DATA;

/** Initializations */
    const P_ADDR P_ADDR_NULL = 0;
    const L_ADDR L_ADDR_NULL = 0;
    const string basedata_sig("bd");
    const string baseIndex_sig("bi");

    bool endbrac(int c) {
        switch (c) {
            case ')':
                return true;
            case '}':
                return true;
            case ']':
                return true;
            case '>':
                return true;
            default:
                return false;
        }
    }

    bool startbrac(int c) {
        switch (c) {
            case '(':
                return true;
            case '{':
                return true;
            case '[':
                return true;
            case '<':
                return true;
            default :
                return false;
        }
    }

    char opbrac(char c) {
        switch (c) {
            case ')':
                return '(';
            case '}':
                return '{';
            case ']':
                return '[';
            case '(' :
                return ')';
            case '{':
                return '}';
            case '[':
                return ']';
            default :
                throw c;
        }
    }

    int strnicmp(const string &a, const string &b, size_t len) {
        if (a.size() < len) return -1;
        if (b.size() < len) return 1;
        for (unsigned int i = 0; i < len; ++i) {
            int diff = tolower(a[i]) - tolower(b[i]);
            if (diff < 0) return -1;
            else if (diff > 1) return 1;
        }
        return 0;
    }

    int stricmp(const string &a, const string &b) {
        int diff = int(a.size()) - int(b.size());
        if (diff < 0) return -1;
        if (diff > 0) return 1;
        for (unsigned int i = 0; i < a.size(); ++i) {
            diff = tolower(a[i]) - tolower(b[i]);
            if (diff < 0) return -1;
            else if (diff > 1) return 1;
        }
        return 0;
    }


    vector<string> Split(const string &str, const string &sep) {
        int i = 0, clevel = 0, prev = -1, len = str.size();
        bool bp1 = sep.find("[") != string::npos || sep.find("{") != string::npos || sep.find("(") != string::npos;
        bool bp2 =
                sep.find("]") != string::npos || sep.find("}") != string::npos || sep.find(")") != string::npos || bp1;
        bool qp1 = sep.find("'") != string::npos || sep.find("\"") != string::npos;
        bool qou1 = false, qou2 = false;
        vector<string> v;
        char tmp[2] = {0, 0};
        for (i = 0; i < len + 1; i++) {
            tmp[0] = str[i];
            if (startbrac(str[i]) && !bp2 && (!qou2 && !qou1)) clevel++;
            else if (endbrac(str[i]) && !bp2 && (!qou2 && !qou1)) clevel--;
            if (str[i] == '\'' && !qp1 && !qou2) qou1 = !qou1;
            if (str[i] == '"' && !qp1 && !qou1) qou2 = !qou2;
            if (clevel == 0 && !qou1 && !qou2) {
                if (sep.find(tmp) != string::npos || str[i] == '\0') {
                    if (i - prev > 1) {
                        if (prev != 0) prev++;
                        string s = str.substr(prev, i - prev);//
                        v.push_back(s);//printvos(v)
                    }
                    prev = i;
                }
            }
        }
        if (clevel)throw sdb_error("Brackets Or Braces Did Not Match.");
        if (qou2 || qou1)throw sdb_error("String Quotes Did Not Match.");
        v.shrink_to_fit();
        return v;
    }

    bool isInteger(const string &str) {
        size_t i;
        for (i = 0; i < str.size(); i++) {
            if (isdigit(str[i]) != true) return false;
        }
        return true;
    }

    static int comma_count(const char *cmd, int size) {
        int i, cnt = 0;
        bool qou1 = false, qou2 = false;
        for (i = 0; i <= size; i++) {
            if (cmd[i] == '\'' && qou2 == false) qou1 = !qou1;
            if (cmd[i] == '"' && qou1 == false) qou2 = !qou2;
            if (qou1 == false && qou2 == false) {
                if (cmd[i] == ',') cnt++;
            }
        }
        return cnt;
    }

/*
db_TYPE sdb_ParseType(const char * str,int len)
{
    int i;
    for(i = 0; i<len; i++)
    {
        if( isalnum(*str) != false )  break;
        str++;
    }
    if( strnicmp(str,"int",3) == 0)
    {
        return db_INT;
    }
    if( strnicmp(str,"double",6) == 0)
    {
        return db_DOUBLE;
    }
    if( strnicmp(str,"string",6) == 0)
    {
        return db_STRING;
    }
    if( strnicmp(str,"str",3) == 0)
    {
        return db_STRING;
    }
    if( strnicmp(str,"float",5) == 0)
    {
        return db_DOUBLE;
    }
    if( strnicmp(str,"bool",4) == 0)
    {
        return db_BOOL;
    }
    if( strnicmp(str,"tuple",5) == 0)
    {
        return db_TUPLE;
    }
    if( strnicmp(str,"array",5) == 0)
    {
        return db_ARRAY;
    }
    return db_NULL;
}

db_TYPE* generate_TypeFormat(const char * str, int len,int * cnt1)
{
    int size = comma_count(str,len) + 1,cnt = 0;
    bool qou1 = false,qou2 = false;
    db_TYPE * t = new db_TYPE[size];
    int i,p = 0;
    for(i=0; i<len+1; i++)
    {
        if( str[i]  == '\'' && qou2 == false ) qou1 = !qou1;
        if( str[i]  == '"' && qou1 == false ) qou2 = !qou2;
        if( qou1 == false && qou2 == false)
        {
            if(str[i] == ',' || str[i] == 0 )
            {
                t[cnt++] = sdb_ParseType(&str[p],i-p);
                p = i + 1;
            }
        }
    }
    *cnt1 = cnt;
    return t;
}
*/

    static inline void rtrim(std::string &s) {
        s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
            return !std::isspace(ch);
        }).base(), s.end());
    }

    string multi_line_input(void) {
        string line = "a";
        bool mul = false;
        string result;
        while (line.length() != 0) {
            //cout << "Enter text line: " << endl;
            getline(cin, line);
            rtrim(line);
            bool p = line[line.size() - 1] == '/';
            if (!p && !mul) {
                return line;
            }
            if (p)line.erase(line.size() - 1, 1);
            result = result + " " + line;
            if (line.length() != 0)cout << "...\t";
            mul = true;
        }
        return result;
    }


    std::string string_vsprintf(const char *format, std::va_list args) {
        va_list tmp_args; //unfortunately you cannot consume a va_list twice
        va_copy(tmp_args, args); //so we have to copy it
        const int required_len = vsnprintf(nullptr, 0, format, tmp_args) + 1;
        va_end(tmp_args);

        std::string buf(required_len, '\0');
        if (std::vsnprintf(&buf[0], buf.size(), format, args) < 0) {
            throw std::runtime_error{"string_vsprintf encoding error"};
        }
        return buf;
    }

    std::string string_sprintf(const char *format, ...) {
        std::va_list args;
        va_start(args, format);
        std::string str{string_vsprintf(format, args)};
        va_end(args);
        return str;
    }

    const string &get_version() {
        using namespace AutoVersion;
        static string s;
        if (s.empty()) s = string_sprintf("%ld.%ld.%ld.%s", MAJOR, MINOR, BUILD, STATUS);
        return s;
    }

    const string &get_StartMsg(void) {
        static string StrMSG;
        if (!StrMSG.length())
            StrMSG = StrMSG + "Static Database v" + get_version() +
                     "[This is a prototype]\nCopyright Under GNU Public License 3.\n\n";
        return StrMSG;
    }


}
#endif // SDB_UTILITY_H
