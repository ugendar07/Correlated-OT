"use strict";
var __createBinding = (this && this.__createBinding) || (Object.create ? (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    var desc = Object.getOwnPropertyDescriptor(m, k);
    if (!desc || ("get" in desc ? !m.__esModule : desc.writable || desc.configurable)) {
      desc = { enumerable: true, get: function() { return m[k]; } };
    }
    Object.defineProperty(o, k2, desc);
}) : (function(o, m, k, k2) {
    if (k2 === undefined) k2 = k;
    o[k2] = m[k];
}));
var __setModuleDefault = (this && this.__setModuleDefault) || (Object.create ? (function(o, v) {
    Object.defineProperty(o, "default", { enumerable: true, value: v });
}) : function(o, v) {
    o["default"] = v;
});
var __importStar = (this && this.__importStar) || (function () {
    var ownKeys = function(o) {
        ownKeys = Object.getOwnPropertyNames || function (o) {
            var ar = [];
            for (var k in o) if (Object.prototype.hasOwnProperty.call(o, k)) ar[ar.length] = k;
            return ar;
        };
        return ownKeys(o);
    };
    return function (mod) {
        if (mod && mod.__esModule) return mod;
        var result = {};
        if (mod != null) for (var k = ownKeys(mod), i = 0; i < k.length; i++) if (k[i] !== "default") __createBinding(result, mod, k[i]);
        __setModuleDefault(result, mod);
        return result;
    };
})();
var __importDefault = (this && this.__importDefault) || function (mod) {
    return (mod && mod.__esModule) ? mod : { "default": mod };
};
Object.defineProperty(exports, "__esModule", { value: true });
const net = __importStar(require("net"));
const protobuf = __importStar(require("protobufjs"));
const path_1 = __importDefault(require("path"));
const readline = __importStar(require("readline"));
async function main() {
    const root = await protobuf.load(path_1.default.join(__dirname, '../protos/message.proto'));
    const ClientMessage = root.lookupType('ClientMessage');
    const ServerResponse = root.lookupType('ServerResponse');
    const client = new net.Socket();
    const rl = readline.createInterface({
        input: process.stdin,
        output: process.stdout,
        prompt: 'Enter command> '
    });
    client.connect(8080, '127.0.0.1', () => {
        console.log('[Client] Connected to server');
        rl.prompt();
    });
    // Handle input from terminal
    rl.on('line', (line) => {
        const inputCommand = line.trim();
        if (inputCommand.toLowerCase() === 'exit') {
            rl.close();
            client.end();
            return;
        }
        const messagePayload = { command: inputCommand };
        const errMsg = ClientMessage.verify(messagePayload);
        if (errMsg) {
            console.error('[Client] Invalid command:', errMsg);
            rl.prompt();
            return;
        }
        const messageBuffer = ClientMessage.encode(ClientMessage.create(messagePayload)).finish();
        client.write(messageBuffer);
    });
    // Handle response from server
    client.on('data', (data) => {
        const decoded = ServerResponse.decode(data);
        const obj = ServerResponse.toObject(decoded, { bytes: String });
        console.log(`[Server] status: ${obj.status} | payload: ${obj.payload}`);
        rl.prompt();
    });
    client.on('close', () => {
        console.log('🔌 Connection closed');
        process.exit(0);
    });
    client.on('error', (err) => {
        console.error('❌ Client error:', err.message);
        rl.close();
    });
    rl.on('close', () => {
        console.log('[Client] Exiting...');
        client.end();
    });
}
main().catch(console.error);
