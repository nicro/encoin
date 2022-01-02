#ifndef RESPONSES_H
#define RESPONSES_H

#include <string>
#include <iostream>
#include <nlohmann/json.hpp>
#include <variant>
#include <transaction.h>
#include <block.h>

namespace encoin {

using nlohmann::json;

class node;

class message {
public:
    message() = default;
    virtual ~message() {};
    virtual std::string type() = 0;
    virtual json make_response(node*, json) = 0;

    static std::string parse_type(const json &j)
    {
        return j.contains("type") ? j["type"] : "";
    }

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

    static std::string get_type(json &j) { return j.contains("type") ? j["type"] : ""; }
    static std::string get_status(json &j) { return j.contains("status") ? j["status"] : ""; }
    static std::string get_payload(json &j) { return j.contains("payload") ? j["payload"] : ""; }

    static void set_type(json &j, const std::string &tp) { j["type"] = tp; }
    static void set_status(json &j, const std::string &st) { j["status"] = st; }
    static void set_payload(json &j, const json &pl) { j["payload"] = pl; }


};

template <class rtype = json, class atype = std::monostate>
class callable_message: public message {
public:
    using arg_type = atype;
    virtual rtype make_request(node*, const atype&) = 0;
};


class new_tx_message: public callable_message<json, transaction> {
public:
    new_tx_message() = default;

    json make_response(node *node, json request) override;
    json make_request(node *node, const transaction &tx) override;

    std::string type() override { return "new_tx"; }
};

class new_block_message: public callable_message<json, block> {
public:
    new_block_message() = default;

    json make_response(node *node, json request) override;
    json make_request(node *node, const block &bl) override;

    std::string type() override { return "new_block"; }
};

class get_peers_message: public callable_message<> {
public:
    get_peers_message() = default;

    json make_response(node *node, json request) override;
    json make_request(node *node, const std::monostate&) override;

    std::string type() override { return "get_peers"; }
};

class get_pool_message: public callable_message<> {
public:
    get_pool_message() = default;

    json make_response(node *node, json request) override;
    json make_request(node *node, const std::monostate&) override;
    std::string type() override { return "get_pool"; }
};

class get_latest_block_message: public callable_message<> {
public:
    get_latest_block_message() = default;

    json make_response(node *node, json request) override;
    json make_request(node *node, const std::monostate&) override;
    std::string type() override { return "get_latest_block"; }
};

class get_chain_message: public callable_message<> {
public:
    get_chain_message() = default;

    json make_response(node *node, json request) override;
    json make_request(node *node, const std::monostate&) override;
    std::string type() override { return "get_chain"; }
};

}

#endif // RESPONSES_H
