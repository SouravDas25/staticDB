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
            json so = Parser::parse(query);
            if( so.find("TYPE") != so.end() )
            {
                if(so["TYPE"] == "FETCH")
                {
                    const string& key = so["VALUE"];
                    vector<string>v;
                    Parser::split(key,v,".[]");
                    json j = sm.fetch(v[0]);
                    j = Parser::walkJson(v,j);
                    return j.dump();
                }
                else if(so["TYPE"] == "DELETE")
                {
                    const string& key = so["VALUE"];
                    sm.forget(key);
                    return "{ \"MSG\" : \"Deleted Successfully\"} ";
                }
                else if(so["TYPE"] == "STORE")
                {
                    const string& key = so["KEY"];
                    const string& value = so["VALUE"];
                    sm.store(key,json::parse(value));
                    return "{ \"MSG\" : \"Stored Successfully\"} ";
                }
                else if(so["TYPE"] == "COMMIT")
                {
                    bool b = sm.commit();
                    return (b)?"{ \"MSG\" : \"Committed Successfully\"} ":"{ \"MSG\" : \"Commit Failed\"} ";
                }
                else if(so["TYPE"] == "KEYS")
                {
                    return sm.keys().dump();
                }
                else if(so["TYPE"] == "DEBUG")
                {
                    if(so["VALUE"] == "PRINT")
                    {
                        stringstream s;
                        sm.print(s);
                        return s.str();
                    }
                    return "{ MSG: \"Statement Not Recognized.\" } ";
                }
            }
            return so.dump();
        }
    };
}




#endif //STATICDB_COMPILER_H