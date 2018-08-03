class QuizCube:
	def __init__(self,sendMessageCallback):
		self.tests=[]
		self.sendMessageCallback=sendMessageCallback
	def registerTest(self,test):
		test.setSendMessageCallback(self.sendMessageCallback)
		self.tests.append(test)
		pass
	def propogateMessage(self,message):
		for test in self.tests:
			test.onMessage(message)
	def printStatus(self):
		for test in self.tests:
			print(test.getTestName,"Is not done")

	def tick(self,time):
		for test in self.tests:
			test.tick(time)
	def clearTests(self):
		self.tests=[]

