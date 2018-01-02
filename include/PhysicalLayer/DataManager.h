#ifndef SDB_DATAMANAGER_H
#define SDB_DATAMANAGER_H

#include "utility.h"
#include "fileObj.h"
#include "Base.h"

namespace StaticDB{


const string db_signature = "staticDB V1.0" ;

bool isSignatureMatching(const FileLayer& f,const string& s)
{
    char sig[s.size()+1];
    f.readRaw(sig,sizeof(char)*s.size()); //! signature
    sig[s.size()] = 0;
    if( stricmp(sig,s) != 0)
    {
        return false ;
    }
    return true;
}


typedef struct _sdbMasterDB
{
    P_ADDR baseArr;
    char password[32];

    _sdbMasterDB():baseArr(P_ADDR_NULL)
    {
        memset(password,0,sizeof(password));
    }
} sdbMasterDB;

P_ADDR default_start(void)
{
    return db_signature.size() + sizeof(sdbMasterDB);
}

class StoreHeader
{
public:
    StoreHeader(): baseAddress(default_start()), password()
    {
    }

    void close()
    {
        baseAddress = default_start();
        password = "";
    }

    P_ADDR getBaseAddr()
    {
        return baseAddress;
    }

    void setBaseAddress(P_ADDR ba)
    {
        baseAddress = ba;
    }

    bool setPassword(const string& pwd)
    {
        if( pwd.size() > 31)
        {
            throw sdb_error("Security Issue : Password Have To Be Between (0-31) Characters.");
        }
        password = pwd;
    }

    const string& getPassword()
    {
        return password;
    }

    void prepareNewDatabase(FileLayer& f)
    {
        writeSignature(f);
        writeHeader(f);
        //writeSignature(f);
    }

    static void writeSignature(const FileLayer& f)
    {
        f.write(db_signature);
    }

    static bool isSignatureMatch(const FileLayer& f)
    {
        if( f.getSize() < default_start()  ) return false;
        f.seek(0);
        if( isSignatureMatching(f,db_signature) )
        {
            //f.seek( -db_signature.size(), FileLayer::end );
            //if( isSignatureMatching(f,db_signature)) return true;
            return true;
        }
        return false;
    }

    static bool isAuthenticatedDB(const FileLayer& f)
    {
        if( isSignatureMatch(f) == false )
        {
            throw sdb_error("Not A Valid Database File, Database Signature Did Not Match.");
        }
        return true;
    }

    bool writeHeader(const FileLayer& f) //! READ and WRITE mdb struct
    {
        if( !f.seek(db_signature.size()) ) throw sdb_error("Base Origin Write Error.");
        sdbMasterDB mdb ;
        mdb.baseArr = this->baseAddress;
        strcpy(mdb.password,this->password.c_str());
        f.writeRaw(&mdb,sizeof(sdbMasterDB));
        return true;
    }

    bool readHeader(const FileLayer& f)
    {
        StoreHeader::isAuthenticatedDB(f);

        if( !f.seek(db_signature.size()) ) throw sdb_error("Base Origin Read Error.");
        sdbMasterDB mdb ;
        f.readRaw(&mdb,sizeof(sdbMasterDB));
        this->baseAddress = mdb.baseArr;
        string tp(mdb.password);
        if( verifyPassword(tp) == false )
        {
            throw sdb_error("Incorrect Password.");
        }
        password = tp;
    }

    bool verifyPassword(const string& pwd)
    {
        if(pwd != password) return false;
        return true;
    }


private:
    P_ADDR baseAddress;
    string password;
};



class DataManager
{

public:
    /** Default constructor */
    DataManager() : storefileName(), fileobj(), sh() , sb(fileobj)
    {
    }
    /** Default destructor */
    virtual ~DataManager()
    {
        if(fileobj.isOpen())
        {
            fileobj.close();
        }
    }

    virtual bool open(const string &filename, const string &pwd)
    {
        storefileName = filename;
        sh.setPassword(pwd);
        if( !FileLayer::fileExists(this->storefileName) )
        {
            CreateNewStore();
        }
        readStore();
        return true;
    }

    virtual bool close()
    {
        if(fileobj.isOpen())
        {
            fileobj.close();
        }
        sb.close();
        sh.close();
        storefileName = "";
        return true;
    }

    //! INTERFACE

    /*bool storeInt(L_ADDR key,const int32_t val)
    {
        return sb.setInt(key,(int64_t)val);
    }*/

    bool storeInt(L_ADDR key, const int64_t val)
    {
        return sb.setInt(key, val);
    }

    bool storeDouble(L_ADDR key, const double val)
    {
        return sb.setDouble(key, val);
    }

    bool storeString(L_ADDR key, const string &val)
    {
        return sb.setString(key, val);
    }

    bool storeRaw(L_ADDR key, const MEM_DATA val, MEM_SIZE size)
    {
        return sb.setRaw(key, val, size);
    }

    int64_t getInt(L_ADDR key)
    {
        return sb.getInt(key);
    }

    double getDouble(L_ADDR key)
    {
        return sb.getDouble(key);
    }

    string getString(L_ADDR key)
    {
        return sb.getString(key);
    }

    MEM_SIZE getRaw(L_ADDR key, MEM_DATA val)
    {
        return sb.getRaw(key,val);
    }

    MEM_SIZE getSize(L_ADDR key)
    {
        return sb.getSize(key);
    }

    bool erase(L_ADDR index) {
        return sb.erase(index);
    }

    bool has(L_ADDR index) {
        return sb.has(index);
    }

    virtual bool commit()
    {
        writeStore();
        return true;
    }

    virtual bool rollback()
    {
        return sb.rollback();
    }

    void printBase(ostream &out)
    {
        sb.print(out);
    }

protected:
    void CreateNewStore()
    {
        fileobj.open(storefileName,"wb");
        sh.prepareNewDatabase(fileobj);
        fileobj.close();
    }

    void writeStore()
    {
        fileobj.open(storefileName,"rb+");
        P_ADDR ba = sb.commit();
        sh.setBaseAddress(ba);
        sh.writeHeader(fileobj);
        fileobj.close();
    }

    void readStore()
    {
        fileobj.open(storefileName,"rb+");
        sh.readHeader(fileobj);
        sb.readBase(fileobj,sh.getBaseAddr());
        fileobj.close();
    }

private:
    string storefileName;
    FileLayer fileobj;
    StoreHeader sh;
    Base sb;
};

}
#endif // SDB_DATAMANAGER_H
