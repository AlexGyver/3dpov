import processing.core.*; 
import processing.data.*; 
import processing.event.*; 
import processing.opengl.*; 

import java.util.HashMap; 
import java.util.ArrayList; 
import java.io.File; 
import java.io.BufferedReader; 
import java.io.PrintWriter; 
import java.io.InputStream; 
import java.io.OutputStream; 
import java.io.IOException; 

public class Perspective extends PApplet {

int thisZ = 0;
int rotX = 20, prevRotX;
boolean clickFlag, turnFlag, leftClickFlag;
boolean[][][] dotArray = new boolean[8][8][8];
PFont Font1;

public void setup() {
  
  textAlign(CENTER, CENTER);
  Font1 = createFont("Arial Bold", 18);
  textFont(Font1);
  fillArray();
}

public void draw() {  
  background(0);
  circles();
  buttons();
  mouseTick();
  mouseDebug();

  drawBalls();
}

public void buttons() {
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
  
  text("Layer: ", 50, 50);
  text(thisZ, 90, 50);
}

public void mouseDebug() {
  fill(255, 0, 0);
  textSize(16);
  text(mouseX, 580, 20);
  text(mouseY, 620, 20);
}

public void circles() {
  stroke(255);
  for (byte x = 0; x < 8; x++) {
    for (byte y = 0; y < 8; y++) {
      if (dotArray[x][y][thisZ]) fill(255);
      else fill(0);
      circle(x*25+50, y*25+90, 20);
    }
  }
}

public boolean overRect(int x, int y, int width, int height) {
  if (mouseX >= x && mouseX <= x+width && 
    mouseY >= y && mouseY <= y+height) {
    return true;
  } else {
    return false;
  }
}

public boolean overCircle(int x, int y, int diameter) {
  float disX = x - mouseX;
  float disY = y - mouseY;
  if (sqrt(sq(disX) + sq(disY)) < diameter/2 ) {
    return true;
  } else {
    return false;
  }
}

public void clearCube() {
  for (byte k = 0; k < 8; k++)
    for (byte i = 0; i < 8; i++)
      for (byte j = 0; j < 8; j++) {        
        dotArray[i][j][k] = false;
      }
}

public void saveBitmap() {
}


public void mouseTick() {
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

public void drawBalls() {
  noStroke();
  fill(255);
  lights();

  translate(640/2+150, 360/2, -25*4);
  rotateY(rotX * 0.017f);
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

public void mouseDragged() {
  if (turnFlag) {
    if (clickFlag) {
      clickFlag = false;
      prevRotX = mouseX;
    }
    rotX -= prevRotX - mouseX;
    prevRotX = mouseX;
  }
}

public void fillArray() {
  for (byte k = 0; k < 8; k++)
    for (byte i = 0; i < 8; i++)
      for (byte j = 0; j < 8; j++) {        
        dotArray[i][j][k] = PApplet.parseBoolean(round(random(1)));
      }
}

public void mouseReleased() {
  clickFlag = false;
  leftClickFlag = false;
  turnFlag = false;
}

public void mouseWheel(MouseEvent event) {
  float e = event.getCount();
  thisZ -= e;
  if (thisZ < 0) thisZ = 0;
  if (thisZ > 7) thisZ = 7;
}
  public void settings() {  size(640, 360, P3D); }
  static public void main(String[] passedArgs) {
    String[] appletArgs = new String[] { "Perspective" };
    if (passedArgs != null) {
      PApplet.main(concat(appletArgs, passedArgs));
    } else {
      PApplet.main(appletArgs);
    }
  }
}
