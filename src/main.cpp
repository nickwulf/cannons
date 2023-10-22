#include <iostream>
#include "GL/glut.h"
#include <stdlib.h>
#include <stdarg.h>
#include <cstdlib.>
#include <sstream>
#include <cmath>
#include <memory.h>
#include <time.h>
#include <fstream>

#include "Bullet.h"
#include "Joint.h"
#include "Rect.h"
#include "Cannon.h"
#include "Ground.h"
#include "Text.h"

using namespace std;

// Definitions
#define DEG_TO_RAD 0.0174532925

// Default Image dimensions
const int imageWidth = 1200;
const int imageHeight = 750;

// Other parameters
const int groundCnt = 20;

// Game Variables
bool specialMode = false;  // the extra mode included to satisfy the class requirements
float specialTranslate = 150;
float specialScale = 1;
float specialRotate = 0;
bool turnState;  // false = player1, true = player2
int clockThen = clock();  // used to determine how long a second is
int frameCount = 0;  // used to determine how many frames have been in the last second
int frameRateTarget = 60;
float frameRateTargetMs = 1.0 * CLOCKS_PER_SEC / frameRateTarget;
float frameRateCredit = 0;
int frameRateClock = clock();
int frameRateCalc = 0;
Bullet bullet;
Text text;
Cannon cannons[3];
Ground grounds[groundCnt];

// Keyboard Array
int KeyDown[256];

//OpenGL Calls
void setupGL();
void CheckGLError();

//Callbacks
void Display();
void Idle();
void Timer(int t);
void Reshape(int w, int h);
void Keyboard (unsigned char key, int , int );
void KeyboardUp (unsigned char key, int , int );
void MouseButton(int button, int state, int x, int y);
void MouseMotion(int x, int y);

//Other Methods
void setupGame(bool first);
void testCollisions();
void displayHUD();
void doSpecial();
void drawString(float x, float y, void *font, string s, float r, float g, float b);
string intToString(int i);
string floatToString(float f, int decPlace);

// a cleanup function
void quit(int i = 0);

int main(int argc, char **argv) {

   // INITIALIZE THE GLUT WINDOW
   memset(KeyDown,0,sizeof(KeyDown));
   srand((unsigned)time(0));
   glutInit(&argc, argv);  
   glutInitWindowSize(imageWidth, imageHeight);
   glutInitDisplayString("rgb double");
   glutInitWindowPosition(0, 0);
   glutCreateWindow("Cannon Game");

   //SETUP MISC GL
   setupGL();

   //SETUP GLUT CALLBACKS
   cout << "Setting up callbacks... ";
   glutDisplayFunc(Display);
   glutKeyboardFunc(Keyboard);
   glutKeyboardUpFunc(KeyboardUp);
   glutMouseFunc(MouseButton);
   glutMotionFunc(MouseMotion);
   glutReshapeFunc(Reshape);
   // glutIdleFunc(Idle);
   cout << "[completed]\n";
   glDisable(GL_DEPTH_TEST);
   //glEnable(GL_DEPTH_TEST);
   CheckGLError();

   //SETUP INITIAL GAME PARAMETERS
   setupGame(true);

   //MAIN PROGRAM LOOP
   glutTimerFunc(frameRateTargetMs, Timer, 0);
   glutMainLoop();

}

// This function checks the state of openGL and prints 
// an error if an error has occurred
void CheckGLError() {
     GLenum error;
     error = glGetError();
     if (error!=GL_NO_ERROR)   {
      cout << "OpenGL reports an error: "<< gluErrorString(error) << endl;
      quit(1);
     }
}

// This function draws the scene
void Display() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glLoadIdentity();

   if (!specialMode) {
      if (KeyDown[32]) {
         if (cannons[turnState].getActive()) bullet = cannons[turnState].shoot();
         KeyDown[32] = 0;
      }

      cannons[0].update(KeyDown);
      cannons[1].update(KeyDown);
      if (bullet.update()) {
         turnState = !turnState;
         cannons[turnState].setActive(true);
         text.setText("Your Turn!");
         text.setPosition(cannons[turnState].getPosX()-31, cannons[turnState].getPosY()+45);
         text.reset();
      }
      testCollisions();
   }

   text.update();
   cannons[0].draw();
   cannons[1].draw();
   for (int i=0; i<20; i++) {
      grounds[i].draw();
   }
   bullet.draw();
   displayHUD();

   if (specialMode) doSpecial();

   glutSwapBuffers();
}

void setupGame(bool first) {
   int halfWidth = imageWidth/2 + 100;
   float groundWidth = halfWidth*2 / groundCnt;
   int tempLeft = -200 + rand()%250;
   float posX1 = -imageWidth/2 + 50 + rand()%150;
   float posX2 = imageWidth/2 - 50 - rand()%150;
   int index1, index2;
   for (int g=0; g<groundCnt; g++) {
      int tempRight = -200 + rand()%250;
      float posLeft = -halfWidth + groundWidth*g;
      float posRight = posLeft + groundWidth;
      if (posLeft <= posX1 && posX1 < posRight) {index1 = g;};
      if (posLeft <= posX2 && posX2 < posRight) {index2 = g;};
      grounds[g].setPositions(posLeft, tempLeft, posRight, tempRight);
      tempLeft = tempRight;
   }

   // float posX = -375 + rand()%150;
   float pLX = grounds[index1].getPosLeftX();
   float pLY = grounds[index1].getPosLeftY();
   float pRX = grounds[index1].getPosRightX();
   float pRY = grounds[index1].getPosRightY();
   float slope = (pRY-pLY)/(pRX-pLX);
   float yIntercept = pLY - slope*pLX;
   float posY = slope*posX1 + yIntercept;
   if (first) {
      cannons[0] = Cannon(posX1, posY+30, 45, 1, 0, 0);
        cannons[0].setActive(true);
      turnState = false;
      text.setText("Your Turn!");
      text.setPosition(posX1-31, posY+75);
      text.reset();
   }
   else {
      cannons[0].setPosition(posX1, posY+30);
      cannons[0].setAngle(45);
      cannons[0].setPower(.5);
   }

   pLX = grounds[index2].getPosLeftX();
   pLY = grounds[index2].getPosLeftY();
   pRX = grounds[index2].getPosRightX();
   pRY = grounds[index2].getPosRightY();
   slope = (pRY-pLY)/(pRX-pLX);
   yIntercept = pLY - slope*pLX;
   posY = slope*posX2 + yIntercept;
   if (first) {
      cannons[1] = Cannon(posX2, posY+30, 135, 0, 0, 1);
      cannons[2] = Cannon(0, 0, 45, .5, .5, .5);
   }
   else {
      cannons[1].setPosition(posX2, posY+30);
      cannons[1].setAngle(135);
      cannons[1].setPower(.5);
   }
}

void testCollisions() {
   bool test = false;
   if (bullet.getActive()) {
      for (int i=0; i<2; i++) {
         if (cannons[i].testCollision(bullet.getPosX(),bullet.getPosY(),bullet.getDiameter()*.5)) {
            test = true;
            cannons[1-i].setScore(cannons[1-i].getScore() + 1);
         }
      }
      if (test) {
         bullet.setActive(false);
         turnState = !turnState;
         cannons[turnState].setActive(true);
         setupGame(false);
         text.setText("Your Turn!");
         text.setPosition(cannons[turnState].getPosX()-31, cannons[turnState].getPosY()+45);
         text.reset();
      }
      else {
         for (int i=0; i<20; i++) {
            if (grounds[i].testCollision(bullet.getPosX(),bullet.getPosY(),bullet.getDiameter()*.5)) test = true;
         }
         if (test) {
            bullet.setActive(false);
            turnState = !turnState;
            cannons[turnState].setActive(true);
            text.setText("Your Turn!");
            text.setPosition(cannons[turnState].getPosX()-31, cannons[turnState].getPosY()+45);
            text.reset();
         }
      }
   }
}

void displayHUD() {
   string text;
   int textLeftX = -imageWidth/2 + 50;
   text = "Player 1";
   drawString(textLeftX, 270, GLUT_BITMAP_HELVETICA_18, text, 0, 0, 0);
   text = "Power: " + floatToString(cannons[0].getPower()*100, 1);
   drawString(textLeftX, 245, GLUT_BITMAP_HELVETICA_12, text, 0, 0, 0);
   text = "Angle: " + floatToString(cannons[0].getAngle(), 1);
   drawString(textLeftX, 225, GLUT_BITMAP_HELVETICA_12, text, 0, 0, 0);
   text = "Score: " + floatToString(cannons[0].getScore(), 1);
   drawString(textLeftX, 200, GLUT_BITMAP_HELVETICA_12, text, 0, 0, 0);

   int textRightX = imageWidth/2 -130;
   text = "Player 2";
   drawString(textRightX, 270, GLUT_BITMAP_HELVETICA_18, text, 0, 0, 0);
   text = "Power: " + floatToString(cannons[1].getPower()*100, 1);
   drawString(textRightX, 245, GLUT_BITMAP_HELVETICA_12, text, 0, 0, 0);
   text = "Angle: " + floatToString(180 - cannons[1].getAngle(), 1);
   drawString(textRightX, 225, GLUT_BITMAP_HELVETICA_12, text, 0, 0, 0);
   text = "Score: " + floatToString(cannons[1].getScore(), 1);
   drawString(textRightX, 200, GLUT_BITMAP_HELVETICA_12, text, 0, 0, 0);
   
   frameCount++;
   int clockNow = clock();
   int clockCount = clockNow - clockThen;
   if ((clockCount>=1000) || (clockCount<=-1)) {
      frameRateCalc = (int)(1000*frameCount/clockCount);
      frameCount = 0;
      clockThen = clockNow;
   }
   text = "FPS: " + intToString(frameRateCalc);
   drawString(textLeftX, -260, GLUT_BITMAP_HELVETICA_12, text, 0, 0, 0);

   if (specialMode) {
      text = "~PAUSE~";
      drawString(-60, 250, GLUT_BITMAP_HELVETICA_18, text, 0, 0, 0);
      text = "Turn Counterclockwise: A";
      drawString(-60, 225, GLUT_BITMAP_HELVETICA_12, text, 0, 0, 0);
      text = "Turn Clockwise: D";
      drawString(-60, 205, GLUT_BITMAP_HELVETICA_12, text, 0, 0, 0);
      text = "Increase Power: W";
      drawString(-60, 185, GLUT_BITMAP_HELVETICA_12, text, 0, 0, 0);
      text = "Decrease Power: S";
      drawString(-60, 165, GLUT_BITMAP_HELVETICA_12, text, 0, 0, 0);
      text = "Shoot: Spacebar";
      drawString(-60, 145, GLUT_BITMAP_HELVETICA_12, text, 0, 0, 0);
      text = "Pause: Right Mouse";
      drawString(-60, 125, GLUT_BITMAP_HELVETICA_12, text, 0, 0, 0);
      text = "~SPECIAL PAUSE COMMANDS~";
      drawString(-60, 95, GLUT_BITMAP_HELVETICA_12, text, 0, 0, 0);
      text = "Translate Object: 1";
      drawString(-60, 80, GLUT_BITMAP_HELVETICA_10, text, 0, 0, 0);
      text = "Rotate Object: 2";
      drawString(-60, 65, GLUT_BITMAP_HELVETICA_10, text, 0, 0, 0);
      text = "Scale Object: 3";
      drawString(-60, 50, GLUT_BITMAP_HELVETICA_10, text, 0, 0, 0);
      text = "Rotate Part: 4";
      drawString(-60, 35, GLUT_BITMAP_HELVETICA_10, text, 0, 0, 0);
      text = "Scale Part: 5";
      drawString(-60, 20, GLUT_BITMAP_HELVETICA_10, text, 0, 0, 0);
   }
}

void doSpecial() {
   if (KeyDown['1']) {
      specialTranslate += 4;
      if (specialTranslate > 500) specialTranslate = -500;
   }
   if (KeyDown['2']) {
      specialRotate += 5;
      if (specialRotate > 360) specialRotate -= 360;
   }
   if (KeyDown['3']) {
      specialScale += .01;
      if (specialScale > 2) specialScale = .5;
   }
   if (KeyDown['4']) {
      cannons[2].setSpecialRotate(cannons[2].getSpecialRotate() + 5);
      if (cannons[2].getSpecialRotate() > 360) cannons[2].setSpecialRotate(cannons[2].getSpecialRotate() - 360);
   }
   if (KeyDown['5']) {
      cannons[2].setSpecialScale(cannons[2].getSpecialScale() + .01);
      if (cannons[2].getSpecialScale() > 2) cannons[2].setSpecialScale(.5);
   }
   glPushMatrix();
   glTranslatef(-150, specialTranslate, 0);
   glScalef(specialScale, specialScale, 0);
   glRotatef(specialRotate, 0, 0, 1);
   cannons[2].draw();
   glPopMatrix();
}
// set up GL stuff
void setupGL() {

   // set the clear color
   glClearColor(0.75, 1.0, 1.0, 0.0);  // background color

   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   gluOrtho2D(-imageWidth/2, imageWidth/2, -imageHeight/2, imageHeight/2);
   glMatrixMode(GL_MODELVIEW);
}

// // This function is continuously called when events are not being received
// // by the window.  This is a good place to update the state of your objects 
// // after every frame.
void Idle() {
   glutPostRedisplay();
}
void Timer(int t) {
   // Update display
   glutPostRedisplay();

   // Reset timer
   int clockNew = clock();
   frameRateCredit += (clockNew - frameRateClock) * 1000.0 / CLOCKS_PER_SEC;
   frameRateCredit -= frameRateTargetMs;
   frameRateClock = clockNew;
   int delayMs = (int) round(frameRateTarget - frameRateCredit);
   delayMs = min(int (10*frameRateTargetMs), max(0, delayMs));
   glutTimerFunc(delayMs, Timer, 0);
}

// keyboard handler
void Keyboard (unsigned char key, int , int ) {
   KeyDown[key]=1;
}

void KeyboardUp (unsigned char key, int , int ) {
   KeyDown[key]=0;
}

void MouseButton(int button, int state, int x, int y) {
   if ((button==GLUT_RIGHT_BUTTON) && (state==GLUT_DOWN)) specialMode = !specialMode;
   if (specialMode) {
      specialTranslate = 150;
      specialScale = 1;
      specialRotate = 0;
      cannons[2].setSpecialScale(1);
      cannons[2].setSpecialRotate(0);
   }
}

void MouseMotion(int x, int y) {
}

// This functions handles what happens when the window is reshaped
void Reshape(int w, int h) {
}

// a cleanup function.  call this when you want to exit.
void quit(int i) {   
}

void drawString(float x, float y, void *font, string s, float r, float g, float b) {
   glColor3f(r, g, b);
   glRasterPos2f(x, y);
   for (int i=0; i<(signed)s.length(); i++)
      glutBitmapCharacter(font, s[i]);
}

string intToString(int i) {
   stringstream s;
   s << i;
   return s.str();
}

string floatToString(float f, int decNum) {
   stringstream s;
   string final;
   s << f;
   final = s.str();
   int position = final.find('.');
   if (position != string::npos) {
       final = final.substr(0, position+1+decNum);
   }
   return final;
}