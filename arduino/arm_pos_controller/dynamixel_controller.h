#include <stdint.h>
#ifndef _DYNAMIXEL_CONTROLLER_H_
#define _DYNAMIXEL_CONTROLLER_H_

#include <Arduino.h>
#include <Dynamixel2Arduino.h>
#include <SoftwareSerial.h>
// #include <AltSoftSerial.h>

#include "dynamixel_config.h"

#define DXL_DIR_PIN 2
#define MAX_NUM_DYNAMIXEL 10
#define TIMEOUT 10

#define DEBUG_SERIAL Serial1

extern Dynamixel2Arduino dxls;
// extern SoftwareSerial soft_serial;
// extern AltSoftSerial soft_serial;  // RX=8, TX=9 (고정 핀)

class DynamixelController
{
  public:
    DynamixelController(void);

    int serial_setup(void);

    int setup(void);

    int ping(void);

    int enable_torque(uint16_t *target_dxls);

    int _set_pos_lim(uint16_t *target_dxls);

    int _set_profile(uint16_t *target_dxls);

    int sync_write_pos(uint16_t *target_dxls, float *pos_deg);

    int _pos_to_rev(float *pos_deg);

    int update_pos(uint16_t *target_dxls);

    int write_led(uint16_t *target_dxls);


  private:
    /*
     * 
     */
    

    const int dxl_num_;

    uint32_t pos_rev_[MAX_NUM_DYNAMIXEL];



};


#endif