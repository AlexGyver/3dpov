/*
  Скетч к проекту "3D голограмма"
  Страница проекта (схемы, описания): https://alexgyver.ru/3dpov/
  Исходники на GitHub: https://github.com/AlexGyver/3dpov/
  Нравится, как написан и закомментирован код? Поддержи автора! https://alexgyver.ru/support_alex/
  Автор: AlexGyver Technologies, 2020
  http://AlexGyver.ru/
*/

#define OFFSET 10
// подгон - позволяет скомпенсировать 
// неправильно выставленную длину кривошипа

#define START_EFFECT 5  // эффект при старте
// также эффекты можно переключать кнопкой

#define CS_PIN 9
#include <SPI.h>
#include <GyverTimers.h>
#include <FastLED.h>
#include "buttonMinim.h"
#include "fonts.h"
buttonMinim butt(3);

byte buffer[8][8];
#define drawDot(x, y, z) bitSet(buffer[(x)][(z)], (y))
#define clearDot(x, y, z) bitClear(buffer[(x)][(z)], (y))
// макросы
//===========================
#define EVERY_MS(x) \
  static uint32_t tmr;\
  bool flag = millis() - tmr >= (x);\
  if (flag) tmr = millis();\
  if (flag)
//===========================
// ленивая жопа
#define FOR_i(from, to) for(int i = (from); i < (to); i++)
#define FOR_j(from, to) for(int j = (from); j < (to); j++)
#define FOR_k(from, to) for(int k = (from); k < (to); k++)
//===========================

#define EFF_AMOUNT 6
bool loading = true;
byte effNum = START_EFFECT;
const byte sinDivider[] = {215, 115, 89, 82, 82, 89, 115, 215};
volatile uint32_t cycle;
volatile uint32_t debounce;
volatile int8_t counter = 0;
volatile byte state = 0;
bool flag = false;
int cycleF;

void setup() {
  pinMode(CS_PIN, OUTPUT);
  SPI.setBitOrder(MSBFIRST);
  SPI.begin();
  maxInit();
  clearMatrix();
  setBright(15);
  Serial.begin(9600);
  attachInterrupt(0, isr, FALLING);
  Timer1.setPeriod(1000);
  Timer1.enableISR(CHANNEL_A);

  /*clearBuffer();
  for (byte i = 0; i < 8; i++) {
    for (byte j = 0; j < 8; j++) {
      drawDot(i, j, i);
    }
  }*/
}

void loop() {
  systemTick();
  effectsTick();
  controlTick();
}

void controlTick() {
  if (butt.clicked()) {
    if (++effNum >= EFF_AMOUNT) effNum = 0;
    loading = true;
  }
}

void systemTick() {
  EVERY_MS(100) {
    // фильтруем период
    cycleF = (12 * cycleF + 4 * cycle) >> 4;

    // проверить  есть ли вращение, если нет то очистить матрицу
    if (millis() - debounce > 1000) {
      if (flag) {
        flag = false;
        clearMatrix();
      }
    } else {
      if (!flag) {
        flag = true;
      }
    }
  }
}
