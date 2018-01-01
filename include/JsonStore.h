#ifndef JSONSTORE_H
#define JSONSTORE_H

#include "PhysicalLayer/DataManager.h"
#include "json.hpp"

using json = nlohmann::json;

namespace StaticDB {


    class JsonStore : public DataManager {
    public:
        /** Default constructor */
        JsonStore() {}

        /** Default destructor */
        virtual ~JsonStore() {}

        bool storeJson(L_ADDR key, json j) {
            string s = j.dump();
            return store(key, s);
        }

        json getJson(L_ADDR key) {
            string s = getString(key);
            return json::parse(s);
        }


    protected:

    private:
    };

}
#endif // JSONSTORE_H
