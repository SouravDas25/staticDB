#ifndef SDB_DEFS_C
#define SDB_DEFS_C

#include "../include/sdbdefs.h"

namespace StaticDB {

    static tuple *NoSqlReturnTuple = nullptr;

    nosqlRT noSql_ReturnTuple(const tuple &t) {
        if (NoSqlReturnTuple) delete NoSqlReturnTuple;
        NoSqlReturnTuple = new tuple(t);
        return noSql_Msg(RETURN_TUPLE, "Tuple Returned.");
    }

    void sdb_freeNoSqlReturnTuple() {
        if (NoSqlReturnTuple)delete NoSqlReturnTuple;;
        NoSqlReturnTuple = nullptr;
    }

    const tuple &get_NoSqlReturnTuple() {
        return *NoSqlReturnTuple;
    }

    static string nosqlcpmsg;

    nosqlRT noSql_Msg(nosqlRT rt, const string &msg) {
        nosqlcpmsg = msg;
        return rt;
    }

    const string &get_NoSqlMsg() {
        return nosqlcpmsg;
    }

    void sdb_resetNoSqlRT() {
        sdb_freeNoSqlReturnTuple();
        noSql_Msg(ERROR_MSG, "");
    }


    string encrypt(const string &password) {
        string s = "";
        unsigned int key = 123;
        unsigned int i, len = password.size();
        for (i = 0; i < len; ++i) {
            s.push_back(password[i] - key);
        }
        return s;
    }

    string decrypt(const string &password) {
        string s = "";
        unsigned int key = 123;
        unsigned int i, len = password.size();
        for (i = 0; i < len; ++i) {
            s.push_back(password[i] + key);
        }
        return s;
    }

/*
uint64_t checksum(void *buffer, size_t len, uint64_t seed)
{
      unsigned char *buf = (unsigned char *)buffer;
      size_t i;

      for (i = 0; i < len; ++i)
            seed += (unsigned int)(*buf++);
      return seed;
}
*/
}

#endif // SDB_DEFS_C
