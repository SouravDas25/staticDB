#ifndef staticDATABASE_QUERY_H
#define staticDATABASE_QUERY_H

#include "pairs.h"

namespace StaticDB {

    nosqlRT sdb_parse(staticDB * db,const string& str);

    nosqlRT cmd_find(staticDB * db,const tuple& pt,const string& kt,const tuple& ft);

    bool is_sel_match(staticDB * db,const db_tuple_var& srt,const tuple& sel,int match);

    bool selection_OR(staticDB * db,const db_tuple_var& srt,const tuple& sel);

    bool selection_AND(staticDB * db,const db_tuple_var& srt,const tuple& sel);

}


#endif
