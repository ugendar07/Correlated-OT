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


## Prerequisites

### Server (C++)
- C++17+
- Asio
- NanoPB
- Trezor Crypto

### Client (Node.js)
- Node.js (v18+)
- TypeScript
- `protobufjs`, `noble-secp256k1`, `@noble/hashes`

---

## COT Calculations 

### Message Construction (Server)
For each bit `i`:
    Ui = H(Ri) // first byte of ECDH hash
    m0 = Ui
    m1 = Ui + x

    mc = Ui + yi ∗ x

    c0[i] = m0
    c1[i] = m1


###Alice’s share:
    U = - ∑ (2^i * Ui)

### Message Processing (Client)
Bob computes:
    mcᵢ = mᵢ - H(Rᵢ)

computes 
    V = ∑ (2^i * mcᵢ)

---

## Running the Project

### 1. Compile the Server

Make sure Boost, NanoPB, and Trezor Crypto are included properly. Use `CMakeLists.txt` or compile manually:

generate message.ph.h and message.pb.c and move them to include and src respectively 
```bash
    python3 path=/nanopb/generator/nanopb_generator.py message.proto
```

Compile and Run
```bash
    cmake -G "MinGW Makefiles" ..
    cmake --build .
    ./COTServer

```

### 1. Compile the Client

generate message.js and message.d.ts and move them to generated
```bash
    pbjs -t static-module -w commonjs -o message.js message.proto
    pbts -o message.d.ts message.js
```
Run
```bash
    npx ts-node .\src\cot_client.ts
```





