#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>
#include <nlohmann/json.hpp>

namespace encoin {

using nlohmann::json;

class settings {
public:
    settings();
    ~settings();

    static constexpr auto SETTINGS_FILENAME = "settings.json";

    template <class _type>
    inline void set(const std::string &key, const _type &value)
    { _json[key] = value; }

    template <class _type>
    inline _type get(const std::string &key, const _type &defvalue = _type{}) const
    { return _json.contains(key) ? _json[key].get<_type>() : defvalue; }


#define SETTING_GETTER(_type, _name, _default) \
    inline _type _name() { return get<_type>(#_name, _default); }

#define SETTING_SETTER(_type, _name) \
    inline void set_##_name(const _type &value) { set<_type>(#_name, value); }

#define SETTING(_type, _name, _default) \
    SETTING_GETTER(_type, _name, _default) \
    SETTING_SETTER(_type, _name)

public:
    SETTING(std::string, reward_address, "");
    SETTING(unsigned short, port, 5001);

    void load(const std::string &name);
    void save(const std::string &name);

protected:
    json _json;

};

}

#endif // SETTINGS_H
