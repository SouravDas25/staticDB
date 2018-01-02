


#include <iostream>
#include "include/Database.h"

int main() {
    using namespace StaticDB;

    Database db;
    try {
        db.open("testdb", "root");
        int c = true;
        while (c) {
            string s;
            cout << endl << "$ : ";
            cin >> s;
            try {
                if (stricmp(s, "store") == 0) {
                    string key;
                    json val;
                    cout << "key : ";
                    cin >> key;
                    cout << "value : ";
                    cin >> val;
                    db.store(key, val);
                } else if (stricmp(s, "fetch") == 0) {
                    string key;
                    cout << "key : ";
                    cin >> key;
                    json val = db.fetch(key);
                    cout << val;
                } else if (stricmp(s, "forget") == 0) {
                    string key;
                    cout << "key : ";
                    cin >> key;
                    db.forget(key);
                } else if (stricmp(s, "print") == 0) {
                    db.print(cout);
                } else if (stricmp(s, "clear") == 0) {
                    cout << flush;
                    system("CLS");
                } else if (stricmp(s, "quit") == 0) {
                    c = false;
                } else if (stricmp(s, "commit") == 0) {
                    db.commit();
                    cout << "Successfully Commited";
                } else {
                    cout << "Command Not Recognized.";
                }
            }
            catch (const std::exception &e) {
                std::cout << e.what();
            }
        }
        db.close();

    }
    catch (const std::exception &e) {
        std::cout << e.what();
    }


    return 0;
}


