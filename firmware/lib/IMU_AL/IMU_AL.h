#ifndef IMU_AL_h
#define IMU_AL_h

//#include "Arduino.h"

#define MPU_ADDR1 0x68
#define MPU_ADDR2 0x69
//---register definitions---
#define ACCEL_XOUT_H 0x3B
#define ACCEL_YOUT_H 0x3D
#define ACCEL_ZOUT_H 0x3F
#define TEMP_OUT_H 0x41
#define GYRO_XOUT_H 0x43
#define GYRO_YOUT_H 0x45
#define GYRO_ZOUT_H 0x47
#define CONFIG 0x1A
#define GYRO_CONFIG 0x1B
#define ACCEL_CONFIG 0x1C
#define PWR_MGMT_1 0x6B
//---settings---
#define DLPF_CFG_260 0 //0b000
#define DLPF_CFG_184 1 // 0b001
#define DLPF_CFG_94 2 //0b010
#define DLPF_CFG_44 3 //0b011
#define DLPF_CFG_21 4 //0b100
#define DLPF_CFG_10 5 //0b101
#define DLPF_CFG_5 6 //0b110
#define FS_SEL_250 0b00
#define FS_SEL_500 0b01
#define FS_SEL_1000 0b10
#define FS_SEL_2000 0b11
#define AFS_SEL_2 0b00
#define AFS_SEL_4 0b01
#define AFS_SEL_8 0b10
#define AFS_SEL_16 0b11

//---constants---
#define GYRO_SEN_250 131.0
#define GYRO_SEN_500 65.5
#define GYRO_SEN_1000 32.8
#define GYRO_SEN_2000 16.4
#define ACCEL_SEN_2 16384.0
#define ACCEL_SEN_4 8192.0
#define ACCEL_SEN_8 4096.0
#define ACCEL_SEN_16 2048.0
#define TEMP_SEN 340.0
#define G 9.8065
//DEFAULTS-----------
#define DEF_AC_SEN ACCEL_SEN_2
#define DEF_GY_SEN GYRO_SEN_250
class IMU{
    private:
    //raw values-
    int16_t acX, acY, acZ;
    int16_t gyX, gyY, gyZ;
    int16_t temp;
    int16_t active_ADD;
    float ACSEN;
    float GYSEN;
    public:
    //real values-
    float realAC[3];
    float realGY[3];
    float realTEMP;
    IMU(int16_t ADDR);
    void initialize();
    void getAccel();
    void getGyro();
    void getTemp();
    void showRaw();
    void showdata(char);
    int8_t REGISTER_STATUS(int);
    void WRITE_REGISTER(int,int8_t);
    void setDLPF(int8_t);
};
#endif
