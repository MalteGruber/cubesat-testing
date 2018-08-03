var colors = require('colors');
var fs= require('fs');
var DEV_PATH="/dev"
var SerialPort = require('serialport');
var onReadyCallback=null;
function containsString(str,x){
	if(str.indexOf(x)<0)
		return false;
	return true;
}

function isUartDeviceName(name){
	var uartNames=["ttyUSB","ttyACM","ttyAMC"];
	for (var i = 0; i < uartNames.length; i++) {
		if(containsString(name,uartNames[i]))
			return true;
	}
	return false;
}

function getAllUartDevices(){
	var uartDevices=[];
	/*tty{ACM|AMC|USB}*/
	var devFiles=getFileNames(DEV_PATH);
	for (var i = 0; i < devFiles.length; i++) {
		if(isUartDeviceName(devFiles[i])){
			uartDevices.push(devFiles[i]);
		}
	}
	return uartDevices;
}

function getFileNames(path) {
	var names=[];
	fs.readdirSync(path).forEach(file => {
		names.push(file);
	})
	return names;
}

function isJsonString(str){
	try{
		JSON.parse(str)
		return true;
	}catch(e){
		return false;
	}
}


/*Each uart device is opened in this function*/
function deviceHandler(device){

		//TODO: How to shut down? :P
			var port = new SerialPort(device, {baudRate: 115200});
			var recStr="";


	port.on('open', function() {
			port.write('\nv\n');
	});
	this.testing=function(){
		console.log("CLOSING DEVICE PORT ",device,"!!!");

		try{
		port.close();
	}catch(e){console.log("COULD NOT CLOSE DEVICE PORT",device,e)}
	};
	/*We  handle the returned data in this function, this is since we need to keep track of the device id for piceing together the ascii bytes to a complete string and send that string on a newline. We also need to do this to make sure to assing the device id to the correct port. The stuff going back to the user is sent to a generic return function that does not contain the id, device etc.*/

	port.on('data', 
	function onUartMsg(data) {
	    for (var k = 0; k < data.length; k++) {
	    	var c=String.fromCharCode(data[k]);
	    	if(c!=="\n"){	
	    		recStr+=c;
	    	}else{
	    		/*Hey! we got a complete line, lets handle it*/
	    		//console.log(recStr)
	    		if(isJsonString(recStr)){
	    			var obj=JSON.parse(recStr);
	    			if(obj.id){
	    				console.log("We got an id of ",obj.id)

	    				/*Update the device list and add the port so that the user can send stuff. OBS receiving is done here!!!*/
	    				devices.push({dev:device,port:port,id:obj.id})
	    			}
	    		}

	    		/*Send to a generic callback that all devices use*/
	    		onMessageCallback({msg:recStr,id:getDeviceIdFromDev(device)});
	    		//console.log("MESSAGE OK FOR ",index)
	    		recStr=""	
	    	}
	    }
		}
	);
	return this;
}

function getDeviceIdFromDev(dev){
		for (var i = 0; i < devices.length; i++) {
			if(devices[i].dev===dev)
				return devices[i].id;
	}
}

function closeAllPorts(){
		for (var i = 0; i < devices.length; i++) {
			devices[i].port.close();
	}
}

function getPortByDeviceById(id){
	for (var i = 0; i < devices.length; i++) {
	//	console.log("Testing",devices[i].id)
		if(devices[i].id===id)
			return devices[i].port;
	}
	return null;
}

/*Holds a list of devices as id and ports*/
var devices=[];

var deviceHandlers=[];
function createScanPromises(){
	var uartDevs=getAllUartDevices();
	for (var i = 0; i < uartDevs.length; i++) {
		var x=deviceHandler(DEV_PATH+"/"+uartDevs[i])
		deviceHandlers.push(x);
	}
}

exports.scanDevices =(e)=>{

	console.log("Scanning devices")


	for (var i = 0; i < deviceHandlers.length; i++) {
		deviceHandlers[i].testing();
	}
	return;



	/*First we close any open port*/
	//closeAllPorts();


	setTimeout(()=>{

	/*Next we get all uart devices that might have an experiment simulator connected*/
	uartDevs=getAllUartDevices();


	console.log(uartDevs)
	/*We then issue scanners for these ports*/
	createScanPromises();
},3000)
}

createScanPromises();




exports.sendToExpSim=function(msg,id){

	var port=getPortByDeviceById(id);

	if(!port){
		console.error("Could not find a device for id",id);
		return;
	}
	port.write(msg+'\n', function(err) {
		if (err) {
			return console.log('Error on write: ', err.message);
		}
	});	
};




/*
on receive issues events, we can bind callbacks to


*/


exports.setOnReadyCallback=function(cb){
	onReadyCallback=cb;
}
exports.setOnExpMessageCallback = function(cb){
	onMessageCallback=cb;
}



exports.getDevices = function(cb){
	var d=[];
	for (var i = 0; i < devices.length; i++) {
		d.push({id:devices[i].id,dev:devices[i].dev})
	}
	return {devices:d};
}

