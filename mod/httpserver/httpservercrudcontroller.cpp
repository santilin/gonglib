#include <gongdbrecord.h>
#include <gonglibrary.h>
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
        *response << getServer()->getResponse(result_code, response_str);
    };
    getServer()->resource[Xtring("^/") + getPrefix() + "/([A-Za-z_]+)/([0-9]+)$"]["GET"]=[this] ( std::shared_ptr<HttpServer::Response> response, std::shared_ptr<HttpServer::Request> request ) {
        std::string request_table=request->path_match[1];
        std::string request_number=request->path_match[2];
        dbRecordID id = Xtring ( request_number.c_str() ).toInt();
		int result_code;
        Xtring response_str(getResource ( Xtring ( request_table.c_str() ).upper(), id, result_code));
        *response << getServer()->getResponse(result_code, response_str);
    };
	return this;
}
	
Xtring CrudController::getResource ( const Xtring &table, dbRecordID id, int &result_code)
{
    Xtring response_str;
    dbRecord *record = GongLibraryInstance->createRecord ( table, id );
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
	XtringList params;
	result_code = 200;
	params_str.tokenize(params,"&");
	long int count = 0, page = 1;
	JsonTree filters, sort;
	for( auto &i : params ) {
		Xtring decoded;
		if( url_decode(i, decoded) ) {
			Xtring param, value;
			decoded.splitIn2(param, value, "=");			
			param = param.upper();
			if( param == "COUNT") {
				count = value.toLong();
			} else if( param == "PAGE" ) {
				page = value.toLong();
				if( page == 0 ) page = 1;
			} else if( param == "FILTER" && !value.isEmpty()) {
				if( !filters.parse(value) ) {
					result_code = 400; 
					return "WRONG filter specification";
				}
			} else if( param == "SORTING" && !value.isEmpty() ) {
				if( !sort.parse(value) ) {
					result_code = 400; 
					return "WRONG sort specification";
				}
			}
		} else {
			result_code = 400; 
			return "Wrong URI parameters";
		}
	}
    dbRecord *record = GongLibraryInstance->createRecord(table);
	Xtring filter;
	for( JsonTree::const_iterator fit = filters.begin(); fit != filters.end(); ++fit ) {
		if (!filter.isEmpty() )
			filter += " AND ";
		else 
			filter = " WHERE ";
		filter += pConnection->toSQLLike(fit->first, fit->second.get_value<Xtring>());
	}
    if ( !record ) {
		result_code = 422;
        return "No se ha podido crear un registro para la tabla " + table;
    } 
	int records = pConnection->selectInt("SELECT COUNT(*) FROM " + pConnection->nameToSQL(table) + filter);
	Xtring response_str = "{ \"total\": " + Xtring::number(records) + ",\n \"data\": [\n";
	if( records ) {
		dbResultSet *rs( pConnection->select("SELECT ID FROM " + pConnection->nameToSQL(table) + filter 
			+ " LIMIT " + pConnection->toSQL((page-1)*count) + "," + pConnection->toSQL(count) ) );
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
	delete record;
	response_str += "] }";
	return response_str;
}


} // namespace httpserver 
} // namespace gong 

