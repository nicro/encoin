#ifndef WALLET_H
#define WALLET_H

#include "ecdsa.h"

#include <vector>
#include <memory>
#include <list>

namespace encoin {

typedef key_t address_t;

class wallet {
public:
    wallet();
    ~wallet();

    address_t create_new_address();
    std::list<address_t> addresses() const;

    void save();

protected:
    std::list<std::shared_ptr<ec_point>> _keypairs;
};

};

#endif // WALLET_H
