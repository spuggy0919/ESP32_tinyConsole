// websocket protocol
var gateway = `ws://${window.location.hostname}/ws`;
var websocket;
var pingTimer;
let replyPromise;
var charbuf;
var fifo = new Array("");
let retries=0;
let maxRetries=3;
var restartws=true;
window.addEventListener('load', onload);

function onload(event) {
    restart=true;
    initWebSocket();

}
function initWebSocket() {
    MonitorConsoleLog('Trying to open a WebSocket connection…');
    MonitorConsoleLog(gateway);
    if (!restartws) {
        websocket=null;
        restartws=false;
        MonitorConsoleLog('reentry');
        return false;
    }
    websocket = new WebSocket(gateway);
    websocket.onopen = onOpen;
    websocket.onclose = onClose;
    // websocket.onerror = onError;
    websocket.onmessage = onMessage;
    // setInterval(wsGetCharRequest, 200);
    MonitorConsoleLog('done');
    return true;
}
/* websocket reliablity Test Function */
/* websocket Send function */
function sendMessage(msg){
    // Wait until the state of the socket is not ready and send the message when it is...
    waitForSocketConnection(websocket, function(){
        let nows = Date.now()
        MonitorConsoleLog(nows+":ESP32<---CLIENT:"+msg);
        websocket.send(msg);
        if (msg[0]!='P') resetPingTimer();
    });
}

// Make the function wait until the connection is made...
function waitForSocketConnection(socket, callback){
    setTimeout(
        function () {
            if (socket.readyState === WebSocket.OPEN) {
                // MonitorConsoleLog("Connection is made")
                if (callback != null){
                    callback();
                }
            } else {
                // **BUG?** MonitorConsoleLog("wait for connection...")
                if (socket==null || socket.readtState==WebSocket.CLOSED){
                         initWebSocket();
                }
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
        // Reconnect after a delay (e.g., 5 seconds)

        restartws=true;
        if (!initWebSocket){//if restart fail keep old for retry
            websocket = oldws;
        }
    });
}
function resetPingTimer() {
    // Clear the existing timer
    clearTimeout(pingTimer);
  
    // Start a new timer
    startPingTimer();
  }
function startPingTimer() {
  pingTimer = setTimeout(() => {
    // Send ping message to the server
    wsPingRequest()
  }, 5000); // Set the ping interval to 5 seconds (adjust as needed)
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
    // sendTime(); change on demand
    uiInit();
    editor.dispatchEvent(eventk); // check editor for IME
    fifo = [];
    wsSentVersion("0.01"); // send textarea keyin 
    retries = 0; // Reset retry counter on successful connection

    // setInterval(wswsupdateText, 50);
    // Periodically send PING frames (adjust as needed)
    const pingTimer = setTimeout(() => {
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
    clearTimeout(pingTimer);
    if (retries < maxRetries) {
        var oldws = websocket;
        retries++;
        // Reconnect after a delay (e.g., 5 seconds)

        restartws=true;
        if (!initWebSocket){//if restart fail keep old for retry
            websocket = oldws;
        }
      } else {
        MonitorConsoleLog('Maximum retry attempts reached, closed');
      }
}
const messageQueue = [];
function onMessage(event) {
    var lines= event.data.split(":");
    let ldata ;
    if (lines[0]=='1') {
        ldata = atob(lines[1]);
        // Resolve the reply promise with the received message
        if (ldata=='P:PONG' && replyPromise) { // for pong
            let nowt=Date.now();
            MonitorConsoleLog(nowt+':PONG received');

            replyPromise.resolve(ldata);
            replyPromise = null;        
        }else {
            updateConsoleLog(ldata);
        }
        resetPingTimer();
        return ;
    }
    // Add the received message to the queue
    messageQueue.push(event.data);
    resetPingTimer();
    websocket.send("A:"); // reply ack for avoid reentry so atfer process data
    // Process the message queue asynchronously
    setTimeout(processMessagesQueue(),0);
}
// Asynchronous function to process the message queue
let processing = false
function processMessagesQueue() {
    // Process messages one by one
    // Check if there is already a background processing task
    if (!processing) {
        processing = true;
    if (messageQueue.length > 0) {
        // Start processing messages asynchronously
        const msg = messageQueue.shift();
        processMessages(msg)
        setTimeout(processMessagesQueue(),0);
    }
        processing = false;
    }
}

// Asynchronous function to simulate processing messages  
function processMessages(msg) {
        // console.log(event.data);
        // var msg = event.data;
        var lines= msg.split(":");
        let ldata;


        if (lines[0]!='2') {
            ldata = atob(lines[1]);
        }
        // MonitorConsoleLog('processMessage:'+lines[0]+ldata);
        switch(lines[0]){
            case "0": // command shell
            updateTextarea(ldata);
            break;
            case "1": // Serial Monitor
            // Resolve the reply promise with the received message
            if (ldata=='P:PONG' && replyPromise) { // for pong
                    MonitorConsoleLog('PONG received');

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
        // if (lines[0]=="0") {
            // updateTextarea(lines[1]);
        // }else{
        //   updateConsoleLog(lines[1]);
        // }
}

// WS endding -----