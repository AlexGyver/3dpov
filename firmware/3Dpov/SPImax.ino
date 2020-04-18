void maxInit() {
  sendCMD(0x0f, 0x00);  // отключить режим теста
  sendCMD(0x09, 0x00);  // выключить декодирование
  sendCMD(0x0a, 0x00);  // яркость
  sendCMD(0x0b, 0x0f);  // отображаем всё
  sendCMD(0x0C, 0x01);  // включить
}

void sendCMD(uint8_t address, uint8_t value) {
  digitalWrite(CS_PIN, LOW);
  SPI.transfer(address);
  SPI.transfer(value);
  SPI.transfer(address);
  SPI.transfer(value);
  digitalWrite(CS_PIN, HIGH);
}

void setBright(byte value) {
  sendCMD(0x0a, value);  // яркость 0-15
}

void sendDATA(uint8_t address, uint8_t value) {
  digitalWrite(CS_PIN, LOW);
  SPI.transfer(++address);
  SPI.transfer(value);
  digitalWrite(CS_PIN, HIGH);
}

void clearMatrix() {
  for (byte i = 0; i < 8; i++) {
    sendDATA(i, 0);
  }
}
/*
void drawLine(byte x0, byte y0, byte x1, byte y1) {
  if (x0 == x1) {
    for (byte i = y0; i < y1; i++) setPixel(x0, i);
  } else if (y0 == y1) {
    for (byte i = x0; i < x1; i++) setPixel(i, y0);
  } else {
    int sx, sy, e2, err;
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);
    if (x0 < x1) sx = 1; else sx = -1;
    if (y0 < y1) sy = 1; else sy = -1;
    err = dx - dy;
    for (;;) {
      setPixel(x0, y0);
      if (x0 == x1 && y0 == y1) return;
      e2 = err << 1;
      if (e2 > -dy) {
        err = err - dy;
        x0 = x0 + sx;
      }
      if (e2 < dx) {
        err = err + dx;
        y0 = y0 + sy;
      }
    }
  }
}
*/
/*
void setPixel(byte x, byte y) {
  bitSet(buffer[x], y);
}

void sendBuffer() {
  for (byte i = 0; i < 8; i++) {
    sendDATA(i, buffer[i]);
  }
}
*/
