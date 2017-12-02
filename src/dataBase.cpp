
#ifndef _SATATICDATABSE_DB_C
#define _SATATICDATABSE_DB_C


#include "../include/dataBase.h"

namespace StaticDB {


struct _staticDB {
    db_file f;
    string password;
    pairBase * pb;
    stringBase * sb;

    _staticDB() : f() , password() , pb(nullptr) ,sb(nullptr) {};
    _staticDB(const _staticDB& other) = delete;
    operator=(const _staticDB& other) = delete;

};

typedef struct _sdbMasterDB {
    filepos baseArr;
    baseIndex pairIndex;
    char passwrd[32];
} sdbMasterDB;

static bool checkPassword(const string& s1,const string& enc);
static nosqlRT readDB(const db_file& fp,sdbMasterDB * mdb);


nosqlRT ads_connectDB(staticDB** addr_db,const string& file,const string& password)
{
    if( password.size() > 31 )
    {
        return noSql_Msg(ERROR_MSG,"Security Issue : Password Have To Be Between (0-31) Characters.");
    }
    if( !fileExists(file) )
    {
        sdbMasterDB mdb;
        memset(&mdb,0,sizeof(mdb));
        string p = encrypt(password);
        strcpy(mdb.passwrd,p.c_str());
        db_write_dummy(file.c_str(),&mdb);
    }
    db_file f;
    f.open(file,"rb+");
    sdbMasterDB mdb;
    memset(&mdb,0,sizeof(mdb));
    if( readDB(f,&mdb) == ERROR_MSG )
    {
        return ERROR_MSG;
    }
    if( checkPassword(password,mdb.passwrd) == false)
    {
        return noSql_Msg(ERROR_MSG,"Security Issue : Incorrect Password.");
    }
    *addr_db = new staticDB;
    staticDB * db = *addr_db;
    db->f = f;
    db->password = encrypt(password);
    db->sb = baseCreate(f,mdb.baseArr);
    db->pb = ads_pairBaseCreate();
    if(mdb.pairIndex)  pairRead(db,mdb.pairIndex);
    return noSql_Msg(SUCCESS_MSG,"Database Successfully Connected.");
}

void ads_disconnectDB(staticDB*db)
{
    ads_pairDestroy(db->pb);
    baseDestroy(db->sb);
    db->f.close();
    delete db;
    sdb_freeNoSqlReturnTuple();
}

filepos default_start(void)
{
    return db_signature.size() + sizeof(sdbMasterDB);
}

const db_file& get_db_file(staticDB * db)
{
    return db->f;
}

stringBase* getDB_BaseTree(staticDB * db)
{
    return db->sb;
}

pairBase* getDB_PairTree(staticDB * db)
{
    return db->pb;
}

/*
static void printFileTill(FILE*fp,int till,char * msg)
{
    printf("\n\nStart Printing : %s \n'",msg);
    int i = 0;
    fseek(fp,0,SEEK_SET);
    while(i<till)
    {
        printf("%c",fgetc(fp));
        i++;
    }
    fseek(fp,till,SEEK_SET);
    printf("'\n");
}*/

static bool checkPassword(const string& s1,const string& enc)
{
    string d = decrypt(enc);
    if(s1 == d) return true;
    return false;
}

static nosqlRT databaseIO(bool RoW,sdbMasterDB * mdb,const db_file& f) //! READ and WRITE mdb struct
{
    if(RoW == WRITE)
    {
        if( !f.seek(db_signature.size()) ) return noSql_Msg(ERROR_MSG,"Base Origin Write Error.");
        f.write(mdb,sizeof(sdbMasterDB));
    }
    else
    {
        if( !f.seek(db_signature.size()) ) return noSql_Msg(ERROR_MSG,"Base Origin Read Error.");
        f.read(mdb,sizeof(sdbMasterDB));
    }
    return SUCCESS_MSG;
}

static nosqlRT isAuthenticatedDB(const db_file& f)
{
    if( isSignatureMatch(f) == false )
    {
        return noSql_Msg(ERROR_MSG,"Not A Valid Database File, Database Signature Did Not Match.");
    }
    return SUCCESS_MSG;
}

static nosqlRT readDB(const db_file& f,sdbMasterDB * mdb)
{
    memset(mdb,0,sizeof(sdbMasterDB));
    if( isAuthenticatedDB(f) == ERROR_MSG )
    {
        return ERROR_MSG;
    }
    return databaseIO(READ,mdb,f);
}

static void ads_dbReadyForWrite(staticDB * db,sdbMasterDB * mdb)
{
    mdb->pairIndex = pairWrite(db);
}

void db_write_dummy(const char * file,sdbMasterDB * mdb)
{
    db_file f;
    f.open(file,"wb");
    writeSignature(f);
    databaseIO(WRITE,mdb,f);

    writeSignature(f);
    f.close();
}

nosqlRT ads_dbCommit(staticDB * db)
{
    sdbMasterDB mdb ;
    memset(&mdb,0,sizeof(mdb));

    ads_dbReadyForWrite(db,&mdb);

    const db_file& f = get_db_file(db);

    if( base_Commit(db->sb,f) == ERROR_MSG ) return ERROR_MSG;

    baseWrite(db->sb,f,&mdb.baseArr);
    strcpy(mdb.passwrd,db->password.c_str());

    databaseIO(WRITE,&mdb,f);

    if( isSignatureMatch(f) == false )
    {
        f.seek(0,db_file::end);
        writeSignature(f);
    }
    return noSql_Msg(SUCCESS_MSG,"Database Successfully Committed.");
}

}

#endif // _SATATICDATABSE_DB_C
