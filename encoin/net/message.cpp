#include <net/message.h>

namespace encoin {

std::string json_wrapper::type()
{
    return contains("type") ? at("type").template get<std::string>() : "";
}

void json_wrapper::set_type(const std::string &type)
{
    (*this)["type"] = type;
}

json json_wrapper::payload()
{
    return contains("payload") ? at("payload") : json{};
}

void json_wrapper::set_payload(const nlohmann::json &payload)
{
    (*this)["payload"] = payload;
}

std::string json_wrapper::status()
{
    return contains("status") ? at("status").template get<std::string>() : "";
}

void json_wrapper::set_status(const std::string &status)
{
    (*this)["status"] = status;
}

json json_wrapper::as_plain()
{
    return *reinterpret_cast<json*>(this);
}

}
