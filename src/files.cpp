
#ifndef _SATATICFILES_DB_C
#define _SATATICFILES_DB_C


#include "../include/files.h"

namespace StaticDB {

    static const string GetSignature(const File &f) {
        string tmp;
        size_t i = 0;
        while (i++ < db_signature.size()) {
            tmp.push_back(f.read());
        }
        return tmp;
    }

    void writeSignature(const File &f) {
        f.write(db_signature);
    }

    bool isSignatureMatch(const File &f) {
        f.seek(0, File::end);
        if (unsigned(f.tell()) < (2 * db_signature.size())) return false;
        f.seek(0);
        if (GetSignature(f) == db_signature) {
            f.seek(-db_signature.size(), File::end);
            if (GetSignature(f) == db_signature) return true;
        }
        return false;
    }

}

#endif // _SATATICFILES_DB_C
