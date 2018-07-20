#ifndef GONG_JSONTREE_H
#define GONG_JSONTREE_H

// https://github.com/nlohmann/json

#include "json.hpp"

using json = nlohmann::json;

namespace gong {

typedef json JsonTree;

JsonTree readJson(const std::string &jsonfile);

} // namespace gong


#endif // GONG_JSONTREE_H
