#include "httpserverserver.h"
#include "httpserverauthcontroller.h"

namespace gong {
namespace httpserver {

Controller* AuthController::addRoutes()
{	
    getServer()->resource[Xtring("^/") + getPrefix() + "/auth$"]["POST"]=[this] ( std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request ) {
		int result_code = 200;
		Xtring content;
        try {
			JsonTree pt;
			if( pt.parse(request->content.string() ) ) {
				Xtring name=pt.get<Xtring> ( "email" ) +" "+pt.get<Xtring> ( "password" );
				pt.put("role", "administrador");
				pt.put("permissions", "administrator/agentes/read");
				pt.write(content);
			} else {
				result_code = 400;
				content = "Request contains invalid JSON parameters";
			}
        } catch ( std::exception& e ) {
			content = e.what();
        }
		*response << getServer()->getResponse(result_code, content);
    };
    getServer()->resource[Xtring("^/") + getPrefix() + "/logout$"]["POST"]=[this] ( std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request ) {
		*response << getServer()->getResponse(200, "Logged out");
    };
}

} // namespace httpserver 
} // namespace gong 

