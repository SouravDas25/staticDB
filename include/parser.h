#ifndef SDB_PARSER_H
#define SDB_PARSER_H

#include "sdbdefs.h"
#include "query.h"

#include <cstring>
#include <string>
#include <vector>

namespace StaticDB {

    typedef enum nosqlcmd {
        nosql_NOCMD,
        nosql_CREATE,
        nosql_SET,
        nosql_GET,
        nosql_FIND,
        nosql_DELETE,
        nosql_UPDATE,
        nosql_INSERT,
        nosql_IMPORT,
        nosql_EXPORT,
        nosql_COMMIT,
        nosql_HELP,
        nosql_QUIT
    } nosqlcmd;

    typedef enum nosqOT {
        nosql_NOOT,
        nosql_KEY,
        nosql_KEYLIST,
        nosql_ARRAY,
        nosql_ARRAYLIST
    } nosqOT;

    typedef struct proc {
        nosqlcmd cmd;
        string key;

        proc() : cmd(nosql_NOCMD), key(string()) {};
    } proc;

    tuple tupleLiteral(staticDB *db, const string &n);


    nosqlcmd GetCommand(const string &str);

    nosqOT GetOperationType(const string &str);

    nosqlRT sdb_execute(staticDB *db, vector <string> &cmd);

}

#endif // SDB_PARSER_H
