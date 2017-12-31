#include "staticDB.h"

staticDB::staticDB() {
    //ctor
}

staticDB::~staticDB() {
    //dtor
}

staticDB::staticDB(const staticDB &other) {
    //copy ctor
}

staticDB &staticDB::operator=(const staticDB &rhs) {
    if (this == &rhs) return *this; // handle self assignment
    //assignment operator
    return *this;
}
