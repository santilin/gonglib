#include <fstream>
#include <boost/filesystem.hpp>
#include <vector>
#include <algorithm>
#include <gongdbrecord.h>
#include <gongdbdefinition.h>
#include <gongfileutils.h>
#include <dbappdbapplication.h>

#include "httpserverserver.h"

using namespace std;

namespace gong {
namespace httpserver {

//Unless you do more heavy non-threaded processing in the resources,
//1 thread is usually faster than several threads
Server::Server(const Xtring &document_root,
			   short unsigned int port, size_t num_threads, long int timeout_request, long int timeout_content)
		: HttpServer(port, num_threads, timeout_request, timeout_content),
		mDocumentRoot(document_root)
{
	if( !FileUtils::isDir(document_root.c_str()) ) {
		throw new std::runtime_error("Document root: '" + document_root + "' no existe");
	}
    //Default GET-example. If no other matches, this anonymous function will be called.
    //Will respond with content in the web/-directory, and its subdirectories.
    //Default file: index.html
    //Can for instance be used to retrieve an HTML 5 client that uses REST-resources on this server
    default_resource["GET"]=[this](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
        const auto web_root_path=boost::filesystem::canonical(mDocumentRoot);
        boost::filesystem::path path = web_root_path;
        path/=request->path;
        if(boost::filesystem::exists(path)) {
            path=boost::filesystem::canonical(path);
            //Check if path is within web_root_path
            if(distance(web_root_path.begin(), web_root_path.end())<=distance(path.begin(), path.end()) &&
               equal(web_root_path.begin(), web_root_path.end(), path.begin())) {
                if(boost::filesystem::is_directory(path))
                    path/="index.html";
                if(boost::filesystem::exists(path) && boost::filesystem::is_regular_file(path)) {
					Xtring filename( path.c_str() );
					if( filename.endsWith('blade.php') ) {

					} else {
						auto ifs=make_shared<ifstream>();
						ifs->open(path.string(), ifstream::in | ios::binary);

						if(*ifs) {
							//read and send 128 KB at a time
							streamsize buffer_size=131072;
							auto buffer=make_shared<vector<char> >(buffer_size);

							ifs->seekg(0, ios::end);
							auto length=ifs->tellg();

							ifs->seekg(0, ios::beg);

							*response << "HTTP/1.1 200 OK\r\nContent-Length: " << length << "\r\n\r\n";
							default_resource_send(response, ifs, buffer);
							return;
						}
					}
                }
            }
        }
        string content="Could not open path " + request->path;
        *response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;
    };
}


int Server::run()
{

    std::thread server_thread([this](){
        start();
    });
    server_thread.join();
	_GONG_DEBUG_PRINT(0, Xtring::printf("Servidor http iniciado en localhost:%d, document_root:%s", config.port, mDocumentRoot.c_str()) );
    return 0;
}

void Server::default_resource_send(shared_ptr<HttpServer::Response> response,
                           shared_ptr<ifstream> ifs, shared_ptr<vector<char> > buffer) {
    streamsize read_length;
    if((read_length=ifs->read(&(*buffer)[0], buffer->size()).gcount())>0) {
        response->write(&(*buffer)[0], read_length);
        if(read_length==static_cast<streamsize>(buffer->size())) {
            send(response, [this, response, ifs, buffer](const boost::system::error_code &ec) {
                if(!ec)
                    default_resource_send(response, ifs, buffer);
                else
                    cerr << "Connection interrupted" << endl;
            });
        }
    }
}

// Ejemplos de resources
void Server::addRestRoutes()
{
    //GET-example for the path /match/[number], responds with the matched string in path (number)
    //For instance a request GET /match/123 will receive: 123
    resource["^/([A-Za-z_]+)/([0-9]+)$"]["GET"]=[this](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
        string request_table=request->path_match[1];
        string request_number=request->path_match[2];
		dbRecordID id = Xtring(request_number.c_str()).toInt();
		Xtring response_str = getResource(Xtring(request_table.c_str()).upper(), id);
        *response << "HTTP/1.1 200 OK\r\nContent-Length: " << response_str.length() << "\r\n\r\n" << response_str;
    };
}

Xtring Server::getResource(const Xtring &table, dbRecordID id)
{
	Xtring response_str;
	dbConnection *conn = DBAPP->getConnection();
	dbRecord *record = DBAPP->createRecord(table, id);
	if (!record) {
		response_str = "No se ha podido crear un registro para la tabla " + table;
	} else if( record->read(id) ) {
		response_str = record->toString(TOSTRING_JSON);
	} else {
		response_str = Xtring::printf("No se ha encontrado el registro de id %d en la tabla %s", id, table.c_str());
	}
}

#if 0
    //Add resources using path-regex and method-string, and an anonymous function
    //POST-example for the path /string, responds the posted string
    server.resource["^/string$"]["POST"]=[](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
        //Retrieve string:
        auto content=request->content.string();
        //request->content.string() is a convenience function for:
        //stringstream ss;
        //ss << request->content.rdbuf();
        //string content=ss.str();

        *response << "HTTP/1.1 200 OK\r\nContent-Length: " << content.length() << "\r\n\r\n" << content;
    };

    //POST-example for the path /json, responds firstName+" "+lastName from the posted json
    //Responds with an appropriate error message if the posted json is not valid, or if firstName or lastName is missing
    //Example posted json:
    //{
    //  "firstName": "John",
    //  "lastName": "Smith",
    //  "age": 25
    //}
    server.resource["^/json$"]["POST"]=[](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
        try {
            ptree pt;
            read_json(request->content, pt);

            string name=pt.get<string>("firstName")+" "+pt.get<string>("lastName");

            *response << "HTTP/1.1 200 OK\r\nContent-Length: " << name.length() << "\r\n\r\n" << name;
        }
        catch(exception& e) {
            *response << "HTTP/1.1 400 Bad Request\r\nContent-Length: " << strlen(e.what()) << "\r\n\r\n" << e.what();
        }
    };

    //GET-example for the path /info
    //Responds with request-information
    server.resource["^/info$"]["GET"]=[](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> request) {
        stringstream content_stream;
        content_stream << "<h1>Request from " << request->remote_endpoint_address << " (" << request->remote_endpoint_port << ")</h1>";
        content_stream << request->method << " " << request->path << " HTTP/" << request->http_version << "<br>";
        for(auto& header: request->header) {
            content_stream << header.first << ": " << header.second << "<br>";
        }

        //find length of content_stream (length received using content_stream.tellp())
        content_stream.seekp(0, ios::end);

        *response <<  "HTTP/1.1 200 OK\r\nContent-Length: " << content_stream.tellp() << "\r\n\r\n" << content_stream.rdbuf();
    };


    //Get example simulating heavy work in a separate thread
    server.resource["^/work$"]["GET"]=[&server](shared_ptr<HttpServer::Response> response, shared_ptr<HttpServer::Request> /*request*/) {
        thread work_thread([response] {
            this_thread::sleep_for(chrono::seconds(5));
            string message="Work done";
            *response << "HTTP/1.1 200 OK\r\nContent-Length: " << message.length() << "\r\n\r\n" << message;
        });
        work_thread.detach();
    };
#endif

} // namespace httpsercer
} // namespace gong
