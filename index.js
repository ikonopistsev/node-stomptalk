const EventEmitter = require("node:events");
const os = require("os");

let modulePath = './build/Release/node-stomptalk-native';
if (os.type() == 'Windows_NT') {
    nativePath = './build/RelWithDebInfo/node-stomptalk-native';
};

const { NativeStomptalk } = require(modulePath);

class StompTalk extends EventEmitter {
    constructor() {
        super();
        this.nativeStompTalk = new NativeStomptalk();
    }

    parse(data) {
        return this.nativeStompTalk.parse(data,
            err => {
                this.emit('error', err);
            },
            () => {
                this.emit('frameStart');
            },
            methodName => {
                this.emit('method', methodName);
            },
            headerKey => {
                this.emit('headerKey', headerKey);
            },
            headerVal => {
                this.emit('headerVal', headerVal);
            },
            body => {
                this.emit('body', body);
            },
            () => {
                this.emit('frameEnd');
            });
    }
}

const stompTalk = new StompTalk();
stompTalk.on('error', err => {
    console.log('error:', err);
});
stompTalk.on('frameStart', () => {
    console.log('frameStart');
});
stompTalk.on('method', methodName => {
    console.log('method:', methodName);
});
stompTalk.on('headerKey', headerKey => {
    console.log('headerKey:', headerKey);
});
stompTalk.on('headerVal', headerVal => {
    console.log('headerVal:', headerVal);
});
stompTalk.on('body', body => {
    console.log('body:', body);
});
stompTalk.on('frameEnd', () => {
    console.log('frameEnd');
});

const rc = stompTalk.parse(Buffer.from('CONNECTED\nversion:1.2\nsession:STOMP-PARSER-TEST\nserver:stomp-parser/1.0.0\n\n\0'));
console.log("parsed: ", rc);