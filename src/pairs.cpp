

#ifndef _SATATICPAIRS_DB_C
#define _SATATICPAIRS_DB_C



#include "../include/pairs.h"

namespace StaticDB {

    typedef struct _pairBase {
        map<uint64_t,sdb_pair*>* pairs;
        baseIndex pair_index_start;
    } pairBase;

    using namespace std;

    pairBase* ads_pairBaseCreate()
    {
        pairBase * pb = new pairBase;
        pb->pairs = new map<uint64_t,sdb_pair*>();
        pb->pair_index_start = 0;
        return pb;
    }

    void ads_pairDestroy(pairBase* pb)
    {
        for ( map<uint64_t,sdb_pair*>::iterator it = pb->pairs->begin(); it != pb->pairs->end(); ++it  )
        {
            delete it->second ;
        }
        delete pb->pairs;
        delete pb;
    }

    int ads_pairCount(pairBase* pb)
    {
        return pb->pairs->size();
    }

    tuple ads_pairKeys(staticDB * db)
    {
        pairBase* pb = getDB_PairTree(db);

        tuple t = tuple( ads_pairCount(pb) ,true);

        for ( map<uint64_t,sdb_pair*>::iterator it = pb->pairs->begin(); it != pb->pairs->end(); ++it  )
        {
            const string s = baseNodeCStr(db,it->second->keyname);
            t.append_string(s);
        }
        return t;
    }

    bool pairExists(staticDB * db,const string& keyname)
    {
        if(!db)return false;
        pairBase* pb = getDB_PairTree(db);
        if(!pb)return false;
        map<uint64_t,sdb_pair*>::iterator it = pb->pairs->find(cstrHash(keyname));
        if (it != pb->pairs->end()) return true;
        return false;
    }

    sdb_pair* getPairNode(staticDB * db,const string& key)
    {
        if(!db)return nullptr;
        if(pairExists(db,key))
        {
            pairBase* pb = getDB_PairTree(db);
            int64_t index = cstrHash(key);
            sdb_pair * p = pb->pairs->at(index) ;
            return p;
        }
        return nullptr;
    }

    nosqlRT pairInsert(staticDB * db,const tuple& kt,const tuple& t)
    {
        if(!db)return noSql_Msg(ERROR_MSG,"Database Not Valid | Not Connected");

        if(t.empty() || t.size() == 0) return noSql_Msg(ERROR_MSG,"Cannot Insert Null Tuple.");

        sdb_pair * p = getPairNode(db,kt.get_string(0));

        if(!p) return  noSql_Msg(ERROR_MSG,"Key Not Present In Database.");

        storeStruct rt = get_tupleIndex(db,p->data,kt,1);
        if( rt.t != db_TUPLE && rt.t != db_ARRAY ) return noSql_Msg(ERROR_MSG,"Value Cannot Be Inserted Into The Given key.");

        extendBuffer(db,rt.bi,t);
        return noSql_Msg(SUCCESS_MSG,"Key Value Inserted.");
    }

    nosqlRT pairCreate(staticDB * db,const string& key,const tuple& t)
    {
        if( identifierValidity(key.c_str(),key.size()) == false )//t.print()
        {
            return noSql_Msg(ERROR_MSG,"Key Name Not Valid.");
        }
        //t.print();
        if(t.empty() || t.size() == 0)
        {
            return noSql_Msg(ERROR_MSG,"Cannot Insert Null Tuple.");
        }
        if(!db)
        {
            return noSql_Msg(ERROR_MSG,"Database Not Valid | Not Connected");
        }
        int64_t data = tupleToIndex(db,t);

        sdb_pair * p = getPairNode(db,key.c_str());
        if(p == nullptr)
        {
            p = new sdb_pair;
            p->keyname = baseInsertCStr(db,key);
            getDB_PairTree(db)->pairs->insert( make_pair( cstrHash(key),p) );
        }
        else baseDelete(db,p->data);
        p->data = data;
        return noSql_Msg(SUCCESS_MSG,"New Key Created.");
    }

    nosqlRT ads_pairFetch(staticDB * db,const tuple& kt)
    {
        if(!db)return noSql_Msg(ERROR_MSG,"Database Not Valid | Not Connected");
        if( kt.size()<1)return noSql_Msg(ERROR_MSG,"Key Not Present In Database.");

        sdb_pair * p = getPairNode(db,kt.get_string(0));

        if(!p) return noSql_Msg(ERROR_MSG,"Key Not Present In Database.");
        tuple rt ;
        if(kt.size() == 1)
        {
            rt = index_to_tuple(db,p->data);
        }
        else
        {
            storeStruct srt = get_tupleIndex(db,p->data,kt,1);
            if( is_null_storeStruct(srt) )
            {
                return noSql_Msg(ERROR_MSG,"Key Modifier Incorrect.");
            }
            rt = storeStruct_to_tuple(db,&srt,1);
        }
        if( rt.empty() )
        {
            return noSql_Msg(ERROR_MSG,"Key | Key Modifier Not Present In Database.");
        }
        return noSql_ReturnTuple(rt);
    }

    nosqlRT pair_get(staticDB * db,const string& identifier)
    {
        tuple kt = splitKey(identifier);
        if( stricmp(kt.get_string(0),"$globals") == 0 )
        {
            tuple rt;
            rt.append( get_db_globals(db).traverse(kt,2));
            return noSql_ReturnTuple(rt);
        }
        if( kt.size() && kt.get_type(0) == db_STRING)
        {
            if( pairExists(db,kt.get_string(0)) )
            {
                return ads_pairFetch(db,kt);
            }
        }
        return noSql_Msg(ERROR_MSG,string_sprintf("Identifier '%s' Not Present In Database.",identifier.c_str()));
    }

    tuple get_db_globals(staticDB * db)
    {
        if(!db) return noSql_Msg(ERROR_MSG,"Database Not Valid | Not Connected");
        tuple rt(3) ;
        rt.append_array(ads_pairKeys(db),"identifiers");
        rt.append_string(string_sprintf("StaticDb v%s",get_version().c_str()),"version");
        rt.append_int(get_cache_size(db),"cache_size");
        return rt;
    }

    nosqlRT ads_pairDelete(staticDB * db,const tuple& kt)
    {
        if(!db)
        {
            return noSql_Msg(ERROR_MSG,"Database Not Valid | Not Connected");
        }
        sdb_pair * p = getPairNode(db,kt.get_string(0));
        if(!p)
        {
            return noSql_Msg(ERROR_MSG,"Key Not Present In Database.");
        }
        bool b = true;
        if(kt.size() == 1)
        {
            getDB_PairTree(db)->pairs->erase( cstrHash(kt.get_string(0)) );
            if(p->data)
            {
                deleteBuffer(db,p->data);
            }
            baseDelete(db,p->keyname);
            delete p;
        }
        else
        {
            b = delete_tupleIndex(db,p->data,kt,1);
        }
        if(b)
        {
            return noSql_Msg(SUCCESS_MSG,"Key Deleted From Database.");
        }
        return noSql_Msg(ERROR_MSG,"Key Cannot Be Deleted From Database.");
    }

    nosqlRT pairUpdate(staticDB * db,const tuple& kt,const tuple& t)
    {
        if(!db)
        {
            return noSql_Msg(ERROR_MSG,"Database Not Valid | Not Connected");
        }
        if(t.empty() || t.size() == 0)
        {
            return noSql_Msg(ERROR_MSG,"Cannot Insert Null Tuple.");
        }
        if( kt.size()<1)
        {
            return noSql_Msg(ERROR_MSG,"Key Not Present In Database.");
        }
        sdb_pair * p = getPairNode(db,kt.get_string(0));
        if(!p)
        {
            return noSql_Msg(ERROR_MSG,"Key Not Present In Database.");
        }
        if(kt.size() == 1)
        {
            return noSql_Msg(ERROR_MSG,"Absolute Key Cannot Be Updated Create Another Key With That Name To Replace.");
        }
        else
        {
            bool b = update_tupleIndex(db,p->data,kt,1,t);
            if( !b )
            {
                return noSql_Msg(ERROR_MSG,"Value Cannot Be Updated.");
            }
        }
        return noSql_Msg(SUCCESS_MSG,"Value Updated.");
    }

    bool pairRead(staticDB * db,baseIndex pos)
    {
        if(!db) return false;
        if( pos == BaseIndex_NULL )return false;
        pairBase* pb = getDB_PairTree(db);
        int i,len = baseNodeSize(db,pos)/sizeof(sdb_pair);
        const byte_array ba = baseData(db,pos);
        const sdb_pair * pairIndex = (const sdb_pair*)ba.data();
        if(!pairIndex)return false;
        for(i=0;i<len;i++)
        {
            sdb_pair * p = new sdb_pair;
            *p = pairIndex[i];
            pb->pairs->insert( make_pair( cstrHash( baseNodeCStr(db,p->keyname) ) ,p ) );
        }
        //baseDelete(db,pos);
        pb->pair_index_start = pos;
        return true;
    }

    baseIndex pairWrite(staticDB * db)
    {
        pairBase* pb = getDB_PairTree(db);
        if( pb->pairs->size() <= 0 ) return BaseIndex_NULL;

        sdb_pair pairIndex[pb->pairs->size()+1];
        memset(pairIndex,0,sizeof(sdb_pair)*pb->pairs->size()+1);
        int cnt = 0;
        for ( map<uint64_t,sdb_pair*>::iterator it = pb->pairs->begin(); it != pb->pairs->end(); ++it  )
        {
            sdb_pair* p = it->second;
            pairIndex[cnt++] = *p;
        }
        if( pb->pair_index_start == 0 ) pb->pair_index_start = baseInsert(db,pairIndex,cnt*sizeof(sdb_pair));
        else baseUpdate(db, pb->pair_index_start , pairIndex , cnt*sizeof(sdb_pair) );
        return pb->pair_index_start;
    }


}

#endif
