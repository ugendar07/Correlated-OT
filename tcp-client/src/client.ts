import * as net from 'net';
import * as protobuf from 'protobufjs';
import path from 'path';
import * as readline from 'readline';

async function main() {
    const root = await protobuf.load(path.join(__dirname, '../protos/message.proto'));
    const ClientMessage = root.lookupType('ClientMessage');
    const ServerResponse = root.lookupType('ServerResponse');

    const client = new net.Socket();

    const rl = readline.createInterface({
        input: process.stdin,
        output: process.stdout
    });

    client.connect(8080, '127.0.0.1', () => {
        console.log('[Client] Connected to server');

        rl.question('Enter command: ', (inputCommand) => {
            const messagePayload = { command: inputCommand };
            const errMsg = ClientMessage.verify(messagePayload);
            if (errMsg) throw Error(errMsg);

            const messageBuffer = ClientMessage.encode(ClientMessage.create(messagePayload)).finish();
            client.write(messageBuffer);
        });
    });

    client.on('data', (data) => {
        const decoded = ServerResponse.decode(data);
        const obj = ServerResponse.toObject(decoded, { bytes: String });

        console.log('[Client] From server:', obj);

        client.end();
        rl.close();
    });

    client.on('close', () => {
        console.log('Connection closed');
    });

    client.on('error', (err) => {
        console.error('Client error:', err.message);
        rl.close();
    });
}

main().catch(console.error);
