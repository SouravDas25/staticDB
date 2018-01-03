# StaticDB
A Json DataStore Engine written in C++.

1. `Only Header` Import Library.
2. Direct `JSON` Storage.
3. File Based Database.
4. Extremely light weight.
5. Extended With `Cython` to import in `Python`.

## Dependencies
 1. [JSON for Modern C++](https://nlohmann.github.io/json/) 
 2. File Object Wrapper by Aaron McKinney.
 

## License
[GNU GENERAL PUBLIC LICENSE Version 3](./LICENSE)

## Examples

##### Storing Data
```C++
#include "Database.h"

using Database = StaticDB::Database;
using namespace std;

int main() {
    Database db;
    db.open(filename,password);
    
    json j = { 
        { "username" , "John Doe"},
        { "type" , "admin"},
    };
    string key = "user";
    db.store(key,j);
    
    db.commit();
    db.close();
}
```

##### Retrieving Data
```C++
#include "Database.h"

using Database = StaticDB::Database;
using namespace std;

int main() {
    Database db;
    db.open(filename,password);
    
    string key = "user";
    json j = db.fetch(key);
    cout << j;
    
    db.close();
}
```

##### Deleting Data
```C++
#include "Database.h"

using Database = StaticDB::Database;
using namespace std;

int main() {
    Database db;
    db.open(filename,password);
    
    string key = "user";
    bool b = db.forget(key);
    if(b) cout << "Deleted Succussfully";
    
    db.close();
}
```

##### CRUD with Query
```C++
#include "Database.h"

using Database = StaticDB::Database;
using namespace std;

int main() {
    Database db;
    string query;
    db.open(filename,password);
    
    query = " key = { \"Apple\" : 50 } ";
    string response = db.execute(query);
    cout << response;
    
    db.close();
}
```


