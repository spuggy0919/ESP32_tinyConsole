// websocket protocol
var gateway = `ws://${window.location.hostname}/ws`;
var websocket;
var pingInterval;
let replyPromise;
var charbuf;
var fifo = new Array("");
let retries=0;
let maxRetries=3;
window.addEventListener('load', onload);

function onload(event) {
    initWebSocket();

}
function initWebSocket() {
    MonitorConsoleLog('Trying to open a WebSocket connection…');
    MonitorConsoleLog(gateway);

    websocket = new WebSocket(gateway);
    websocket.onopen = onOpen;
    websocket.onclose = onClose;
    // websocket.onerror = onError;
    websocket.onmessage = onMessage;
    // setInterval(wsGetCharRequest, 200);

}
/* websocket reliablity Test Function */
/* websocket Send function */
function sendMessage(msg){
    // Wait until the state of the socket is not ready and send the message when it is...
    waitForSocketConnection(websocket, function(){
        // MonitorConsoleLog("ESP32<---CLIENT:"+msg);
        websocket.send(msg);

    });
}

// Make the function wait until the connection is made...
function waitForSocketConnection(socket, callback){
    setTimeout(
        function () {
            if (socket.readyState === 1) {
                // MonitorConsoleLog("Connection is made")
                if (callback != null){
                    callback();
                }
            } else {
                // **BUG?** MonitorConsoleLog("wait for connection...")
                // waitForSocketConnection(socket, callback);
            }

        }, 5); // wait 5 milisecond for the connection...
}
function createPromiseWithTimeout(timeoutMillis) {
    let resolveFn, rejectFn;

    const promise = new Promise((resolve, reject) => {
        resolveFn = resolve;
        rejectFn = reject;
    });

    const timeoutId = setTimeout(() => {
        rejectFn('Timeout');
        replyPromise = null;
    }, timeoutMillis);

    return {
        promise,
        resolve: (value) => {
            clearTimeout(timeoutId);
            resolveFn(value);
        },
        reject: (reason) => {
            clearTimeout(timeoutId);
            rejectFn(reason);
        }
    };
}
function wsPingRequest(){
    if (!websocket) return;
    // if (websocket.readyState!==1) return;
    sendMessage("P:PING");
    // Set up a new promise for the reply with a timeout of 5 seconds
    replyPromise = createPromiseWithTimeout(2000);
    replyPromise.promise.then((reply) => {
        // MonitorConsoleLog(`Reply received: ${reply}`);
    }).catch((error) => {
        MonitorConsoleLog(`Reply timeout or error: ${error}`);
    });
}
function wsSentDelayChar(key){
    if (!websocket) return;
    sendMessage("ACK");
}
function wsGetCharRequest(){
    if (!websocket) return;
    sendMessage("R:");

}
function wsSentChar(key){ // send textarea keyin 
    if (!websocket) {
    // setTimeout(wsSentDelayChar(key), 2000);
    return;
    }
    console.log("key");
    console.log(key.length);
    console.log("X:"+key);
    sendMessage("X:"+key);

}

function wsSentDelayChar(key){
    sendMessage("X:"+key);
}

function wswsupdateText(){
    // console.log(wswsupdateText)
    if (fifo.length) {    
        // console.log(fifo.length);
        updateTextarea(fifo.shift());
    }
    
}

function wsSentTouch(start,x,y){ // send textarea keyin 
    if (!websocket) {
    // setTimeout(wsSentDelayChar(key), 2000);
    return;
    }
    console.log("touch");
    console.log("T:"+x+":"+y);
    sendMessage("T:"+start+":"+x+":"+y);

}
function wsSentMouse(start,x,y){ // send textarea keyin 
    if (!websocket) {
    // setTimeout(wsSentDelayChar(key), 2000);
    return;
    }
    console.log("mouse");
    console.log("M:"+x+":"+y);
    sendMessage("M:"+start+":"+x+":"+y);

}

function wsSentVersion(version){ // send textarea keyin 
    if (!websocket) {
    // setTimeout(wsSentDelayChar(key), 2000);
    return;
    }
    console.log("key");
    console.log(version.length);
    console.log("V:"+version);
    sendMessage("V:"+version);

}

function wsSentTest(msg){ // send textarea keyin 
    if (!websocket) {
    // setTimeout(wsSentDelayChar(key), 2000);
    return;
    }
    console.log("key");
    console.log(version.length);
    console.log("T:"+version);
    sendMessage("T:"+msg);

}
function onOpen(event) {
    // console.log('Connection Websocket opened');
    // getValues();
    sendTime();
    uiInit();
    editor.dispatchEvent(eventk); // check editor for IME
    fifo = [];
    wsSentVersion("0.01"); // send textarea keyin 
    retries = 0; // Reset retry counter on successful connection

    // setInterval(wswsupdateText, 50);
    // Periodically send PING frames (adjust as needed)
    const pingInterval = setInterval(() => {
        wsPingRequest();
        // websocket.ping();
    }, 5000); // Adjust the interval as needed

}

function onClose(event) {
    // console.log('Connection Websocket closed');
//    setTimeout(initWebSocket, 2000);
    if (replyPromise) {
        replyPromise.reject('WebSocket connection closed');
        replyPromise = null;
    }
    clearInterval(pingInterval);
    if (retries < maxRetries) {
        retries++;
        // Reconnect after a delay (e.g., 5 seconds)
      //  setTimeout(initWebSocket, 5000); bug don't restart
      } else {
        MonitorConsoleLog('Maximum retry attempts reached, closed');
      }
}


function onMessage(event) {
    // console.log(event.data);
    var msg = event.data;
    var lines= msg.split(":");
    let ldata;


    if (lines[0]!='2') {
        ldata = atob(lines[1]);
    }
    switch(lines[0]){
        case "0": // command shell
        updateTextarea(ldata);
        break;
        case "1": // Serial Monitor
        // Resolve the reply promise with the received message
        if (ldata=='P:PONG' && replyPromise) { // for pong
                // MonitorConsoleLog('PONG received');

                replyPromise.resolve(ldata);
                replyPromise = null;
                return;
            
        }else {
            updateConsoleLog(ldata);
        }
        break;
        case "2": // Serial Monitor test fro ack 
            updateConsoleLog(lines[1]);
            break;
        case "8": // audio
        AudioFunc(ldata);
        break;
        case "9": // video 
        VideoFunc(ldata);
        break;
        case "T": // test protocol 
        var line=atob(lines[1]);
        // VideoFunc(line);
        break;

    }
    websocket.send("A:"); // reply ack for avoid reentry so atfer process data
    // if (lines[0]=="0") {
        // updateTextarea(lines[1]);
    // }else{
    //   updateConsoleLog(lines[1]);
    // }
}

// WS endding -----