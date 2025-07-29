#define ASIO_STANDALONE
#define ASIO_DISABLE_THREADS

#include <iostream>
#include <asio.hpp>

#include "message.pb.h"
#include "pb_encode.h"
#include "pb_decode.h"
#include "pb_common.h"

using asio::ip::tcp;

int main() {
    try {
        asio::io_context io_context;
        tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 8080));

        std::cout << "[Server] Listening on port 8080...\n";

        for (;;) {
            tcp::socket socket(io_context);
            acceptor.accept(socket);
            std::cout << "[Server] Client connected.\n";

            // Step 1: Read from client
            uint8_t buffer[256];
            asio::error_code ec;
            size_t length = socket.read_some(asio::buffer(buffer), ec);

            if (ec) {
                std::cerr << "[Server] Read error: " << ec.message() << "\n";
                continue;
            }

            // Step 2: Decode ClientMessage
            ClientMessage message = ClientMessage_init_zero;
            pb_istream_t istream = pb_istream_from_buffer(buffer, length);

            if (!pb_decode(&istream, ClientMessage_fields, &message)) {
                std::cerr << "[Server] Failed to decode ClientMessage: " << PB_GET_ERROR(&istream) << "\n";
                continue;
            }

            std::cout << "[Server] Received command: " << message.command << "\n";

            // Step 3: Prepare ServerResponse
            ServerResponse response = ServerResponse_init_zero;
            strncpy(response.status, "OK", sizeof(response.status));

            const char* payload_text = "Hello from standalone Asio TCP server!";
            size_t payload_len = strlen(payload_text);

            // Allocate and fill payload buffer
            static ServerResponse_payload_t payload_buffer;
            payload_buffer.size = payload_len;
            memcpy(payload_buffer.bytes, payload_text, payload_len);

            // Assign the address of the static payload buffer
            response.payload.size = payload_len;
            memcpy(response.payload.bytes, payload_text, payload_len);

            // Step 4: Encode ServerResponse
            uint8_t outbuf[256];
            pb_ostream_t ostream = pb_ostream_from_buffer(outbuf, sizeof(outbuf));

            if (!pb_encode(&ostream, ServerResponse_fields, &response)) {
                std::cerr << "[Server] Failed to encode ServerResponse: " << PB_GET_ERROR(&ostream) << "\n";
                continue;
            }

            // Step 5: Send to client
            asio::write(socket, asio::buffer(outbuf, ostream.bytes_written));
            std::cout << "[Server] Response sent.\n";
        }

    } catch (std::exception& e) {
        std::cerr << "Server exception: " << e.what() << "\n";
    }

    return 0;
}
