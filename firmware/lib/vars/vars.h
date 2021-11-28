#include "quat.h"

//! Gyro update quaternion q(t)
quaternion qt = {1.00f, 0.00f, 0.00f, 0.00f};
//! IMU body vector (Body FoR)
quaternion qU_body = {0.00f, 0.00f, 0.00f, 1.00f};
//! imu body vector in World FoR
quaternion qU_world_hand = {0.00f, 0.00f, 0.00f, 0.00f};
quaternion qU_world_ua = {0.00f, 0.00f, 0.00f, 0.00f};
//! Acceleration quaternion in World FoR
quaternion qA_world = {0.00f, 0.00f, 0.00f, 0.00f};
//! qGyro
quaternion qGyro = {0.00f, 0.00f, 0.00f, 0.00f};
//! qAccel
quaternion qAccel = {0.00f, 0.00f, 0.00f, 0.00f}; 
//! Complementary filter rotation quaternion>> qC
quaternion qC = {0.00f, 0.00f, 0.00f, 0.00f};
//! q_delta or qD omega rotation quaternion
quaternion qD = {0.00f, 0.00f, 0.00f, 0.00f};
quaternion qt_ua = {1.00f, 0.00f, 0.00f, 0.00f};
quaternion qC_ua = {0.00f, 0.00f, 0.00f, 0.00f};