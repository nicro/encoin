#include "settings.h"

#include <fstream>
#include <sstream>


namespace encoin {

void settings::load()
{
    std::ifstream t(SETTINGS_NAME);
    std::stringstream buffer; buffer << t.rdbuf();
    std::string string = buffer.str();
    _json = string.empty() ? json{} : json::parse(string);
}

void settings::save()
{
    std::ofstream out(SETTINGS_NAME);
    out << _json.dump();
    out.close();
}

}
