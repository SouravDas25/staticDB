


#include <iostream>
#include "../src/StringMapper.h"

int main() {
    using namespace StaticDB;

    StringMapper sm;
    try {
        sm.open("testdb", "root");
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
                    sm.store(key, val);
                } else if (stricmp(s, "fetch") == 0) {
                    string key;
                    cout << "key : ";
                    cin >> key;
                    json val = sm.fetch(key);
                    cout << val;
                } else if (stricmp(s, "forget") == 0) {
                    string key;
                    cout << "key : ";
                    cin >> key;
                    sm.forget(key);
                } else if (stricmp(s, "print") == 0) {
                    sm.print(cout);
                } else if (stricmp(s, "clear") == 0) {
                    cout << flush;
                    system("CLS");
                } else if (stricmp(s, "quit") == 0) {
                    c = false;
                } else if (stricmp(s, "commit") == 0) {
                    sm.commit();
                    cout << "Successfully Commited";
                } else {
                    cout << "Command Not Recognized.";
                }
            }
            catch (const std::exception &e) {
                std::cout << e.what();
            }
        }
        sm.close();

    }
    catch (const std::exception &e) {
        std::cout << e.what();
    }


    return 0;
}


