void isr() {
  if (millis() - debounce > 5) {
    cycle = millis() - debounce;
    debounce = millis();
    counter = 0;
    state = 0;
    drawBuffer(counter);
    //Timer1.setPeriod(cycleF / 2 / 8 * 1000);
    Timer1.setPeriod(cycleF / 2 * (sinDivider[counter] + OFFSET));
  }
}

ISR(TIMER1_A) {
  // рисуем и вверх и вниз
  switch (state) {
    case 0:
      counter++;
      if (counter > 7) {
        counter = 7;
        state = 1;
        break;
      }
      drawBuffer(counter);
      Timer1.setPeriod(cycleF / 2 * (sinDivider[counter] + OFFSET));
      break;
    case 1:
      counter--;
      drawBuffer(counter);
      Timer1.setPeriod(cycleF / 2 * (sinDivider[counter] + OFFSET));
      if (counter < 1) {
        state = 2;
        //clearMatrix();
        Timer1.stop();
      }
      break;
  }

  /*
    // рисование только на пути вверх
    if (counter < 7) {
      counter++;
      drawBuffer(counter);
      Timer1.setPeriod(cycleF / 2 * (sinDivider[counter] + OFFSET));

    } else {
      Timer1.stop();
      clearMatrix();
    }
  */
}
