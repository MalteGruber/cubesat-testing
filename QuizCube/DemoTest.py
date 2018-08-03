"""
from AbstractTest import AbstractTest
from Verify import *

class ExampleTest(AbstractTest):
	def __init__(self, testName, testDescription,someTestSpecificVariable):
		AbstractTest.__init__(self,testName, testDescription)
		self.someTestSpecificVariable=someTestSpecificVariable
		
	def tick(self,time):
		#Create a sequencer for the current time
		step=Sequencer(time);

		for i in range(6):
			#This is the staging step
			if(step.ready()):
				self.stage("Testing sun sensor number ",i+1)

			#Send request to sun sensor simulator
			if(step.ready()):
				self.sendCommand({'sss':{'cmd':"V100",'channel':i}})

			#Send request to OBC to deliver sun sensor readings
			if(step.ready()):
				self.sendCommand({'obc':{'cmd':"GET_SUNSENSOR_READINGS"}})

			#This is the verification step
			if(step.ready()):
				self.check(RangeTest(["obc","sunsensorADC",i],97,103))
				self.check(RangeTest(["obc","sunsensorADC",i],97,103))
				
				self.runVerification()

		#return step.wasLast()
"""