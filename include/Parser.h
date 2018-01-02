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
        static json walkJson(const string& key,const json& j)
        {
            vector <string> v;
            split(key,v,".[]");
            json ans = j;
            for(vector<string>::iterator it = v.begin(); it != v.end() ; ++it)
            {
                if(it->length() > 0)
                {
                    if(ans.type() == json::value_t::array)
                    {
                        int64_t n = toNumber(*it);
                        ans = ans.at(n);
                    }
                    else
                    {
                        ans = ans.at(*it);
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

    };


};



#endif //STATICDB_PARSER_H
