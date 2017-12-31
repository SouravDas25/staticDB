


#include <iostream>
#include "include/JsonStore.h"

int main() {
    using StaticDB::sdb_error;

    StaticDB::JsonStore js;
    try {
        js.open("testdb", "root");
        //dm.printBase(std::cout);

        //dm.erase(2);
//        js.store(1,"Sourav Das is a good boy.");
//        js.store(2,23);
//        js.store(3,5.36);
        json j2 = js.getJson(4);


        //js.storeJson(4,j2);
        std::cout << j2;

        js.printBase(std::cout);

        js.commit();
        js.close();
    }
    catch (const std::exception &e) {
        std::cout << e.what();
    }
    //dm.store(1,"SOURAV DAS");


    return 0;
}


