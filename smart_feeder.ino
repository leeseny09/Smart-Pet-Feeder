#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <DHT.h>

#define TRIG_PIN 4
#define ECHO_PIN 5
#define SERVO_PIN 9
#define BUZZER_PIN 8
#define SWITCH_PIN 7

#define DHT_PIN 6
#define DHT_TYPE DHT11

#define LED_DRY 11
#define LED_WET 10

Servo myServo;
LiquidCrystal_I2C lcd(0x27, 16, 2);
DHT dht(DHT_PIN, DHT_TYPE);

const int openAngle = 90;
const int closeAngle = 180;
const int detectDistance = 20;
const unsigned long openDuration = 2000;
const unsigned long melodyInterval = 120000UL;

const unsigned long detectWindow = 20000UL;   

unsigned long lastMelodyTime = 0;
unsigned long detectWindowStart = 0;
bool detectWindowActive = false;
bool prevDetected = false;
bool feedingDoneInWindow = false;
unsigned long lastDhtUpdate = 0;
bool waitingForManualFeed = false;

void setup() {
Serial.begin(9600);
Serial.println("---------------------");
pinMode(TRIG_PIN, OUTPUT);
pinMode(ECHO_PIN, INPUT);
pinMode(BUZZER_PIN, OUTPUT);
pinMode(SWITCH_PIN, INPUT_PULLUP);
pinMode(LED_DRY, OUTPUT);
pinMode(LED_WET, OUTPUT);
myServo.attach(SERVO_PIN);
myServo.write(closeAngle);

lcd.init();
lcd.backlight();
lcd.setCursor(0, 0);
lcd.print("Smart Feeder");

dht.begin();

playMelody();
startDetectWindow();
}

void loop() {
unsigned long now = millis(); 
/
if (!detectWindowActive) {
displayTimeToNextFeed(now);
} else {
lcd.setCursor(0, 1);
lcd.print("                "); 
}

if (now - lastDhtUpdate > 2000) { 
updateDhtAndLed();
lastDhtUpdate = now;
}

if (digitalRead(SWITCH_PIN) == LOW) {
Serial.println("수동 급식 버튼이 눌렸습니다. 밥을 줍니다.");
feed();
lastMelodyTime = millis();
delay(1000);
}

if (detectWindowActive) {
int distance = measureDistance();
if (!feedingDoneInWindow && distance > 0 && distance < detectDistance && !prevDetected) {
Serial.println("강아지가 접근했습니다! 밥을 줍니다.");
feed();
feedingDoneInWindow = true;
prevDetected = true;
lastMelodyTime = millis();
}
if (distance >= detectDistance) prevDetected = false;
if (now - detectWindowStart >= detectWindow) {
  detectWindowActive = false;
  if (!feedingDoneInWindow) {
    Serial.println("강아지가 밥을 먹으러 오지 않았어요.");
    Serial.println("밥을 줄까요? (예/아니요)");
    waitingForManualFeed = true;
    lastMelodyTime = now;
  }
  feedingDoneInWindow = false;
}
delay(100);
return;
}   

if (!detectWindowActive && (now - lastMelodyTime >= melodyInterval)) {
playMelody();
startDetectWindow();
}

int distance = measureDistance();
if (!detectWindowActive && distance > 0 && distance < detectDistance && !prevDetected && !waitingForManualFeed) {
Serial.println("강아지가 접근했어요 !");
Serial.println("밥을 줄까요? (예/아니요) 입력 후 엔터");
waitingForManualFeed = true;
prevDetected = true;
}
if (distance >= detectDistance) prevDetected = false; 

if (waitingForManualFeed && Serial.available()) {
String answer = Serial.readStringUntil('\n');
answer.trim();
if (answer == "예") {
Serial.println("밥을 줍니다.");
feed();
waitingForManualFeed = false;
lastMelodyTime = millis();
} else if (answer == "아니요") {
Serial.println("밥을 주지 않습니다.");
waitingForManualFeed = false;
} else {
Serial.println("예 또는 아니요로 입력해주세요.");
}
}

delay(50);
}

void displayTimeToNextFeed(unsigned long now) {
unsigned long elapsed = now - lastMelodyTime;
unsigned long remain;
if (elapsed < melodyInterval) {
remain = melodyInterval - elapsed;
} else {
remain = 0;
}
unsigned long seconds = remain / 1000;
unsigned int h = seconds / 3600;
unsigned int m = (seconds % 3600) / 60;
unsigned int s = seconds % 60;

lcd.setCursor(0, 1);
lcd.print("Next:");
lcd.print(h);
lcd.print("h ");
if (m < 10) lcd.print('0');
lcd.print(m);
lcd.print("m ");
if (s < 10) lcd.print('0');
lcd.print(s);
lcd.print("s ");
lcd.print("   ");
}

void updateDhtAndLed() {
float humidity = dht.readHumidity();
Serial.print("현재 습도: ");
Serial.print(humidity);
Serial.println("%");
if (isnan(humidity)) {
digitalWrite(LED_DRY, LOW);
digitalWrite(LED_WET, LOW);
return;
}
if (humidity < 70 ) {
digitalWrite(LED_DRY, HIGH);
digitalWrite(LED_WET, LOW);
} else {
digitalWrite(LED_DRY, LOW);
digitalWrite(LED_WET, HIGH);
}
}

void startDetectWindow() {
detectWindowActive = true;
detectWindowStart = millis();
feedingDoneInWindow = false;
prevDetected = false;
Serial.println("20초 자동급식 감지창이 시작되었습니다.");
}

int measureDistance() {
digitalWrite(TRIG_PIN, LOW);
delayMicroseconds(2);
digitalWrite(TRIG_PIN, HIGH);
delayMicroseconds(10);
digitalWrite(TRIG_PIN, LOW);

long duration = pulseIn(ECHO_PIN, HIGH, 30000);
int distance = duration * 0.034 / 2;
return distance;
}  

void feed() {
myServo.write(openAngle);
delay(openDuration);
myServo.write(closeAngle);
}

void playMelody() {
int melody[] = {
262, 262, 392, 392, 440, 440, 392,
349, 349, 330, 330, 294, 294, 262
};
int noteDurations[] = {
400, 400, 400, 400, 400, 400, 800,
400, 400, 400, 400, 400, 400, 800
}; 

for (int thisNote = 0; thisNote < 14; thisNote++) {
tone(BUZZER_PIN, melody[thisNote], noteDurations[thisNote]);
delay(noteDurations[thisNote] * 1.3);
}
noTone(BUZZER_PIN);
} 
