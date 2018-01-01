#ifndef SDB_FREELIST_H
#define SDB_FREELIST_H


#include "FreeNode.h"

namespace StaticDB
{


class FreeList
{
public:
    /** Default constructor */
    FreeList() : fl(), emptyAddress(P_ADDR_NULL)
    {

    }
    /** Default destructor */
    virtual ~FreeList() {}

    void setEmptyAddress(P_ADDR ndna)
    {
        emptyAddress = ndna;
    }

    P_ADDR getEmptyAddress()
    {
        return emptyAddress;
    }

    void insert(P_ADDR addr,MEM_SIZE memory)
    {
        if(!addr) return;
        FreeNode fn;
        fn.set(addr,memory + DataNode::headerSize());
        this->fl.push_back( fn );
    }

    void insertRaw(P_ADDR addr,MEM_SIZE size)
    {
        if(!addr) return;
        FreeNode fn;
        fn.set(addr,size);
        this->fl.push_back( fn );
    }

    const FreeNode& at(int i)
    {
        return fl.at(i);
    }

    uint64_t count()
    {
        return this->fl.size();
    }

    bool is_side_by_side(int i,int p)
    {
        const FreeNode& fi = this->fl.at(i);
        const FreeNode& fp = this->fl.at(p);
        if( fi.get_addr()+fi.get_size() == fp.get_addr() )return true;
        if( fp.get_addr()-fi.get_size()== fi.get_addr() )return true;
        return false;
    }

    void shrinkAdjacent()
    {
        vector<FreeNode>& v = this->fl;
        sort (v.begin(),v.end(),FreeNode_CMP);
        for ( uint64_t i = 0 ; i < v.size() ; ++i )
        {
            for ( uint64_t p = 0 ; p < v.size() ; ++p )
            {
                if(this->is_side_by_side(i,p) && v[i].addr != v[p].addr )
                {
                    if(v[i].addr > v[p].addr) v[i].addr = v[p].addr;
                    v[i].size += v[p].size;
                    v.erase( v.begin() + p );
                    p=-1;
                }
            }
        }
    }


    P_ADDR requestMemory(size_t req_size,MEM_SIZE & memory) //! VERY SENSITIVE FUNCTION
    {
        this->shrinkAdjacent();
        vector<FreeNode>& v = this->fl;
        for ( uint64_t i = 0 ; i < v.size() ; ++i )
        {
            if( v[i].size >= req_size )
            {
                FreeNode & d = v[i];
                if( req_size/static_cast<float>(d.size) > 0.75 )
                {
                    memory = d.get_size();
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
        if(!this->emptyAddress) throw sdb_error("Base New Data Start Address Error.");
        P_ADDR addr = this->emptyAddress;
        this->emptyAddress += req_size;
        return addr;
    }

    void updateMemory(DataNode & bn)
    {
        MEM_SIZE basic = DataNode::headerSize();
        MEM_SIZE size_got = 0;
        MEM_SIZE min_size = bn.size() + basic;
        P_ADDR na = requestMemory(min_size,size_got);
        bn.addr(na);
        bn.memory(size_got - basic);
    }

    void writeFreelist(FileLayer& f)
    {
        MEM_SIZE len = count();
        f.writeRaw(&len,sizeof(MEM_SIZE)); //! length of free memory index;
        for ( MEM_SIZE i = 0 ; i < len ; ++i )
        {
            FreeNode& fn = fl.at(i);
            fn.write(f); //! address and size of the memory
        }
    }

    void readFreelist(const FileLayer& f)
    {
        MEM_SIZE len = 0;
        int i;
        f.readRaw(&len,sizeof(MEM_SIZE)); //! length of free memory index;
        for ( i = 0 ; i < len ; ++i )
        {
            FreeNode fn;
            fn.read(f);
            fl.push_back(fn);
        }
    }

    void print( ostream &out ) const
    {
        for(auto it = fl.begin(); it != fl.end(); ++it)
        {
            out << "{ addr :" <<  it->get_addr() <<  " ,size :" << it->get_size() << " }" << endl ;
        }
    }

    void close()
    {
        fl.clear();
        emptyAddress = P_ADDR_NULL;
    }
protected:

private:
    vector <FreeNode> fl;
    P_ADDR emptyAddress;
};

}

#endif // FREELIST_H
