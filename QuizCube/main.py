import sys
import websocket
if sys.version_info[0] < 3:
	raise("! ! ! ! ! This program requires Python 3 ! ! ! ! !")

from QuizCube import QuizCube
from ExampleTest import ExampleTest
from ExpSimDemoTest import ExpSimDemoTest
from ExpSimI2C import ExpSimI2C
import time
from wsInterface import *



#----------------- This is all the code needed for the websocket connection, if 
#a different interface is used change the following code:
def onWsMessage(msg):
	#print("Got message",msg)
	qq.propogateMessage(json.loads(msg))

def testMessageCallback(msg):
	#TODO: Connect to outbound communication channel
	#print("outbound test cmd:",msg)
	sendWSMessage(msg)

startWebSocketHandler(onWsMessage)
#-------------------------------

#Create QuizCube and register a callback for tests to send messages
qq=QuizCube(testMessageCallback)
#qq.registerTest(ExampleTest("Demo test","Extended description",1))

expSimId=101 #The id stored in the exp sim EEPROM!
#qq.registerTest(ExpSimDemoTest("Demo Exp Sim Test","A test to demonstrate the experiment simulator ws functionallity",expSimId))

for id in [100,101,102,109,156,150]:

	
	qq.registerTest(ExpSimI2C("Demo Exp Sim Test","A test to demonstrate the experiment simulator ws functionallity",id))
	
	for t in range(10):
		#wsSendJson({'es':{'cmd':"V",'id':1}})
		qq.tick(t)
		time.sleep(0.1)
	qq.clearTests()






