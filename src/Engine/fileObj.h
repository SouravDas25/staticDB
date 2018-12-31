#ifndef SFileSOBJ_H
#define SFileSOBJ_H

#include "utility.h"
#include "file/File.hpp"

extern int errno;

namespace StaticDB
{
typedef uint64_t P_ADDR;
typedef uint8_t BYTE;
typedef vector<BYTE> BYTE_ARRAY;

class FileLayer : public File
{
    string filename;
    string mode;
public :

    FileLayer() : File() {};

    FileLayer(const FileLayer& df) = delete;

    const FileLayer& operator=(const FileLayer& df) = delete;

    virtual ~FileLayer() {};

    bool setFileName(const string& fn,const string& m)
    {
        filename = fn;
        this->mode = m;
        return true;
    }

    bool open(const string& fn,const string& m)
    {
        setFileName(fn,m);
        return load(fn.c_str(),m.c_str());
    }

    bool open()
    {
        return load(filename.c_str(),mode.c_str());
    }

    char read(void) const
    {
        return this->getC();
    }

    int read(BYTE_ARRAY& ba,int length) const
    {
        if( !this->isOpen() ) return 0;
        BYTE * bt = new BYTE[length];
        memset(bt,0,length);
        int red = this->readRaw((void*)bt,length,1);
        ba = BYTE_ARRAY(bt,bt+red/sizeof(BYTE));
        delete bt;
        return red;
    }

    BYTE_ARRAY read(int length) const
    {
        BYTE_ARRAY ba;
        read(ba,length);
        return ba;
    }

    BYTE_ARRAY read(P_ADDR pos,int length) const
    {
        if( !seek(pos) ) return BYTE_ARRAY();
        BYTE_ARRAY ba;
        read(ba,length);
        return ba;
    }

    int write(int byt) const
    {
        FILE * fp = this->getFilePtr();
        return fputc(byt,fp);
    }

    int write(const BYTE_ARRAY& ba) const
    {
        return this->writeRaw(ba.data(),ba.size());
    }

    int write(const string& s) const
    {
        return this->writeRaw(s.data(),s.size());
    }

    P_ADDR tell() const
    {
        FILE * fp = this->getFilePtr();
#if UINTPTR_MAX == 0xffffffffffffffff
        return ftello64(fp);
#else
        return ftell(fp);
#endif
    }

    bool seek(int64_t pos,int whence = FileLayer::start) const
    {
        FILE * fp = this->getFilePtr();
#if UINTPTR_MAX == 0xffffffffffffffff
        if ( fseeko64(fp,pos,whence) != 0 )
        {
            cerr << endl << strerror( errno ) << endl ;
            return false;
        }
#else
        if ( fseek(fp,pos,whence) != 0 )
        {
            cerr << endl << strerror( errno ) << endl ;
            return false;
        }
#endif
        return true;
    }

    static const int end = SEEK_END;
    static const int current = SEEK_CUR;
    static const int start = SEEK_SET;

    static bool fileExists(const string& file)
    {
        FILE * fp = fopen(file.c_str(),"r");
        if(fp)
        {
            fclose(fp);
            return true;
        }
        return false;
    }
};

ostream& operator<<(ostream& out,BYTE_ARRAY ba)
{
    out << '"';
    for(unsigned int i=0; i<ba.size(); ++i)
    {
        if( ba[i] == '"' ) out << '\\' << ba[i];
        else if( ba[i] == '\\' ) out << '\\' << ba[i];
        else if( isprint (ba[i]) != 0) out << ba[i];
        else out << "\\x"<< int(ba[i]);
    }
    out << '"';
    return out;
}


}

#endif
