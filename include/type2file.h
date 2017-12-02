#ifndef staticDATABASET2F_H
#define staticDATABASET2F_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "base.h"

#define READ 0
#define WRITE 1
#define DELETE 2
#define TUPLE 3

namespace StaticDB {

    typedef float float32_t;
    typedef double float64_t;

    typedef struct _storeStruct {
        union {
            int64_t i;
            bool b;
            double d;
            baseIndex bi;
        };
        baseIndex key;
        db_TYPE t;
    }storeStruct;

    const storeStruct storeStruct_NULL = {-1,BaseIndex_NULL,db_NULL};

    bool is_null_storeStruct(const storeStruct& srt );

    int bufferSize(int len);
    int tupleSize(int size);

    int getBufferSize(const tuple& t);

    baseIndex tupleToIndex(staticDB *db,const tuple& t);
    bool extendBuffer(staticDB *db, baseIndex index,const tuple& t);
    bool updateBuffer(staticDB *db, baseIndex index,const tuple& t);
    tuple index_to_tuple(staticDB *db, baseIndex index );
    int deleteBuffer(staticDB *db,baseIndex index);

    tuple splitKey(const string& key);

    bool verifyTupleFormat(const tuple& t , const db_TYPE * format,int len);

    storeStruct get_tupleIndex(staticDB *db,baseIndex index,const tuple& kt,int start);

    bool storeStruct_extract(staticDB *db,const storeStruct * data,int len,tuple& t);
    tuple storeStruct_to_tuple(staticDB *db,const storeStruct * data,int len);

    bool delete_tupleIndex(staticDB *db,baseIndex index,const tuple& kt,int start);
    bool update_tupleIndex(staticDB *db,baseIndex index,const tuple& kt,int start,const tuple& t);

    string remove_space(const string& str);


}

#endif // staticDATABASET2F_H
