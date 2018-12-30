//
// Created by SD on 12/30/2018.
//

#ifndef STATICDB_SYMBOLTREE_H
#define STATICDB_SYMBOLTREE_H

#include "Constants.h"
#include "PhysicalLayer/Utility.h"

namespace StaticDB {

    class SymbolTree {
    private:
        const TokenInterface * type = TOKEN::NOP;
        string value,key;
        bool error;
        string msg;

    public:

        const string execute(StringMapper& stringMapper){
            return type->execute(stringMapper,*this);
        }

        void setType(const TokenInterface* type) {
            this->type = type;
        }

        const string &getKey() const {
            return key;
        }

        void setKey(const string &key) {
            this->key = key;
        }

        const string &getValue() const {
            return value;
        }

        void setValue(const string &value) {
            this->value = value;
        }

        bool isError() const {
            return error;
        }

        void setError(bool error) {
            this->error = error;
        }

        const string &getMsg() const {
            return msg;
        }

        void setMsg(const string &msg) {
            this->msg = msg;
        }
    };
}

#endif //STATICDB_SYMBOLTREE_H
