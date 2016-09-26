#include <gongdebug.h>
#include "httpserverjsontree.h"

namespace gong {
namespace httpserver {

bool JsonTree::parse(const Xtring &source) 
{
	try {
		std::istringstream icontent(source);
		boost::property_tree::read_json(icontent, static_cast<boost::property_tree::ptree &>(*this) );
		return true;
	} catch (boost::property_tree::json_parser::json_parser_error &e ) {
		_GONG_DEBUG_WARNING( e.what() );
		return false;
	}
};
bool JsonTree::write(Xtring &content) {
	try {
		std::stringstream ocontent(content);
		boost::property_tree::write_json( ocontent, static_cast<boost::property_tree::ptree &>(*this) );
	} catch( boost::property_tree::json_parser::json_parser_error &e) {
		_GONG_DEBUG_WARNING( e.what() );
		return false;
	}
	return true;
}
	
} // namespace httpserver
} // namespace gong
	