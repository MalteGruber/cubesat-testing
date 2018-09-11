/*Parameters fade out over time, this is the minimum transperancy*/
var MIN_TRANSP = 0.15;

/*This is a list of all parameter displays and their filters*/
var paramList = [];

function sendJSONMessage(msg) {
    console.log("SENDING: ", (msg));
    websocket.send(msg);
}

function sendMessage(msg) {
    console.log("SENDING: ", JSON.stringify(msg));
    websocket.send(JSON.stringify(msg));
}

function quoteStyleJSON(noQuoteJson){
    return noQuoteJson.replace(/(['"])?([a-zA-Z0-9_]+)(['"])?:/g, '"$2": ');
}

/*Here we check for all html elements that contain the data-cubesat-param attribute
     for the once that do we get the filter and parameter string and parse it.
    The filter is contained within the brackets and there can be more than one filter
 sepparated by a ";". The parameter to displayed is shown to the right of the "->" sign.
 After the attribute is parsed the filter and parameter is added to a list of parameters
that is checked every time a message is received.
 **/
(function() {
    var allParamElements = document.querySelectorAll('[data-cubesat-param]');
    for (var i = 0; i < allParamElements.length; i++) {
        var element = allParamElements[i];
        /*Fading out wont work if this is not set*/
        element.style.opacity = MIN_TRANSP;

        var atrib = element.dataset.cubesatParam;
        /*Format: [FILTER;FILTER;FILTER]->PARAM*/
        var filter = atrib.split("->")[0];
        var param = atrib.split("->")[1];

        /*Remove brackets from filter and split at ";" to get an array of individual
        filters*/        
        filter = filter.replace("[", "");
        filter = filter.replace("]", "");

        var filters = filter.split(";")
        //TODO: Add scaling and units here
        paramList.push({
            filters: filters,
            param: param,
            element: element
        })
    }
})();

/*This function tests if the filter strings are triggered by the incomming message*/
function filterTriggered(filters, message) {
    console.log(filters)
    for (var i = 0; i < filters.length; i++) {
        /*Using eval is really bad practice, however; all data in the system 
         * should be trusted and this is not trivial to do in another way so 
         * I will leave this for now, maybe consider this as a TODO:...*/
       // console.log(eval("message." + filters[i]))
        try{
            /*We want to be able to detect zeros AND use equals signs in the
             * html code, hence we only check for null if there are no equals signs
             * in the filter!*/
            var rhs="!=null";
            if(filters[i].indexOf("=")>0){
                rhs=""//kill rhs, use the one provided in the html instead!
            }
            console.log("message." + filters[i]+rhs)
        if (!(eval("message." + filters[i]+rhs))) {
            return false;
        }
        }catch(e){
            return false;
        };
    }
    /*All filters matched or if there are no filters*/
    return true;
}
function setNiceParamText(element,message,param){
    
    var param=eval("message." + param);
    var unit="";
    var name="";
    var scale="";
    if(element.dataset.cubesatFormat){
        try{
        var obj=JSON.parse(quoteStyleJSON(element.dataset.cubesatFormat))        
        unit=" "+obj.unit;
        name=obj.name+": ";
        
        }catch(e){
            console.log("Could not read format attribute:",e)
        }
    }
    element.innerHTML = name+param+unit;
    element.style.opacity = 1;
}

/*This function checks if the any of the parameter displays filters matches
 * the incomming message*/
function testForParams(message) {
    console.log(paramList.length)
    for (var i = 0; i < paramList.length; i++) {
        var filters = paramList[i].filters;
        var param = paramList[i].param;
        /*Here we check the filter, if all filters match we set the parameter
         * value to the received value and set it to full oppacity*/
        if (filterTriggered(filters, message)) {
            setNiceParamText(paramList[i].element,message,param)

            
        }
    }
}

(function() {
    var allElements = document.querySelectorAll('[data-cubesat-btn]');

    function setButton(element) {
        (function() {
            allElements[i].addEventListener("click", function() {
                /*The JSON in the button declaraction can be written without quotes, we add them here*/
                var msg = quoteStyleJSON( element.dataset.cubesatBtn)
                sendJSONMessage(msg)
            });
        })();
    }
    for (var i = 0; i < allElements.length; i++) {
        var element = allElements[i];
                setButton(element)
        }
        })();
                /*Parameter displays fade out over time to indicate stale data. This is done
                 * here*/
                        (function() {
                        setInterval(()=>{
                        for(var i=0;i<paramList.length;i++) {
                        /*Set the parameter to zero tansparancy to indicate the update*/
                        console.log()
                                var transp=paramList[i].element.style.opacity;
                                if(transp>MIN_TRANSP)
                                paramList[i].element.style.opacity=transp-0.05;
                        }

                        },200);
                                })();
                        function onWsMessage(message) {
                        console.log("received ",message)
                                testForParams(message)
                                }
////var web
//socket = new WebSocket("ws:localhost:8080");
                console.log("Set IP here!")
                        var websocket=new WebSocket("ws://localhost:9002/");
                        websocket.onopen=function(event) {

                        websocket.onmessage=(msg)=>{

                        var message={};
                                try{
                                message=JSON.parse(msg.data);
                                }catch(e) {

                        }
                        onWsMessage(message)
    }
};



(function() {
    var allElements = document.querySelectorAll('[data-cubesat-input]');
    function setInput(element) {
 
            allElements[i].addEventListener("change", function() {
                var tmp=element.value              
              var data=element.dataset.cubesatInput;
             data= data.replace("INPUT",tmp);
                /*The JSON in the button declaraction can be written without quotes, we add them here*/
  
                var msg = data.replace(/(['"])?([a-zA-Z0-9_]+)(['"])?:/g, '"$2": ');
                sendJSONMessage(msg)
            });

    }
    for (var i = 0; i < allElements.length; i++) {
        var element = allElements[i];
        setInput(element)
    }
})();