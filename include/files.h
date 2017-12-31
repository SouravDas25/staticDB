
#ifndef SFileLayerS
#define SFileLayerS


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

    bool fileExists(const string &FileLayer);

    bool isSignatureMatch(const FileLayer &f);

    void writeSignature(const FileLayer &f);

}

#endif // SFileLayerS
