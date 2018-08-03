#include <SPI.h>
#include <stdint.h>
#include <Wire.h>

#include "command.h"
#define VERSION 1.0
// set up the speed, data order and data mode

#define SSS_LED 10
#define I2C_EN 4
#define UART_REC_LEN 32
char uartStringAlloc[UART_REC_LEN];
command_state_t cmdState;

// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(I2C_EN, OUTPUT);

    cmdState.uartString = uartStringAlloc;
  SPI.begin();
  Serial.begin(115200);
  Serial.println("HELLO WORLD!");
      Wire.begin(8);                // join i2c bus with address #8
  Wire.onRequest(requestEvent); // register event
  registerUartCmd(&setADC,'a',"set ADC value for current channel","0-1023");  
    registerUartCmd(&setChannel,'c',"set ADC channel","0-7");  
    registerUartCmd(&debug,'t',"debug","0-7");  



  //255 = 2x gain on all output buffer, divide 2.5 reference by 1. I.e, max output =5V
  sendDacCmd(0x04,0,0x00ff);

  
    for(int i=0;i<7;i++){
    setVoltage(2.0,i);
    }

  Serial.println("HELLO WORLD2 !");
}


void printVar(char * msg, int val) {
  Serial.print(msg);
  Serial.println(val);
}
int debug(int arg) {
  Serial.println(arg);
  }

int currentChannel=0;
/*UART COMMAND CALLBACKS*/
int setChannel(int arg) {
  currentChannel=arg;
  return 9001;
  }
int setADC(int arg) {
  currentChannel++;
  Serial.print(currentChannel);
  Serial.print(" to ");

    Serial.println(arg);
  uint32_t val=((uint32_t)arg)<<2;//The DAC blanks the two LSB for the 14-bit model
  setRawAdcVoltage(val,currentChannel);
  return 1;
  }



void enableI2C_Buffer(int state){
  //Active high en pin
  digitalWrite(I2C_EN,state);
}


void sendDacCmd(uint32_t addr, uint32_t rw, uint32_t data) {
  digitalWrite(8, LOW);
  SPISettings settingsA(1600000, MSBFIRST, SPI_MODE1);
  SPI.beginTransaction(settingsA);

  uint32_t dac_cmd = ((0x1 & rw) << 23) | ((addr & 0xf) << 16) | data & 0xffff;

  uint8_t * cmd;
  cmd = ( unsigned char*)&dac_cmd;
  SPI.transfer(cmd[2]);
  SPI.transfer(cmd[1]);
  SPI.transfer(cmd[0]);
  SPI.endTransaction();
  digitalWrite(8, HIGH);
}


void receiveDacData(uint16_t * data) {
  SPISettings settingsA(1600000, MSBFIRST, SPI_MODE1);
  SPI.beginTransaction(settingsA);
  delay(1);
  digitalWrite(8, LOW);

  int a = 0;
  delay(12);
  uint8_t *rec = (uint8_t*)data;

  //Echo byte ie. 24:16
  SPI.transfer(0);
  //15:8
  rec[1] = SPI.transfer(0);
  //7:0
  rec[0] = SPI.transfer(0);
  SPI.endTransaction();
  digitalWrite(8, HIGH);


}
void setRawAdcVoltage(uint32_t adcVal,int channel){
    sendDacCmd(0b1000+channel,0,adcVal);
    //printVar("dac set to",adcVal);
    //printVar("or channel",channel);
    
    }

void setVoltage(double voltage,int channel){
  int val=(int)(voltage*6553.6f)*2;
  sendDacCmd(0b1000+channel,0,val<<2);
  }


// the loop function runs over and over again forever

int idle=0;
int firstTimeNotIdle=0;
void loop() {

  checkUart(&cmdState);  


  

  
 delay(1);    

}


void requestEvent() {
  Wire.write("hello "); // respond with message of 6 bytes
  // as expected by master
}
