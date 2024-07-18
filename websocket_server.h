#ifndef WEBSOCKET_SERVER_H
#define WEBSOCKET_SERVER_H

#include <boost/asio.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <string>

namespace net = boost::asio;            // from <boost/asio.hpp>
namespace websocket = boost::beast::websocket; // from <boost/beast/websocket.hpp>
using tcp = net::ip::tcp;       // from <boost/asio/ip/tcp.hpp>
namespace http = boost::beast::http;    // from <boost/beast/http.hpp>

class WebsocketServer {
public:
    WebsocketServer(net::io_context& ioc, tcp::endpoint endpoint);
    void run();

private:
    void doSession(tcp::socket socket);
    net::io_context& ioc_;
    tcp::acceptor acceptor_;
};

std::string generateSensorData();

#endif // WEBSOCKET_SERVER_H
