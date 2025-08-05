# Correlated Oblivious Transfer (COT) over TCP

This project implements a 32-bit **Correlated Oblivious Transfer** protocol using:
- **Server**: C++ with Boost Asio, NanoPB, and Trezor Crypto
- **Client**: Node.js with TypeScript using `net`, `crypto`, `protobufjs`, and `noble-secp256k1`

The protocol allows Alice (Server) and Bob (Client) to compute additive shares of a product:


Where:
- Alice chooses `x` randomly
- Bob inputs `y`
- `U` is held by Alice, `V` is computed by Bob

## Project Structure


├── build

│ └── CMake files and server.ex file

├── client/

│ └── client.ts # Node.js client implementation

├── external/trezor_crypto # trezor_crypto library (submodules and .h files)

├── include/

│ ├── message.proto # Protocol Buffers schema

│ └── message.pb.h/.c # Auto-generated via NanoPB

├── nanopb/ # NanoPB library (submodules and downloaded)

├── node_modules/ # Node Modules (protobufjs and lock.json)

├── src/

│ ├── main.cpp # Entry point (Boost Asio TCP server)

│ ├── cot.cpp / cot.hpp # Core COT logic

│ ├── message.proto # Protocol Buffers schema

│ └── message.pb.h/.c # Auto-generated via NanoPB

├── nanopb/ # NanoPB library (submodule or downloaded)

│
└── CMakeLists.txt

│
└── message.proto

│
└── out_put.txt

│
└── README.md


---





