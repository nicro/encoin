#ifndef RESPONSES_H
#define RESPONSES_H

#include <string>
#include <iostream>
#include <nlohmann/json.hpp>
#include <variant>
#include <transaction.h>
#include <block.h>
#include <net/peer.h>
#include <blockchain.h>

namespace encoin {

using nlohmann::json;

class node;

class message {
public:
    message() = default;
    virtual ~message() {};
    virtual std::string type() = 0;
    virtual json handle_response(node*, json j) = 0;

    template <class arg>
    static message *match(const std::string &name)
    {
        if (arg *obj = new arg; auto ptr = reinterpret_cast<message*>(obj))
        {
            return ptr->type() == name ? obj : nullptr;
        }
        return nullptr;
    }

    template <class arg, class arg2, class ...args>
    static message *match(const std::string &name)
    {
        if (auto ptr = match<arg>(name))
        {
            return ptr;
        }
        return match<arg2, args...>(name);
    }
};

template <class pt>
class broadcast_message: public message {
public:
    using ptype = pt;
    virtual std::string build_request(const pt &payload) = 0;
};

class new_tx_message: public broadcast_message<transaction> {
public:
    json handle_response(node*, json request) override;
    std::string build_request(const transaction &tx) override;
    std::string type() override { return "new_tx"; }
};
class new_block_message: public broadcast_message<block> {
public:
    json handle_response(node*, json request) override;
    std::string build_request(const block &bl) override;
    std::string type() override { return "new_block"; }
};

template <class rt>
class query_message: public message {
public:
    using rtype = rt;
    virtual rt parse_result(json j) = 0;
};

class get_pool_message: public query_message<tx_list> {
public:
    json handle_response(node *node, json request) override;
    virtual tx_list parse_result(json j) override;
    std::string type() override { return "get_pool"; }
};
class get_latest_block_message: public query_message<block> {
public:
    json handle_response(node *node, json request) override;
    virtual block parse_result(json j) override;
    std::string type() override { return "get_latest_block"; }
};
class get_chain_message: public query_message<block_list> {
public:
    json handle_response(node *node, json request) override;
    virtual block_list parse_result(json j) override;
    std::string type() override { return "get_chain"; }
};

}

#endif // RESPONSES_H
