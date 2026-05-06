<div align="center">

# 🐾 스마트 반려동물 자동 급식기

### Smart Pet Auto Feeder

> 아두이노 UNO3를 활용한 IoT 기반 반려동물 자동 급식기

<br>

[![Arduino](https://img.shields.io/badge/Arduino-UNO3-00979D?style=flat-square&logo=arduino&logoColor=white)](https://www.arduino.cc/)
![Type](https://img.shields.io/badge/Type-IoT%20하드웨어-FF6B6B?style=flat-square)
![Status](https://img.shields.io/badge/Status-구현%20완료-brightgreen?style=flat-square)
![Solo](https://img.shields.io/badge/개인%20프로젝트-💛-yellow?style=flat-square)

<br>

</div>

---

## 📖 프로젝트 소개

**스마트 반려동물 자동 급식기**는 아두이노 UNO3를 사용하여 직접 설계·제작한 IoT 프로젝트입니다.

정해진 급식 시간에 멜로디 알림이 울리고, 초음파 센서로 반려동물의 접근을 감지하여 자동으로 사료를 배출합니다. 온습도 센서로 사료 상태를 모니터링하고 LCD로 다음 배식까지 남은 시간을 표시합니다.

<br>

## 💡 개발 배경

- 2023년 기준 1인 가구 비율 **35.5%** (통계청)
- 2024년 기준 반려동물 양육 비율 **28.6%** (농림축산식품부)
- 1인 가구 10가구 중 1가구가 반려동물 보유

바쁜 일상 속 규칙적인 급식이 어려운 반려동물 보호자들을 위해 자동 급식 시스템의 필요성을 느껴 개발하게 되었습니다.

<br>

## ✨ 주요 기능

| 기능 | 설명 |
|------|------|
| 🍚 **자동 급식** | 정해진 시간에 멜로디 알림 후 반려동물 접근 감지 시 자동 사료 배출 |
| 🎵 **멜로디 알림** | 급식 시간마다 부저로 '작은별' 멜로디 재생 |
| 📡 **접근 감지** | 초음파 센서로 반려동물 접근 여부 실시간 감지 |
| 💧 **사료 모니터링** | 온습도 센서로 사료통 내부 습도 측정 → LED로 건식/습식 구분 표시 |
| 🖥️ **LCD 디스플레이** | 다음 배식까지 남은 시간 실시간 표시 |
| 🔘 **수동 급식** | 택트스위치 버튼으로 수동 사료 배출 가능 |
| 💬 **미접근 알림** | 감지 시간 내 미접근 시 시리얼 모니터로 수동 급식 여부 확인 |

<br>

## 🔧 사용 부품 및 센서

| 부품 | 용도 |
|------|------|
| **Arduino UNO3** | 메인 컨트롤러 |
| **초음파 센서** | 반려동물 접근 감지 |
| **DHT11 온습도 센서** | 사료통 내부 습도 측정 |
| **서보 모터** | 사료통 뚜껑 개폐 |
| **LCD (I2C)** | 남은 시간 디스플레이 |
| **부저** | 멜로디 알림 |
| **LED x2** | 건식/습식 사료 상태 표시 |
| **택트스위치** | 수동 급식 버튼 |

<br>

## ⚙️ 동작 흐름

```
급식 시간 도달
    └── 부저 멜로디 재생 (작은별 🎵)
        └── 20초 감지창 시작
            ├── 반려동물 접근 감지 → 서보모터 구동 → 사료 배출 ✅
            └── 20초 내 미접근 → "밥을 줄까요?" 수동 확인

평상시
    └── 초음파 센서 상시 감지
        └── 접근 감지 → "밥을 줄까요?" 수동 확인

수동
    └── 택트스위치 버튼 누름 → 즉시 사료 배출
```

<br>

## 💻 소스 코드 주요 구조

```cpp
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include <DHT.h>

// 핀 설정
#define TRIG_PIN 4      // 초음파 센서 Trig
#define ECHO_PIN 5      // 초음파 센서 Echo
#define SERVO_PIN 9     // 서보 모터
#define BUZZER_PIN 8    // 부저
#define SWITCH_PIN 7    // 택트스위치
#define DHT_PIN 6       // 온습도 센서
#define LED_DRY 11      // 건식 LED
#define LED_WET 10      // 습식 LED
```

> 📎 전체 소스 코드는 `smart_feeder.ino` 파일을 참고해주세요!

<br>

## 🏗️ 하드웨어 구조

- **상단** : 수동 급식 택트스위치
- **중단** : LCD 디스플레이, 건식/습식 LED
- **사료통** : 페트병 + 서보모터 개폐 방식, 경사로로 사료 배출
- **하단** : 초음파 센서, 아두이노 및 회로 내장

<br>

## 🔍 향후 보완 사항

사료가 경사면을 타고 내려올 때 빠른 속도로 인해 밥그릇 밖으로 튀는 현상이 발생했습니다. 경사 각도를 더 완만하게 조정하면 해결될 것으로 예상합니다.

<br>


## 📁 파일 구성

```
Smart-Pet-Feeder/
├── README.md
├── smart_feeder.ino      # 아두이노 소스 코드
└── 개발보고서.pdf         # 프로젝트 개발 보고서
```

<br>

---

<div align="center">

🐾 *개인 프로젝트 · Arduino IoT*

</div>
