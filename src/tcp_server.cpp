#define ASIO_STANDALONE
#define ASIO_DISABLE_THREADS


#include <iostream>
#include <asio.hpp>  // or boost/asio.hpp

#include "pb.h"
#include "pb_encode.h"
#include "pb_decode.h"
#include "message.pb.h"

using asio::ip::tcp;

// Optional helper function to print errors
void print_error(const char* label, const pb_ostream_t& stream) {
    std::cerr << label << ": " << PB_GET_ERROR(&stream) << "\n";
}

// Function to handle client: decode input, encode response
void handle_client(tcp::socket& socket) {
    try {
        uint8_t buffer[256];
        size_t len = socket.read_some(asio::buffer(buffer));
        
        // 1. Decode the incoming protobuf message
        ClientMessage message = ClientMessage_init_zero;
        pb_istream_t istream = pb_istream_from_buffer(buffer, len);

        if (!pb_decode(&istream, ClientMessage_fields, &message)) {
            std::cerr << "Failed to decode message: " << PB_GET_ERROR(&istream) << "\n";
            return;
        }

        std::cout << "[Server] Received command: " << message.command << "\n";

        // 2. Create a ServerResponse message
        ServerResponse response = ServerResponse_init_zero;
        strcpy(response.status, "OK");

        std::string response_text = "Command received: ";
        response_text += message.command;

        response.payload.size = response_text.size();
        memcpy(response.payload.bytes, response_text.c_str(), response_text.size());

        // 3. Encode response to buffer
        uint8_t out_buf[256];
        pb_ostream_t ostream = pb_ostream_from_buffer(out_buf, sizeof(out_buf));

        if (!pb_encode(&ostream, ServerResponse_fields, &response)) {
            print_error("Encoding failed", ostream);
            return;
        }

        size_t written = ostream.bytes_written;

        asio::write(socket, asio::buffer(out_buf, written));
        std::cout << "[Server] Sent response.\n";

    } catch (std::exception& e) {
        std::cerr << "Exception in handle_client: " << e.what() << "\n";
    }
}

// Main server logic
void run_server() {
    try {
        asio::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 8080));

        std::cout << "[Server] Listening on port 8080...\n";

        for (;;) {
            tcp::socket socket(io_context);
            acceptor.accept(socket);
            std::cout << "[Server] Client connected.\n";
            handle_client(socket);
        }
    } catch (std::exception& e) {
        std::cerr << "Server exception: " << e.what() << "\n";
    }
}
