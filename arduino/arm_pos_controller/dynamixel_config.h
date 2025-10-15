#ifndef _DYNAMIXEL_CONFIG_H_
#define _DYNAMIXEL_CONFIG_H_

#include <stdint.h>

extern const unsigned long dxl_baud_rate;
extern const float dxl_protocol_ver;

/* DYNAMIXEL Control Table Field */
struct dynamixel_control_table 
{
	uint16_t addr;
	uint16_t len;
};

struct unit_convert_table
{
	uint16_t orgin_rev;
	float rev_to_deg;
	float deg_to_rev;
};

struct dynamixel_model_config 
{
	const struct unit_convert_table				unit;
	const struct dynamixel_control_table 	torque_enable;
	const struct dynamixel_control_table 	led;
	const struct dynamixel_control_table 	goal_pos;
	const struct dynamixel_control_table 	min_pos_lim;
	const struct dynamixel_control_table 	max_pos_lim;
	const struct dynamixel_control_table 	present_position;
	const struct dynamixel_control_table 	present_speed;
	const struct dynamixel_control_table 	moving;
	const struct dynamixel_control_table 	profile_acceleration;
	const struct dynamixel_control_table 	profile_velocity;
};


extern const struct dynamixel_model_config MX_28;
extern const struct dynamixel_model_config MX_64;
extern const struct dynamixel_model_config MX_106;

struct dynamixel_pos_lim
{
	uint32_t min_pos;
	uint32_t max_pos;
};

struct dynamixel_profile
{
	uint32_t acceleration;
	uint32_t velocity;
};

struct dynamixel_config 
{
	const uint8_t id;
	const struct dynamixel_pos_lim pos_lim;
	const struct dynamixel_profile profile;
	const struct dynamixel_model_config *dxl_model;  
};

extern const int num_dxls;
extern const struct dynamixel_config dxls_config[];

#endif
