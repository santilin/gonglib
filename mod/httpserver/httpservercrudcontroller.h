#ifndef HTTPSERVER_CRUDCONTROLLER_H
#define HTTPSERVER_CRUDCONTROLLER_H

#include "httpservercontroller.h"

namespace gong {
namespace httpserver {
	
class CrudController: public Controller
{
public:
	CrudController(const char *name, const char *prefix)
		: Controller(name, prefix) {}
	virtual Controller *addRoutes(Server &server) override;
	Xtring getResource(const Xtring &table, dbRecordID id);
	Xtring getResources(const Xtring &table, const Xtring &params);	
};
	
} // namespace httpserver 
} // namespace gong 



#endif // HTTPSERVER_CRUDCONTRLLER_H
