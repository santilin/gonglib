#include <dbappdbapplication.h>
#include "httpserverserver.h"
#include "httpservercrudcontroller.h"

namespace gong {
namespace httpserver {

Controller *CrudController::addRoutes(Server& server)
{
    server.resource[Xtring("^/") + getPrefix() + "/filter/([A-Za-z_]+)\\?(.*)$"]["GET"]=[this] ( std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request ) {
        std::string request_table=request->path_match[1];
        std::string request_params=request->path_match[2];
        Xtring response_str = getResources(Xtring ( request_table.c_str() ).upper(), request_params );
        *response << "HTTP/1.1 200 OK\r\nContent-Length: " << response_str.length() << "\r\n\r\n" << response_str;
    };
    server.resource[Xtring("^/") + getPrefix() + "/([A-Za-z_]+)/([0-9]+)$"]["GET"]=[this] ( std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request ) {
        std::string request_table=request->path_match[1];
        std::string request_number=request->path_match[2];
        dbRecordID id = Xtring ( request_number.c_str() ).toInt();
        Xtring response_str = getResource ( Xtring ( request_table.c_str() ).upper(), id );
        *response << "HTTP/1.1 200 OK\r\nContent-Length: " << response_str.length() << "\r\n\r\n" << response_str;
    };
	return this;
}
	
Xtring CrudController::getResource ( const Xtring &table, dbRecordID id )
{
    Xtring response_str;
    dbConnection *conn = DBAPP->getConnection();
	assert( conn );
    dbRecord *record = DBAPP->createRecord ( table, id );
    if ( !record ) {
        response_str = "No se ha podido crear un registro para la tabla " + table;
    } else if ( record->read ( id ) ) {
        response_str = record->toString ( TOSTRING_JSON );
    } else {
        response_str = Xtring::printf ( "No se ha encontrado el registro de id %d en la tabla %s", id, table.c_str() );
    }
    return response_str;
}

Xtring CrudController::getResources(const Xtring& table, const Xtring& params_str)
{
    Xtring response_str;
	XtringList params;
	params_str.tokenize(params,"&");
    dbConnection *conn = DBAPP->getConnection();
	assert( conn );
    dbRecord *record = DBAPP->createRecord(table);
    if ( !record ) {
        response_str = "No se ha podido crear un registro para la tabla " + table;
    } else {
		long int records = conn->selectInt("SELECT COUNT(*) FROM " + conn->nameToSQL(table));
		response_str = "{ \"total\": " + Xtring::number(records) + ",\n \"data\": [\n";
		if( records ) {
			dbResultSet *rs( conn->select("SELECT ID FROM " + conn->nameToSQL(table) + " LIMIT 1, 10" ) );
			bool first = true;
			while( rs->next() ) {
				if (record->read( rs->toInt((uint) 0) ) ) {
					if (!first)
						response_str += ",\n";
					first = false;
					response_str += record->toString( TOSTRING_JSON );
				}
			}
		}
		response_str += "] }";
	}
	return response_str;
}


} // namespace httpserver 
} // namespace gong 

