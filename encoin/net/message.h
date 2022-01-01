#ifndef RESPONSES_H
#define RESPONSES_H

#include <string>
#include <iostream>
#include <nlohmann/json.hpp>
#include <variant>

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
};

template <class rtype = json, class atype = std::monostate>
class callable_message: public message {
public:
    using arg_type = atype;
    virtual rtype make_request(node*, atype) = 0;
};

struct json_wrapper: public json {
    json_wrapper() : json() {}

    std::string type();
    void set_type(const std::string &type);

    json payload();
    void set_payload(const json &payload);

    std::string status();
    void set_status(const std::string &status);

    json as_plain();
};

class new_tx_message: public callable_message<> {
public:
    new_tx_message() = default;

    json make_response(node *node, json request) override
    {
        json_wrapper j;
        j.set_type(type());
        j.set_status("success");
        return j.as_plain();
    }
    json make_request(node *node, std::monostate) override
    {
        json_wrapper j;
        j.set_type(type());
        return j.as_plain();
    }

    std::string type() override
    {
        return "new_tx";
    }
};

}

#endif // RESPONSES_H
