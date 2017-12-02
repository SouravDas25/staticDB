#ifndef staticDATABASESTRING_H
#define staticDATABASESTRING_H


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include <string>
#include <map>

#include "sdbdefs.h"

using namespace std;

namespace StaticDB {

    typedef uint64_t baseIndex ;

    const baseIndex BaseIndex_NULL = 0;

typedef struct _baseNode baseNode;

typedef struct _stringBase stringBase;

stringBase* baseCreate(const db_file& fp,filepos fip);

string base_mem_detail(staticDB * db);

void baseDestroy(stringBase*sb);

uint64_t baseCount(stringBase * sb);

uint64_t base_del_count(stringBase * sb);

string base_del_print(staticDB * db);

void basePrint(staticDB * db);

baseIndex baseInsertCStr(staticDB * db,const string& str);
baseIndex baseInsert(staticDB * db,const void * data,int size);
bool baseUpdate(staticDB * db,baseIndex index,const void * data,int size);
bool baseExtend(staticDB * db,baseIndex index,const void * data,int size);

int get_cache_size(staticDB * db);
nosqlRT set_cache_size(staticDB * db , size_t size);

int baseNodeSize(staticDB * db,baseIndex index);

bool base_HasIndex(staticDB * db,baseIndex index);

const string baseNodeCStr(staticDB * db,baseIndex index);
const byte_array baseData(staticDB * db,baseIndex index);

void base_del_insert(stringBase * sb,filepos addr,int64_t memory);
size_t baseNode_utility_disk_size(void);

bool baseDelete(staticDB * db,baseIndex index);

void ads_baseCacheAll(stringBase * sb, const db_file& f);

bool baseRead(stringBase * sb,const db_file& f,filepos pos);

void baseWrite(stringBase * sb,const db_file& f,filepos * pos);

nosqlRT base_Commit(stringBase * sb,const db_file& f);
void base_Force_CommitAll(stringBase * sb,const db_file& f);

}

#endif // staticDATABASESTRING_H
