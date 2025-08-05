# Correlated Oblivious Transfer (COT) over TCP

This project implements a 32-bit **Correlated Oblivious Transfer** protocol using:
- **Server**: C++ with Boost Asio, NanoPB, and Trezor Crypto
- **Client**: Node.js with TypeScript using `net`, `crypto`, `protobufjs`, and `noble-secp256k1`

The protocol allows Alice (Server) and Bob (Client) to compute additive shares of a product:


Where:
- Alice chooses `x` randomly
- Bob inputs `y`
- `U` is held by Alice, `V` is computed by Bob





