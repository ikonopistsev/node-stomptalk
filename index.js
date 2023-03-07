const EventEmitter = require("node:events");
const os = require("os");

let nativePath;
if (os.type() == 'Windows_NT') {
    nativePath = './build/RelWithDebInfo/node-stomptalk-native';
} else {
    nativePath = './build/Release/node-stomptalk-native';
};

const { NativeStomptalk } = require(nativePath);

class StompTalk extends EventEmitter {
    constructor() {
        super();
        const nativeStompTalk = new NativeStomptalk(19, (text) => {
            console.log("onFrame:", text);
        });
        nativeStompTalk.plusOne((text)=>{
            console.log("callback:", text);
        });
        nativeStompTalk.onFrame();
    }
}

new StompTalk();