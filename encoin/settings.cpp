#include "settings.h"

#include <fstream>
#include <sstream>

namespace encoin {

settings::settings()
{
    if (std::filesystem::exists(SETTINGS_FILENAME))
    {
        load(SETTINGS_FILENAME);
    }
}

settings::~settings()
{
    save(SETTINGS_FILENAME);
}

void settings::load(const std::string &name)
{
    std::ifstream t(name);
    std::stringstream buffer; buffer << t.rdbuf();
    std::string string = buffer.str();
    _json = string.empty() ? json{} : json::parse(string);
}

void settings::save(const std::string &name)
{
    std::ofstream out(name);
    out << _json.dump();
    out.close();
}

}
