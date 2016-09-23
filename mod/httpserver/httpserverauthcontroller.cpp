#include <boost/property_tree/json_parser.hpp>

#include <dbappdbapplication.h>
#include "httpservercontroller.h"
#include "httpserverauthcontroller.h"
#include "httpserverserver.h"

namespace gong {
namespace httpserver {

Controller* AuthController::addRoutes(Server& server)
{	
    server.resource[Xtring("^/") + getPrefix() + "/auth$"]["POST"]=[] ( std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request ) {
        try {
			std::istringstream icontent(request->content.string());
            boost::property_tree::ptree pt;
            boost::property_tree::read_json ( icontent, pt );
            Xtring name=pt.get<Xtring> ( "email" ) +" "+pt.get<Xtring> ( "password" );

			pt.put("role", "administrador");
			pt.put("permissions", "administrator/agentes/read");
			std::string content;
			std::stringstream ocontent(content);
			boost::property_tree::write_json( ocontent, pt );
			_GONG_DEBUG_PRINT(0, content);
			
            *response << "HTTP/1.1 200 OK\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;
        } catch ( std::exception& e ) {
            *response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << strlen ( e.what() ) << "\r\n\r\n" << e.what();
        }
    };
    server.resource[Xtring("^/") + getPrefix() + "/logout$"]["POST"]=[] ( std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request ) {
		std::string content("logged out");
		*response << "HTTP/1.1 200 OK\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;
    };
}

} // namespace httpserver 
} // namespace gong 

