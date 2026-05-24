#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "cao";
const char* password = "cao666666";

const int TOUCH_PIN = T0; // GPIO4
WebServer server(80);

// 实时触摸值
int touchValue = 0;

// 仪表盘网页
String makePage() {
  String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
  <meta charset="UTF-8">
  <title>实时触摸传感器仪表盘</title>
  <style>
    body { text-align:center; font-family:Arial; margin-top:100px; }
    .value { font-size:80px; color:#2c3e50; font-weight:bold; }
    .label { font-size:24px; color:#7f8c8d; }
  </style>
</head>
<body>
  <h1>ESP32 触摸传感器实时仪表盘</h1>
  <div class="label">触摸值</div>
  <div class="value" id="touchVal">0</div>

  <script>
    // AJAX 实时拉取数据
    function updateValue() {
      fetch("/data")
        .then(res => res.text())
        .then(val => {
          document.getElementById("touchVal").innerText = val;
        });
    }
    // 每 100ms 更新一次
    setInterval(updateValue, 100);
  </script>
</body>
</html>
)rawliteral";
  return html;
}

// 根页面
void handleRoot() {
  server.send(200, "text/html; charset=UTF-8", makePage());
}

// 返回实时触摸值
void handleData() {
  touchValue = touchRead(TOUCH_PIN);
  server.send(200, "text/plain", String(touchValue));
}

void setup() {
  Serial.begin(115200);

  WiFi.begin(ssid, password);
  Serial.print("连接WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi OK");
  Serial.println("访问：http://" + WiFi.localIP().toString());

  server.on("/", handleRoot);
  server.on("/data", handleData); // 实时数据接口
  server.begin();
}

void loop() {
  server.handleClient();
}