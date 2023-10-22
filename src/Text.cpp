#include ".\bullet.h"
#include ".\joint.h"
#include ".\rect.h"
#include ".\cannon.h"
#include ".\ground.h"
#include ".\text.h"
// #using <mscorlib.dll>

#include "GL\glut.h"
#include <stdlib.h>
#include <stdarg.h>
#include <iostream>
#include <cmath>
#include <memory.h>
#include <time.h>
#include <fstream>
using namespace std;

#define TIMER_STALL 30.0
#define TIMER_MOVE 75.0
#define TIMER_KILL 105.0

Text::Text(string t, float pX, float pY) {
   posX = pX;
   posY = pY;
   text = t;
   timer = 0;
}
void Text::update() {
   if (timer <= TIMER_KILL) {
      timer++;
      float percent = 0;
      float newY = posY;
      if (timer <= TIMER_STALL) {
         percent = (TIMER_STALL-timer)/(TIMER_STALL);
         newY = posY - 40*pow(percent,2);
      }
      else if (timer >= TIMER_MOVE) {
         percent = (timer-TIMER_MOVE)/(TIMER_KILL-TIMER_MOVE);
         newY = posY + 40*pow(percent,2);
      }
      glColor3f(.75*percent, percent, percent);
      glRasterPos2f(posX, newY);
      void *font = GLUT_BITMAP_HELVETICA_12;
      for (int i=0; i<(signed)text.length(); i++)
         glutBitmapCharacter(font, text[i]);
   }
}
void Text::reset() {
   timer = 0;
}
void Text::setPosition(float pX, float pY) {
   posX = pX;
   posY = pY;
}
void Text::setText(string t) {
   text = t;
}
float Text::getPosX() {
   return posX;
}
float Text::getPosY() {
   return posY;
}
string Text::getText() {
   return text;
}
int Text::getTimer() {
   return timer;
}
Text::~Text(void) {
}
