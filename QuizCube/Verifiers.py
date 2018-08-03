
class ValueTest():
	def __init__(self,filters,val):
		self.filters=filters;
		self.val=val
		self.passed=False

	def getValue(self,message):
		val=message
		for f in self.filters:
			if(self.isIndex(f)):
				val=val[f]
			else:
				val=val.get(f)			
		return val
	def getInfoStr(self):
		return "Value test for "+str(self.val)+" with filters "+str(self.filters)

	def isIndex(self,s):
		try: 
			int(s)
			return True
		except ValueError:
			return False

	def accept(self):
		self.passed=True
	def wasAccepted(self):
		return self.passed

	def testMessage(self,message):
		val=self.getValue(message)

		try:
			if(val==self.val):
				return PASSED
			else:
				return FOUND_NOT_PASSED
		except:
			return NOT_FOUND
	def rangeTest():
		pass


class RangeTest():
	def __init__(self,filters,low,high):
		self.filters=filters;
		self.low=low
		self.high=high
		self.passed=False

	def getValue(self,message):
		val=message
		for f in self.filters:
			if(self.isIndex(f)):
				val=val[f]
			else:
				val=val.get(f)			
		return val
	def getInfoStr(self):
		return "Range test from "+str(self.low)+" to "+str(self.high)+" with filters "+str(self.filters)

	def isIndex(self,s):
		try: 
			int(s)
			return True
		except ValueError:
			return False

	def accept(self):
		self.passed=True
	def wasAccepted(self):
		return self.passed


	def testMessage(self,message):
		val=self.getValue(message)
		def inRange(val,low,high):
			#print(val,low,high,(val>low and val<high))
			if(val>low and val<high):
				#print("passed")
				return True
			return False
		try:
			if(inRange(val,self.low,self.high)):
				return PASSED
			else:
				return FOUND_NOT_PASSED
		except:
			return NOT_FOUND
	def rangeTest():
		pass

import json
PASSED=1
FOUND_NOT_PASSED=2
NOT_FOUND=4

class VerificationHandler():
	def __init__(self):
		self.messages=[]
		self.verifiers=[]
		self.info="WARNING: Info not set at staging"
	def pushMessage(self,message):
		self.messages.append(message)

	def clearMessages(self):
		self.messages=[]
	def stage(self,info):
		self.info=info
	def pushVerifier(self,test):
		self.verifiers.append(test)
	def clearVerifiers(self):
		self.verifiers=[]
	def runTest(self):
		testPassed=False;
		for message in self.messages:
			#For every message first assume it was found and passed. Tests negate this
			#if it is not the case
			messagePassed=True;
			messageFound=True;
			#print("Tests will be executed for message",message)
			for v in self.verifiers:
				result=v.testMessage(message)
			#	print("Verification passed =",result)
				if(result==PASSED):
					v.accept()
				if(result==FOUND_NOT_PASSED):
					#print("NOT FOUND")
					messagePassed=False
				if(result==NOT_FOUND):
					messagePassed=False
					messageFound=False
			#The message has been tested, lets deal with the result.
			if(messagePassed):
				#print("PASSED!")
				testPassed=True;
			elif(messageFound):
				print("WARNING: Found-not-passed: ",message)
			else:
				#print("FAILED")
				pass
		if(testPassed):

			print("PASSED! "+self.info)
		else:
			print("FAILED! "+self.info)
#			for v in self.verifiers:
#				if not v.wasAccepted()
#				print("Verification failed:",v.getInfoStr())

#Not a part of this file, move to some sensible location...
class Sequencer:
	def __init__(self,t):
		self.time=0;
		self.triggerTime=t

	def ready(self,dt=1):
		ret=False
		if(self.time==self.triggerTime):
			ret=True
		self.time+=dt
		return ret

