#ifndef STATICDB_BASE_H
#define STATICDB_BASE_H


#include "DataNode.h"
#include "FreeList.h"

using namespace std;

namespace StaticDB
{


class Base
{

public:
    Base(FileLayer& fo) : indexTree(), cacheList(), cacheSize(20), fl(), fileobj(fo)
    {
    }

    ~Base()
    {
        for ( map<L_ADDR,DataNode*>::iterator it = indexTree.begin(); it != indexTree.end(); ++it  )
        {
            delete (it->second);
        }
    }


    DataNode * _FetchOrCreateDN(L_ADDR key)
    {
        DataNode * bn = getKey(key);
        if(bn == nullptr) bn = new DataNode;
        return bn;
    }

    bool _insertUpdateIC(L_ADDR key,DataNode *bn)
    {
        indexTree.insert( make_pair(key,bn) );
        removeCache(key);
        return true;
    }

    //! INTERFACE

    bool setRaw(const L_ADDR key, const MEM_DATA val, const MEM_SIZE size)
    {
        if(!val) return false ;
        DataNode * bn = _FetchOrCreateDN(key);
        if (!bn->putRaw(val, size)) {
            operator delete(bn);
            return false;
        }
        _insertUpdateIC(key,bn);
        return true;
    }

    bool setInt(L_ADDR key, const int64_t &val)
    {
        DataNode * bn = _FetchOrCreateDN(key);
        bn->putInt(val);
        _insertUpdateIC(key,bn);
        return true;
    }

    bool setDouble(L_ADDR key, const double &val)
    {
        DataNode * bn = _FetchOrCreateDN(key);
        bn->putDouble(val);
        _insertUpdateIC(key,bn);
        return true;
    }

    bool setString(L_ADDR key, const string &val)
    {
        DataNode * bn = _FetchOrCreateDN(key);
        bn->putString(val);
        _insertUpdateIC(key,bn);
        return true;
    }

    int64_t getInt(L_ADDR key)
    {
        DataNode * bn = getKey(key);
        if(bn == nullptr) throw sdb_error("Key Not Found. ");
        return bn->getInt();
    }

    double getDouble(L_ADDR key)
    {
        DataNode * bn = getKey(key);
        if(bn == nullptr) throw sdb_error("Key Not Found. ");
        return bn->getDouble();
    }

    string getString(L_ADDR key)
    {
        DataNode * bn = getKey(key);
        if(bn == nullptr) throw sdb_error("Key Not Found. ");
        return bn->getString();
    }

    MEM_SIZE getRaw(L_ADDR key, void *val)
    {
        DataNode * bn = getKey(key);
        if(bn == nullptr) throw sdb_error("Key Not Found. ");
        return bn->getRaw(val);
    }

    MEM_SIZE getSize(L_ADDR key)
    {
        DataNode * bn = getKey(key);
        if(bn == nullptr) throw sdb_error("Key Not Found. ");
        return bn->size();
    }

    bool erase(L_ADDR key)
    {
        DataNode * bn = getKey(key);
        if( bn == nullptr )
        {
            return false;
        }
        fl.insert(bn->addr(),bn->memory());
        indexTree.erase(key);
        removeCache(key);
        delete bn;
        return true;
    }

    bool has(L_ADDR index)
    {
        map<L_ADDR,DataNode*>::iterator it;
        try
        {
            it = this->indexTree.find(index);
        }
        catch(...)
        {
            return false;
        }
        return it != this->indexTree.end();
    }

    DataNode* getKey(L_ADDR key)
    {
        if (!this->has(key))
        {
            return nullptr;
        }
        DataNode * bn = indexTree.at(key) ;
        if(bn->status() == DataNode::NOT_CACHED)
        {
            this->cacheInsert(key);
            bn->cache(this->fileobj);
        }
        return bn;
    }


    uint64_t count()
    {
        return this->indexTree.size();
    }

    void _insertPair(L_ADDR key,P_ADDR addr)
    {
        DataNode * dn = new DataNode;
        dn->addr(addr);
        this->indexTree.insert(make_pair(key,dn));
    }

    void writeIndex(FileLayer& f)
    {
        MEM_SIZE len = this->count();
        f.writeRaw(&len,sizeof(MEM_SIZE));
        for ( map<L_ADDR,DataNode*>::iterator it = indexTree.begin(); it != indexTree.end(); ++it  )
        {
            P_ADDR pa = it->second->addr();
            L_ADDR index = it->first;
            f.writeRaw(&index,sizeof(L_ADDR));  //! index
            f.writeRaw(&pa,sizeof(P_ADDR)); //! address
        }
    }

    P_ADDR writeBase()
    {
        FileLayer& f = fileobj;
        fl.shrinkAdjacent();
        P_ADDR pos = fl.getEmptyAddress();
        if( !fileobj.seek(fl.getEmptyAddress()) ) return P_ADDR_NULL;
        f.writeRaw(baseIndex_sig.c_str(), (int) (baseIndex_sig.size() * sizeof(char))); //! signature
        writeIndex(f);
        fl.writeFreelist(f);
        return pos;
    }

    void readIndex(const FileLayer& f)
    {
        MEM_SIZE len = 0,i;
        L_ADDR index = 0 ;
        f.readRaw(&len,sizeof(MEM_SIZE));
        for(i =0; i< len ; i++)
        {
            P_ADDR pd = P_ADDR_NULL;
            f.readRaw(&index,sizeof(L_ADDR));       //! index
            f.readRaw(&pd,sizeof(P_ADDR));      //! address of index
            _insertPair(index,pd);
        }
    }

    bool readBase(const FileLayer& f,P_ADDR baseAddr)
    {
        if( baseAddr == 0) return false;
        if( !f.seek(baseAddr) ) return false;
        fl.setEmptyAddress(baseAddr);

        char sig[baseIndex_sig.size()+1];
        f.readRaw(sig, sizeof(char)*baseIndex_sig.size() );  //! signature
        sig[baseIndex_sig.size()] = 0;
        if( stricmp(sig,baseIndex_sig) != 0) return false;

        readIndex(f);
        fl.readFreelist(f);
        return true;
    }

    bool rollback()
    {
        return true;
    }

    P_ADDR commit()
    {
        if(!fileobj.isOpen())
        {
            throw sdb_error("File Not open For Writting");
        }
        commitAllData();
        return writeBase();
    }

    bool commitAllData()
    {
        const FileLayer& f = fileobj;
        //int amt_written = 0;
        if( this->fl.getEmptyAddress() ==  P_ADDR_NULL )
        {
            throw sdb_error("Not A Valid File To Commit To BaseIndex Origin Not Found.");
        }
        for ( map<L_ADDR,DataNode*>::iterator it = indexTree.begin(); it != indexTree.end(); ++it  )
        {
            DataNode * bn = it->second;
            if( bn->status() == DataNode::PENDING_WRITE )
            {
                if( bn->hasSufficientValidMemory() )
                {
                    bn->write(f);
                    //amt_written += bn->memory + baseNode_utility_disk_size();
                }
                else
                {
                    if( this->fl.getEmptyAddress() ==  P_ADDR_NULL )
                    {
                        throw sdb_error("INCONSISTENCY STATE >> EmptyAddress = NULL ");
                    }
                    if( bn->addr() && bn->memory() )
                    {
                        fl.insert(bn->addr(),bn->memory());
                    }
                    fl.updateMemory(*bn);
                    bn->write(f);
                    //amt_written += bn->memory + baseNode_utility_disk_size();
                }
            }
        }
        //cout << "\nAmount of Data Written : " << amt_written << " bytes." << endl;
        return true;
    }

    size_t getCacheSize()
    {
        return this->cacheSize;
    }

    bool removeCache(L_ADDR key)
    {
        vector <L_ADDR>& v = cacheList;
        auto it = std::find(v.begin(), v.end(), key);
        if (it != v.end())
        {
            using std::swap;
            swap(*it, v.back());
            v.pop_back();
        }
        return true;
    }

    bool setCacheSize(size_t size)
    {
        if(size > 4  && size <= 512)
        {
            this->cacheSize = size;
            return true;
        }
        return false;
    }

    void cacheInsert(L_ADDR index)
    {
        vector <L_ADDR> & cl = this->cacheList;
        if( cl.size() > this->cacheSize )
        {
            const L_ADDR& deli = cl[0];
            if (this->indexTree.count(deli))
            {
                DataNode& bn = *this->indexTree.at(deli);
                if(bn.status() == DataNode::CACHED )
                {
                    bn.deCache();
                }
            }
            cl.erase( cl.begin() );
        }
        cl.push_back(index);
    }


    void print( ostream &out ) const
    {
        out << "{ " << endl ;
        out << "IndexList : " << endl ;
        for(auto it = indexTree.begin(); it != indexTree.end(); ++it)
        {
            out << ' ' << it->first << " : " ;
            it->second->print(out) ;
            out << "," << endl;
        }
        out << "FreeList : " << endl ;
        fl.print(out);
        out << "CacheList : " ;
        out << "[" ;
        for(auto it = cacheList.begin(); it != cacheList.end(); ++it)
        {
            out << *it << "," ;
        }
        out << "]"<< endl ;
        out << "}"<< endl ;
    }


    void close()
    {
        map<L_ADDR,DataNode*>::const_iterator it;
        for( it = indexTree.begin(); it != indexTree.end(); ++it)
        {
            delete it->second;
        }
        indexTree.clear();
        cacheList.clear();
        fl.close();
    }


private:

    map <L_ADDR,DataNode*> indexTree;
    vector <L_ADDR> cacheList;
    FreeList fl;
    size_t cacheSize;
    FileLayer & fileobj;
};

}

#endif // staticDATABASESTRING_H




