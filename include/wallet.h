#ifndef WALLET_H
#define WALLET_H

#include <ecdsa.h>

#include <sqlite_orm/sqlite_orm.h>
#include <list>

namespace encoin {

struct keypair {
    std::string public_key;
    std::string private_key;
};

namespace sql = sqlite_orm;

template <typename... Args>
inline auto make_storage_query() {
    auto keys = sql::make_table("keys",
        sql::make_column("public_key", &keypair::public_key, sql::primary_key()),
        sql::make_column("private_key", &keypair::private_key)
    );
    return sql::make_storage("db.sqlite", keys);
}

class wallet {
public:
    using storage_t = decltype (make_storage_query());

    wallet();
    ~wallet();

    void remove_all();
    address_t create_new_address();
    std::list<address_t> addresses();

    std::string get_private_key(const std::string &pubkey);
    static address_t pubkey_to_address(const std::string &pubkey_hex);

protected:
    storage_t _storage;
};

};

#endif // WALLET_H
