#include "dynamixel_controller.h"

DynamixelController dxl_control;

#define MAX_PACKET_LEN 128
#define MAX_DXL 12

/* -------------------- 데이터 구조 -------------------- */
struct ControlPacket
{
  int motor_count;
  uint16_t target_flags[MAX_DXL];
  float target_pos[MAX_DXL];
};

/* -------------------- 전역 변수 -------------------- */
char rx_buf[MAX_PACKET_LEN];
int rx_index = 0;
bool packet_started = false;

void setup() 
{
  dxl_control.serial_setup();

  dxl_control.ping();

  if (dxl_control.setup() < 0) {
    DEBUG_SERIAL.print("DXL setup fail : err code : ");
    DEBUG_SERIAL.println(dxls.getLastLibErrCode());
  }
}

void loop()
{
  while (DEBUG_SERIAL.available()) {
    char c = DEBUG_SERIAL.read();
    if (c == '\r' || c == '\n') continue; // 루프 맨 위에 추가
    // DEBUG_SERIAL.print(c); // 받은 문자 그대로 에코

    /* --- 패킷 시작 --- */
    if (c == 'S') {
      packet_started = true;
      rx_index = 0;
      rx_buf[rx_index++] = c;
      continue;
    }

    /* --- 패킷 종료 --- */
    if (c == 'E' && packet_started) {
      rx_buf[rx_index] = '\0';
      ControlPacket pkt;

      if (parse_packet(rx_buf, &pkt)) {
        execute_packet(&pkt);
      } else {
        DEBUG_SERIAL.println("Packet parse error");
      }

      packet_started = false;
      rx_index = 0;
      continue;
    }

    /* --- 수신 중 --- */
    if (packet_started && rx_index < MAX_PACKET_LEN - 1)
      rx_buf[rx_index++] = c;

  }
}

bool parse_packet(char *buf, ControlPacket *pkt)
{
  if (buf[0] != 'S') return false;

  /* --- 1. 모터 개수 (2바이트) --- */
  char num_str[3] = { buf[1], buf[2], '\0' };
  pkt->motor_count = atoi(num_str);
  if (pkt->motor_count <= 0 || pkt->motor_count > MAX_DXL)
    return false;

  /* --- 2. 플래그 영역 (공백 보정 추가) --- */
  int flag_start = 3;

  for (int i = 0; i < pkt->motor_count; i++) {
    pkt->target_flags[i] = (buf[flag_start + i] == '1') ? 1 : 0;
  }

  /* --- 3. 실수 데이터 시작 --- */
  // 플래그 뒤에 공백이 하나 있다고 가정
  char *float_str = &buf[flag_start + pkt->motor_count];

  // 공백 문자 스킵
  while (*float_str == ' ') float_str++;

  int idx = 0;
  char *token = strtok(float_str, " ");
  while (token && idx < pkt->motor_count) {
    pkt->target_pos[idx] = atof(token);
    token = strtok(NULL, " ");
    idx++;
  }

  return true;
}

void execute_packet(ControlPacket *pkt)
{
  DEBUG_SERIAL.println("=== Packet Received ===");
  DEBUG_SERIAL.print("Motor Count: ");
  DEBUG_SERIAL.println(pkt->motor_count);

  DEBUG_SERIAL.print("Flags: ");
  for (int i = 0; i < pkt->motor_count; i++)
    DEBUG_SERIAL.print(pkt->target_flags[i]);
  DEBUG_SERIAL.println();

  DEBUG_SERIAL.print("Positions: ");
  for (int i = 0; i < pkt->motor_count; i++) {
    DEBUG_SERIAL.print(pkt->target_pos[i], 3);
    DEBUG_SERIAL.print(" ");
  }
  DEBUG_SERIAL.println();
  DEBUG_SERIAL.println("=======================");

  uint16_t target_dxls[MAX_DXL] = {0};
  float pos_deg[MAX_DXL] = {0.0f};

  for (int i = 0; i < pkt->motor_count; i++) {
    target_dxls[i] = pkt->target_flags[i];
    pos_deg[i] = pkt->target_pos[i];
  }

  dxl_control.sync_write_pos(target_dxls, pos_deg);
}
