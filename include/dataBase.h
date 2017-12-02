
#ifndef staticDATABASE
#define staticDATABASE


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>

#include <map>
#include <string>

#include "files.h"
#include "table.h"
#include "pairs.h"

#define sdb_defaultBufferSize 32

namespace StaticDB {

    typedef struct _sdbMasterDB sdbMasterDB;

    nosqlRT ads_connectDB(staticDB** addr_db,const string& db_file,const string& password);

    void ads_disconnectDB(staticDB*db);

    nosqlRT ads_dbCommit(staticDB * db);

    void db_write_dummy(const char * db_file,sdbMasterDB * mdb);

}

#endif // staticDATABASE
