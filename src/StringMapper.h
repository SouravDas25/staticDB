//
// Created by SD on 1/2/2018.
//

#ifndef STATICDB_STRINGMAPPER_H
#define STATICDB_STRINGMAPPER_H

#include "JsonStore.h"


namespace StaticDB {

    class StringMapper : protected JsonStore {

        static const L_ADDR GereratorIndex = 1;
        static const L_ADDR SizeIndex = 2;
        static const L_ADDR MapperIndex = 3;

        map<string, L_ADDR> stringTree;
        map<L_ADDR, L_ADDR> keyTree;
        L_ADDR autoIndexer;

        struct saveStruct {
            L_ADDR key, value;
            //saveStruct() : key(L_ADDR_NULL) , value(L_ADDR_NULL){}
        };

    protected:
        /*L_ADDR generateIndex() {
            return autoIndexer + 1;
        }*/

        L_ADDR incIndex() {
            return ++autoIndexer;
        }

        bool insertRaw(const string &key, L_ADDR keyIndex, L_ADDR dataIndex) {
            stringTree.insert(make_pair(key, keyIndex));
            insertKeyIndex(keyIndex, dataIndex);
            return true;
        }

        bool insertKeyIndex(L_ADDR keyIndex, L_ADDR dataIndex) {
            keyTree.insert(make_pair(keyIndex, dataIndex));
            return true;
        }

        L_ADDR addKey(const string &key) {
            L_ADDR dataIndex = incIndex();
            L_ADDR keyIndex = incIndex();
            JsonStore::storeString(keyIndex, key);
            insertRaw(key, keyIndex, dataIndex);
            return dataIndex;
        }

        L_ADDR getKeyIndex(const string &key) {
            if (!has(key)) {
                return L_ADDR_NULL;
            }
            return stringTree.at(key);
        }

        L_ADDR getDataIndex(const string &key) {
            if (!has(key)) {
                return L_ADDR_NULL;
            }
            L_ADDR keyIndex = getKeyIndex(key);
            return keyTree.at(keyIndex);
        }

    public:
        StringMapper() : JsonStore(), keyTree(), autoIndexer(MapperIndex) {
        }

        ~StringMapper() {}

        bool has(const string &key) {
            map<string, L_ADDR>::iterator it;
            try {
                it = stringTree.find(key);
            }
            catch (...) {
                return false;
            }
            return stringTree.end() != it;
        }

        json keys(){
            json j;
            for (auto it = stringTree.begin(); it != stringTree.end(); ++it) {
                j.push_back( it->first );
            }
            return j;
        }

        bool store(const string &key, const json &j) {
            if (has(key)) throw sdb_error("Key Already Present. ");
            L_ADDR index = addKey(key);
            return JsonStore::storeJson(index, j);
        }

        json fetch(const string &key) {
            L_ADDR index = getDataIndex(key);
            if (index == L_ADDR_NULL) throw sdb_error("Key Not Found. ");
            return getJson(index);
        }

        bool forget(const string &key) {
            L_ADDR keyIndex = getKeyIndex(key);
            L_ADDR dataIndex = keyTree.at(keyIndex);
            if (dataIndex == L_ADDR_NULL) return false;
            if (keyIndex == L_ADDR_NULL) return false;
            JsonStore::erase(dataIndex);
            JsonStore::erase(keyIndex);
            keyTree.erase(keyIndex);
            stringTree.erase(key);
            return true;
        }

        bool commit() {
            JsonStore::storeInt(GereratorIndex, autoIndexer);
            int64_t size = keyTree.size();
            JsonStore::storeInt(SizeIndex, size);
            saveStruct mapper[size];
            MEM_SIZE count = 0;
            map<L_ADDR, L_ADDR>::const_iterator it;
            for (it = keyTree.begin(); it != keyTree.end(); ++it) {
                mapper[count].key = it->first;
                mapper[count].value = it->second;
                count++;
            }
            JsonStore::storeRaw(MapperIndex, (MEM_DATA) &mapper, (MEM_SIZE) size * sizeof(saveStruct));
            //JsonStore::printBase(std::cout);
            return JsonStore::commit();
            //JsonStore::printBase(std::cout);
        }

        bool open(const string &filename, const string &pwd) {
            JsonStore::open(filename, pwd);
            if (JsonStore::has(GereratorIndex)) {
                autoIndexer = (L_ADDR) JsonStore::getInt(GereratorIndex);
                int64_t size = JsonStore::getInt(SizeIndex);
                if (size < 1) return true;
                saveStruct mapper[size];
                MEM_SIZE n = JsonStore::getRaw(MapperIndex, (MEM_DATA) &mapper);
                for (int i = 0; i < size; i++) {
                    L_ADDR dataIndex = mapper[i].value;
                    L_ADDR keyIndex = mapper[i].key;
                    const string &key = JsonStore::getString(keyIndex);
                    insertRaw(key, keyIndex, dataIndex);
                }

            }
            return true;
        }

        bool close() {
            stringTree.clear();
            keyTree.clear();
            return JsonStore::close();
        }

        void print(ostream &out) {
            out << "StringMapper : { " << endl;
            out << "KeyList : " << endl;
            for (auto it = stringTree.begin(); it != stringTree.end(); ++it) {
                out << "{";
                out << ' ' << it->first << " : " << it->second;
                out << "}" << endl;
            }
            out << "MapperList : " << endl;
            for (auto it = keyTree.begin(); it != keyTree.end(); ++it) {
                out << "{";
                out << ' ' << it->first << " : " << it->second;
                out << "}" << endl;
            }
            out << "}" << endl << endl;
            JsonStore::printBase(out);
        }


    };

}


#endif //STATICDB_STRINGMAPPER_H
