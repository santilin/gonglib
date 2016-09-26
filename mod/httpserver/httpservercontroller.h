#ifndef HTTPSERVER_CONTROLLER_H
#define HTTPSERVER_CONTROLLER_H

#include <gongxtring.h>
#include "httpserverjsontree.h"

namespace gong {
namespace httpserver {
	
class Server;

class Controller
{
public:
	Controller(Server *server, const char *name, const char *prefix)
		: pServer(server), pName(name), pPrefix(prefix) {}
	virtual ~Controller() {}
	
	const char *getName() const { return pName; }
	const char *getPrefix() const { return pPrefix; }
	virtual Controller *addRoutes();
	static bool url_decode(const Xtring& in, Xtring &out);
protected:
	Server *getServer() const { return pServer; }
	
private:
	const char *pName, *pPrefix;
	Server *pServer;
};
	
} // namespace httpserver 
} // namespace gong 


#endif // HTTPSERVER_CONTROLLER_H