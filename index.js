const EventEmitter = require("node:events");
const os = require("os");

let modulePath = './build/Release/node-stomptalk-native';
if (os.type() == 'Windows_NT') {
    nativePath = './build/RelWithDebInfo/node-stomptalk-native';
};

const { NativeStompTalk } = require(modulePath);

class StompTalk extends EventEmitter {
    constructor() {
        super();
        this.nativeStompTalk = new NativeStompTalk();
    }

    parse(data) {
        return this.nativeStompTalk.parse(data,
            errCode => {
                this.emit('error', errCode);
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
            (body, contentLength, contentLeft) => {
                this.emit('body', body, contentLength, contentLeft);
            },
            () => {
                this.emit('frameEnd');
            });
    }
}

module.exports = StompTalk;