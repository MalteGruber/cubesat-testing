
#include "command.h"
#include <EEPROM.h>

#include <Wire.h>

#define LED_I2C 10
#define LED_COM 9
#define LED_EN 8
#define FET0 5

int IS_I2C_SLAVE=1;
/*For leds and such*/
#define LOOP_DELAY_US 100
#define FET1 6
#define FET2 7
#define FET3 A0
#define FET4 A1

#define NBITS 5
#define UART_REC_LEN 32
#define VERSION "DEVELOPMENT"

#define I2C_EN 4
#define I2C_READY 2

#define I_SENSE A2
#define V_SENSE A3

//#define EXP_SIM 
#define SUNSENSOR_SIM

char uartStringAlloc[UART_REC_LEN];
int resistors[] = {FET0, FET1, FET2, FET3, FET4};

#define NUM_LEDS 3
typedef struct{
  int pin;
  int state;
  int counter;
}led_t;

led_t leds[]={{.pin=LED_I2C,.state=0,.counter=0},{.pin=LED_COM,.state=0,.counter=0},{.pin=LED_EN,.state=0,.counter=0}};



void flashLeds(int del){
  digitalWrite(LED_I2C,HIGH);  
  digitalWrite(LED_COM,HIGH);
  digitalWrite(LED_EN,HIGH);
  delay(del);
   digitalWrite(LED_I2C,LOW);  
  digitalWrite(LED_COM,LOW);
  digitalWrite(LED_EN,LOW); 
}
command_state_t cmdState;

void blipLed(){
    digitalWrite(LED_I2C,HIGH);  
  digitalWrite(LED_COM,HIGH);
  digitalWrite(LED_EN,HIGH);
  leds[0].counter=200;
    leds[1].counter=400;
      leds[2].counter=800;
}

void printVar(char * msg, int val) {
  Serial.print(msg);
  Serial.println(val);
}



/*Returns device id from eeprom*/
int getDeviceId() {
    return EEPROM.read(0);
}


void enableI2C_Buffer(int enI2c){
  //Active high en pin
  digitalWrite(I2C_EN,enI2c);
}
void printJsonInt(char * name_,int value){
  /*Lets try not to include sprintf...*/
  Serial.print("{\"");
  Serial.print(name_);  
  Serial.print("\":");
  Serial.print(value);
  Serial.print("}\n");
  }

void setLoad(int load) {

  /*
  When changing the load, not all pins can be changed simultaneous.
  
  FET4:PORTC1 - least significant, ie. lowest current
  FET3:PORTC0
  FET2:PORTD7
  FET1:PORTD6
  FET0:PORTD5 - most significant, ie. highest current
  */

/*Uggly but fast!*/

 static int prev=0;

 if(load>prev){
 //FET 4
if(load&0b00001)
  PORTC |= 0b00000010; //ON
else
  PORTC &= 0b11111101; //OFF

//FET 3
if(load&0b00010)
  PORTC |= 0b00000001; //ON
else
  PORTC &= 0b11111110; //OFF

 
//FET 2
if(load&0b00100)
  PORTD |= 0b10000000; //ON
else
  PORTD &= 0b01111111; //OFF

 

//FET 1
if(load&0b01000)
  PORTD |= 0b01000000; //ON
else
  PORTD &= 0b10111111; //OFF


//FET 0
if(load&0b10000)
  PORTD |= 0b00100000; //ON
else
  PORTD &= 0b11011111; //OFF

 }else{

  
//FET 0
if(load&0b10000)
  PORTD |= 0b00100000; //ON
else
  PORTD &= 0b11011111; //OFF
//FET 1
if(load&0b01000)
  PORTD |= 0b01000000; //ON
else
  PORTD &= 0b10111111; //OFF
 
//FET 2
if(load&0b00100)
  PORTD |= 0b10000000; //ON
else
  PORTD &= 0b01111111; //OFF

//FET 3
if(load&0b00010)
  PORTC |= 0b00000001; //ON
else
  PORTC &= 0b11111110; //OFF

  //FET 4
if(load&0b00001)
  PORTC |= 0b00000010; //ON
else
  PORTC &= 0b11111101; //OFF
  }

  prev=load;
  /* NICE BUT SLOW
  for (int i = 0; i < NBITS; i++) {
    if (load & (1 << i)) {
      digitalWrite(resistors[NBITS - 1 - i], HIGH);
    } else {
      digitalWrite(resistors[NBITS - 1 - i], LOW);
    }
  }
  */
}
int readI2C_ready_pin_Request(int arg){
  printJsonInt("i2cReady",digitalRead(I2C_READY));
}
int setI2C_EN_pinRequest(int arg){
  printJsonInt("i2cEnabled",arg);
  enableI2C_Buffer(arg);
}

int getCurrentRequest(int arg) {
  Serial.println("CURRENT HERE");
    int adc=analogRead(I_SENSE);
    
      printJsonInt("current",adc);
  return 666;
}
int getVoltageRequest(int arg) {
  Serial.println("VOLTAGE HERE");
  int adc=analogRead(V_SENSE);
    printJsonInt("voltage",adc);
  return 9001;
}



int sendI2CMasterPing(int arg) {

  int i2cAddress=arg;
  int len=6;
Wire.requestFrom(i2cAddress, len);    // request 6 bytes from slave device #8
Serial.print("i2c master read ");
Serial.print(len);
Serial.print(" bytes from device ");
Serial.println(i2cAddress);

//delay(200);

  for(int i=0;i<10;i++){

    if(Wire.available()){
    char c = Wire.read(); // receive a byte as character
    Serial.print(c);         // print the character
    i++;
    continue;

    }
    
    delay(100);
      Serial.println(i);
  }
  delay(50);
  return 9001;
}


int i2cEvent=0;
void requestEvent() {
      
      digitalWrite(LED_I2C,1);

  Wire.write('h');
  Wire.write('e');
  Wire.write('l');
  Wire.write('!');
  Wire.write('o');
  Wire.write('D');
  // respond with message of 6 bytes
  // as expected by master
  delay(100);
      digitalWrite(LED_I2C,0);
      i2cEvent=1;
}

int setLoadRequest(int arg) {
  printVar("LOAD REQEST: ", arg);
  setLoad(arg);
  return 9001;
}
int eepromLock=1;

int unlockEEPROMRequest(int arg) {
  if(arg==14120){
      Serial.println("WARNING: EEPROM unlocked!");
      eepromLock=0;
      return 0;
  }else{
      printVar("Not a valid key (valid key is 14120), received: ", arg);
    eepromLock=1;
  return -200;
  }
}

int lockEEPROMRequest(int arg) {

      Serial.println("EEPROM locked.");
  
    eepromLock=1;
  return 200;


}



int writeAddressEPROMRequest(int arg) {
  if(arg>255){
    Serial.println("Address must be less than 255!");
    return -256;
  }
  if(eepromLock){

    Serial.println("EEPROM is locked, use unlock command first!");
    return -200;
  }else{
    EEPROM.write(0, arg);

    printVar("Device address updated and saved in EEPROM, address is now ",getDeviceId());
    eepromLock=1;
  }

}




int readAddressEPROMRequest(int arg) {
  printVar("Device address is ",getDeviceId());
  printJsonInt("id",getDeviceId());
}
int todoRequest(int arg) {
  printVar("TODO!!! Argument: ", arg);
  return arg - 200;
}
void setup() {
  initPins();
  Serial.begin(115600);
  cmdState.uartString = uartStringAlloc;
  registerCommands();

  int thisI2cAddress=getDeviceId();
    Wire.begin(thisI2cAddress);                // join i2c bus with address #8
   enableI2C_Buffer(1);
  /*Init as I2C callback, this is for slave opperation but it should 
  be fine to keep when opperating as master (Done during PCB checkout)*/
    Wire.onRequest(requestEvent); // register event

    
  printUsage();

  analogReference(INTERNAL);

  flashLeds(100);
  delay(100);
    flashLeds(100);
  delay(100);
    flashLeds(1000);

  
}


void checkLeds(){
  for(int i=0;i<NUM_LEDS;i++){
    if(leds[i].counter>0){
      leds[i].counter--;
      if(leds[i].counter==0){
        digitalWrite(leds[i].pin,LOW);
        }
      }
    }  
}

void triangleWave(){
static int load=0;
static int dir=1;

load+=dir;
if(load<1)
  dir=1;
if(load>30)//ie. is 31, should change direction
  dir=-1;
 setLoad(load);
 //Serial.println(load);
}

void randomLoad(){
  static int awgLoad=0;
  int load=random(15);
  int target=4;
  if(awgLoad<target)
    load=load+16;
  awgLoad+=load-16;
  
  setLoad(load);
}


void toggleLoad(){
  static int old=0;
  old=31-old;
  setLoad(old);
  }

  
void loop() {
  //triangleWave();
  //randomLoad();
  //toggleLoad();
  checkUart(&cmdState);
  checkLeds();
  if(i2cEvent){
    i2cEvent=0;
    Serial.println("I2C EVENT !!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
    }
 // delayMicroseconds(LOOP_DELAY_US);
}



