/* =========================================================
 *  UART1(RX1=19, TX1=18)을 사용하여 라즈베리파이 데이터 송신
 *  보드레이트: 115200bps
 *  시리얼 모니터 출력용 기본 UART(USB 연결): Serial
 * ========================================================= */

#define MAX_DXL 6
#define MOTION_COUNT 4       // 전송할 모션 개수
#define SEND_INTERVAL 2200   // 5초(5000ms) 간격

typedef struct {
    int motor_count;
    int target_flags[MAX_DXL];
    float target_pos[MAX_DXL];
} ControlPacket;

/* ---------------------------------------------------------
 * build_packet() : 구조체 → 문자열 패킷
 * --------------------------------------------------------- */
void build_packet(char *buf, size_t buflen, const ControlPacket *pkt)
{
  buf[0] = '\0';
  snprintf(buf, buflen, "S%02d", pkt->motor_count);

  // 플래그
  for (int i = 0; i < pkt->motor_count; i++) {
    char f[2] = { char('0' + (pkt->target_flags[i] ? 1 : 0)), '\0' };
    strlcat(buf, f, buflen);
  }

  strlcat(buf, " ", buflen);

  // 실수값
  for (int i = 0; i < pkt->motor_count; i++) {
    char num[24];
    dtostrf(pkt->target_pos[i], 0, 3, num);
    strlcat(buf, num, buflen);
    if (i < pkt->motor_count - 1) strlcat(buf, " ", buflen);
  }

  strlcat(buf, "E", buflen);  // 종료 문자
}

/* ---------------------------------------------------------
 * send_packet() : 패킷 송신
 * --------------------------------------------------------- */
void send_packet(const ControlPacket *pkt)
{
  char buf[256];
  build_packet(buf, sizeof(buf), pkt);
  Serial1.println(buf);
  Serial.print("TX → ");
  Serial.println(buf);
}

/* ---------------------------------------------------------
 * setup()
 * --------------------------------------------------------- */
void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial1.begin(115200);
  Serial.println("UART1 송신 예제 시작");
}

/* ---------------------------------------------------------
 * loop()
 *   - 5초마다 순차적으로 여러 모션을 송신
 *   - 4번 모터 = (2번 모터 - 3번 모터 - 90)
 * --------------------------------------------------------- */
void loop() {
  static unsigned long last_send_time = 0;
  static int motion_index = 0;

  // 모션 시퀀스 정의
  static const float motion_set[MOTION_COUNT][MAX_DXL] = {
    {  38,  4,  -60,  0.0,  120,  1.41 },
    { 7,  21,   -14, 0.0,  110,  1 },
    { -28,  14,  -48,   0.0, -174,  10.0 },
    {  -47,    -3,   -1,   0.0,   65,   0.0 }
  };

  if (millis() - last_send_time >= SEND_INTERVAL) {
    last_send_time = millis();

    ControlPacket pkt;
    pkt.motor_count = MAX_DXL;

    for (int i = 0; i < MAX_DXL; i++) {
      pkt.target_flags[i] = 1;
      pkt.target_pos[i] = motion_set[motion_index][i];
    }

    // --- 4번 모터 계산식 적용 (index 3은 4번 모터) ---
    pkt.target_pos[3] = pkt.target_pos[1] - pkt.target_pos[2] - 90.0;

    send_packet(&pkt);

    motion_index++;
    if (motion_index >= MOTION_COUNT) motion_index = 0; // 순환
  }
}
