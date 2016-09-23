#ifndef HTTPSERVER_SERVER_H
#define HTTPSERVER_SERVER_H

#include <gongxtring.h>
#include "server_http.hpp"
#include "httpservercontroller.h"

namespace gong {
namespace httpserver {

typedef SimpleWeb::Server<SimpleWeb::HTTP> HttpServer;
typedef std::map<Xtring, Controller *> ControllersList;

class Server: public HttpServer
{
public:
	Server(const Xtring &document_root,
		   unsigned short port, size_t num_threads=1, long timeout_request=5, long timeout_content=300);
	~Server();
	int run();
	void takeController(Controller *controller);
	const Dictionary<Xtring> &getVariables() const { return mVariables; }
	static const char *getStatusText(int response_code);
protected:
	void default_resource_send(std::shared_ptr<HttpServer::Response> response,
                           std::shared_ptr<std::ifstream> ifs, std::shared_ptr<std::vector<char> > buffer);
private:
	Xtring mDocumentRoot;
	Dictionary<Xtring> mVariables;
	ControllersList mControllers;
};


} // namespace httpserver
} // namespace gong


#endif // HTTPSERVER_SERVER_H
