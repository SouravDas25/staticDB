//
// Created by SD on 1/3/2018.
//

#ifndef STATICDB_COMPILER_H
#define STATICDB_COMPILER_H

#include "StringMapper.h"
#include "Parser.h"

namespace StaticDB{

    class Compiler {

    public:
        static string compile(StringMapper& sm, const string& query){
            SymbolTree symbolTree = Parser::parse(query);
            return symbolTree.execute(sm);
        }
    };
}




#endif //STATICDB_COMPILER_H
