#include <fstream>
#include <sstream>
#include <boost/filesystem.hpp>
#include <vector>
#include <algorithm>
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

const char* Server::getStatusText(int response_code)
{
	switch(response_code) {
		case 100: return "100 Continue";
		case 101: return "101 Switching Protocols";
		case 102: return "102 Processing";
		case 200: return "200 OK";
		case 201: return "201 Created";
		case 202: return "202 Accepted";
		case 203: return "203 Non-Authoritative Information";
		case 204: return "204 No Content";
		case 205: return "205 Reset Content";
		case 206: return "206 Partial Content";
		case 207: return "207 Multi";
		case 208: return "208 Already Reported";
		case 226: return "226 IM Used";
		case 300: return "300 Multiple Choices";
		case 301: return "301 Moved Permanently";
		case 302: return "302 Found";
		case 303: return "303 See Other";
		case 304: return "304 Not Modified";
		case 305: return "305 Use Proxy";
		case 307: return "307 Temporary Redirect";
		case 308: return "308 Permanent Redirect";
		case 400: return "400 Bad Request";
		case 401: return "401 Unauthorized";
		case 402: return "402 Payment Required";
		case 403: return "403 Forbidden";
		case 404: return "404 Not Found";
		case 405: return "405 Method Not Allowed";
		case 406: return "406 Not Acceptable";
		case 407: return "407 Proxy Authentication Required";
		case 408: return "408 Request Timeout";
		case 409: return "409 Conflict";
		case 410: return "410 Gone";
		case 411: return "411 Length Required";
		case 412: return "412 Precondition Failed";
		case 413: return "413 Payload Too Large";
		case 414: return "414 URI Too Long";
		case 415: return "415 Unsupported Media Type";
		case 416: return "416 Range Not Satisfiable";
		case 417: return "417 Expectation Failed";
		case 421: return "421 Misdirected Request";
		case 422: return "422 Unprocessable Entity";
		case 423: return "423 Locked";
		case 424: return "424 Failed Dependency";
		case 425: return "425 Unassigned";
		case 426: return "426 Upgrade Required";
		case 427: return "427 Unassigned";
		case 428: return "428 Precondition Required";
		case 429: return "429 Too Many Requests";
		case 430: return "430 Unassigned";
		case 431: return "431 Request Header Fields Too Large";
		case 451: return "451 Unavailable For Legal Reasons";
		case 500: return "500 Internal Server Error";
		case 501: return "501 Not Implemented";
		case 502: return "502 Bad Gateway";
		case 503: return "503 Service Unavailable";
		case 504: return "504 Gateway Timeout";
		case 505: return "505 HTTP Version Not Supported";
		case 506: return "506 Variant Also Negotiates";
		case 507: return "507 Insufficient Storage";
		case 508: return "508 Loop Detected";
		case 509: return "509 Unassigned";
		case 510: return "510 Not Extended";
		case 511: return "511 Network Authentication Required";
		default:  return "Unassigned"; // @todo devolver el número
	}
}


} // namespace httpsercer
} // namespace gong
