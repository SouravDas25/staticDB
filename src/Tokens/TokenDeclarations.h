//
// Created by SD on 12/30/2018.
//

#ifndef STATICDB_TOKENDECLARATIONS_H
#define STATICDB_TOKENDECLARATIONS_H

#include "../Intefaces/TokenInterface.h"


namespace StaticDB{
    namespace TokenImpl{

        class Nop : public TokenInterface{
        public:
            const string execute(StringMapper& stringMapper,const SymbolTree &symbolTree) const;
        };

        class Store : public TokenInterface {
        public:
            const string execute(StringMapper& stringMapper,const SymbolTree &symbolTree) const;
        };

        class Fetch : public TokenInterface {
        public:
            const string execute(StringMapper& stringMapper,const SymbolTree &symbolTree) const;
        };

        class Help : public TokenInterface{
        public:
            const string execute(StringMapper& stringMapper,const SymbolTree &symbolTree) const;
        };

        class Keys : public TokenInterface{
        public:
            const string execute(StringMapper& stringMapper,const SymbolTree &symbolTree) const;
        };

        class Commit : public TokenInterface{
        public:
            const string execute(StringMapper& stringMapper,const SymbolTree &symbolTree) const;
        };

        class Debug : public TokenInterface{
        public:
            const string execute(StringMapper& stringMapper,const SymbolTree &symbolTree) const;
        };

        class Delete : public TokenInterface{
        public:
            const string execute(StringMapper& stringMapper,const SymbolTree &symbolTree) const;
        };
    }



}


#endif //STATICDB_TOKENDECLARATIONS_H
