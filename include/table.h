
#ifndef _SATATICTABLE_DB_H
#define _SATATICTABLE_DB_H


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include <vector>
#include <string>
#include <exception>

#define sdb_tableSize 16

#include "pairs.h"

using namespace std;

namespace StaticDB {

    nosqlRT createTable(staticDB *db, const string &tablename, const tuple &t);

    nosqlRT tableInsert(staticDB *db, const string &tablename, const tuple &t);

}

#endif // _SATATIC_DB
