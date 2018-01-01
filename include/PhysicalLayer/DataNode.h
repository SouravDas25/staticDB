#ifndef SDB_DATANODE_H
#define SDB_DATANODE_H

#include "utility.h"
#include "fileObj.h"

namespace StaticDB {


    bool isDataSignatureMatching(const FileLayer &f) {
        char sig[basedata_sig.size() + 1];
        f.readRaw(sig, sizeof(char) * basedata_sig.size()); //! signature
        sig[basedata_sig.size()] = 0;
        return stricmp(sig, basedata_sig) == 0;
    }


    class DataNode {

    public:
        enum DN_STATUS {
            NOT_CACHED = 0, CACHED, PENDING_WRITE
        };
        enum DN_TYPE {
            NO_TYPE = 0, INT, DOUBLE, STRING, VOID_PTR
        };

        /** Default constructor */
        DataNode() : _addr(0), _size(0), _data(nullptr), _status(NOT_CACHED), _type(NO_TYPE), _memory(0) {}

        /** Default destructor */
        virtual ~DataNode() {
            if (_data)operator delete(_data);
        }

        P_ADDR addr() {
            return _addr;
        }

        void addr(P_ADDR val) {
            _addr = val;
        }

        MEM_DATA data() {
            return _data;
        }

        void data(MEM_DATA val) {
            _data = val;
        }

        MEM_SIZE size() {
            return _size;
        }

        void size(MEM_SIZE val) {
            _size = val;
        }

        const char *typeToString() {
            switch (_type) {
                case NO_TYPE: {
                    return "NoType";
                }
                case INT: {
                    return "Int";
                }
                case DOUBLE: {
                    return "Double";
                }
                case STRING: {
                    return "String";
                }
                case VOID_PTR: {
                    return "Void Pointer";
                }
                default: {
                    return "Type Not Recognized";
                }
            }
        }

        DN_TYPE type() {
            return _type;
        }

        void type(DN_TYPE val) {
            _type = val;
        }

        MEM_SIZE memory() {
            return _memory;
        }

        void memory(MEM_SIZE val) {
            _memory = val;
        }

        DN_STATUS status() {
            return _status;
        }

        void status(DN_STATUS val) {
            _status = val;
        }

        const char *statusToString() {
            switch (_status) {
                case CACHED: {
                    return "Cached";
                }
                case NOT_CACHED: {
                    return "NotCached";
                }
                case PENDING_WRITE: {
                    return "PendingWrite";
                }
                default: {
                    return "Status Not Recognized";
                }
            }
        }

        static size_t headerSize(void) {
            return 2 * sizeof(MEM_SIZE) + sizeof(DN_TYPE) + basedata_sig.size() * sizeof(char);
        }

        void deCache() {
            if (this->data())operator delete(_data);
            this->data(nullptr);
            this->memory(0);
            this->size(0);
            this->status(NOT_CACHED);
        }

        void cache(FileLayer &f) {
            if (this->data()) {
                std::cout << "INCONSISTENCY STATE >> bn->data = NOT nullptr" << std::cout;
                return;
            }
            if (!this->addr()) {
                std::cout << "INCONSISTENCY STATE >> bn->addr = " << this->addr() << std::cout;
                return;
            }
            if (!f.isOpen()) f.open();
            if (!f.seek(this->addr())) {
                f.close();
                std::cout << "INCONSISTENCY STATE >> bn->addr = " << this->addr() << std::cout;
                return;
            }
            read(f);
            f.close();
        }

        template<typename T>
        bool modify(T *value, MEM_SIZE size, DN_TYPE type) {
            size_t nob = size * sizeof(T);
            if (size > this->size()) {
                if (this->data()) {
                    operator delete(_data);
                }
                _data = operator new(nob);
            }
            memcpy(_data, value, nob);
            this->size(nob);
            this->type(type);
            this->status(PENDING_WRITE);
            return true;
        }

        bool put(const MEM_DATA val, MEM_SIZE size) {
            return modify(&val, size, VOID_PTR);
        }

        bool put(const int64_t &val) {
            return modify(&val, 1, INT);
        }

        bool put(const double &val) {
            return modify(&val, 1, DOUBLE);
        }

        bool put(const char *val) {
            return modify(val, strlen(val), STRING);
        }

        bool put(const string &val) {
            const char *c = val.c_str();
            return put(c);
        }

        int64_t getInt() {
            if (type() != INT) {
                string s = "TypeError: Data is of type : ";
                throw sdb_error(s + typeToString());
            }
            return *(int64_t *) this->data();
        }

        double getDouble() {
            if (type() != DOUBLE) {
                string s = "TypeError: Data is of type : ";
                throw sdb_error(s + typeToString());
            }
            return *(double *) this->data();
        }

        string getString() {
            if (type() != STRING) {
                string s = "TypeError: Data is of type : ";
                throw sdb_error(s + typeToString());
            }
            string s((const char *) this->data(), this->size());
            return s;
        }

        bool getCStr(char *val) {
            if (type() != STRING) {
                string s = "TypeError: Data is of type : ";
                throw sdb_error(s + typeToString());
            }
            strncpy(val, (const char *) this->data(), this->size());
            return true;
        }

        MEM_SIZE getRaw(void *dest) {
            if (type() != VOID_PTR) {
                string s = "TypeError: Data is of type : ";
                throw sdb_error(s + typeToString());
            }
            if (this->status() != CACHED) {
                throw sdb_error("DataNode Not Cached.");
            }
            memcpy(dest, this->data(), this->size());
            return this->size();
        }

        int write(const FileLayer &f) {
            if (!f.seek(this->addr())) {
                cout << "INCONSISTENCY : bn->addr not seek_able " << this->addr() << endl;
                return false;
            }
            P_ADDR test = f.tell();
            f.writeRaw(basedata_sig.c_str(), sizeof(char) * basedata_sig.size());
            f.writeRaw(&_size, sizeof(MEM_SIZE));
            f.writeRaw(&_type, sizeof(DN_TYPE));
            f.writeRaw(&_memory, sizeof(MEM_SIZE));
            f.writeRaw(this->data(), (int) this->size());
            int i;
            for (i = (int) this->size(); i < this->memory(); ++i) {
                f.write(0);
            }
            if ((f.tell() - test) - this->memory() != DataNode::headerSize()) {
                cout << "INCONSISTENCY : DataNode calculated Header != actual write Header " << f.tell() - test << endl;
            }
            this->deCache();
            return true;
        }

        bool read(const FileLayer &f) {
            if (!f.seek(this->addr())) {
                cout << "INCONSISTENCY : bn->addr not seek_able " << this->addr() << endl;
                return false;
            }

            if (!isDataSignatureMatching(f)) {
                return false;
            }

            f.readRaw(&_size, sizeof(MEM_SIZE));
            f.readRaw(&_type, sizeof(DN_TYPE));
            f.readRaw(&_memory, sizeof(MEM_SIZE));

            if (this->size() == 0) {
                cout << "INCONSISTENCY : base Node Size " << this->size() << endl;
                return false;
            }

            this->data(operator new(this->size()));
            memset(this->data(), 0, this->size());

            f.readRaw(this->data(), (const int &) this->size());

            this->status(CACHED);
            return true;
        }

        void print(ostream &out) {
            out << "{ ";
            out << " Address : " << addr() << ",";
            out << " data : " << data() << ",";
            out << " Size : " << size() << ",";
            out << " type : " << typeToString() << ",";
            out << " memory : " << memory() << ",";
            out << " status : " << statusToString() << ",";
            out << " }";
        }


    protected:

    private:
        P_ADDR _addr; //!< Member variable "_addr"
        MEM_DATA _data; //!< Member variable "_data"
        MEM_SIZE _size; //!< Member variable "_size"
        DN_TYPE _type; //!< Member variable "_ref"
        MEM_SIZE _memory; //!< Member variable "_memory"
        DN_STATUS _status; //!< Member variable "_status"
    };

}

#endif // DATANODE_H
