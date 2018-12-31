//
// Created by SD on 12/30/2018.
//

#ifndef STATICDB_COMMONUTIL_H
#define STATICDB_COMMONUTIL_H

#include "json.hpp"
#include "Engine/Utility.h"

namespace StaticDB {

    using json = nlohmann::json;

    class CommonUtil {

    public:

        static int64_t toNumber(const string& s)
        {
            stringstream geek(s);
            int64_t x = 0;
            geek >> x;
            return x;
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
    };
}


#endif //STATICDB_COMMONUTIL_H
