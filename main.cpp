


#include <iostream>
#include "src/Database.h"

int main() {
    using namespace StaticDB;

    Database db;
    try {
        db.open("testdb", "root");
        int c = true;
        cout << get_StartMsg();
        while (c) {
            string s;
            cout << endl << ">>> ";
            s = multi_line_input();
            if( stricmp(s,"clear") == 0 ) {
                system("CLS");
                continue;
            }
            if( stricmp(s,"quit") == 0){
                c = false;
                continue;
            }
            try {
                cout << db.execute(s) ;
            }
            catch (const std::exception &e) {
                std::cout << e.what() ;
            }
            cout << endl;
        }
        db.close();

    }
    catch (const std::exception &e) {
        std::cout << e.what();
    }


    return 0;
}


