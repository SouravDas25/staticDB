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
#include "../version.h"

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

    bool isEndingBracket(int c) {
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

    bool isStartingBracket(int c) {
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

    char getOppositeBracket(char c) {
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


    vector<string> SplitQuery(const string &str, const string &sep) {
        int i = 0, clevel = 0, prev = -1;
        unsigned long long int len = str.size();
        bool bp1 = sep.find("[") != string::npos || sep.find("{") != string::npos || sep.find("(") != string::npos;
        bool bp2;
        bp2 = sep.find("]") != string::npos || sep.find("}") != string::npos || sep.find(")") != string::npos || bp1;
        bool qp1 = sep.find("'") != string::npos || sep.find("\"") != string::npos;
        bool qou1 = false, qou2 = false;
        vector<string> v;
        char tmp[2] = {0, 0};
        for (i = 0; i < len + 1; i++) {
            tmp[0] = str[i];
            if (isStartingBracket(str[i]) && !bp2 && (!qou2 && !qou1)) clevel++;
            else if (isEndingBracket(str[i]) && !bp2 && (!qou2 && !qou1)) clevel--;
            if (str[i] == '\'' && !qp1 && !qou2) qou1 = !qou1;
            if (str[i] == '"' && !qp1 && !qou1) qou2 = !qou2;
            if (clevel == 0 && !qou1 && !qou2) {
                if (sep.find(tmp) != string::npos || str[i] == '\0') {
                    if (i - prev > 1) {
                        if (prev != 0) prev++;
                        string s = str.substr(prev, (unsigned long long int) (i - prev));//
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
            if (isdigit(str[i]) == 0) return false;
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


    std::string stringVsprintf(const char *format, std::va_list args) {
        va_list tmp_args; //unfortunately you cannot consume a va_list twice
        va_copy(tmp_args, args); //so we have to copy it
        const int required_len = vsnprintf(nullptr, 0, format, tmp_args) + 1;
        va_end(tmp_args);

        std::string buf(required_len, '\0');
        if (std::vsnprintf(&buf[0], buf.size(), format, args) < 0) {
            throw std::runtime_error{"stringVsprintf encoding error"};
        }
        return buf;
    }

    std::string stringSprintf(const char *format, ...) {
        std::va_list args;
        va_start(args, format);
        std::string str{stringVsprintf(format, args)};
        va_end(args);
        return str;
    }

    const string &get_version() {
        using namespace AutoVersion;
        static string s;
        if (s.empty()) s = stringSprintf("%ld.%ld.%ld.%s", MAJOR, MINOR, BUILD, STATUS);
        return s;
    }

    const string &get_StartMsg(void) {
        static string StrMSG;
        if (!StrMSG.length())
            StrMSG = StrMSG + "Static Database v" + get_version() +
                     "[This is a prototype]\nCopyright Under GNU Public License 3.\n\n";
        return StrMSG;
    }

    uint64_t cstrHash(const string &_buffer) /*produces a hash value of a c-string */
    {
        const char *buffer = _buffer.c_str();
        uint64_t len = strlen(buffer);
        char i;
        uint64_t val = 0;
        val = (uint64_t) (buffer[0] >> 7);
        for (i = 0; i < len; i++) {
            val += 1000003 * val ^ buffer[i];
        }
        val = (uint64_t) (val ^ len);
        return val;
    }


    static int numtoal(int ch) {
        int i = ch;
        if (i >= 10 && i <= 35) {
            return i + 87;
        } else if (i >= 0 && i <= 9) {
            return i + 48;
        } else {
            return 0;
        }
    }

    static int altonum(char ch) {
        int i = ch;
        if (i >= 97 && i <= 122) {
            return i - 87;
        } else if (i >= 65 && i <= 90) {
            return i - 55;
        } else if (i >= 48 && i <= 57) {
            return i - 48;
        } else {
            return 0;
        }
    }

    int64_t str_to_int64(const string &s, int base) {
        const char *src = s.c_str();
        if (base < 2 || base > 36) {
            return 0;
        }
        int sign = 1;
        if (*src == '-')sign = -1, src++;
        int i, len = s.size(), alpha;
        int64_t con = 0;
        for (i = len - 1; i >= 0; i--) {
            alpha = altonum(src[len - i - 1]);
            if (alpha >= base) {
                return 0;
            }
            con += (alpha) * pow(base, i);
        }
        return con * sign;
    }

    bool ads_isCommanPresent(const char *cmd, int size) {
        int i, clevel = 0;
        for (i = 0; i <= size; i++) {
            if (isStartingBracket(cmd[i]) == true) clevel++;
            else if (isEndingBracket(cmd[i]) == true) clevel--;
            if (clevel == 0) {
                if (cmd[i] == ',' || cmd[i] == ':') return true;
            }
        }
        return false;
    }

    int ads_isValidFormatting(const char *format, int *adi) {
        int i = *adi;
        if (format[i] == '{') {
            if (format[i + 1] == '\0') return 0;
            else if (format[i + 1] == '{') {
                (*adi)++;
                return 0;
            } else if (i > 1) {
                if (format[i - 1] == '{') return 0;
            }
            return 1;
        } else if (format[i] == '%') {
            if (format[i + 1] == '\0') return 0;
            else if (format[i + 1] == '%') {
                (*adi)++;
                return 0;
            } else if (i > 1) {
                if (format[i - 1] == '%') return 0;
            }
            return 2;
        }
        return 0;
    }

    const char *ads_getOldCMD(const char *format, int *i) {
        static char cmd[16] = "";
        int cnt = 0;
        while ((isalnum(format[*i + 1]) || ispunct(format[*i + 1])) && format[*i + 1] != '\0' && cnt < 15) {
            cmd[cnt++] = format[*i + 1];
            (*i)++;
        }
        cmd[cnt++] = '\0';
        return cmd;
    }

    const char *ads_getCMD(const char *format, int *i) {
        int cmdmaxsize = 64;
        static char cmd[64] = "";
        int cnt = 0;
        (*i)++;
        while ((format[*i] != '}' && format[*i] != '\0') && cnt < cmdmaxsize - 1) {
            if (!isspace(format[*i]))cmd[cnt++] = format[*i];
            (*i)++;
        }
        cmd[cnt++] = '\0';
        return cmd;
    }


    bool identifierValidity(const string &str, bool is_tupleKey = false) {
        unsigned long long int len = str.size();
        int i;
        bool n = true;
        for (i = 0; i < len; i++) {
            if (n) {
                if (isdigit(str[i]) != 0) return false;
            }
            if (isalpha(str[i]) != 0) {
                n = false;
                continue;
            }
            if (isdigit(str[i]) != 0) {
                continue;
            }
            if (is_tupleKey) {
                if (str[i] == '.')continue;
                if (str[i] == '[')continue;
                if (str[i] == ']')continue;
            }
            if (str[i] == '-')continue;
            if (str[i] == '_')continue;
            if (str[i] == '$')continue;
            return false;
        }
        return true;
    }


}

namespace std{

template <typename T>
  string to_string ( T Number )
  {
     std::ostringstream ss;
     ss << Number;
     return ss.str();
  }

  int stoi(string s, size_t * n)
  {
  	  char ** c = NULL;
      long int i =  strtol(s.c_str(),c,10);
      if(c==NULL) *n = 0;
      else *n = *c - s.c_str();
      return i;
  }

}

#endif // SDB_UTILITY_H
