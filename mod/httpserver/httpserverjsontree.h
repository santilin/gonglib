#ifndef HTTPSERVER_JSONTREE_H
#define HTTPSERVER_JSONTREE_H

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <istream>
#include <ostream>
#include <gongxtring.h>

namespace gong {
namespace httpserver {

class JsonTree: public boost::property_tree::ptree 
{
public:
	bool parse(const Xtring &source);
	bool write(Xtring &dest);
};

	
} // namespace httpserver
} // namespace gong


#endif // HTTPSERVER_JSONTREE_H
