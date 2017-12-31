
#ifndef SDB_DEFS
#define SDB_DEFS

#include "db_tuple.h"

namespace StaticDB {

    typedef struct _tableBase tableBase;
    typedef struct _pairBase pairBase;
    typedef struct _stringBase stringBase;

    typedef uint64_t filepos;
    typedef struct _staticDB staticDB;
    typedef struct _sdbTable sdbTable;
    typedef struct sdb_pair sdb_pair;

    stringBase* getDB_BaseTree(staticDB * db);
    pairBase* getDB_PairTree(staticDB * db);
    const string& getDB_FileName(staticDB * db);
    tableBase* getDB_TableTree(staticDB * db);
    filepos default_start(void);

    const FileLayer &get_FileLayer(staticDB *db);

    typedef enum nosqlRT {  SUCCESS_MSG,
                            QUIT_MSG,
                            ERROR_MSG,
                            RETURN_TUPLE } nosqlRT;



    nosqlRT noSql_ReturnTuple(const tuple& t);

    void sdb_freeNoSqlReturnTuple();

    const tuple& get_NoSqlReturnTuple();

    nosqlRT noSql_Msg(nosqlRT rt,const string& msg);

    const string& get_NoSqlMsg();

    void sdb_resetNoSqlRT();

    string encrypt(const string& password);

    string decrypt(const string& password);

}

#endif // SDB_DEFS
