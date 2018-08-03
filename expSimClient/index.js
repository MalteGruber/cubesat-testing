var maltuino=require("./expSim");
var colors = require('colors');



//ws
var connection_=null;
function sendWsJSON(msg){
	if(connection_){
	//	console.log("SENDING ",JSON.stringify(msg))
		connection_.sendUTF(JSON.stringify(msg));
	}else{
        console.error("Websocket not yet connected, message not sent!")
    }
	}

function printUsage(){
	console.log('Experiment simulator UART interface handler'.rainbow);
	console.log('Hello world'.inverse);
}

printUsage();
maltuino.setOnExpMessageCallback((a)=>{

	if(isJsonString(a.msg)){
        //    console.log("Got message from simulator:".rainbow,a)
		//WTF, go home, its late...
	//	msg.msg.msg=JSON.parse(msg.msg.)
		var x=JSON.parse(a.msg);
			x.id=a.id
		sendWsJSON({es:x})
	}
});

maltuino.setOnReadyCallback(()=>{console.log("READY!!!".green)});


function isJsonString(str){
	try{
		JSON.parse(str)
		return true;
	}catch(e){
		return false;
	}
}



console.log("CONNECTING TO WebSocket".rainbow)


var WebSocketClient = require('websocket').client;
var client = new WebSocketClient();


client.on('connectFailed', function(error) {
    console.log('Connect Error: ' + error.toString());
});
 

client.on('connect', function(connection) {
    console.log('WebSocket Client Connected');
    

    connection_=connection;
              sendWsJSON({es:"hellO THERE!"})

    connection.on('error', function(error) {
        console.log("Connection Error: " + error.toString());
    });
  
    connection.on('close', function() {
        console.log('echo-protocol Connection Closed');
    });
   

   /** Messages from the web socket are received here. If they are addressed for the experiment simulator 
   the command is executed */
    connection.on('message', function(message) {
        console.log("MESSAGE",message)

        try{
        if (message.type === 'utf8') {
      //      console.log("Received: '" + message.utf8Data + "'");
            if(isJsonString(message.utf8Data)){
            var obj=JSON.parse(message.utf8Data);
        //    console.log("JSON".green,obj);
            if(obj.es.cmd){

            		maltuino.sendToExpSim(obj.es.cmd,obj.es.id);
            }
            if(obj.es.list){
            	sendWsJSON(maltuino.getDevices());
            }
            if(obj.es.scan){
                maltuino.scanDevices();
            }
        }
        }
    }catch(e){}
    });

});
 
//client.connect('ws://localhost:8080/');
client.connect("ws://46.101.210.197:9002/");