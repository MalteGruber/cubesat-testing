import sys
import websocket
if sys.version_info[0] < 3:
	raise("! ! ! ! ! This program requires Python 3 ! ! ! ! !")

from QuizCube import QuizCube
from ExampleTest import ExampleTest
import time
from wsInterface import *

def testMessageCallback(msg):
	#TODO: Connect to outbound communication channel
	print("outbound test cmd:",msg)

#Create QuizCube and register a callback for tests to send messages
qq=QuizCube(testMessageCallback)
qq.registerTest(ExampleTest("Demo test","Extended description",1))

print("----- step 1, staging")
qq.tick(0)

print("----- step 2, commanding")
qq.tick(1)

#This is a example response from the device being tested
qq.propogateMessage({'es':{'cmd':"C","voltage":12.3,"current":103,'id':2}})
qq.propogateMessage({'es':{'cmd':"C","current":103,'id':2}})
qq.propogateMessage({'es':{'cmd':"C","current":103,'id':2}})

print("----- step 3, verification")
qq.tick(2)
qq.tick(3)
qq.tick(4)
qq.tick(5)
#Tick functions should just be called in a while loop and return something to indicate 
#that they are done!
time.sleep(0.1)





while True:
	time.sleep(1)
	wsSendJson({'es':{'cmd':"V",'id':1}})