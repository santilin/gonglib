#include <fstream>
#include "gongdebug.h"
#include "gongxtring.h"
#include "gongjson.h"

namespace gong {

JsonTree readJson(const Xtring &jsonfile)
{
    std::ifstream f(jsonfile);
    return json::parse(f);
}

} // namespace gong
     

