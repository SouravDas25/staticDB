


#include <iostream>
#include "../src/StringMapper.h"

int main() {
    using namespace StaticDB;
    StringMapper sm;
    try {
        sm.open();
        json j2 = {
                {"pi",      3.141},
                {"happy",   true},
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

        sm.close();


    }
    catch (const std::exception &e) {
        std::cout << e.what();
    }


    return 0;
}


