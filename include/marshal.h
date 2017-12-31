#ifndef MARSHAL_H_INCLUDED
#define MARSHAL_H_INCLUDED


#include "dataBase.h"
#include "parser.h"

namespace StaticDB {

    class Database {
    public:
        staticDB *obj;
    public :
        Database() : obj(nullptr) {};

        nosqlRT connect(const string &file, const string &passwrd);

        nosqlRT execute(const string &str);

        nosqlRT disconnect();
    };

}

#endif // MARSHAL_H_INCLUDED
