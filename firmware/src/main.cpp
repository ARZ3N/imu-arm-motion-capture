#include <Arduino.h>
#include "IMU_AL.h"
#include "quat.h"
#include "vars.h"

//#define ARDUINO_DEBUG
#define HOST_DEBUG
#define PRINT_DISP_VECTOR
#define DELTA_T 0.10f
#define C_ALPHA 0.80f

//! ISR toggle variable------
int flag = 0;
//! IMU OBJECT---------------
IMU HAND(MPU_ADDR1);
IMU UPPR_ARM(MPU_ADDR2);

float GY_OFFSETS_HAND[3] = {0.0, 0.0, 0.0};
float GY_OFFSETS_UA[3] = {0.0, 0.0, 0.0};

void setup() {
  Serial.begin(9600);
  
  Serial.write('r');   //- send RDY status to Host-
  while(Serial.read() != 'a'){}  //- Wait for Host to ACK
  Serial.write('h');  //- Send CNFRMTN

  HAND.initialize();
  //! -----TIMER SEQUENCE---------------------
  cli();
  //set timer1 interrupt at 10Hz
  TCCR1A = 0;// set entire TCCR1A register to 0
  TCCR1B = 0;// same for TCCR1B
  TCNT1  = 0;//initialize counter value to 0
  // set compare match register for 1hz increments
  OCR1A = 1562;// = (16*10^6) / (1*1024) - 1 (must be <65536)
  // turn on CTC mode
  TCCR1B |= (1 << WGM12);
  // Set CS10 and CS12 bits for 1024 prescaler
  TCCR1B |= (1 << CS12) | (1 << CS10);  
  // enable timer compare interrupt
  TIMSK1 |= (1 << OCIE1A);
  sei();

  for(int i=0; i<1000; ++i){
    GY_OFFSETS_HAND[0] += HAND.realGY[0];
    GY_OFFSETS_HAND[1] += HAND.realGY[1];
    GY_OFFSETS_HAND[2] += HAND.realGY[2];
    GY_OFFSETS_UA[0] += UPPR_ARM.realGY[0];
    GY_OFFSETS_UA[1] += UPPR_ARM.realGY[1];
    GY_OFFSETS_UA[2] += UPPR_ARM.realGY[2];

  }
  GY_OFFSETS_HAND[0] /= 1000.0;
  GY_OFFSETS_HAND[1] /= 1000.0;
  GY_OFFSETS_HAND[2] /= 1000.0;
  GY_OFFSETS_UA[0] /= 1000.0;
  GY_OFFSETS_UA[1] /= 1000.0;
  GY_OFFSETS_UA[2] /= 1000.0;
}//! END OF SETUP-----------------------

ISR(TIMER1_COMPA_vect){
  flag = 1;
}

void loop() {
  int8_t stat_h = HAND.REGISTER_STATUS(PWR_MGMT_1);
  if(stat_h != 0){
  HAND.WRITE_REGISTER(PWR_MGMT_1,0);
  }
  int8_t stat_u = UPPR_ARM.REGISTER_STATUS(PWR_MGMT_1);
  if(stat_u != 0){
    UPPR_ARM.WRITE_REGISTER(PWR_MGMT_1,0);
  }
  
  if( flag == 1){
    flag = 0;
    HAND.getAccel();
    HAND.getGyro();
    qGyro.w = 0.00f;
    qGyro.x = (HAND.realGY[0] - GY_OFFSETS_HAND[0]) * DEG_TO_RAD;
    qGyro.y = (HAND.realGY[1] - GY_OFFSETS_HAND[1]) * DEG_TO_RAD;
    qGyro.z = (HAND.realGY[2] - GY_OFFSETS_HAND[2]) * DEG_TO_RAD;
    
    qAccel.w = 0.00f;
    qAccel.x = HAND.realAC[0];
    qAccel.y = HAND.realAC[1];
    qAccel.z = HAND.realAC[2];

    qD = q_form_rq( (DELTA_T*(q_norm(qGyro))), q_normalize(qGyro) );
    qt = q_V_mul(qt, qD);
    qt = q_normalize(qt);
    qA_world = q_rotate(qt, qAccel);
    quaternion N = {
      0.0,
      qA_world.y/q_norm(qA_world),
      -1.0*(qA_world.x/q_norm(qA_world)),
      0.0
    };
    N = q_normalize(N);
    float phi = acos(qA_world.z/q_norm(qA_world));
    //qC = q_V_mul(qt, q_form_rq((1 - C_ALPHA)*phi, N));
    qC = q_V_mul(q_form_rq((1 - C_ALPHA)*phi, N), qt);
    qU_world_hand = q_rotate(qC, qU_body);
    
    qt = qC;

    UPPR_ARM.getAccel();
    UPPR_ARM.getGyro();
    qGyro.w = 0.00f;
    qGyro.x = (UPPR_ARM.realGY[0] - GY_OFFSETS_UA[0]) * DEG_TO_RAD;
    qGyro.y = (UPPR_ARM.realGY[1] - GY_OFFSETS_UA[1]) * DEG_TO_RAD;
    qGyro.z = (UPPR_ARM.realGY[2] - GY_OFFSETS_UA[2]) * DEG_TO_RAD;
    
    qAccel.w = 0.00f;
    qAccel.x = UPPR_ARM.realAC[0];
    qAccel.y = UPPR_ARM.realAC[1];
    qAccel.z = UPPR_ARM.realAC[2];

    qD = q_form_rq( (DELTA_T*(q_norm(qGyro))), q_normalize(qGyro) );
    qt_ua = q_V_mul(qt_ua, qD);
    qt_ua = q_normalize(qt_ua);
    qA_world = q_rotate(qt_ua, qAccel);
    quaternion Nu = {
      0.0,
      qA_world.y/q_norm(qA_world),
      -1.0*(qA_world.x/q_norm(qA_world)),
      0.0
    };
    Nu = q_normalize(Nu);
    float phiu = acos(qA_world.z/q_norm(qA_world));
    //qC = q_V_mul(qt, q_form_rq((1 - C_ALPHA)*phi, N));
    qC_ua = q_V_mul(q_form_rq((1 - C_ALPHA)*phiu, Nu), qt_ua);
    qU_world_ua = q_rotate(qC_ua, qU_body);
    
    qt_ua = qC_ua;
  }
  #ifdef ARDUINO_DEBUG
    #ifdef PRINT_DISP_VECTOR
      Serial.print(float(sizeof(qU_world.x))); Serial.print("\n");
      /*
      Serial.print(mv.x); Serial.print("\t ");
      Serial.print(mv.y); Serial.print("\t ");
      Serial.print(mv.z); Serial.println();
      */
    #endif  
  #endif

  #ifdef HOST_DEBUG
    #ifdef PRINT_DISP_VECTOR
      Serial.print(qU_world_hand.w);Serial.print(',');
      Serial.print(qU_world_hand.x);Serial.print(',');
      Serial.print(qU_world_hand.y);Serial.print(',');
      Serial.print(qU_world_hand.z);Serial.print(',');
      Serial.print(qU_world_ua.w);Serial.print(',');
      Serial.print(qU_world_ua.x);Serial.print(',');
      Serial.print(qU_world_ua.y);Serial.print(',');
      Serial.print(qU_world_ua.z);Serial.print('\n');
      
    #endif
  #endif  
}