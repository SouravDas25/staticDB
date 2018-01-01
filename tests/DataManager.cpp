



#include <iostream>
#include "../include/PhysicalLayer/DataManager.h"

int main()
{
    using StaticDB::sdb_error;

    StaticDB::DataManager dm;
    try
    {
        dm.open("testdb","root");
        //dm.printBase(std::cout);

        std::string s = dm.getString(1);
        int a = dm.getInt(2);
        std::cout << s << ", " << a << std::endl;

        //dm.erase(2);
        //dm.store(1,"Sourav Das is a good boy.");
        //dm.store(2,23);
        //dm.store(3,5.36);

        dm.printBase(std::cout);

        dm.commit();
        dm.close();
    }
    catch(const std::exception& e)
    {
        std::cout << e.what();
    }
    //dm.store(1,"SOURAV DAS");


    return 0;
}


