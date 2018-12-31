//
// Created by SD on 1/2/2018.
//

#ifndef STATICDB_DATABASE_H
#define STATICDB_DATABASE_H

#include "Compiler.h"

// TODO password change option
namespace StaticDB {

    class Database : public StringMapper {

    public:
        Database() {
        }
        ~Database() {
        }

        bool open(const string &filename, const string &pwd)
        {
            return StringMapper::open(filename,pwd);
        }

        bool close()
        {
            return StringMapper::close();
        }

        string execute(const string& query)
        {
            return Compiler::compile(*this,query);
        }

    };
}

#endif //STATICDB_DATABASE_H
