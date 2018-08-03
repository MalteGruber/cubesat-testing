void initPins(){
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(FET0, OUTPUT);
  pinMode(FET1, OUTPUT);
  pinMode(FET2, OUTPUT);
  pinMode(FET3, OUTPUT);
  pinMode(FET4, OUTPUT);
  pinMode(FET4, OUTPUT);
  digitalWrite(LED_EN,1);
  pinMode(I2C_EN, OUTPUT);
  pinMode(I2C_READY, INPUT_PULLUP); // Needs pullup, see datasheet http://www.ti.com/lit/ds/symlink/tca4311a.pdf
  pinMode(I_SENSE, INPUT);
  pinMode(V_SENSE, INPUT);

}

void registerCommands(){
  registerUartCmd(&getCurrentRequest,'C',"get current","none");
  registerUartCmd(&getVoltageRequest,'V',"get Voltage","none");
  registerUartCmd(&setLoadRequest,'L',"set awg load","int:load");
  registerUartCmd(&todoRequest,'m',"set load min","int:min");
  registerUartCmd(&todoRequest,'M',"set load max","int:max");
  registerUartCmd(&todoRequest,'T',"set load type","int:type");
  registerUartCmd(&todoRequest,'E',"enable load","int:1|on, 0|off");
  registerUartCmd(&readAddressEPROMRequest,'v',"get pcb id","none");
  registerUartCmd(&unlockEEPROMRequest,'u',"unlock eeprom","key:14120");
  registerUartCmd(&lockEEPROMRequest,'l',"lock eeprom","none");
  registerUartCmd(&writeAddressEPROMRequest,'P',"set pcb id","int:id");
  registerUartCmd(&readI2C_ready_pin_Request,'i',"read i2c ready pin","nonei");
  registerUartCmd(&setI2C_EN_pinRequest,'e',"set i2c enable pin","1:on | 0:off");  
  registerUartCmd(&sendI2CMasterPing,'r',"Read from i2c slave as master","slaveAddr");  

}

