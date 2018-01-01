


#include <iostream>
#include "include/Parser.h"

int main() {
    using namespace StaticDB;

    try {

        json j2 = {
                {"pi",      3.141},
                {"happy", true},
                {"name",    "Niels"},
                {"nothing", nullptr},
                {"answer",  {
                                    {"everything", 42}
                            }},
                {"list",    {       1, 0, 2}},
                {"object",  {
                                    {"currency",   "USD"},
                                       {"value", 42.99}
                            }}
        };

        std::cout << Parser::walkJson("list[2]",j2);


    }
    catch (const std::exception &e) {
        std::cout << e.what();
    }
    //dm.store(1,"SOURAV DAS");


    return 0;
}


