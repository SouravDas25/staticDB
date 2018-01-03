//
// Created by SD on 1/1/2018.
//

#ifndef STATICDB_PARSER_H
#define STATICDB_PARSER_H

#include "json.hpp"
#include "PhysicalLayer/Utility.h"

using json = nlohmann::json;
using string = std::string;

namespace StaticDB
{

    int64_t toNumber(const string& s)
    {
        stringstream geek(s);
        int64_t x = 0;
        geek >> x;
        return x;
    }


    class Parser {

    public:
        static json walkJson(vector <string>& v,const json& j,int start = 1)
        {
            json ans = j;
            for(vector<string>::iterator it = v.begin(); it != v.end() ; ++it)
            {
                if(start) {
                    start--;
                    continue;
                }
                if(it->length() > 0)
                {
                    if(ans.type() == json::value_t::array)
                    {
                        int64_t n = toNumber(*it);
                        ans = ans.at(n);
                    }
                    else if( ans.type() == json::value_t::object ){
                        ans = ans.at(*it);
                    }
                    else
                    {
                        return ans;
                    }
                }
            }
            return ans;
        }

        static void split(const std::string& str, std::vector<std::string>& cont, const std::string& delims = " ")
        {
            std::size_t current, previous = 0;
            current = str.find_first_of(delims);
            while (current != std::string::npos) {
                cont.push_back(str.substr(previous, current - previous));
                previous = current + 1;
                current = str.find_first_of(delims, previous);
            }
            cont.push_back(str.substr(previous, current - previous));
        }

        static json parse(const string& query) {
            json syntaxObj;
            vector<string> vos = SplitQuery(query," ");
            if(vos.size() == 3) // apple = {1,2,3};
            {
                if( identifierValidity(vos[0]) && vos[1] == "=" )
                {
                    syntaxObj["TYPE"] = "STORE";
                    syntaxObj["KEY"] = vos[0];
                    syntaxObj["VALUE"] = vos[2];
                    return syntaxObj;
                }
            }
            else if(vos.size() == 2) // del apple
            {
                if( stricmp(vos[0],"del") == 0 && identifierValidity(vos[1]) )
                {
                    syntaxObj["TYPE"] = "DELETE";
                    syntaxObj["VALUE"] = vos[1];
                    return syntaxObj;
                }
                if( stricmp(vos[0],"debug") == 0 )
                {
                    if(stricmp(vos[1],"print") == 0 )
                    {
                        syntaxObj["TYPE"] = "DEBUG";
                        syntaxObj["VALUE"] = "PRINT";
                        return syntaxObj;
                    }
                }
            }
            else if(vos.size() == 1) // apple
            {
                if( stricmp(vos[0],"commit") == 0 )
                {
                    syntaxObj["TYPE"] = "COMMIT";
                    return syntaxObj;
                }
                if( stricmp(vos[0],"$keys") == 0 )
                {
                    syntaxObj["TYPE"] = "KEYS";
                    return syntaxObj;
                }
                if( identifierValidity(vos[0],true) )
                {
                    syntaxObj["TYPE"] = "FETCH";
                    syntaxObj["VALUE"] = vos[0];
                    return syntaxObj;
                }
            }
            syntaxObj["ERROR"] = true;
            syntaxObj["MSG"] = "Statement Not Recognized.";
            return syntaxObj;
        }

    };


};



#endif //STATICDB_PARSER_H
