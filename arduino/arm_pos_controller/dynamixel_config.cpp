#include "dynamixel_config.h"

const unsigned long dxl_baud_rate = 115200;
const float dxl_protocol_ver = 2.0;

/* MX-28 */
const struct dynamixel_model_config MX_28 = {
  .unit                   = {2048, 0.087912f, 11.375f},
  .torque_enable          = {64, 1},
  .led                    = {65, 1},
  .goal_pos               = {116, 4},
  .min_pos_lim            = {52, 4},
  .max_pos_lim            = {48, 4},
  .present_position       = {132, 4},
  .present_speed          = {128, 4},
  .moving                 = {122, 1},
  .profile_acceleration   = {108, 4},
  .profile_velocity       = {112, 4}
};

/* MX-64 */
const struct dynamixel_model_config MX_64 = {
  .unit                   = {2048, 0.087912f, 11.375f},
  .torque_enable          = {64, 1},
  .led                    = {65, 1},
  .goal_pos               = {116, 4},
  .min_pos_lim            = {52, 4},
  .max_pos_lim            = {48, 4},
  .present_position       = {132, 4},
  .present_speed          = {128, 4},
  .moving                 = {122, 1},
  .profile_acceleration   = {108, 4},
  .profile_velocity       = {112, 4}
};

/* MX-106 */
const struct dynamixel_model_config MX_106 = {
  .unit                   = {2048, 0.087912f, 11.375f},
  .torque_enable          = {64, 1},
  .led                    = {65, 1},
  .goal_pos               = {116, 4},
  .min_pos_lim            = {52, 4},
  .max_pos_lim            = {48, 4},
  .present_position       = {132, 4},
  .present_speed          = {128, 4},
  .moving                 = {122, 1},
  .profile_acceleration   = {108, 4},
  .profile_velocity       = {112, 4}
};


const struct dynamixel_config dxls_config[] = {
  { 1, {0, 4095},     {500, 2000}, &MX_64   },
  { 2, {1046, 4095},  {500, 2000}, &MX_106  },
  { 3, {901, 3213},   {500, 2000}, &MX_64   },
  { 4, {860, 3220},   {500, 2000},    &MX_28   },
  { 5, {0, 4095},     {500, 2000},    &MX_28   },
  { 6, {0, 4095},     {500, 2000},    &MX_28   }
};

const int num_dxls = sizeof(dxls_config) / sizeof(dxls_config[0]);