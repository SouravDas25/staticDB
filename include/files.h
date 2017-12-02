
#ifndef SDB_FILES
#define SDB_FILES


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>
#include <string>

#include "fileObj.h"

using namespace std;

namespace StaticDB {

    const string db_signature = "staticDB v1.0" ;

    bool fileExists(const string& db_file);

    bool isSignatureMatch(const db_file& f);

    void writeSignature(const db_file& f);

}

#endif // SDB_FILES
