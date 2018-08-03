#ifndef COMMAND_HEADER_FILE
#define COMMAND_HEADER_FILE

#define ERR_NOT_CMD -100
#define MAX_NUM_UART_CMDS 16

typedef struct{
  int uartEvent;
  char * uartString;
  char * recPtr;
}command_state_t;


typedef struct{
  int (*callback)(int);
  char cmdTrigger;
  char help[16];
  char arg[16];
}command_t;



int receiveCommands(command_state_t * cmdState);

#endif

