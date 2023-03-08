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
const rc = stompTalk.parse((value) => {
    console.log("cb: ", value);
})
console.log("rc: ", rc);