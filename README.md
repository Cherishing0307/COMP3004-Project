# COMP3004-DenasProject
Repository for COMP3004 group project

COMP3004A Winter 2020
Group Project
Team #9

Graham Airth
Tom Roberts
Rishi Parameswaran
Hussien Dabarani

Qt project to run a simulator of a Microcurrent Technology (MCT) device similar to the Denas or Avazzia.

Instructions:
-Load the project (COMP3004-DenasApp.pro) in Qt Creator, and compile/run.

Code Description:

We simulate the device (represented by [device.cpp]), along with its battery (battery.cpp), and the electrode output (electrode.cpp) that it produces. A simulator control (SimulatorControl.cpp) addresses issues specific to the overall simulation rather than just the device itself;  e.g. it allows you to recharge the battery, and apply the device to the skin. 

Some Extra Features:
-Pop-up display messages for some events (changing power level, auto-shutdown).
-Click or hold left/right arrows to adjust power.
-Electrode output visualized with a waveform.
-Some program modes with alternating frequencies.
-Use SimulatorControl "Close App" button to close entire simulator.

Contents:

battery.cpp                        device.h       README.txt
battery.h                          device.ui      REAME.txt~
battery.ui                         electrode.cpp  simulatorcontrol.cpp
COMP3004-DenasApp.pro              electrode.h    simulatorcontrol.h
COMP3004-DenasApp.pro.user         electrode.ui   simulatorcontrol.ui
COMP3004-ProjectDocumentation.pdf  main.cpp
device.cpp                         README.md