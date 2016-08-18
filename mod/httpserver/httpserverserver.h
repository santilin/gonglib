#ifndef HTTPSERVER_SERVER_H
#define HTTPSERVER_SERVER_H

#include "server_http.hpp"

namespace gong {
namespace httpserver {

typedef SimpleWeb::Server<SimpleWeb::HTTP> HttpServer;

class Server: public HttpServer
{
public:
	Server(const Xtring &document_root,
		   unsigned short port, size_t num_threads=1, long timeout_request=5, long timeout_content=300);
	void addRestRoutes();
	int run();
protected:
	Xtring getResource(const Xtring &table, dbRecordID id);

protected:
	void default_resource_send(std::shared_ptr<HttpServer::Response> response,
                           std::shared_ptr<std::ifstream> ifs, std::shared_ptr<std::vector<char> > buffer);
private:
	Xtring mDocumentRoot;
};


} // namespace httpserver
} // namespace gong


#endif // HTTPSERVER_SERVER_H