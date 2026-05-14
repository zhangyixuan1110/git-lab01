// ex02.ino - 用millis()实现1Hz无阻塞LED闪烁（ESP32）
const int ledPin = 2; // ESP32板载LED引脚

unsigned long previousMillis = 0;
const long interval = 500; // 亮/灭各500ms → 1Hz
bool ledState = LOW;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
    Serial.print("LED状态：");
    Serial.println(ledState ? "点亮" : "熄灭");
  }
}
