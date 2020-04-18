
#define BALL_SPEED 100
#define RAIN_SPEED 100
#define CUBE_SPEED 200
#define NOISE_SPEED 100
//#define HYPER_SPEED 100
#define TEXT_SPEED 50

// битмапа из программы (эффект №2)
const byte bitmap[8][8] = {
  {0xFF, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xFF},
  {0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81},
  {0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81},
  {0x81, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x81},
  {0x81, 0x00, 0x00, 0x18, 0x18, 0x00, 0x00, 0x81},
  {0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81},
  {0x81, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x81},
  {0xFF, 0x81, 0x81, 0x81, 0x81, 0x81, 0x81, 0xFF},
};

int effSpeed = 100;

void effectsTick() {
  EVERY_MS(effSpeed) {
    switch (effNum) {
      case 0: ball();
        break;
      case 1: rain();
        break;
      case 2: drawBitmap();
        break;
      case 3: cube();
        break;
      case 4: noise();
        break;
      case 5: text("SUBSCRIBE ");
        break;
    }
  }
}


// кубик
void ball() {
  static int8_t coord[3];
  static int8_t speed[3];
  if (loading) {
    effSpeed = BALL_SPEED;
    loading = false;
    FOR_i(0, 3) {
      coord[i] = random(80);
      speed[i] = random(5, 15);
    }
  }
  clearBuffer();
  FOR_i(0, 3) {
    int newCoord = coord[i] + speed[i];
    if (newCoord < 0 || newCoord > 70) speed[i] = -speed[i];
    else coord[i] = newCoord;
  }
  byte newCoord[3];
  FOR_i(0, 3) newCoord[i] = coord[i] / 10;

  drawDot(newCoord[0], newCoord[1], newCoord[2]);
  drawDot(newCoord[0] + 1, newCoord[1], newCoord[2]);
  drawDot(newCoord[0], newCoord[1] + 1, newCoord[2]);
  drawDot(newCoord[0], newCoord[1], newCoord[2] + 1);
  drawDot(newCoord[0] + 1, newCoord[1] + 1, newCoord[2]);
  drawDot(newCoord[0] + 1, newCoord[1], newCoord[2] + 1);
  drawDot(newCoord[0], newCoord[1] + 1, newCoord[2] + 1);
  drawDot(newCoord[0] + 1, newCoord[1] + 1, newCoord[2] + 1);
}

// ===== дождь =====
#define RAIN_PROB 10  // вероятность дождя
void rain() {
  if (loading) {
    effSpeed = RAIN_SPEED;
    loading = false;
    clearBuffer();
  }

  FOR_i(0, 8) {
    FOR_j(0, 8) {
      clearDot(i, j, 7);
      if (random8() < RAIN_PROB) drawDot(i, j, 7);
    }
  }
  shiftDown();
}

void shiftDown() {
  FOR_i(0, 7) {
    FOR_j(0, 8) {
      buffer[j][i] = buffer[j][i + 1];
    }
  }
}

// ===== битмап =====
void drawBitmap() {
  if (loading) {
    loading = false;
    clearBuffer();
    FOR_i(0, 8) {
      FOR_j(0, 8) {
        buffer[i][j] = bitmap[i][j];
      }
    }
  }
}

// ===== куб =====
void cube() {
  static int8_t counter = 0;
  static bool dir = true;
  if (loading) {
    loading = false;
    clearBuffer();
    effSpeed = CUBE_SPEED;
  }
  clearBuffer();
  drawCube(counter, counter, counter, 8 - counter * 2);
  counter += dir ? 1 : -1;
  if (dir && counter > 4) {
    dir = false;
  }
  if (!dir && counter < 0) {
    dir = true;
    counter = 0;
  }
}

// рисовать куб
void drawCube(byte x, byte y, byte z, byte size) {
  size -= 1;
  if (x + size + 1 > 8 || y + size + 1 > 8 || z + size + 1 > 8) return;
  FOR_i(x, x + size + 1) {
    drawDot(i, y, z);
    drawDot(i, y + size, z);
    drawDot(i, y, z + size);
    drawDot(i, y + size, z + size);
  }
  FOR_i(y, y + size + 1) {
    drawDot(x, i, z);
    drawDot(x + size, i, z);
    drawDot(x, i, z + size);
    drawDot(x + size, i, z + size);
  }
  FOR_i(z, z + size + 1) {
    drawDot(x, y, i);   // АХАХАХ ХУЙ
    drawDot(x + size, y + size, i);
    drawDot(x, y + size, i);
    drawDot(x + size, y, i);
  }
}

// ===== шум =====
#define STEP 50
void noise() {
  static int value = 0;
  if (loading) {
    loading = false;
    effSpeed = NOISE_SPEED;
  }
  value += 30;
  clearBuffer();
  FOR_i(0, 8) {
    FOR_j(0, 8) {
      drawDot(i, j, inoise8(i * STEP, j * STEP, value) / 32);
    }
  }
}

// ===== текст =====
void text(char* thisText) {
  static int8_t charPos = 0;
  static byte currentChar = 0;
  if (loading) {
    loading = false;
    effSpeed = TEXT_SPEED;
    charPos = -1;
    currentChar = 0;
  }  
  if (charPos == -1) {
    loadChar(thisText[currentChar]);
    charPos++;
  }
  if (charPos > 0) shiftForward();
  if (charPos == 1) loadChar(thisText[currentChar]);
  if (charPos > 1) loadChar(32);
  charPos++;
  
  if (charPos > 7) {
    charPos = -1;
    currentChar++;
    if (thisText[currentChar] == '\0') currentChar = 0;
  }
}

void loadChar(char thisChar) {
  FOR_i(0, 8) {
    byte thisFont = getFont(thisChar, i); // 32 - пусто!
    byte mirrorFont = 0;
    FOR_j(0, 7) {
      if (thisFont & (1 << 6 - j)) mirrorFont |= (1 << j);
      else mirrorFont |= (0 << j);
    }
    buffer[0][7 - i] = mirrorFont;
  }
}

void shiftForward() {
  FOR_i(0, 8) {
    FOR_j(0, 7) {
      buffer[7 - j][i] = buffer[7 - j - 1][i];
    }
  }
}


// ===== гиперштука =====
/*
  void hyperthing() {
  static int value = 0;
  if (loading) {
    loading = false;
    effSpeed = HYPER_SPEED;
  }
  clearBuffer();
  value += 50;

  // считаем только для четверти
  FOR_i(0, 4) {
    FOR_j(0, 4) {
      drawSymmetricLineH(4 + i, 4 + j, 2 + sin(radians(value - (i * i + j * j) * 3)) * 2);
    }
  }
  FOR_i(0, 8) {
    FOR_j(0, 4) {
      buffer[j][i] = buffer[7 - j][i];
    }
  }
  FOR_i(0, 8) {
    FOR_j(0, 8) {
      FOR_k(0, 4) {
        bitWrite(buffer[j][i], k+4, bitRead(buffer[j][i], 3-k));
      }
    }
  }
  }

  void drawSymmetricLineH(byte x, byte y, byte size) {
  if (size > 4) return;
  FOR_i(4 - size, 4 + size) {
    drawDot(x, y, i);
  }
  }
*/
