void drawBuffer(byte layer) {
  for (byte i = 0; i < 8; i++) {
    sendDATA(i, buffer[i][layer]);
  }
}

void clearBuffer() {
  for (byte i = 0; i < 8; i++) {
    for (byte j = 0; j < 8; j++) {
      buffer[i][j] = 0;
    }
  }
}
