const StompTalk = require("./index.js");

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
   
const stompTalk = new StompTalk();

stompTalk.on('error', errCode => {
    console.log('error:', errCode);
});
stompTalk.on('frameStart', () => {
    console.log('frameStart');
});
stompTalk.on('method', name => {
    console.log('method:', name);
});
stompTalk.on('headerKey', (headerKey) => {
    console.log('headerKey:', headerKey);
});
stompTalk.on('headerVal', headerVal => {
    console.log('headerVal:', headerVal);
});
stompTalk.on('body', (data, contentLength, contentLeft) => {
    console.log('body:', data.toString('ascii'), contentLength, contentLeft )
})
stompTalk.on('frameEnd', () => {
    console.log('frameEnd');
});

// parse all buffer
stompTalk.parse(noNR);

// parse stream of chars
// const strBuf = noNR.toString('ascii');
// for (let i = 0; i < strBuf.length; ++i) {
//     stompTalk.parse(Buffer.from(strBuf[i]));
// }