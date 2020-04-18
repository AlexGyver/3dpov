int thisZ = 0;
int rotX = 20, prevRotX;
boolean clickFlag, turnFlag, leftClickFlag;
boolean[][][] dotArray = new boolean[8][8][8];
PFont Font1;

void setup() {
  size(640, 360, P3D);
  textAlign(CENTER, CENTER);
  Font1 = createFont("Arial Bold", 18);
  textFont(Font1);
  fillArray();
}

void draw() {  
  background(0);
  circles();
  buttons();
  mouseTick();
  mouseDebug();

  drawBalls();
}

void buttons() {
  stroke(0);
  textSize(20);
  fill(255);
  textSize(25);
  text("GyverCUBE generator", 640/2, 20);

  fill(60);
  rect(25, 300, 100, 40, 8);
  rect(135, 300, 100, 40, 8);
  //rect(245, 300, 100, 40, 8);

  fill(255);
  textSize(20);
  text("CLEAR", 25+100/2, 300+40/2);
  text("SAVE", 135+100/2, 300+40/2);

  text("Layer: ", 75, 50);
  text(thisZ, 115, 50);
}

void mouseDebug() {
  fill(255, 0, 0);
  textSize(16);
  text(mouseX, 580, 20);
  text(mouseY, 620, 20);
}

void circles() {
  stroke(255);
  for (byte x = 0; x < 8; x++) {
    for (byte y = 0; y < 8; y++) {
      if (dotArray[x][y][thisZ]) fill(255);
      else fill(0);
      circle(x*25+50, y*25+90, 20);
    }
  }
}

boolean overRect(int x, int y, int width, int height) {
  if (mouseX >= x && mouseX <= x+width && 
    mouseY >= y && mouseY <= y+height) {
    return true;
  } else {
    return false;
  }
}

boolean overCircle(int x, int y, int diameter) {
  float disX = x - mouseX;
  float disY = y - mouseY;
  if (sqrt(sq(disX) + sq(disY)) < diameter/2 ) {
    return true;
  } else {
    return false;
  }
}

void clearCube() {
  for (byte k = 0; k < 8; k++)
    for (byte i = 0; i < 8; i++)
      for (byte j = 0; j < 8; j++) {        
        dotArray[i][j][k] = false;
      }
}

void saveBitmap() {
  String[] lines = new String[10];
  lines[0] = "const byte bitmap[8][8] = {";

  for (int z = 0; z < 8; z++) {    
    lines[z+1] = "\t{";    

    for (byte y = 0; y < 8; y++) {  
      int thisByte = 0;
      for (byte x = 0; x < 8; x++) {
        thisByte |= int(dotArray[x][y][z]) << (7-x);
      }
      
      lines[z+1] += "0x" + hex(thisByte, 2) + "";
      if (y < 7) lines[z+1] += ", ";
    }

    lines[z+1] += "},";
  }

  lines[9] = "};";
  saveStrings("lines.h", lines);
}


void mouseTick() {
  if (mousePressed && (mouseButton == RIGHT)) {
    clickFlag = true;
    turnFlag = true;
  }

  noStroke();

  if (mousePressed && (mouseButton == LEFT)) {
    if (!leftClickFlag) {
      leftClickFlag = true;
      if (overRect(25, 300, 100, 40)) clearCube();
      if (overRect(135, 300, 100, 40)) saveBitmap();

      //fillArray();
      for (byte x = 0; x < 8; x++) {
        for (byte y = 0; y < 8; y++) {
          if (overCircle(x*25+50, y*25+90, 20)) {
            dotArray[x][y][thisZ] = !dotArray[x][y][thisZ];
            if (dotArray[x][y][thisZ]) circle(x*25+50, y*25+90, 20);
          }
        }
      }
    }
  }
}

void drawBalls() {
  noStroke();
  fill(255);
  lights();

  translate(640/2+150, 360/2, -25*4);
  rotateY(rotX * 0.017);
  translate(-25*4+25/2, -25*4+25/2, 25*4);

  for (byte z = 0; z < 8; z++) {
    if (z == thisZ) fill(0, 255, 255);
    else fill(255);

    for (byte y = 0; y < 8; y++) {
      for (byte x = 0; x < 8; x++) {

        if (dotArray[x][y][z] == true) {
          fill(0, 255, 255);
          sphere(10);
        } else {
          fill(0, 255, 255, 30);
          if (z == thisZ) sphere(10);
          fill(255);
        }

        translate(25, 0, 0);
      }
      translate(-25*8, 25, 0);
    }
    translate(0, -25*8, -25);
  }
}

void mouseDragged() {
  if (turnFlag) {
    if (clickFlag) {
      clickFlag = false;
      prevRotX = mouseX;
    }
    rotX -= prevRotX - mouseX;
    prevRotX = mouseX;
  }
}

void fillArray() {
  for (byte k = 0; k < 8; k++)
    for (byte i = 0; i < 8; i++)
      for (byte j = 0; j < 8; j++) {        
        dotArray[i][j][k] = boolean(round(random(1)));
      }
}

void mouseReleased() {
  clickFlag = false;
  leftClickFlag = false;
  turnFlag = false;
}

void mouseWheel(MouseEvent event) {
  float e = event.getCount();
  thisZ -= e;
  if (thisZ < 0) thisZ = 0;
  if (thisZ > 7) thisZ = 7;
}
