#include <fstream>
#include <sstream>
#include <boost/filesystem.hpp>
#include <vector>
#include <algorithm>
#include <gongdbrecord.h>
#include <gongdbdefinition.h>
#include <gongfileutils.h>

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

    mVariables["BASEURL"] = mDocumentRoot;

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
                        *response << this->getResponse(200, content);
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
                            *response << this->getStatusHeader(200) << "Content-Length: " << length << "\r\n\r\n";
                            default_resource_send ( response, ifs, buffer );
                            return;
                        }
                    }
                } else {
                    string content="file " + request->path + " not found.";
                    *response << this->getResponse(400, content);
                }
            } else {
                string content="path " + request->path + " outside of webroot path: " + web_root_path.string();
				*response << this->getResponse(400, content);
            }
        } else {
            string content="Could not open path " + request->path;
			*response << this->getResponse(400, content);
        }
    };
}

Server::~Server()
{
	// Borra todos los controladores que ha tomado
	for( ControllersList::const_iterator it = mControllers.cbegin();
		it != mControllers.cend(); ++it ) {
		delete it->second;
	}
}


/**
 * @brief Añade un controlador y toma posesión de él
 * 
 * @param controller ...
 * @param prefix ...
 * @return void
 */
void Server::takeController(Controller *controller)
{
	if (mControllers.find(controller->getName()) == mControllers.end() ) {
		mControllers[controller->getName()] = controller;
		controller->addRoutes();
	}
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

Xtring Server::getResponse(int response_code, const Xtring& content)
{
	return getStatusHeader(response_code) + "Content-Length: " + Xtring::number(content.length()) + "\r\n\r\n" + content;
}


Xtring Server::getStatusHeader(int response_code) 
{
	return "HTTP/1.1 " + Xtring::number(response_code) + " " + getStatusMessage(response_code) + "\r\n";
}


const char* Server::getStatusMessage(int response_code)
{
	switch(response_code) {
		case 100: return "Continue";
		case 101: return "Switching Protocols";
		case 102: return "Processing";
		case 200: return "OK";
		case 201: return "Created";
		case 202: return "Accepted";
		case 203: return "Non-Authoritative Information";
		case 204: return "No Content";
		case 205: return "Reset Content";
		case 206: return "Partial Content";
		case 207: return "Multi";
		case 208: return "Already Reported";
		case 226: return "IM Used";
		case 300: return "Multiple Choices";
		case 301: return "Moved Permanently";
		case 302: return "Found";
		case 303: return "See Other";
		case 304: return "Not Modified";
		case 305: return "Use Proxy";
		case 307: return "Temporary Redirect";
		case 308: return "Permanent Redirect";
		case 400: return "Bad Request";
		case 401: return "Unauthorized";
		case 402: return "Payment Required";
		case 403: return "Forbidden";
		case 404: return "Not Found";
		case 405: return "Method Not Allowed";
		case 406: return "Not Acceptable";
		case 407: return "Proxy Authentication Required";
		case 408: return "Request Timeout";
		case 409: return "Conflict";
		case 410: return "Gone";
		case 411: return "Length Required";
		case 412: return "Precondition Failed";
		case 413: return "Payload Too Large";
		case 414: return "URI Too Long";
		case 415: return "Unsupported Media Type";
		case 416: return "Range Not Satisfiable";
		case 417: return "Expectation Failed";
		case 421: return "Misdirected Request";
		case 422: return "Unprocessable Entity";
		case 423: return "Locked";
		case 424: return "Failed Dependency";
		case 426: return "Upgrade Required";
		case 428: return "Precondition Required";
		case 429: return "Too Many Requests";
		case 431: return "Request Header Fields Too Large";
		case 451: return "Unavailable For Legal Reasons";
		case 500: return "Internal Server Error";
		case 501: return "Not Implemented";
		case 502: return "Bad Gateway";
		case 503: return "Service Unavailable";
		case 504: return "Gateway Timeout";
		case 505: return "HTTP Version Not Supported";
		case 506: return "Variant Also Negotiates";
		case 507: return "Insufficient Storage";
		case 508: return "Loop Detected";
		case 510: return "Not Extended";
		case 511: return "Network Authentication Required";
		default:  return "Unassigned";
	}
}


} // namespace httpsercer
} // namespace gong
