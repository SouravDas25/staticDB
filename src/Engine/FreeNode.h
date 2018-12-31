#ifndef SDB_FREENODE_H_INCLUDED
#define SDB_FREENODE_H_INCLUDED

#include "utility.h"
#include "DataNode.h"


namespace StaticDB
{

class FreeList;

class FreeNode
{
    friend class FreeList;
public:
    FreeNode() : addr(P_ADDR_NULL), size(0) {}

    void set(P_ADDR addr,MEM_SIZE size)
    {
        this->addr = addr;
        this->size = size;
    }

    P_ADDR get_addr() const
    {
        return addr;
    }

    MEM_SIZE get_size() const
    {
        return size;
    }

    void write(FileLayer& f)
    {
        f.writeRaw(&addr,sizeof(P_ADDR));
        f.writeRaw(&size,sizeof(MEM_SIZE));
    }

    void read(const FileLayer& f)
    {
        f.readRaw(&addr,sizeof(P_ADDR));
        f.readRaw(&size,sizeof(MEM_SIZE));
    }

    friend bool FreeNode_CMP (const FreeNode & i,const FreeNode& j);

private:
    P_ADDR addr;
    MEM_SIZE size;

};


bool FreeNode_CMP (const FreeNode & i,const FreeNode& j)
{
    return (i.addr<j.addr);
}


}










#endif // SDB_FREENODE_H_INCLUDED
