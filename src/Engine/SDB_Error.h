#ifndef SDB_ERROR_H
#define SDB_ERROR_H

#include <exception>
#include <string>

namespace StaticDB {

    struct sdb_error : public std::exception {
        const std::string _msg;

        sdb_error(const std::string &msg) : _msg(msg) {};

        virtual const char *what() const throw() {
            return _msg.c_str();
        }

        ~sdb_error() throw() {};
    };

}


#endif // SDB_ERROR_H
