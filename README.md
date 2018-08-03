# CubeSat Testing
This repository contains files relating to my master thesis. While the focus of the thesis was on hardware, I did work quite a bit on software as well. That said, the software that exists is more of a proof of concept than a release-ready framework. It was created because I had fun and I share it in the hope that It might be of some use.

## Content
This repository contains both code for a test coordination system and hardware for physical testing, code that is embedded into the hardware is also included.
 ## Hardware

### Experiment Simulator
The experiment simulators are PCBs that can emulate the electrical load from a satellite subsystem. They are desinged to provide a dynamic load as to produce realistic power telemetry and provide a realistic load for the satellite's electrical power system. 
![alt text](.media/exp_sim_top.jpg)

The load is made up by resistors that can be switched with MOSFET tranistors.
![alt text](.media/bob_resistors.jpg)

### Breakout Board
The breakout board is used to safly connect the experiment simulators to the satellite. By using the same cable lenghts as in flight the effect off cabling on the I2C bus can be tested.
![alt text](.media/topBottomPCB.jpg)

### Sun Sensor Simulator
This PCB can simulate the satellites sun-sensors and is used for testing the Attitude Determination and Control system.
![alt text](.media/sunsensorsim.jpg)

## QuizCube
QuizCube is an experimental testing system for coordinating test system interactions using a web socket based broad cast commuinication interface.
![Image of graph](.media/testingSystem.png | width=60 )

### Experiment Simulator Client
