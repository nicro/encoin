#ifndef RESPONSES_H
#define RESPONSES_H

#include <string>
#include <iostream>
#include <nlohmann/json.hpp>

namespace encoin {

using nlohmann::json;

class node;

class message {
public:
    virtual json make_response(node*, json) = 0;
    virtual json make_request(node*, void* = nullptr) = 0;

    virtual std::string type() = 0;

    virtual ~message() = 0;

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

    static std::string parse_type(const json &j)
    {
        return j.contains("type") ? j["type"] : "";
    }

struct json_wrapper: public json {
    json_wrapper() : json() {}

    std::string type()
    {
        return contains("type") ? at("type").get<std::string>() : "";
    }
    void set_type(const std::string &type)
    {
        (*this)["type"] = type;
    }

    json payload()
    {
        return contains("payload") ? at("payload") : json{};
    }
    void set_payload(const json &payload)
    {
        (*this)["payload"] = payload;
    }

    std::string status()
    {
        return contains("status") ? at("status").get<std::string>() : "";
    }
    void set_status(const std::string &status)
    {
        (*this)["status"] = status;
    }

    json as_plain()
    {
        return *reinterpret_cast<json*>(this);
    }
};

};

class new_tx_message: public message {
public:
    new_tx_message() = default;

    json make_response(node *node, json request) override
    {
        json_wrapper j;
        j.set_type(type());
        j.set_status("success");
        return j.as_plain();
    }

    json make_request(node *node, void*) override
    {
        json_wrapper j;
        j.set_type(type());
        return j;
    }

    std::string type() override
    {
        return "new_tx";
    }
};

}

#endif // RESPONSES_H
