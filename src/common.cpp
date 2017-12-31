
/** @cond ADS_INTERNAL */

#ifndef ADS_IO_COMMON_C
#define ADS_IO_COMMON_C


#include "../include/common.h"

using namespace std;

namespace StaticDB {

    uint64_t cstrHash(const string &_buffer) /*produces a hash value of a c-string */
    {
        const char *buffer = _buffer.c_str();
        uint64_t val = 0, i, len = strlen(buffer);
        val = buffer[0] >> 7;
        for (i = 0; i < len; i++) {
            val += 1000003 * val ^ buffer[i];
        }
        val = val ^ len;
        return val;
    }

/*
static int numtoal(int ch)
{
	int i = ch;
	if (i>=10 && i <= 35)
	{
		return i+87;
	}
	else if (i>=0 && i<= 9)
	{
		return i+48;
	}
	else
	{
		return 0;
	}
}*/

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
            if (startbrac(cmd[i]) == true) clevel++;
            else if (endbrac(cmd[i]) == true) clevel--;
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


    bool identifierValidity(const string &str, bool is_tupleKey) {
        int len = str.size();
        int i;
        bool n = true;
        for (i = 0; i < len; i++) {
            if (n) {
                if (isdigit(str[i]) != false) return false;
            }
            if (isalpha(str[i]) != false) {
                n = false;
                continue;
            }
            if (isdigit(str[i]) != false) {
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
#endif

/** @endcond */
