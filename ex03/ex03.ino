// ex03.ino - SOS闪烁 millis 无阻塞
const int ledPin = 2;

const long shortOn = 200;
const long shortOff = 200;
const long longOn = 600;
const long longOff = 200;
const long pauseTime = 2000;

unsigned long previousMillis = 0;
int state = 0;
int flashCount = 0;
bool ledState = LOW;

void setup() {
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
}

void loop() {
  unsigned long currentMillis = millis();

  switch(state) {
    case 0:
      state = 1;
      flashCount = 0;
      previousMillis = currentMillis;
      break;

    case 1:
      if(flashCount < 3) {
        if(ledState == LOW && currentMillis - previousMillis >= shortOff) {
          ledState = HIGH;
          digitalWrite(ledPin, ledState);
          previousMillis = currentMillis;
        } else if(ledState == HIGH && currentMillis - previousMillis >= shortOn) {
          ledState = LOW;
          digitalWrite(ledPin, ledState);
          previousMillis = currentMillis;
          flashCount++;
        }
      } else {
        state = 2;
        flashCount = 0;
        previousMillis = currentMillis;
      }
      break;

    case 2:
      if(flashCount < 3) {
        if(ledState == LOW && currentMillis - previousMillis >= longOff) {
          ledState = HIGH;
          digitalWrite(ledPin, ledState);
          previousMillis = currentMillis;
        } else if(ledState == HIGH && currentMillis - previousMillis >= longOn) {
          ledState = LOW;
          digitalWrite(ledPin, ledState);
          previousMillis = currentMillis;
          flashCount++;
        }
      } else {
        state = 3;
        flashCount = 0;
        previousMillis = currentMillis;
      }
      break;

    case 3:
      if(flashCount < 3) {
        if(ledState == LOW && currentMillis - previousMillis >= shortOff) {
          ledState = HIGH;
          digitalWrite(ledPin, ledState);
          previousMillis = currentMillis;
        } else if(ledState == HIGH && currentMillis - previousMillis >= shortOn) {
          ledState = LOW;
          digitalWrite(ledPin, ledState);
          previousMillis = currentMillis;
          flashCount++;
        }
      } else if(currentMillis - previousMillis >= pauseTime) {
        state = 0;
      }
      break;
  }
}