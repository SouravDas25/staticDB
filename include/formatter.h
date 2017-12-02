#ifndef ADS_FORMATTER_H
#define ADS_FORMATTER_H

#include<stdio.h>
#include<stdarg.h>
#include<stdint.h>
#include<string.h>
#include<stdbool.h>

#include "common.h"
#include <vector>

#include "typecode.h"
#include <regex>

using namespace std;

namespace StaticDB {

vector<ads_typeCode> ads_parseFormat(const char * cmd,int len);

ads_typeCode ads_formatAppend(const char * format,int len);

ads_typeCode ads_typeCodeOne(const char * cmd,int len);

}

#endif // ADS_FORMATTER_H
