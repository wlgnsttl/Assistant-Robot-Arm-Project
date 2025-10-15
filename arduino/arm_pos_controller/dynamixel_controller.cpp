#include "dynamixel_controller.h"

Dynamixel2Arduino dxls(Serial, DXL_DIR_PIN);
// SoftwareSerial soft_serial(7, 8); // DYNAMIXELShield UART RX/TX
// AltSoftSerial soft_serial;  // RX=8, TX=9 (고정 핀)

DynamixelController::DynamixelController(void)
	: dxl_num_(num_dxls) 
{

}

int DynamixelController::serial_setup(void) 
{
	DEBUG_SERIAL.begin(115200);   //Set debugging port baudrate to 115200bps
	
	dxls.begin(dxl_baud_rate);
	dxls.setPortProtocolVersion(dxl_protocol_ver);

	return 0;
}

int DynamixelController::setup(void)
{
	uint16_t target_dxls[dxl_num_] = { 0 };

	if (enable_torque(target_dxls) == 0)
	 	DEBUG_SERIAL.println("torque disable!");

	/* 토크 off 대기*/
	delay(200);

	for (int i = 0; i < dxl_num_; i++)
		target_dxls[i] = 1;

	if (_set_pos_lim(target_dxls) == 0)
		DEBUG_SERIAL.println("pos lim set!");

	if (_set_profile(target_dxls) == 0)
		DEBUG_SERIAL.println("profile set!");

	if (enable_torque(target_dxls) == 0)
		DEBUG_SERIAL.println("torque enable!");

	delay(200);

	return 0;
}

int DynamixelController::ping(void)
{
	int ret = 0;

	for (int i = 0; i < dxl_num_; i++) {
		const struct dynamixel_config cfg = dxls_config[i];

		while (Serial.available()) Serial.read();

		if (!dxls.ping(cfg.id)) {
			ret = -1;

			DEBUG_SERIAL.print("ping test fail : id : ");
			DEBUG_SERIAL.print(cfg.id);
			DEBUG_SERIAL.print(" err code : ");
			DEBUG_SERIAL.println(dxls.getLastLibErrCode());
		} else {
			DEBUG_SERIAL.print("ping test pass : id : ");
			DEBUG_SERIAL.println(cfg.id);
		}

		Serial.flush();   // TX 버퍼 비우기
		delay(30);        // 하드웨어 안정화
	}
	
	return ret;
}

int DynamixelController::enable_torque(uint16_t *target_dxls)
{
	int ret = 0;

	for (int i = 0; i < dxl_num_; i++) {
		const uint8_t enable_flag = target_dxls[i];
		const struct dynamixel_config cfg = dxls_config[i];

		while (Serial.available()) Serial.read();

		if (!dxls.write(cfg.id,
		                cfg.dxl_model->torque_enable.addr,
		                (uint8_t *)&enable_flag,
		                cfg.dxl_model->torque_enable.len,
		                TIMEOUT)) {
											
			DEBUG_SERIAL.print("torque setup fail : id : ");
			DEBUG_SERIAL.println(cfg.id);

			ret = -1;
		}

		Serial.flush();   // TX 버퍼 비우기
		delay(30);        // 하드웨어 안정화
	}
	
	return ret;
}

int DynamixelController::_set_pos_lim(uint16_t *target_dxls)
{
	int ret = 0;

	for (int i = 0; i < dxl_num_; i++) {
		if (target_dxls[i] == 0)
			continue;

		const struct dynamixel_config cfg = dxls_config[i];
		const struct dynamixel_model_config *model = cfg.dxl_model;

		// --- RX 버퍼 정리 ---
		while (Serial.available()) Serial.read();

		// --- 최소 위치 제한 설정 (EEPROM write) ---
		bool ok_min = dxls.write(cfg.id,
		                         model->min_pos_lim.addr,
		                         (uint8_t *)&cfg.pos_lim.min_pos,
		                         model->min_pos_lim.len,
		                         TIMEOUT);

		Serial.flush();   // TX 버퍼 비우기
		delay(30);        // 하드웨어 안정화

		while (Serial.available()) Serial.read();

		// --- 최대 위치 제한 설정 (EEPROM write) ---
		bool ok_max = dxls.write(cfg.id,
		                         model->max_pos_lim.addr,
		                         (uint8_t *)&cfg.pos_lim.max_pos,
		                         model->max_pos_lim.len,
		                         TIMEOUT);
		
		Serial.flush();   // TX 버퍼 비우기
		delay(30);        // 하드웨어 안정화

		// --- 결과 확인 ---
		if (!ok_min || !ok_max) {
			DEBUG_SERIAL.print("angle limit set fail : id : ");
			DEBUG_SERIAL.print(cfg.id);
			DEBUG_SERIAL.print(" min :");
			DEBUG_SERIAL.print(ok_min);
			DEBUG_SERIAL.print(" max :");
			DEBUG_SERIAL.println(ok_max);

			ret = -1;
		}
	}

	return ret;
}

int DynamixelController::_set_profile(uint16_t *target_dxls)
{
	int ret = 0;

	for (int i = 0; i < dxl_num_; i++) {
		if (target_dxls[i] == 0)
			continue;

		const struct dynamixel_config cfg = dxls_config[i];
		const struct dynamixel_model_config *model = cfg.dxl_model;

		// --- RX 버퍼 정리 ---
		while (Serial.available()) Serial.read();

		// --- Profile Acceleration 설정 (RAM 영역) ---
		bool ok_profile_acc = dxls.write(cfg.id,
		                                 model->profile_acceleration.addr,
		                                 (uint8_t *)&cfg.profile.acceleration,
		                                 model->profile_acceleration.len,
		                                 TIMEOUT);

		Serial.flush();   // TX 버퍼 비우기
		delay(30);        // 하드웨어 안정화

		while (Serial.available()) Serial.read();

		// --- Profile Velocity 설정 (RAM 영역) ---
		bool ok_profile_vel = dxls.write(cfg.id,
		                                 model->profile_velocity.addr,
		                                 (uint8_t *)&cfg.profile.velocity,
		                                 model->profile_velocity.len,
		                                 TIMEOUT);

		Serial.flush();   // TX 버퍼 비우기
		delay(30);        // 하드웨어 안정화

		// --- 결과 확인 ---
		if (!ok_profile_acc || !ok_profile_vel) {
			DEBUG_SERIAL.print("profile set fail : id : ");
			DEBUG_SERIAL.print(cfg.id);
			DEBUG_SERIAL.print(" acc:");
			DEBUG_SERIAL.print(ok_profile_acc);
			DEBUG_SERIAL.print(" vel:");
			DEBUG_SERIAL.println(ok_profile_vel);

			ret = -1;
		}

		Serial.flush();  // TX 버퍼 비우기
		delay(30);       // 하드웨어 안정화
	}

	return ret;
}

int DynamixelController::sync_write_pos(uint16_t *target_dxls, float *pos_deg)
{
  if (pos_deg == nullptr || target_dxls == nullptr) {
    DEBUG_SERIAL.println("Error: null position pointer");
    return -1;
  }

	_pos_to_rev(pos_deg);

  // --- 1. 활성 모터 수 계산 ---
  int active_count = 0;
  for (int i = 0; i < dxl_num_; i++)
    if (target_dxls[i] == 1)
      active_count++;

  if (active_count == 0) {
    DEBUG_SERIAL.println("No active motors.");
    return 0;
  }

  // --- 2. 활성 모터만 xels에 순차 저장 ---
  DYNAMIXEL::XELInfoSyncWrite_t xels[active_count];
  int idx = 0;
  for (int i = 0; i < dxl_num_; i++) {
    if (target_dxls[i] == 0)
      continue;
    xels[idx].id = dxls_config[i].id;
    xels[idx].p_data = (uint8_t*)&pos_rev_[i];
    idx++;
  }

    // --- 4. SyncWrite 구조체 초기화 ---

	DYNAMIXEL::InfoSyncWriteInst_t sw_infos_;
  sw_infos_.packet.p_buf = nullptr;   // 내부 버퍼 사용
  sw_infos_.packet.is_completed = false;
  sw_infos_.addr = dxls_config[0].dxl_model->goal_pos.addr;
  sw_infos_.addr_length = dxls_config[0].dxl_model->goal_pos.len;
  sw_infos_.p_xels = xels;
  sw_infos_.xel_count = active_count;
  sw_infos_.is_info_changed = true;

  // --- 5. SyncWrite 전송 ---
  if (dxls.syncWrite(&sw_infos_)) {
    DEBUG_SERIAL.println("[SyncWrite] success");
    return 0;
  } else {
    DEBUG_SERIAL.print("[SyncWrite] fail, err code: ");
    DEBUG_SERIAL.println(dxls.getLastLibErrCode());
   	return -1;
  }
}

int DynamixelController::_pos_to_rev(float *pos_deg)
{
	for (int i = 0; i < dxl_num_; i++) {
		float deg = pos_deg[i];

		const uint32_t orgin_rev = dxls_config[i].dxl_model->unit.orgin_rev;
		const float deg2rev = dxls_config[i].dxl_model->unit.deg_to_rev;

		pos_rev_[i] = (uint32_t)((deg * deg2rev) + (float)orgin_rev);
	}

	return 0;
}

int DynamixelController::write_led(uint16_t *target_dxls)
{
	for (int i = 0; i < dxl_num_; i++) {
		const uint8_t led_flag = target_dxls[i];
		const struct dynamixel_config cfg = dxls_config[i];

		if (!dxls.write(cfg.id,
		                cfg.dxl_model->led.addr,
		                (uint8_t *)&led_flag,
		                cfg.dxl_model->led.len,
		                TIMEOUT)) {
			DEBUG_SERIAL.print("led write fail : id : ");
			DEBUG_SERIAL.println(cfg.id);
			// return -(int)(cfg.id);
		}
	}
	
	return 0;
}
