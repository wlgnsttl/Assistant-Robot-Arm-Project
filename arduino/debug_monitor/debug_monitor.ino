void setup() {

  Serial.begin(115200);   // PC ↔ 메가 USB
  Serial1.begin(115200);  // 메인 UNO ↔ 메가
  Serial.println("waiting");
}

void loop() {
  // UNO → PC 출력
  if (Serial1.available()) {
    Serial.write(Serial1.read());
  }

  // PC에서 입력 → UNO 전달 (옵션)
  if (Serial.available()) {
    Serial1.write(Serial.read());
  }
}
