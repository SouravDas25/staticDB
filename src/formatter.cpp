#ifndef ADS_FORMATTER_C
#define ADS_FORMATTER_C

#include<stdio.h>
#include<stdarg.h>
#include<stdint.h>
#include<string.h>
#include<stdbool.h>


#include "../include/formatter.h"

namespace StaticDB {

    struct ads_format_detail {
        ads_typeCode tc;
        regex *re;
    };

    typedef struct ads_format_detail ads_format_detail;

    typedef struct _ads_formatInfo {
        ads_format_detail array[ADS_MAXTYPE];
        unsigned int size;
    } _ads_formatInfo;

    _ads_formatInfo ads_formatINFO = {{}, 0};


    void ads_formatAppendInternal(ads_typeCode tc, const char *format) {
        ads_formatINFO.array[ads_formatINFO.size].tc = tc;
        ads_formatINFO.array[ads_formatINFO.size].re = new regex(format);
        ads_formatINFO.size++;
    }

    int ads_formatAppend(const char *format) {
        ads_formatINFO.array[ads_formatINFO.size].tc = static_cast<ads_typeCode>(ads_formatINFO.size);
        ads_formatINFO.array[ads_formatINFO.size].re = new regex(format);
        return ads_formatINFO.size++;
    }

    void ads_formatInit() {
        ads_formatAppendInternal(CHAR_TC, "char");
        ads_formatAppendInternal(INT_TC, "int");
        ads_formatAppendInternal(INT32_TC, "int32");
        ads_formatAppendInternal(INT64_TC, "int64");
        ads_formatAppendInternal(FLOAT_TC, "float");
        ads_formatAppendInternal(DOUBLE_TC, "double");
        ads_formatAppendInternal(STRING_TC, "varchar");
        ads_formatAppendInternal(STRING_TC, "string");
        ads_formatAppendInternal(ZBUFFER_TC, "buffer\\[[0-9]+\\]");
    }

    ads_format_detail *ads_parseSubFormat(const char *cmd, int size) {
        unsigned int i;
        for (i = 0; i < ads_formatINFO.size; i++) {
            ads_format_detail *tmp = ads_formatINFO.array + i;
            if (std::regex_match(cmd, *tmp->re)) {
                return tmp;
            }
        }
        return ads_formatINFO.array;
    }

    vector<ads_typeCode> ads_parseFormat(const char *cmd, int len) {
        if (!ads_formatINFO.size) ads_formatInit();
        vector<ads_typeCode> v;
        int i, p = 0;
        ads_format_detail *tmp;
        if (ads_isCommanPresent(cmd, len)) {
            for (i = 0; i < len; i++) {
                if (cmd[i] == ',' || cmd[i] == ':') {
                    tmp = ads_parseSubFormat(&cmd[p], (i - p));
                    p = i + 1;
                    v.push_back(tmp->tc);
                }
            }
            tmp = ads_parseSubFormat(&cmd[p], (i - p));
            p = i + 1;
            v.push_back(tmp->tc);
        } else {
            tmp = ads_parseSubFormat(cmd, len);
            v.push_back(tmp->tc);
        }
        return v;
    }


    ads_typeCode ads_typeCodeOne(const char *cmd, int len) {
        vector<ads_typeCode> l = ads_parseFormat(cmd, len);
        ads_typeCode tc;
        if (l.size() > 0) {
            tc = l[0];
        } else tc = VOID_TC;
        return tc;
    }

}

#endif // ADS_FORMATTER_C
