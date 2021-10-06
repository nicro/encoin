#ifndef WALLET_H
#define WALLET_H

#include "ecdsa.h"

#include <vector>
#include <memory>
#include <list>
#include <iostream>
#include <sqlite_orm/sqlite_orm.h>

namespace encoin {

struct keypair {
    std::string private_key;
    std::string public_key;
};

typedef std::string address_t;

namespace sql = sqlite_orm;

template <typename... Args>
inline auto make_storage_query() {
    auto keys = sql::make_table("keys",
        sql::make_column("public_key", &keypair::public_key, sql::unique()),
        sql::make_column("private_key", &keypair::private_key, sql::unique())
    );
    return sql::make_storage("/Users/nicro/Desktop/db.sqlite", keys);
}

class wallet {
public:
    using storage_t = decltype (make_storage_query());

    wallet();
    ~wallet();

    address_t create_new_address();
    std::list<address_t> addresses();

    std::string get_private_key(const std::string &public_key);
    static address_t pubkey_to_address(const key_t &pubkey_hex);

protected:
    storage_t _storage;
};

};

#endif // WALLET_H
