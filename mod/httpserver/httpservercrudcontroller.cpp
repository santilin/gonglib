#include <dbappdbapplication.h>
#include "httpserverserver.h"
#include "httpservercrudcontroller.h"

namespace gong {
namespace httpserver {

Controller *CrudController::addRoutes()
{
    getServer()->resource[Xtring("^/") + getPrefix() + "/filter/([A-Za-z_]+)\\?(.*)$"]["GET"]=[this] ( std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request ) {
        std::string request_table=request->path_match[1];
        std::string request_params=request->path_match[2];
		int result_code;
        Xtring response_str(getResources(Xtring ( request_table.c_str() ).upper(), request_params, result_code ));
        *response << "HTTP/1.1 " << getServer()->getStatusText(result_code)  << "\r\nContent-Length: " << response_str.length() << "\r\n\r\n" << response_str;
    };
    getServer()->resource[Xtring("^/") + getPrefix() + "/([A-Za-z_]+)/([0-9]+)$"]["GET"]=[this] ( std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request ) {
        std::string request_table=request->path_match[1];
        std::string request_number=request->path_match[2];
        dbRecordID id = Xtring ( request_number.c_str() ).toInt();
		int result_code;
        Xtring response_str(getResource ( Xtring ( request_table.c_str() ).upper(), id, result_code));
        *response << "HTTP/1.1 " << getServer()->getStatusText(result_code) << "\r\nContent-Length: " << response_str.length() << "\r\n\r\n" << response_str;
    };
	return this;
}
	
Xtring CrudController::getResource ( const Xtring &table, dbRecordID id, int &result_code)
{
    Xtring response_str;
    dbConnection *conn = DBAPP->getConnection();
	assert( conn );
    dbRecord *record = DBAPP->createRecord ( table, id );
    if ( !record ) {
        response_str = "No se ha podido crear un registro para la tabla " + table;
		result_code = 422;
    } else if ( record->read ( id ) ) {
        response_str = record->toString ( TOSTRING_JSON );
		result_code = 200;
    } else {
        response_str = Xtring::printf ( "No se ha encontrado el registro de id %d en la tabla %s", id, table.c_str() );
		result_code = 404;
    }
    return response_str;
}
//	http://localhost:8080/api/filter/contacto?count=5&filter=%7B%22NOMBRE%22:%22santiago%22%7D&group=%7B%7D&page=1&sorting=%7B%7D
Xtring CrudController::getResources(const Xtring& table, const Xtring& params_str, int &result_code)
{
    Xtring response_str;
	XtringList params;
	result_code = 200;
	params_str.tokenize(params,"&");
	long int count = 0, page = 1;
	XtringList filters, sort;
	for( auto &i : params ) {
		Xtring decoded;
		if( url_decode( i, decoded) ) {
			Xtring param, value;
			i.splitIn2(param, value, "=");
			param = param.upper();
			if( param == "COUNT") {
				count = value.toLong();
			} else if( param == "PAGE" ) {
				page = value.toLong();
				if( page == 0 ) page = 1;
			} else if( param == "FILTER" ) {
				parsePlainJSON(value, filters);
				_GONG_DEBUG_PRINT(0, value);
			} else if( param == "SORT" ) {
				parsePlainJSON(value, sort);
			}
		} else {
			result_code = 400; 
			return "Wrong URI parameters";
		}
	}
    dbConnection *conn = DBAPP->getConnection();
	assert( conn );
    dbRecord *record = DBAPP->createRecord(table);
    if ( !record ) {
        response_str = "No se ha podido crear un registro para la tabla " + table;
		result_code = 422;
    } else {
		int records = conn->selectInt("SELECT COUNT(*) FROM " + conn->nameToSQL(table));
		response_str = "{ \"total\": " + Xtring::number(records) + ",\n \"data\": [\n";
		if( records ) {
			dbResultSet *rs( conn->select("SELECT ID FROM " + conn->nameToSQL(table) 
				+ " LIMIT " + conn->toSQL((page-1)*count) + "," + conn->toSQL(count) ) );
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
		delete record;
	}
	return response_str;
}


} // namespace httpserver 
} // namespace gong 

