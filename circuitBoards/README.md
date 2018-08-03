# PCB files
These  files can be opened with EAGLE. The version used is 8.6.0.

# Most recent files
The ./sss-bob-pcb and ./exp-sim-pcb contain the most recent files. Some changes have been made to the schematic to reflect the components used on the PCB (TODO!!!)

# As ordered
The ./asOrdred directory contains zip files of the ordered directories. These zip files also contains the gerber files sent to the board house.

## Board House
[www.bradner.ee](https://www.brandner.ee/) 

## Suggestions for fixes for new orders

### Exp-Sim
The screw holes are far to big. Use the 3~mm holes used on the BOB.


Use covered vias. This is done with the './sss-bob-pcb/brandner2018.cam' file, this file that can be loaded when exporting the gerber files from eagle. However, also check with a gerber viewer that the vias really are covered.


### BOB
Add a way of disconnecting the DAC from the SPI bus by connecting the CS line to vdd.
