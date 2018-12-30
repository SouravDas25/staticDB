//
// Created by SD on 12/30/2018.
//

#ifndef STATICDB_TOKENINTERFACE_H
#define STATICDB_TOKENINTERFACE_H


#include "../PhysicalLayer/Utility.h"

namespace StaticDB {
    class StringMapper;
    class SymbolTree;

    class TokenInterface {
        static const string empty;
    public:
        virtual const string execute(StringMapper& sm,const SymbolTree& symbolTree) const =0;

    };

    const string TokenInterface::empty = "";
}


#endif //STATICDB_TOKENINTERFACE_H
