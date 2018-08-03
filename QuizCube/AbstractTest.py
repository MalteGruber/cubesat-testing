


"""
This class should be inherited by the specific test classes.
"""
import abc
from abc import ABCMeta, abstractmethod
from Verifiers import *

class AbstractTest(metaclass=ABCMeta):
    __metaclass__ = abc.ABCMeta
    def __init__(self, testName, testDescription):
        self.testName = testName
        self.testDescription = testDescription
        self.vh=VerificationHandler()


    def stage(self,msg,preserverMessages=False,preserverVerifiers=False):
        if not preserverMessages:
            self.vh.clearMessages()
        if not preserverVerifiers:
            self.vh.clearVerifiers()
        self.vh.stage(msg)

    def check(self,verifier):
        self.vh.pushVerifier(verifier)
        pass
    def runVerification(self):
        self.vh.runTest()

    def onMessage(self,message):
        self.vh.pushMessage(message)

    def getTestClassDescription(self):
        return self.testDescription        

    def getTestName(self):
        return self.testName;

    @abc.abstractmethod
    def tick(self,time):
        return

    def setSendMessageCallback(self,callback):
        self.sendMessageCallback=callback

    def sendCommand(self,cmd):
        self.sendMessageCallback(cmd)

        
