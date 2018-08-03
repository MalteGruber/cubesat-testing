from AbstractTest import AbstractTest
from Verifiers import *

class ExpSimI2C(AbstractTest):
	def __init__(self, testName, testDescription,simId):
		AbstractTest.__init__(self,testName, testDescription)

		self.id=simId #Stored in exp sim eeprom
		
	def tick(self,time):
		#Create a sequencer for the current time
		step=Sequencer(time);

		#This is the staging step
		if(step.ready()):
			self.stage("Set EN high and test ready pin")
		#This is the command step
		if(step.ready()):
			self.sendCommand({'es':{'cmd':"e1",'id':self.id}})
		if(step.ready()):
			self.sendCommand({'es':{'cmd':"i",'id':self.id}})			
		#This is the verification step
		if(step.ready()):
			#Verify that the experimet simulator returned the correct id.
			self.check(ValueTest(["es","id"],self.id))
			self.check(ValueTest(["es","i2cReady"],1))
			#self.check(ValueTest(["es","i2cEnabled"],1))
			self.runVerification()

		#This is the staging step
		if(step.ready()):
			self.stage("Set EN low and test ready pin")
		#This is the command step
		if(step.ready()):
			self.sendCommand({'es':{'cmd':"e0",'id':self.id}})
		if(step.ready()):
			self.sendCommand({'es':{'cmd':"i",'id':self.id}})			
		#This is the verification step
		if(step.ready()):
			#Verify that the experimet simulator returned the correct id.
			self.check(ValueTest(["es","id"],self.id))
			self.check(ValueTest(["es","i2cReady"],0))	
			#self.check(ValueTest(["es","i2cEnabled"],0))			
			self.runVerification()
		if(step.ready()):
			return True
		return False


