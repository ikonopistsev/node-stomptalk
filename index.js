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

let frameCount = 0;
stompTalk.on('frameStart', () => {
    //console.log('frameStart');
    ++frameCount;
});
stompTalk.on('method', methodName => {
    //console.log('method:', methodName);
});
stompTalk.on('headerKey', headerKey => {
    //console.log('headerKey:', headerKey);
});
stompTalk.on('headerVal', headerVal => {
    //console.log('headerVal:', headerVal);
});
stompTalk.on('body', body => {
    //console.log('body:', body);
});
stompTalk.on('frameEnd', () => {
    //console.log('frameEnd');
});

const noNR = Buffer.concat([
    Buffer.from("CONNECTED\nversion:1.2\nsession:STOMP-PARSER-TEST\nserver:stomp-parser/1.0.0\n\n\0"),
    Buffer.from("MESSAGE\nid:0\ndestination:/queue/foo\nack:client\n\n\0"),
    Buffer.from("MESSAGE\nid:0\n\n\0"),
    Buffer.from("MESSAGE\nid:0\n\n\0"),
    Buffer.from("MESSAGE\nsubscription:0\nmessage-id:007\ndestination:/queue/a\ncontent-length:13\ncontent-type:text/plain\nmessage-error:false\n\nhello queue a\0"),
    Buffer.from("MESSAGE\nsubscription:0\nmessage-id:007\ndestination:/queue/a\ncontent-type:application/json\nmessage-no-content-length:true\n\n[1,2,3,4,5,6,7]\0\n\n\n\n\0"),
    Buffer.from("MESSAGE\nsubscription:0\nmessage-id:007\ndestination:/queue/a\ncontent-length:13\ncontent-type:text/plain\nmessage-error:false\n\nhello queue a\0"),
    Buffer.from("MESSAGE\nsubscription:0\nmessage-id:007\ndestination:/queue/a\ncontent-length:13\nmessage-error:false\n\nhello queue a\0"),
    Buffer.from("MESSAGE\nsubscription:0\nmessage-id:007\ndestination:/queue/a\n\nhello queue a\0"),
    Buffer.from("MESSAGE\nreceipt:77\n\n\0")
]);

//const rc = stompTalk.parse(Buffer.from('CONNECTED\nversion:1.2\nsession:STOMP-PARSER-TEST\nserver:stomp-parser/1.0.0\n\n\0'));

const s = new Date().getTime();
const count = 25000;
let i = 0;
for ( ; i < count; ++i)
{
    stompTalk.parse(noNR);
}

console.log(frameCount, (new Date().getTime() - s) / 1000.0);
