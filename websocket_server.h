#ifndef WEBSOCKET_SERVER_H
#define WEBSOCKET_SERVER_H

#include <boost/asio.hpp>
#include <boost/beast.hpp>

namespace beast = boost::beast;           // from <boost/beast.hpp>
namespace http = boost::beast::http;      // from <boost/beast/http.hpp>
namespace websocket = boost::beast::websocket; // from <boost/beast/websocket.hpp>
namespace net = boost::asio;              // from <boost/asio.hpp>
using tcp = boost::asio::ip::tcp;         // from <boost/asio/ip/tcp.hpp>

class WebsocketServer {
public:
    explicit WebsocketServer(net::io_context& ioc, tcp::endpoint endpoint);

    // Start accepting incoming connections
    void run();

private:
    void doSession(tcp::socket& socket);

    net::io_context& ioc_;
    tcp::acceptor acceptor_;
};

#endif // WEBSOCKET_SERVER_H
