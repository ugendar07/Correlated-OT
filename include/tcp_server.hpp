#pragma once

#define ASIO_STANDALONE
#define ASIO_DISABLE_THREADS

#include <asio.hpp>

class TcpServer {
public:
    TcpServer(asio::io_context& io_context, int port);
    void start();

private:
    void accept_and_respond();

    asio::io_context& io_context_;
    asio::ip::tcp::acceptor acceptor_;
};
