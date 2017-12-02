
#ifndef _SATATICFILES_DB_C
#define _SATATICFILES_DB_C



#include "../include/files.h"

namespace StaticDB {

bool fileExists(const string& file)
{
    FILE * fp = fopen(file.c_str(),"r");
    if(fp)
    {
        fclose(fp);
        return true;
    }
    return false;
}

static const string GetSignature(const db_file& f)
{
    string tmp;
    size_t i=0;
    while( i++ < db_signature.size() )
    {
        tmp.push_back( f.read() );
    }
    return tmp;
}

void writeSignature(const db_file& f)
{
    f.write(db_signature);
}

bool isSignatureMatch(const db_file& f)
{
    f.seek( 0 , db_file::end );
    if( unsigned(f.tell()) < (2*db_signature.size()) ) return false;
    f.seek(0);
    if( GetSignature(f) == db_signature )
    {
        f.seek( -db_signature.size() , db_file::end );
        if( GetSignature(f) == db_signature ) return true;
    }
    return false;
}

}

#endif // _SATATICFILES_DB_C
