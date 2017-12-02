
#ifndef staticDATABASESTRING_C
#define staticDATABASESTRING_C

#include "../include/base.h"

namespace StaticDB {

    enum data_status { NOT_CACHED = 0 , CACHED , PENDING_WRITE };

    typedef struct _baseNode {
        filepos addr;
        void * data;
        int size;
        int ref;
        int memory;
        data_status status;
    }baseNode;

    struct delNode {
        filepos addr;
        filepos size;
    };

    typedef struct _stringBase {
        map <baseIndex,baseNode*> * base;
        vector <delNode> * base_del;
        vector <baseIndex>* cache_list;
        size_t cache_size;
        baseIndex index_count;
        filepos base_index_pos;
    }stringBase;

    static void base_cache(baseNode * bn,const db_file& f);
    static baseNode * baseGetNode(staticDB * db,baseIndex index);

    stringBase* baseCreate(const db_file& fp,filepos fip)
    {
        stringBase * b = new stringBase;
        b->base = new map<baseIndex,baseNode*>();
        b->base_del = new vector<delNode>();
        b->cache_list = new vector<baseIndex>();
        b->base_index_pos = (fip)?fip:default_start();
        b->index_count = 0;
        b->cache_size = 20;
        baseRead( b , fp , fip );
        return b;
    }

    void baseDestroy(stringBase*sb)
    {
        for ( map<baseIndex,baseNode*>::iterator it = sb->base->begin(); it != sb->base->end(); ++it  )
        {
            if(it->second->data) operator delete (it->second->data);
            delete (it->second);
        }
        delete sb->cache_list;
        delete sb->base_del;
        delete sb->base;
        delete sb;
    }

    static baseIndex getIndex(stringBase *sb)
    {
        return ++sb->index_count;
    }

    int get_cache_size(staticDB * db)
    {
        return getDB_BaseTree(db)->cache_size;
    }

    nosqlRT set_cache_size(staticDB * db , size_t size)
    {
        if(size > 4  && size <= 512)
        {
            getDB_BaseTree(db)->cache_size = size;
            return noSql_Msg(SUCCESS_MSG,"Cache Size Change Successful.");
        }
        return noSql_Msg(SUCCESS_MSG,"Cache Size Must Be Between(4 - 512).");
    }

    static void reset_baseNode(baseNode * bn)
    {
        if(bn->data)operator delete (bn->data);
        bn->data = nullptr;
        bn->memory = 0;
        bn->ref = 0;
        bn->size = 0;
        bn->status = NOT_CACHED;
    }

    static void cache_insert(stringBase *sb,baseIndex index)
    {
        vector <baseIndex> & cl = *sb->cache_list;
        if( cl.size() > sb->cache_size )
        {
            baseIndex deli = cl[0];
            if (sb->base->count(deli))
            {
                baseNode * bn = sb->base->at(deli);
                if(bn->status == CACHED )
                {
                    reset_baseNode(bn);
                }
            }
            cl.erase( cl.begin() );
        }
        cl.push_back(index);
    }

    /*
    void basePrint(staticDB * db)
    {
        stringBase * sb = getDB_BaseTree(db);
        for ( map<uint64_t,baseNode*>::iterator it = sb->base->begin(); it != sb->base->end(); ++it  )
        {
            baseNode* bn  = baseGetNode(db,it->first);
            printf("\n\nIndex : %I64d \n",it->first);
            printf("\tsize : %d\n ",bn->size);
            printf("\tref : %d\n ",bn->ref);
            printf("\tmemory : %d\n ",bn->memory);
            printf("\tFile-address : %I64d \n",bn->addr);
            int i;
            void * data = bn->data;
            printf("Data : \n\t-> ");
            for(i = 0;i<bn->size;i+= sizeof(int64_t))printf("%I64d,",*static_cast<int64_t*>(data+i));
            printf("\n\t-> ");
            for(i = 0;i<bn->size;i+= sizeof(int32_t))printf("%d,",*(int32_t*)(data+i));
            printf("\n\t-> ");
            for(i = 0;i<bn->size;i+= sizeof(int16_t))printf("%d, ",*(int16_t*)(data+i));
            printf("\n\t-> ");
            for(i = 0;i<bn->size;i+= sizeof(int8_t) )printf("%d,",*(int8_t*)(data+i));
            printf("\n\t-> ");
            for(i = 0;i<bn->size;i+= sizeof(char) )printf("%c,",*(char*)(data+i));
        }
    }*/

    string base_mem_detail(staticDB * db)
    {
        stringBase * sb = getDB_BaseTree(db);
        int memory = 0;
        stringstream det ;
        for ( map<baseIndex,baseNode*>::iterator it = sb->base->begin(); it != sb->base->end(); ++it  )
        {
            baseNode * bn= baseGetNode(db,it->first);
            memory += bn->memory;
        }
        det << " memory : " << memory;
        det << " index_size : " << baseCount(sb)*( sizeof(filepos) + sizeof(baseIndex) + baseNode_utility_disk_size() );
        memory = 0;
        for(size_t i =0;i<base_del_count(sb);++i)
        {
            memory += sb->base_del->at(i).size;
        }
        det << " del_mem : " << memory << " del_index :" << (base_del_count(sb)*sizeof(delNode));
        return det.str();
    }

    static baseNode* baseGetNode(staticDB * db,baseIndex index)
    {
        stringBase * sb = getDB_BaseTree(db);
        if( base_HasIndex(db,index) == false )
        {
            return nullptr;
        }
        baseNode * bn = sb->base->at(index) ;
        if(bn->status == NOT_CACHED)
        {
            cache_insert(sb,index);
            base_cache(bn,get_db_file(db));
        }
        return bn;
    }

    uint64_t baseCount(stringBase * sb)
    {
        return sb->base->size();
    }

    uint64_t base_del_count(stringBase * sb)
    {
        return sb->base_del->size();
    }

    string base_del_print(staticDB * db)
    {
        stringBase * sb = getDB_BaseTree(db);
        stringstream det;
        det << "size : " << base_del_count(sb) << " [ ";
        for(uint64_t i =0;i<base_del_count(sb);++i)
        {
            det << "memory : " << sb->base_del->at(i).size << " " ;
            det << "addr : " << sb->base_del->at(i).addr << " , " ;
        }
        det << " ]" << std::endl;
        return det.str();
    }

    bool base_HasIndex(staticDB * db,baseIndex index)
    {
        stringBase * sb = getDB_BaseTree(db);
        map<baseIndex,baseNode*>::iterator it;
        try {
            it = sb->base->find(index);
        }
        catch(...)
        {
            return false;
        }
        if(it != sb->base->end() ) return true;
        return false;
    }

    baseIndex baseInsertCStr(staticDB * db,const string& str)
    {
        stringBase * sb = getDB_BaseTree(db);
        int size = str.size()+1;
        baseIndex ret = getIndex(sb)+1;
        baseNode * bn = new baseNode;
        bn->addr = 0;
        bn->ref = 1;
        bn->size = size;
        bn->memory = 0;
        bn->status = PENDING_WRITE;
        bn->data = operator new (size);
        memset(bn->data,0,size);
        strcpy(static_cast<char*>(bn->data),str.c_str());
        sb->base->insert( make_pair(ret,bn) );
        return ret;
    }

    baseIndex baseInsert(staticDB * db,const void * data,int size)
    {
        stringBase * sb = getDB_BaseTree(db);
        if(!data) return 0 ;
        baseIndex ret = getIndex(sb)+1;
        baseNode * bn = new baseNode;
        bn->addr = 0;
        bn->ref = 1;
        bn->size = size;
        bn->memory = 0;
        bn->status = PENDING_WRITE;
        bn->data = operator new (size);
        memset(bn->data,0,size);
        memcpy(bn->data,data,size);
        sb->base->insert( make_pair(ret,bn) );
        return ret;
    }

    bool baseUpdate(staticDB * db,baseIndex index,const void * data,int size)
    {
        stringBase * sb = getDB_BaseTree(db);
        if(!sb)return false;
        baseNode * bn = baseGetNode(db,index) ;
        if( bn == nullptr )
        {
            return false;
        }
        if(bn->data) operator delete (bn->data);
        bn->data = operator new (size);
        memset(bn->data,0,size);
        memcpy(bn->data,data,size);
        bn->size = size;
        bn->status = PENDING_WRITE;
        return true;
    }

    bool baseExtend(staticDB * db,baseIndex index,const void * data,int size)
    {
        stringBase * sb = getDB_BaseTree(db);
        if(!sb)return false;
        baseNode * bn = baseGetNode(db,index) ;
        if( bn == nullptr )
        {
            return false;
        }
        void * extdata = operator new (size+bn->size);
        memset(extdata,0,size+bn->size);
        memcpy(extdata,bn->data,bn->size);
        memcpy((byte*)extdata+bn->size,data,size);
        if(bn->data)operator delete (bn->data);
        bn->data = extdata;
        bn->size = size + bn->size;
        bn->status = PENDING_WRITE;
        return true;
    }

    const string baseNodeCStr(staticDB * db,baseIndex index)
    {
        stringBase * sb = getDB_BaseTree(db);
        if(!sb) return string();
        baseNode * bn = baseGetNode(db,index) ;//bn->size
        if( bn == nullptr )
        {
            return string();
        }
        const char * tmp = static_cast<char*>(bn->data);
        if(!tmp)return string();
        return string(tmp);
    }

    const byte_array baseData(staticDB * db,baseIndex index)
    {
        stringBase * sb = getDB_BaseTree(db);
        if(!sb)return byte_array();
        baseNode * bn = baseGetNode(db,index) ;
        if( bn == nullptr )
        {
            return byte_array();
        }
        int8_t * data = static_cast<int8_t*>(bn->data);
        return byte_array(data,data+bn->size/sizeof(int8_t));
    }

    int baseNodeSize(staticDB * db,baseIndex index)
    {
        stringBase * sb = getDB_BaseTree(db);
        if(!sb)return -1;
        baseNode * bn = baseGetNode(db,index) ;
        if( bn == nullptr )
        {
            return -1;
        }
        return bn->size;
    }

    bool baseDelete(staticDB * db,baseIndex index)
    {
        stringBase * sb = getDB_BaseTree(db);
        if(!sb)return false;
        baseNode * bn = baseGetNode(db,index);
        if( bn == nullptr )
        {
            return false;
        }
        bn->ref--;//bn->size
        if(bn->ref < 1)
        {
            if(bn->data)operator delete (bn->data);
            base_del_insert(sb,bn->addr,bn->memory);
            delete bn;
            sb->base->erase(index);
        }
        return true;
    }

    /*
    void base_Force_CommitAll(stringBase * sb,FILE *fp)
    {
        if(!sb) return;
        fseek(fp,default_start(),SEEK_SET);
        for ( map<baseIndex,baseNode*>::iterator it = sb->base->begin(); it != sb->base->end(); ++it  )
        {
            baseNode * bn = it->second;
            bn->addr = ftell(fp);
            fwrite(&bn->size,sizeof(int),1,fp);
            fwrite(&bn->ref,sizeof(int),1,fp);

            if(!bn->memory)bn->memory = bn->size + ( bn->size >> 1 ) + 1;
            fwrite(&bn->memory,sizeof(int),1,fp);

            fwrite(bn->data,1,bn->size,fp);
            for(int i=bn->size;i<bn->memory;++i)fputc(0,fp);
            bn->status = WRITTEN;
        }
        sb->base_index_pos = ftell(fp);
    }*/

    /**  DISK IO START EDIT WITH CAUTION **/

    const string basedata_sig("bd");
    const string baseIndex_sig("bi");

    size_t baseNode_utility_disk_size(void)
    {
        return 3*sizeof(int)+basedata_sig.size()*sizeof(char);
    }

    static bool read_bn(baseNode*bn,const db_file& f)
    {
        if( !f.seek( bn->addr ) )
        {
            cout << "INCONSISTENCY : bn->addr not seek_able " << bn->addr << endl;
            return false;
        }

        char sig[basedata_sig.size()+1];
        f.read(sig,sizeof(char)*basedata_sig.size()); //! signature
        sig[basedata_sig.size()] = 0;
        if( stricmp(sig,basedata_sig) != 0)
        {
            cout << "INCONSISTENCY : base Node Sig Not match "<< bn->addr << endl;
            return false ;
        }

        f.read(&bn->size,sizeof(int));
        f.read(&bn->ref,sizeof(int));
        f.read(&bn->memory,sizeof(int));

        if( bn->size == 0 )
        {
            cout << "INCONSISTENCY : base Node Size "<< bn->size << endl;
            return false;
        }

        bn->data = operator new (bn->size);
        memset(bn->data,0,bn->size);

        f.read(bn->data,bn->size);

        bn->status = CACHED;
        return true;
    }

    static bool write_bn(baseNode*bn,const db_file& f)
    {
        if( !f.seek( bn->addr ) )
        {
            cout << "INCONSISTENCY : bn->addr not seek_able " << bn->addr << endl;
            return false;
        }
        filepos test = f.tell();
        f.write( basedata_sig.c_str() , sizeof(char)*basedata_sig.size() );
        f.write(&bn->size,sizeof(int));
        f.write(&bn->ref,sizeof(int));
        f.write(&bn->memory,sizeof(int));
        f.write(bn->data,bn->size);
        for(int i=bn->size;i<bn->memory;++i)f.write(0);
        if( ( f.tell() - test ) - bn->memory != baseNode_utility_disk_size()  )
        {
            cout << "INCONSISTENCY : dn sig error " << f.tell()-test << endl;
        }
        reset_baseNode(bn);
        return true;
    }

    void base_del_insert(stringBase * sb,filepos addr,int64_t memory)
    {
        delNode dn;
        if(!addr) return;
        dn.addr = addr;
        dn.size = memory + baseNode_utility_disk_size();
        sb->base_del->push_back( dn );
    }

    static bool is_side_by_side(stringBase * sb,int i,int p)
    {
        delNode& di = sb->base_del->at(i);
        delNode& dp = sb->base_del->at(p);
        if( di.addr+di.size == dp.addr )return true;
        if( dp.addr-di.size == di.addr )return true;
        return false;
    }

    static bool delNode_cmp (delNode i,delNode j) { return (i.addr<j.addr); }

    static void shrink_adjacent(stringBase * sb)
    {
        vector<delNode>& v = *sb->base_del;
        sort (v.begin(),v.end(),delNode_cmp);
        for ( uint64_t i = 0 ; i < v.size() ; ++i )
        {
            for ( uint64_t p = 0 ; p < v.size() ; ++p )
            {
                if(is_side_by_side(sb,i,p) && v[i].addr != v[p].addr )
                {
                    if(v[i].addr > v[p].addr) v[i].addr = v[p].addr;
                    v[i].size += v[p].size;
                    v.erase( v.begin() + p );
                    p=-1;
                }
            }
        }
    }

    static filepos get_mem_for_write(stringBase * sb,size_t req_size,int & memory) //! VERY SENSITIVE FUNCTION
    {
        shrink_adjacent(sb);
        vector<delNode>& v = *sb->base_del;
        for ( uint64_t i = 0 ; i < v.size() ; ++i )
        {
            if( v[i].size >= req_size )
            {
                delNode d = v[i];
                if( req_size/static_cast<float>(d.size) > 0.75  )
                {
                    memory = d.size;
                    v.erase( v.begin() + i );
                }
                else
                {
                    memory = req_size ;
                    v[i].size -= req_size;
                    v[i].addr += req_size;
                }
                return d.addr;
            }
        }
        memory = req_size ;
        filepos addr = sb->base_index_pos;
        sb->base_index_pos += req_size;
        return addr;
    }

    static void update_mem_for_write(stringBase * sb,baseNode*bn)
    {
        int basic = baseNode_utility_disk_size();
        int size_got = 0;
        int min_size = bn->size + basic + ( bn->size >> 1 );
        bn->addr = get_mem_for_write(sb,min_size,size_got);
        bn->memory = size_got - basic;
    }

    nosqlRT base_Commit(stringBase * sb,const db_file& f)
    {
        //int amt_written = 0;
        if(sb->base_index_pos == 0)
        {
            return noSql_Msg(ERROR_MSG,"Not A Valid File To Commit To BaseIndex Origin Not Found.");
        }
        for ( map<baseIndex,baseNode*>::iterator it = sb->base->begin(); it != sb->base->end(); ++it  )
        {
            baseNode * bn = it->second;
            if( bn->status == PENDING_WRITE )
            {
                if(bn->size <= bn->memory && bn->addr )
                {
                    write_bn(bn,f);
                    //amt_written += bn->memory + baseNode_utility_disk_size();
                }
                else
                {
                    if( !sb->base_index_pos )
                    {
                        return noSql_Msg(ERROR_MSG,"INCONSISTENCY STATE >> base_index_pos = 0");
                    }
                    if( bn->addr && bn->memory )
                    {
                        base_del_insert(sb,bn->addr,bn->memory);
                    }
                    update_mem_for_write(sb,bn);
                    write_bn(bn,f);
                    //amt_written += bn->memory + baseNode_utility_disk_size();
                }
            }
        }
        //cout << "\nAmount of Data Written : " << amt_written << " bytes." << endl;
        return noSql_Msg(SUCCESS_MSG,"base commited properly.");
    }

    static void base_cache(baseNode * bn,const db_file& f)
    {
        if(!bn)
        {
            cout << "INCONSISTENCY STATE >> bn = nullptr" << endl;
            return;
        }
        if(bn->data)
        {
            cout << "INCONSISTENCY STATE >> bn->data = NOT nullptr" << endl;
            return;
        }
        if(!bn->addr)
        {
            cout << "INCONSISTENCY STATE >> bn->addr = " << bn->addr << endl;
            return;
        }
        if( !f.seek(bn->addr) )
        {
            cout << "INCONSISTENCY STATE >> bn->addr = " << bn->addr << endl;
            return;
        }
        read_bn(bn,f);
    }

    /*
    void baseCacheAll(stringBase * sb,FILE *fp)
    {
        for ( map<baseIndex,baseNode*>::iterator it = sb->base->begin(); it != sb->base->end(); ++it  )
        {
            baseNode * bn = it->second;
            if(bn->status == NOT_CACHED )base_cache(bn,fp);
        }
    }*/

    void baseWrite(stringBase * sb,const db_file& f,filepos * pos)
    {
        shrink_adjacent(sb);
        *pos = sb->base_index_pos;
        if( !f.seek(sb->base_index_pos) ) return;
        uint64_t baseNode_count = baseCount(sb);

        f.write(baseIndex_sig.c_str(),baseIndex_sig.size()*sizeof(char)); //! signature

        f.write(&baseNode_count,sizeof(uint64_t));
        f.write(&sb->index_count,sizeof(baseIndex)); //! base_index count
        for ( map<baseIndex,baseNode*>::iterator it = sb->base->begin(); it != sb->base->end(); ++it  )
        {
            baseNode*bn = it->second;
            baseIndex index = it->first;
            f.write(&index,sizeof(baseIndex));  //! index
            f.write(&bn->addr,sizeof(filepos)); //! address
        }
        uint64_t del_len = base_del_count(sb);

        f.write(&del_len,sizeof(uint64_t)); //! length of free memory index;
        for ( uint64_t i = 0 ; i < del_len ; ++i )
        {
            delNode dn = sb->base_del->at(i);
            f.write(&dn,sizeof(delNode)); //! address and size of the memory
        }
    }

    //#define DEBUG

    bool baseRead(stringBase * sb,const db_file& f,filepos pos)
    {
        if( pos == 0) return false;
        if( !f.seek(pos) ) return false;
        uint64_t i;
        baseIndex index =0 ;
        baseNode * bn;
        uint64_t baseNode_count = 0;

        char sig[baseIndex_sig.size()+1];
        f.read(sig , sizeof(char)*baseIndex_sig.size() ); //! signature
        sig[baseIndex_sig.size()] = 0;
        if( stricmp(sig,baseIndex_sig) != 0)return false;

        f.read(&baseNode_count,sizeof(uint64_t));
        f.read(&sb->index_count,sizeof(baseIndex)); //! base_index count
        #ifdef DEBUG
        cout << "bnc : " << baseNode_count << " ic : " << sb->index_count << endl;
        #endif
        for(i =0;i< baseNode_count ;i++)
        {
            bn = new baseNode;
            memset(bn,0,sizeof(baseNode));
            bn->status = NOT_CACHED;                    //! status Not Cached
            f.read(&index,sizeof(baseIndex));       //! index
            f.read(&bn->addr,sizeof(filepos));      //! address of index
            #ifdef DEBUG
            cout << "index : " << index << " ref : " << bn->addr << endl;
            #endif
            sb->base->insert( make_pair(index,bn) );
        }
        uint64_t del_len = 0;
        f.read(&del_len,sizeof(uint64_t)); //! length of free memory index;
        #ifdef DEBUG
        cout << "del_len : " << del_len << endl;
        #endif
        for ( i = 0 ; i < del_len ; ++i )
        {
            delNode dn ;
            f.read(&dn,sizeof(delNode)); //! address and size of the memory
            #ifdef DEBUG
            cout << "size : " << dn.size << " ref : " << dn.addr << endl;
            #endif
            sb->base_del->push_back(dn);
        }
        return true;
    }

}

#endif // staticDATABASESTRING_C


