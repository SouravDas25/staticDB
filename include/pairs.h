#ifndef _SATATICPAIRS_DB_H
#define _SATATICPAIRS_DB_H


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#include <vector>
#include <string>

#include "type2file.h"

namespace StaticDB {

    struct sdb_pair
    {
        int64_t keyname;
        int64_t data;
    };

pairBase * ads_pairBaseCreate();

bool pairExists(staticDB * db,const string& keyname);

int ads_pairCount(pairBase * pb);

sdb_pair* getPairNode(staticDB * db,const string& key);

const char* ads_pairFormat(staticDB * db,const char * key);

nosqlRT pairInsert(staticDB * db,const tuple& kt,const tuple& t);

nosqlRT pairCreate(staticDB * db,const string& key,const tuple& t);

nosqlRT pair_get(staticDB * db,const string& identifier);

nosqlRT ads_pairFetch(staticDB * db,const tuple& key);

tuple get_db_globals(staticDB * db);

 nosqlRT ads_pairDelete(staticDB * db,const tuple& kt);

 nosqlRT pairUpdate(staticDB * db,const tuple& kt,const tuple& t);

tuple ads_pairKeys(staticDB * db);

void ads_pairDestroy(pairBase * pb);

void ads_pairCommitAll(staticDB * db,FILE *fp);
void ads_pairCacheAll(staticDB * db,FILE*fp);
bool pairRead(staticDB * db,baseIndex pos);
baseIndex pairWrite(staticDB * db);

}

#endif
