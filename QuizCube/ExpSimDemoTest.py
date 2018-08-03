from AbstractTest import AbstractTest
from Verifiers import *

class ExpSimDemoTest(AbstractTest):
	def __init__(self, testName, testDescription,simId):
		AbstractTest.__init__(self,testName, testDescription)

		self.id=simId #Stored in exp sim eeprom
		
	def tick(self,time):
		#Create a sequencer for the current time
		step=Sequencer(time);

		#This is the staging step
		if(step.ready()):
			self.stage("Demonstration of id request and verification")
		#This is the command step
		if(step.ready()):
			self.sendCommand({'es':{'cmd':"v",'id':self.id}})
		#This is the verification step
		if(step.ready()):
			#Verify that the experimet simulator returned the correct id.
			self.check(ValueTest(["es","id"],self.id))
			self.runVerification()



		#This is the staging step
		if(step.ready()):
			self.stage("Demonstration of current measurement")
		#This is the commanding step(s)
		if(step.ready()):
			self.sendCommand({'es':{'cmd':"L12",'id':self.id}})
		if(step.ready()):
			self.sendCommand({'es':{'cmd':"C",'id':self.id}})
		#This is the verification step
		if(step.ready()):
			#Verify that the experiment simulator returned the expected current
			self.check(ValueTest(["es","id"],self.id))
			self.check(RangeTest(["es","current"],60,65))
			self.runVerification()



		#This is the staging step
		if(step.ready()):
			self.stage("Demonstration of voltage and current measurement")
		#This is the command step
		if(step.ready()):
			# Set the load to max, my demo setup is bad and the voltage measurement will be out of range otherwise :-)
			self.sendCommand({'es':{'cmd':"L31",'id':self.id}})
		if(step.ready()):
			self.sendCommand({'es':{'cmd':"V",'id':self.id}})
		#This is the verification step
		if(step.ready()):
			#Verify that the experiment simulator returned the expected current
			self.check(ValueTest(["es","id"],self.id))
			self.check(RangeTest(["es","voltage"],850,860))
			self.runVerification()
