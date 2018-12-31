//
// Created by SD on 1/1/2018.
//

#ifndef STATICDB_PARSER_H
#define STATICDB_PARSER_H

#include "json.hpp"
#include "Engine/Utility.h"
#include "Constants.h"
#include "SymbolTree.h"

using json = nlohmann::json;
using string = std::string;

namespace StaticDB
{

    class Parser {

    public:

        static SymbolTree parse(const string& query) {
            SymbolTree symbolTree;
            vector<string> vos = SplitQuery(query," ");
            if(vos.size() == 3) // apple = {1,2,3};
            {
                if( identifierValidity(vos[0]) && vos[1] == "=" )
                {
                    symbolTree.setType(TOKEN::STORE);
                    symbolTree.setKey(vos[0]);
                    symbolTree.setValue(vos[2]);
                    return symbolTree;
                }
            }
            else if(vos.size() == 2) // del apple
            {
                if( stricmp(vos[0],KEYWORDS::DELETE) == 0 && identifierValidity(vos[1]) )
                {
                    symbolTree.setType(TOKEN::DELETE);
                    symbolTree.setValue(vos[1]);
                    return symbolTree;
                }
                if( stricmp(vos[0],KEYWORDS::DEBUG) == 0 )
                {
                    if(stricmp(vos[1],"print") == 0 )
                    {
                        symbolTree.setType(TOKEN::DEBUG);
                        symbolTree.setValue("PRINT");
                        return symbolTree;
                    }
                }
            }
            else if(vos.size() == 1) // apple
            {
                if( stricmp(vos[0],KEYWORDS::HELP) == 0 )
                {
                    symbolTree.setType(TOKEN::HELP);
                    return symbolTree;
                }
                if( stricmp(vos[0],KEYWORDS::COMMIT) == 0 )
                {
                    symbolTree.setType(TOKEN::COMMIT);
                    return symbolTree;
                }
                if( stricmp(vos[0],KEYWORDS::KEYS) == 0 )
                {
                    symbolTree.setType(TOKEN::KEYS);
                    return symbolTree;
                }
                if( identifierValidity(vos[0],true) )
                {
                    symbolTree.setType(TOKEN::FETCH);
                    symbolTree.setValue(vos[0]);
                    return symbolTree;
                }
            }
            symbolTree.setError(true);
            symbolTree.setMsg("Statement Not Recognized.");
            return symbolTree;
        }

    };


};



#endif //STATICDB_PARSER_H
