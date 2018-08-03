
#include "command.h"


int receiveCommands(command_state_t * cmdState){
	checkUart(cmdState);
}


command_t commands[MAX_NUM_UART_CMDS]={0};

int numRegisteredCmds(){
	for (int i = 0; i < MAX_NUM_UART_CMDS; i++)	{

		if(commands[i].cmdTrigger==0)
			return i;
	}
	return -1;
}

int testIfTriggerCmd(char cmd,int arg){
	int ret=ERR_NOT_CMD;
	for (int i = 0; i < numRegisteredCmds(); i++)	{
		if(commands[i].cmdTrigger==cmd){
      blipLed();
			ret=(commands[i].callback)(arg);
		}
	}
	return ret;
}

void registerUartCmd(int (*callback)(int),char cmdChar,char * help,char * arg){

	command_t cmd={callback,cmdChar,help};
	strcpy(cmd.help,help);
	strcpy(cmd.arg,arg);
	commands[numRegisteredCmds()]=cmd;

}
void multiprint(char c, int times){
	for (int i = 0; i < times; ++i)	{
		Serial.print(c);
	}
	Serial.print("\n");
}
void printUsage(){
	Serial.println("================== USAGE ====================");
				
	Serial.println("CMD~\tDESC~\t\tARGS~");

	
	for (int i = 0; i < numRegisteredCmds(); i++){
		Serial.print(commands[i].cmdTrigger);
		Serial.print("\t");
		Serial.print(commands[i].help);	
		Serial.print("\t");
		Serial.println(commands[i].arg);			
	}
	multiprint('-',46);
	Serial.print("Version ");
	Serial.print(VERSION);
	Serial.print(" Build date ");
	Serial.print(__DATE__);
	Serial.print(" ");
	Serial.println(__TIME__);

	multiprint('-',46);
}

static void checkUart(command_state_t * cmdState){

	if(cmdState->uartEvent){
		int ret=testIfTriggerCmd(cmdState->uartString[0],atoi(&cmdState->uartString[1]));
		if(ret==ERR_NOT_CMD){
			Serial.println("Not a valid command! Valid commands are:");
			printUsage();
		}else if(ret<0){
			printVar("Something went wrong! error code: ",ret);
		}else{
			Serial.println("OK!");
		}
  		/*We could theoretically miss a event during this handling, hence*/
		cmdState->uartEvent=0;
		serialEvent();  
	}
}

void serialEvent() {
	while (Serial.available()&&(!cmdState.uartEvent)) {
		char c = (char)Serial.read();
		if(c=='\n'){
			*cmdState.recPtr++='\0';     
			cmdState.recPtr=cmdState.uartString;
			cmdState.uartEvent=1;
		}else{
			*cmdState.recPtr++=c;
		}
	}
}

