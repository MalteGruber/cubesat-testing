from AbstractTest import AbstractTest
from Verifiers import *

class ExampleTest(AbstractTest):
	def __init__(self, testName, testDescription,someTestSpecificVariable):
		AbstractTest.__init__(self,testName, testDescription)
		self.someTestSpecificVariable=someTestSpecificVariable
		
	def tick(self,time):
		#Create a sequencer for the current time
		step=Sequencer(time);

		#This is the staging step
		if(step.ready()):
			self.stage("Demonstration of the three stages of a test")
		#This is the command step
		if(step.ready()):
			self.sendCommand({'es':{'cmd':"v",'id':1}})
		#This is the verification step
		if(step.ready()):
			self.check(RangeTest(["es","voltage"],12.0,13.0))
			self.check(RangeTest(["es","current"],100,110))
			self.check(ValueTest(["es","id"],2))
			self.runVerification()

				#This is the staging step
		if(step.ready()):
			self.stage("Demo that should fail")
		#This is the command step
		if(step.ready()):
			self.sendCommand({'es':{'cmd':"v",'id':1}})
		#This is the verification step
		if(step.ready()):
			self.check(RangeTest(["es","voltage"],12.0,13.0))
			self.check(RangeTest(["es","current"],100,110))
			self.check(ValueTest(["es","id"],2))
			self.runVerification()