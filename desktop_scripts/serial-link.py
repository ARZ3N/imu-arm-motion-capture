import csv
import serial
import time


W = 0.0
X = 0.0
Y = 0.0
Z = 0.0

fieldnames = ["W", "X", "Y", "Z", "Angle"]

with open('imu.csv', 'w') as csv_file:
    csv_writer = csv.DictWriter(csv_file, fieldnames= fieldnames)
    csv_writer.writeheader()
    
#--- HOST USB PORT CONFIGURATION ---
mcu = serial.Serial("COM4")
mcu.baudrate = 9600
print("==============================================")
print("COM PORT -> ", mcu.name)
print("COM PORT Status -> ", mcu.is_open)
#--- ESTABLISHING SERIAL LINK BETWEEN MCU AND HOST ---
while mcu.read() != b'r':
    print("-- NRDY --")
mcu.write(b'a')
while mcu.read() != b'h':
    pass
print("! /SERIAL HANDSHAKE SUCCSFL/ !")


quaternion = list(())

while True:
    input_buf = (mcu.read_until())
    st_index = 0
    end_index = 0
    for index in range(len(input_buf)):
        
        if input_buf[index] != 44 and input_buf[index] != 10:
            end_index = end_index + 1
        else:
            quaternion.append(input_buf[st_index:st_index + end_index])
            st_index = index + 1
            end_index = 0
    #DEBUG PROBE- 1
    # print("quaternion-> ", quaternion)
    W = float(quaternion[0])
    X = float(quaternion[1])
    Y = float(quaternion[2])
    Z = float(quaternion[3])
    quaternion.pop()
    quaternion.pop()
    quaternion.pop()
    quaternion.pop()
    mag_q = (X**2 + Y**2 + Z**2)**0.5
    mag_shadow = (X**2 + Y**2 + 0)**0.5
    Angle = (180.0/3.1415)*((Z**2)/(mag_q * 1.0))
    with open('imu.csv', 'a') as csv_file:
        csv_writer = csv.DictWriter(csv_file, fieldnames=fieldnames)

        info = {
            "W": W,
            "X": X,
            "Y": Y,
            "Z": Z,
            "Angle": Angle
        }
        csv_writer.writerow(info)
    
    #DEBUG PROBE- 2
    #print("original buffer-> ", input_buf)
    #DEBUG PROBE- 3
    print(W, X, Y, Z)    
                

mcu.close()
