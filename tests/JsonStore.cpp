


#include "../src/JsonStore.h"

int main() {
    using namespace StaticDB;

    JsonStore sm;
    try {
        sm.open("testdb", "root");
        int c = true;
        while (c) {
            string s;
            cout << flush;
            cout << endl << "$ : ";
            cin >> s;
            try {
                if (stricmp(s, "store") == 0) {
                    int key;
                    json val;
                    cout << "key : ";
                    cin >> key;
                    cout << "value : ";
                    cin >> val;
                    sm.storeJson(key, val);
                } else if (stricmp(s, "fetch") == 0) {
                    int key;
                    cout << "key : ";
                    cin >> key;
                    json val = sm.getJson(key);
                    cout << val;
                } else if (stricmp(s, "forget") == 0) {
                    int key;
                    cout << "key : ";
                    cin >> key;
                    sm.erase(key);
                } else if (stricmp(s, "quit") == 0) {
                    c = false;
                } else if (stricmp(s, "print") == 0) {
                    sm.printBase(cout);
                } else if (stricmp(s, "clear") == 0) {
                    cout << flush;
                    system("CLS");
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




