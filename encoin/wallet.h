#ifndef WALLET_H
#define WALLET_H

#include <crypto/ecdsa.h>
#include <transaction.h>
#include <sqlite_orm/sqlite_orm.h>
#include <list>

namespace encoin {

struct keypair {
    pubkey_t public_key;
    privkey_t private_key;
};

namespace sql = sqlite_orm;

template <typename... Args>
inline auto create_wallet_storage() {
    auto keys = sql::make_table("keys",
        sql::make_column("public_key", &keypair::public_key, sql::primary_key()),
        sql::make_column("private_key", &keypair::private_key)
    );
    return sql::make_storage("db.sqlite", keys);
}

class wallet {
public:
    using storage_t = decltype(create_wallet_storage());

    wallet();
    ~wallet() = default;

    void remove_all();
    pubkey_t create_new_address();
    std::list<pubkey_t> addresses();
    pubkey_t get_active_address();

    privkey_t get_private_key(const pubkey_t &pubkey);
    transaction send(const pubkey_t &to, const amount_t &amount);

protected:
    storage_t _storage;
};

};

#endif // WALLET_H
