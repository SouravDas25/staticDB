
#ifndef SDB_FILESOBJ_C
#define SDB_FILESOBJ_C

#include "../include/fileObj.h"

using namespace std;

namespace StaticDB {

    ostream& operator<<(ostream& out,byte_array ba)
    {
        out << '"';
        for(unsigned int i=0;i<ba.size();++i)
        {
            if( ba[i] == '"' ) out << '\\' << ba[i];
            else if( ba[i] == '\\' ) out << '\\' << ba[i];
            else if( isprint (ba[i]) != 0) out << ba[i];
            else out << "\\x"<< int(ba[i]);
        }
        out << '"';
        return out;
    }

}

/*
using namespace StaticDB;
int main()
{
	db_file db_file;

	db_file.open("test.txt","wb+");
	int p[]  = {1,2,3,5,6} ;
	byte_array a = db_file.read(1);
//	//p=45;
//	db_file.seek(100);
//	db_file.read(&p,sizeof(p));

	db_file.write(&p,sizeof(p));

	cout << p << "  " << a ;

}*/

#endif
