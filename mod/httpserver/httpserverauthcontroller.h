#ifndef HTTPSERVER_AUTHCONTROLLER_H
#define HTTPSERVER_AUTHCONTROLLER_H

#include "httpservercontroller.h"

namespace gong {
namespace httpserver {
	
class AuthController: public Controller
{
public:
	AuthController(Server *server, const char *name, const char *prefix)
		: Controller(server, name, prefix) {}
	virtual Controller *addRoutes() override;
};
	
} // namespace httpserver 
} // namespace gong 



#endif // HTTPSERVER_AUTHCONTRLLER_H
