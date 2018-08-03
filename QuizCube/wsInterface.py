import sys
import websocket
import json
if sys.version_info[0] < 3:
    raise("! ! ! ! ! This program requires Python 3 ! ! ! ! !")

import websocket
import _thread
import time

port = "9002"


onMessageCallback=None;

def wsSendJson(arg):

    ws.send((json.dumps(arg)))

def on_message(ws, message):
    print("REC",message)
    onMessageCallback(message)

def on_error(ws, error):
    print(error)

def on_close(ws):
    print("### closed ###")

def on_open(ws):
    def run(*args):
        while True:
            time.sleep(5)
          #  ws.send("Hello")
        time.sleep(1)
        ws.close()
    _thread.start_new_thread(run, ())


def wsThread(*args):
    global ws
    #websocket.enableTrace(True)
    ws = websocket.WebSocketApp("ws://46.101.210.197:" + port + "/",
                                  on_message = on_message,
                                  on_error = on_error,
                                  on_close = on_close)
    ws.on_open = on_open
    ws.run_forever()

def sendWSMessage(message):
    #print("WS SENDING",message)    
    wsSendJson(message)


time.sleep(0.4)

def broadcast(msg):
    ws.send(msg)


def startWebSocketHandler(onMessageCallback_):
    global onMessageCallback
    onMessageCallback=onMessageCallback_
    print("Starting websocke client")
    _thread.start_new_thread(wsThread, ())    

#ws.send(json.dumps({'es':{'cmd':"V",'id':(channel)}}))




