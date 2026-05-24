// ex06: 双通道反相PWM呼吸灯（警车双闪）
#define LED_PIN_A  2   // 灯A的PWM引脚（比如红灯）
#define LED_PIN_B  4   // 灯B的PWM引脚（比如蓝灯）
#define PWM_FREQ   5000
#define PWM_RES    8   // 8位分辨率，占空比范围0~255

void setup() {
  // 新版单步初始化PWM通道（无需手动指定通道号）
  ledcAttach(LED_PIN_A, PWM_FREQ, PWM_RES);
  ledcAttach(LED_PIN_B, PWM_FREQ, PWM_RES);
}

void loop() {
  // 灯A占空比从0 → 255，灯B从255 → 0（反相关系）
  for (int duty = 0; duty <= 255; duty++) {
    ledcWrite(LED_PIN_A, duty);        // 灯A逐渐变亮
    ledcWrite(LED_PIN_B, 255 - duty);  // 灯B逐渐变暗
    delay(8);
  }

  // 灯A占空比从255 → 0，灯B从0 → 255（反相）
  for (int duty = 255; duty >= 0; duty--) {
    ledcWrite(LED_PIN_A, duty);
    ledcWrite(LED_PIN_B, 255 - duty);
    delay(8);
  }
}