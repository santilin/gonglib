#ifndef HTTPSERVER_CRUDCONTROLLER_H
#define HTTPSERVER_CRUDCONTROLLER_H

#include <gongdbrecord.h>
#include "httpservercontroller.h"

namespace gong {
namespace httpserver {
	
class CrudController: public Controller
{
public:
	CrudController(Server *server, class dbConnection *conn, const char *name, const char *prefix)
		: Controller(server, name, prefix), pConnection(conn) {}
	virtual Controller *addRoutes() override;
	Xtring getResource(const Xtring &table, dbRecordID id, int &result_code);
	Xtring getResources(const Xtring &table, const Xtring &params, int &result_code);	
	Xtring updateResource (const Xtring &table, dbRecordID &id, 
						const Xtring &params, int &result_code);
private:
	dbConnection *pConnection;
};
	
} // namespace httpserver 
} // namespace gong 



#endif // HTTPSERVER_CRUDCONTRLLER_H
