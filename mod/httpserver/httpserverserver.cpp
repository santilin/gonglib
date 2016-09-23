#include <fstream>
#include <sstream>
#include <boost/filesystem.hpp>
#include <vector>
#include <algorithm>
#include <boost/property_tree/json_parser.hpp>
#include <gongdbrecord.h>
#include <gongdbdefinition.h>
#include <gongfileutils.h>
#include <dbappdbapplication.h>

#include "httpserverserver.h"
#include "httpserverbladeinterpreter.h"

using namespace std;

namespace gong {

namespace httpserver {

//Unless you do more heavy non-threaded processing in the resources,
//1 thread is usually faster than several threads
Server::Server( const Xtring &document_root,
                 short unsigned int port, size_t num_threads, long int timeout_request, long int timeout_content )
    : HttpServer ( port, num_threads, timeout_request, timeout_content ),
      mDocumentRoot ( document_root )
{
    if ( !FileUtils::isDir ( mDocumentRoot.c_str() ) ) {
        _GONG_DEBUG_PRINT ( 0, mDocumentRoot + ": directorio no encontrado" );
        throw new std::runtime_error ( "Document root: '" + mDocumentRoot  + "' no existe" );
    }

    mVariables.insert ( "BASEURL", mDocumentRoot );

    // Responds with request-information
    resource["^/info$"]["GET"]=[] ( shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request )
    {
        stringstream content_stream;
        content_stream << "<h1>Request from " << request->remote_endpoint_address << " (" << request->remote_endpoint_port << ")</h1>";
        content_stream << request->method << " " << request->path << " HTTP/" << request->http_version << "<br>";
        for ( auto& header: request->header ) {
            content_stream << header.first << ": " << header.second << "<br>";
        }
        //find length of content_stream (length received using content_stream.tellp())
        content_stream.seekp ( 0, ios::end );
        *response <<  "HTTP/1.1 200 OK\r\nContent-Length: " << content_stream.tellp() << "\r\n\r\n" << content_stream.rdbuf();
    };


    // Default GET-example. If no other matches, this anonymous function will be called.
    // Will respond with content in the web/-directory, and its subdirectories.
    // Default file: index.html
    default_resource["GET"]=[this] ( shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request ) {
        const auto web_root_path=boost::filesystem::canonical ( mDocumentRoot );
        boost::filesystem::path path = web_root_path;
        path/=request->path;
        if ( boost::filesystem::exists ( path ) ) {
            path=boost::filesystem::canonical ( path );
            //Check if path is within web_root_path
            if ( distance ( web_root_path.begin(), web_root_path.end() ) <=distance ( path.begin(), path.end() ) &&
            equal ( web_root_path.begin(), web_root_path.end(), path.begin() ) ) {
                if ( boost::filesystem::is_directory ( path ) )
                    path/="index.html";
                if ( boost::filesystem::exists ( path ) && boost::filesystem::is_regular_file ( path ) ) {
                    Xtring filename ( path.c_str() );
                    if ( filename.endsWith ( ".blade.php" ) ) {
                        Xtring content = BladeInterpreter::interpret ( FileUtils::readFile ( filename ), this->getVariables() );
                        std::cout << "Enviando el fichero interpretado: " << filename << std::endl;
                        *response << "HTTP/1.1 200 Ok\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;
                    } else {
                        std::cout << "Enviando el fichero: " << filename << std::endl;
                        auto ifs=make_shared<ifstream>();
                        ifs->open ( path.string(), ifstream::in | ios::binary );
                        if ( *ifs ) {
                            //read and send 128 KB at a time
                            streamsize buffer_size=131072;
                            auto buffer=make_shared<vector<char> > ( buffer_size );
                            ifs->seekg ( 0, ios::end );
                            auto length=ifs->tellg();
                            ifs->seekg ( 0, ios::beg );
                            *response << "HTTP/1.1 200 OK\r\nContent-Length: " << length << "\r\n\r\n";
                            default_resource_send ( response, ifs, buffer );
                            return;
                        }
                    }
                } else {
                    string content="file " + request->path + " not found.";
                    *response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;
                }
            } else {
                string content="path " + request->path + " outside of webroot path: " + web_root_path.string();
                *response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;
            }
        } else {
            string content="Could not open path " + request->path;
            *response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;
        }
    };
}


int Server::run()
{
    std::thread server_thread ( [this]() {
        start();
    } );
    _GONG_DEBUG_PRINT ( 0, Xtring::printf ( "Servidor http iniciado en localhost:%d, document_root:%s", config.port, mDocumentRoot.c_str() ) );
    server_thread.join();
    return 0;
}

void Server::default_resource_send ( shared_ptr<HttpServer::Response> response,
                                     shared_ptr<ifstream> ifs, shared_ptr<vector<char> > buffer )
{
    streamsize read_length;
    if ( ( read_length=ifs->read ( & ( *buffer ) [0], buffer->size() ).gcount() ) >0 ) {
        response->write ( & ( *buffer ) [0], read_length );
        if ( read_length==static_cast<streamsize> ( buffer->size() ) ) {
            send ( response, [this, response, ifs, buffer] ( const boost::system::error_code &ec ) {
                if ( !ec )
                    default_resource_send ( response, ifs, buffer );
                else
                    cerr << "Connection interrupted" << endl;
            } );
        }
    }
}

// Ejemplos de resources
void Server::addRestRoutes(const Xtring &prefix)
{
    resource["^/" + prefix + "/filter/([A-Za-z_]+)\\?(.*)$"]["GET"]=[this] ( shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request ) {
        string request_table=request->path_match[1];
        string request_params=request->path_match[2];
        Xtring response_str = getResources(Xtring ( request_table.c_str() ).upper(), request_params );
        *response << "HTTP/1.1 200 OK\r\nContent-Length: " << response_str.length() << "\r\n\r\n" << response_str;
    };
    resource["^/" + prefix + "/([A-Za-z_]+)/([0-9]+)$"]["GET"]=[this] ( shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request ) {
        string request_table=request->path_match[1];
        string request_number=request->path_match[2];
        dbRecordID id = Xtring ( request_number.c_str() ).toInt();
        Xtring response_str = getResource ( Xtring ( request_table.c_str() ).upper(), id );
        *response << "HTTP/1.1 200 OK\r\nContent-Length: " << response_str.length() << "\r\n\r\n" << response_str;
    };

}

void Server::addAuthRoutes(const Xtring &prefix)
{
    resource["^/" + prefix + "/auth$"]["POST"]=[] ( shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request ) {
        try {
			std::istringstream icontent(request->content.string());
            boost::property_tree::ptree pt;
            boost::property_tree::read_json ( icontent, pt );
            string name=pt.get<string> ( "email" ) +" "+pt.get<string> ( "password" );

			pt.put("role", "administrador");
			pt.put("permissions", "administrator/agentes/read");
			std::string content;
			std::stringstream ocontent(content);
			boost::property_tree::write_json( ocontent, pt );
			_GONG_DEBUG_PRINT(0, content);
			
            *response << "HTTP/1.1 200 OK\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;
        } catch ( exception& e ) {
            *response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << strlen ( e.what() ) << "\r\n\r\n" << e.what();
        }
    };
    resource["^/" + prefix + "/logout$"]["POST"]=[] ( shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request ) {
		std::string content("logged out");
		*response << "HTTP/1.1 200 OK\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;
    };
}

Xtring Server::getResource ( const Xtring &table, dbRecordID id )
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

Xtring Server::getResources(const Xtring& table, const Xtring& params_str)
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


bool Server::url_decode(const std::string& in, std::string& out)
{
  out.reserve(in.size());
  for (std::size_t i = 0; i < in.size(); ++i)
  {
    if (in[i] == '%')
    {
      if (i + 3 <= in.size())
      {
        int value = 0;
        std::istringstream is(in.substr(i + 1, 2));
        if (is >> std::hex >> value)
        {
          out += static_cast<char>(value);
          i += 2;
        }
        else
        {
          return false;
        }
      }
      else
      {
        return false;
      }
    }
    else if (in[i] == '+')
    {
      out += ' ';
    }
    else
    {
      out += in[i];
    }
  }
  return true;
}

#if 0
//Add resources using path-regex and method-string, and an anonymous function

//Get example simulating heavy work in a separate thread
server.resource["^/work$"]["GET"]=[&server] ( shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> /*request*/ )
{
    thread work_thread ( [response] {
        this_thread::sleep_for ( chrono::seconds ( 5 ) );
        string message="Work done";
        *response << "HTTP/1.1 200 OK\r\nContent-Length: " << message.length() << "\r\n\r\n" << message;
    } );
    work_thread.detach();
};
#endif

} // namespace httpsercer
} // namespace gong
