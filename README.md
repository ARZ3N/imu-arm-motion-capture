## Source files for Project: Human Arm Motion Capture Using IMU sensors
For people looking forward to tinkering with this, the firmware is nothing but C++ arduino compatible code developed in the PlatformIO environment, for the prototyping phase.

### Directory Structure
  |\
  |-> desktop_scripts\
  |   (Python scripts to be run parallel on the host computer. For communication and data acuisistion from sensor network)\
  |->-= 3Drender.py\
  |->-= Serial-link.py\
  |\
  |-> firmware\
  |   (Arduino firmware, C++ libraries for math and PlatformIO configuration folders)\
  |->-> .pio\
  |->-> .vscode\
  |->-> include\
        (PlatformIO configuration folder- .pio, and MS Visual Studio Code developement environment folder- .vscode)\
  |->-> lib\
  |->->-> IMU_AL\
  |->->->-= embed_IMU_layer.h\
  |->->->-= IMU_AL.cpp\
  |->->->-= IMU_AL.h\
  |->->-> Num_methods\
  |->->->-= Num_methods.h\
  |->->-> quat\
  |->->->-= quat.cpp\
  |->->->-= quat.h\
  |->->-> vars\
  |->->->-= vars.h\
  |->-> src\
  |->-> test\
  |->-= .gitignore\
