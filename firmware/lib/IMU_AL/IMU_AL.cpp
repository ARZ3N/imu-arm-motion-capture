#include "Arduino.h"
#include "IMU_AL.h"
#include<Wire.h>
IMU::IMU(int16_t ADDR){
  active_ADD = ADDR;
  acX = 0;
  acY = 0;
  acZ = 0;
  gyX = 0;
  gyY = 0;
  gyZ = 0;
  temp = 0;
  ACSEN = DEF_AC_SEN;
  GYSEN = DEF_GY_SEN;
}
void IMU::initialize(){
  Wire.begin();
  Wire.beginTransmission(active_ADD);
  Wire.write(PWR_MGMT_1);
  Wire.write(0b00000000);
  Wire.endTransmission(true);
}
void IMU::getAccel(){
  Wire.begin();
  Wire.beginTransmission(active_ADD);
  Wire.write(ACCEL_XOUT_H);
  Wire.endTransmission(false);
  Wire.requestFrom(active_ADD, 6, true);
  acX = Wire.read()<<8|Wire.read();
  acY = Wire.read()<<8|Wire.read();
  acZ = Wire.read()<<8|Wire.read();
  realAC[0] = (acX/ACCEL_SEN_2)*G;
  realAC[1] = (acY/ACCEL_SEN_2)*G;
  realAC[2] = (acZ/ACCEL_SEN_2)*G;
}
void IMU::getGyro(){
  Wire.begin();
  Wire.beginTransmission(active_ADD);
  Wire.write(GYRO_XOUT_H);
  Wire.endTransmission(false);
  Wire.requestFrom(active_ADD, 6, true);
  gyX = Wire.read()<<8|Wire.read();
  gyY = Wire.read()<<8|Wire.read();
  gyZ = Wire.read()<<8|Wire.read();
  realGY[0] = gyX/DEF_GY_SEN;
  realGY[1] = gyY/DEF_GY_SEN;
  realGY[2] = gyZ/DEF_GY_SEN;
}
void IMU::getTemp(){
  Wire.begin();
  Wire.beginTransmission(active_ADD);
  Wire.write(TEMP_OUT_H);
  Wire.endTransmission(false);
  Wire.requestFrom(active_ADD, 2, true);
  temp = Wire.read()<<8|Wire.read();
  realTEMP = temp/TEMP_SEN;
}
void IMU::showRaw(){
  Serial.print(acX);Serial.print("\t");
  Serial.print(acY);Serial.print("\t");
  Serial.print(acZ);Serial.println();
}
void IMU::showdata(char label){
  switch(label){
    case 'a': Serial.print(realAC[0]);Serial.print("\t");
              Serial.print(realAC[1]);Serial.print("\t");
              Serial.print(realAC[2]);Serial.println();
              break;
    case 'g': Serial.print(realGY[0]);Serial.print("\t");
              Serial.print(realGY[1]);Serial.print("\t");
              Serial.print(realGY[2]);Serial.println();
              break;
    case 'x': Serial.print(realAC[0]);Serial.print("   ");
              Serial.print(realAC[1]);Serial.print("   ");
              Serial.print(realAC[2]);Serial.print("   ");
              Serial.print(realGY[0]);Serial.print("   ");
              Serial.print(realGY[1]);Serial.print("   ");
              Serial.print(realGY[2]);Serial.println();
              break;
    default: Serial.println(F("enter valid label."));
  }
}
int8_t IMU::REGISTER_STATUS(int reg){
  Wire.begin();
  Wire.beginTransmission(active_ADD);
  Wire.write(reg);
  Wire.endTransmission(false);
  Wire.requestFrom(active_ADD,1,true);
  int8_t intr = Wire.read();
  return intr;
}
void IMU::WRITE_REGISTER(int reg, int8_t value){
  Wire.begin();
  Wire.beginTransmission(active_ADD);
  Wire.write(reg);
  Wire.write(value);
  Wire.endTransmission(true);
}
void IMU::setDLPF(int8_t setting){
  Wire.begin();
  Wire.beginTransmission(active_ADD);
  Wire.write(CONFIG);
  Wire.write(setting);
  Wire.endTransmission(true);
}
