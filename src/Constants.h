//
// Created by SD on 12/29/2018.
//

#ifndef STATICDB_CONSTANTS_H
#define STATICDB_CONSTANTS_H

#include "Intefaces/TokenInterface.h"
#include "Tokens/TokenDeclarations.h"

#define __STATICDB_HELP_TEXT \
COMMANDS  : \n\
 - STORE  : key = value \n\
 - DELETE : del key \n\
 - COMMIT : commit \n\
 - KEYS   : $keys \n\
 - FETCH  : key.attribute \n\

#define SDV_STRINGIFY(x) #x
#define SDB_TOSTRING(x) SDV_STRINGIFY(x)

#define STATICDB_HELP_TEXT SDB_TOSTRING(__STATICDB_HELP_TEXT)

namespace StaticDB {

    namespace TOKEN {
        const TokenInterface *NOP = new TokenImpl::Nop();
        const TokenInterface *STORE = new TokenImpl::Store();
        const TokenInterface *FETCH = new TokenImpl::Fetch();
        const TokenInterface *HELP = new TokenImpl::Help();
        const TokenInterface *KEYS = new TokenImpl::Keys();
        const TokenInterface *COMMIT = new TokenImpl::Commit();
        const TokenInterface *DEBUG = new TokenImpl::Debug();
        const TokenInterface *DELETE = new TokenImpl::Delete();
    }


    namespace KEYWORDS {
        const string HELP = "help";
        const string DELETE = "del";
        const string DEBUG = "debug";
        const string COMMIT = "commit";
        const string KEYS = "$keys";
    };


}

#include "Tokens/BasicTokens.h"

#endif //STATICDB_CONSTANTS_H
