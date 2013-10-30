#include <iostream>
#include <istream>
#include <ostream>
#include <boost/asio.hpp>
#include <gongxtring.h>
#include <gongdebug.h>
#include "gonghttpclient.h"

using boost::asio::ip::tcp;

namespace gong {


// void set_result(optional<error_code>* a, error_code b)
// {
// 	a->reset(b);
// }
//
// template <typename MutableBufferSequence>
// void read_with_timeout(tcp::socket& sock,
// 	const MutableBufferSequence& buffers)
// {
// 	optional<error_code> timer_result;
// 	deadline_timer timer(sock.io_service());
// 	timer.expires_from_now(seconds(1));
// 	timer.async_wait(boost::bind(set_result, &timer_result, _1));
//
// 	optional<error_code> read_result;
// 	async_read(sock, buffers,
// 		boost::bind(set_result, &read_result, _1));
//
// 	sock.io_service().reset();
// 	while (sock.io_service().run_one())
// 	{
// 		if (read_result)
// 		timer.cancel();
// 		else if (timer_result)
// 		sock.cancel();
// 	}
//
// 	if (*read_result)
// 		throw system_error(*read_result);
// }


Xtring uriEncode(const Xtring& sSrc)
{
    const char DEC2HEX[16 + 1] = "0123456789ABCDEF";
    const unsigned char * pSrc = (const unsigned char *)sSrc.c_str();
    const int SRC_LEN = sSrc.length();
    unsigned char * const pStart = new unsigned char[SRC_LEN * 3];
    unsigned char * pEnd = pStart;
    const unsigned char * const SRC_END = pSrc + SRC_LEN;

    for (; pSrc < SRC_END; ++pSrc)
    {
        if ( (48 <= *pSrc && *pSrc <= 57) ||//0-9
                (65 <= *pSrc && *pSrc <= 90) ||//abc...xyz
                (97 <= *pSrc && *pSrc <= 122) || //ABC...XYZ
                (*pSrc=='~' || *pSrc=='!' || *pSrc=='*' || *pSrc=='(' || *pSrc==')' || *pSrc=='\'') )
            *pEnd++ = *pSrc;
        else {
            // escape this char
            *pEnd++ = '%';
            *pEnd++ = DEC2HEX[*pSrc >> 4];
            *pEnd++ = DEC2HEX[*pSrc & 0x0F];
        }
    }
    Xtring sResult((char *)pStart, pEnd - pStart + 1);
    delete [] pStart;
    return sResult;
}

int read_http_file(XtringList &theResponse, char **theContent,
                   const Xtring &theServer, const Xtring &theFilename,
                   const Xtring &extraHeader,
                   const Xtring &theMethod, const Xtring &theQuery)
{
    unsigned int status_code = 0;
    try
    {
        theResponse.clear();
        boost::asio::io_service io_service;

        // Get a list of endpoints corresponding to the server name.
        tcp::resolver resolver(io_service);
        tcp::resolver::query query(theServer, "http");
        tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
        tcp::resolver::iterator end;

        // Try each endp_queryoint until we successfully establish a connection.
        tcp::socket socket(io_service);
        boost::system::error_code error = boost::asio::error::host_not_found;
        while (error && endpoint_iterator != end)
        {
            socket.close();
            socket.connect(*endpoint_iterator++, error);
        }
        if (error)
            throw boost::system::system_error(error);

        // Form the request. We specify the "Connection: close" header so that the
        // server will close the socket after transmitting the response. This will
        // allow us to treat all data up until the EOF as the content.
        boost::asio::streambuf request;
        std::ostream request_stream(&request);

        request_stream << theMethod << " " << theFilename << " HTTP/1.1\r\n";
        request_stream << "Host: " << theServer << "\r\n";
        request_stream << "Accept: identity\r\n";
        request_stream << "Connection: Keep-Alive\r\n";
        request_stream << extraHeader;
        if( theMethod == "POST" ) {
            request_stream << "Content-type: application/x-www-form-urlencoded\r\n";
            request_stream << "Content-length: " << theQuery.size() << "\r\n\r\n";
            request_stream << theQuery;
        } else {
            request_stream << "\r\n";
        }

        _GONG_DEBUG_PRINT(0, theMethod + " http://" + theServer + theFilename);

        // Send the request.
        boost::asio::write(socket, request);

        // Read the response status line. The response streambuf will automatically
        // grow to accommodate the entire line. The growth may be limited by passing
        // a maximum size to the streambuf constructor.
        boost::asio::streambuf response;
        try {
            boost::asio::read_until(socket, response, "\r\n");
        } catch( std::exception& e ) {
            _GONG_DEBUG_WARNING( e.what() );
            return 404;
        }

        // Check that response is OK.
        std::istream response_stream(&response);
        std::string http_version;
        response_stream >> http_version;
        response_stream >> status_code;
        std::string status_message;
        std::getline(response_stream, status_message);
        if (!response_stream || http_version.substr(0, 5) != "HTTP/") {
            std::cout << "Invalid response\n";
        }
        if (status_code != 200) {
            std::cout << "Response returned with status code " << status_code << "\n";
        }

        // Read the response headers, which are terminated by a blank line.
        boost::asio::read_until(socket, response, "\r\n\r\n");
        // Process the response headers.
        Xtring header;
        while (std::getline(response_stream, header) && header != "\r")
            theResponse << header.trim();
        unsigned long long request_length = 0L;
//         unsigned int _timeout = 0;
        // Get the length of the content
        for( XtringList::const_iterator resp_it = theResponse.begin();
                resp_it != theResponse.end();
                ++ resp_it ) {
            if( resp_it->startsWith( "Content-Length:" ) ) {
                request_length = resp_it->mid( 15 ).toLong();
                break;
            }
            if( resp_it->startsWith( "Timeout:" ) ) {
//                 _timeout = resp_it->mid( 8 ).toInt();
                break;
            }
        }
        // Como para el header usamos un streambuff y para el resto tenemos que usar la longitud leyendo
        // directamente del socket, nos queda en el buffer una determinada cantidad de bytes que no sé como
        // sacarlos directamente. Aquí los saco con >> pero eso quita los espacios, que pueden ser " " o "\r\n"
        // así que la longitud final no coincide
        std::string ret;
        if( request_length != 0L ) {
            // Write whatever content we already have to output.
            std::string s1;
            size_t already_read = response.size();
            while( response.size() > 0) {
                response_stream >> s1; // up to the next blank
                ret += s1 + " ";
            }
            if( already_read < request_length ) {
                char *reply = new char[request_length - already_read + 1];
                // Process the content
                boost::asio::read(socket, boost::asio::buffer(
                                      reply, request_length - already_read ));
                reply[request_length - already_read] = '\0';
                ret += reply;
            }
        } else {
            std::string s1;
            while( response.size() > 0) {
                response_stream >> s1;
                ret += s1;
            }
            while( true ) {
                char reply[2];
                reply[1] = '\0';
                try {
                    boost::asio::read(socket, boost::asio::buffer(reply, 1 ) );
                    ret += Xtring(1, reply[0]);
                } catch (std::exception& e) {
                    break;
                }
            }
        }
        *theContent = strdup( ret.c_str() );
    }
    catch (std::exception& e)
    {
        std::cout << "Exception: " << e.what() << "\n";
    }
    return status_code;
}

} // namespace gong

/*
 * Mostrar el uso del ancho de banda
 * while true; do cat /proc/net/dev; sleep 1; done | awk -v dc="date \"+%T\"" '/eth0/{i = $2 - oi; o = $10 - oo; oi = $2; oo = $10; dc|getline d; close(dc); if (a++) printf "%s %8.2f KiB/s in %8.2f KiB/s out\n", d, i/1024, o/1024}'
*/
