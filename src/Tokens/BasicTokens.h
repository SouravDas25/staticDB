//
// Created by SD on 12/30/2018.
//

#ifndef STATICDB_STORE_H
#define STATICDB_STORE_H

#include "TokenDeclarations.h"
#include "../StringMapper.h"
#include "../SymbolTree.h"
#include "../CommonUtil.h"

namespace StaticDB {
    namespace TokenImpl {

        const string Nop::execute(StringMapper& stringMapper,const SymbolTree &symbolTree) const {
            if(symbolTree.isError()){
                return "{ MSG: \""+ symbolTree.getMsg() +"\" } ";
            }
            return "";
        }

        const string Store::execute(StringMapper& stringMapper,const SymbolTree &symbolTree) const {
            const string& key = symbolTree.getKey();
            const string& value = symbolTree.getValue();
            stringMapper.store(key,json::parse(value));
            return "{ \"MSG\" : \"Stored Successfully\"} ";
        }

        const string Fetch::execute(StringMapper& stringMapper,const SymbolTree &symbolTree) const{
            const string& key = symbolTree.getValue();
            vector<string>v;
            CommonUtil::split(key,v,".[]");
            json j = stringMapper.fetch(v[0]);
            j = CommonUtil::walkJson(v,j);
            return j.dump();
        }

        const string Help::execute(StringMapper& stringMapper,const SymbolTree &symbolTree) const {
            return STATICDB_HELP_TEXT;
        }

        const string Keys::execute(StringMapper& stringMapper,const SymbolTree &symbolTree) const {
            return stringMapper.keys().dump();
        }

        const string Commit::execute(StringMapper& stringMapper,const SymbolTree &symbolTree) const {
            bool b = stringMapper.commit();
            return (b)?"{ \"MSG\" : \"Committed Successfully\"} ":"{ \"MSG\" : \"Commit Failed\"} ";
        }

        const string Debug::execute(StringMapper& stringMapper,const SymbolTree &symbolTree) const{
            if( symbolTree.getValue() == "PRINT")
            {
                stringstream s;
                stringMapperPrint(stringMapper,s);
                return s.str();
            }
            return "{ MSG: \"Statement Not Recognized.\" } ";
        }

        const string Delete::execute(StringMapper& stringMapper,const SymbolTree &symbolTree) const {
            const string& key = symbolTree.getKey();
            stringMapper.forget(key);
            return "{ \"MSG\" : \"Deleted Successfully\"} ";
        }
    }
}


#endif //STATICDB_STORE_H
