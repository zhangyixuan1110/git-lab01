#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "cao";
const char* password = "cao666666";

const int LED_PIN = 2;
const int TOUCH_PIN = T0; 
WebServer server(80);

volatile int systemState = 0;  // 0=未布防  1=布防(等待触摸)  2=报警(闪烁)
unsigned long lastBlink = 0;
const long blinkInterval = 150;

String makePage() {
  String stateText;
  if (systemState == 0) stateText = "未布防";
  else if (systemState == 1) stateText = "已布防 · 等待触摸";
  else stateText = "报警中 ";

  String html = R"rawliteral(
<!DOCTYPE html>
<html lang="zh-CN">
<head>
  <meta charset="UTF-8">
  <meta name="viewport" content="width=device-width, initial-scale=1.0">
  <title>ex08 安防报警器</title>
  <style>
    body{font-family:Arial;text-align:center;margin-top:50px}
    button{padding:15px 30px;font-size:18px;margin:10px}
  </style>
</head>
<body>
  <h1>安防报警器</h1>
  <h2>状态：)rawliteral" + stateText + R"rawliteral(</h2>
  
  <!-- 这里我帮你修好了！-->
  <a href="/arm"><button>布防</button></a>
  <a href="/disarm"><button>撤防</button></a>
  
</body>
</html>
)rawliteral";
  return html;
}

void handleRoot() { server.send(200, "text/html; charset=UTF-8", makePage()); }
void handleArm() { systemState = 1; server.sendHeader("Location", "/"); server.send(303); }
void handleDisarm() { systemState = 0; server.sendHeader("Location", "/"); server.send(303); }

// 触摸触发：只有布防状态，触摸才会亮
void IRAM_ATTR onTouch() {
  if (systemState == 1) {
    systemState = 2;  // 只有摸了，才变成报警
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  delay(2000);  // 超强防上电误触
  touchAttachInterrupt(TOUCH_PIN, onTouch, 100);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) { delay(500); }
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);
  server.on("/arm", handleArm);
  server.on("/disarm", handleDisarm);
  server.begin();
}

void loop() {
  server.handleClient();
  if (systemState == 2) {
    // 只有触发了报警，才闪灯
    unsigned long now = millis();
    if (now - lastBlink >= blinkInterval) {
      lastBlink = now;
      digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    }
  } else {
    // 未布防 / 布防未触摸 → 全部不亮
    digitalWrite(LED_PIN, LOW);
  }
}