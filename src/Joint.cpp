#include "Bullet.h"
#include "Joint.h"
#include "Rect.h"
#include "Cannon.h"
#include "Ground.h"
#include "Text.h"

#include "GL/glut.h"
#include <stdlib.h>
#include <stdarg.h>

#include <iostream>
#include <cmath>
#include <memory.h>
#include <time.h>
#include <fstream>
using namespace std;

Joint::Joint(float dia, float x, float y, float r, float g, float b) {
   diameter = dia;
   posX = x;
   posY = y;
   red = r;
   green = g;
   blue = b;
}
bool Joint::testCollision(float bX, float bY, float rad) {
   bool hit = false;
   // is it within the radius of the joint + radius of bullet?
   if (pow(diameter/2+rad,2) > pow(posX-bX,2)+pow(posY-bY,2)) hit = true;
   return hit;
}
void Joint::draw() {
   glPushMatrix();
   glColor3f(red, green, blue);
   glTranslatef(posX, posY, 0);
   glScalef(diameter, diameter, 1);
   glBegin(GL_POLYGON);
      glVertex3f(0, .5, 0);
      glVertex3f(.3536, .3536, 0);
      glVertex3f(.5, 0, 0);
      glVertex3f(.3536, -.3536, 0);
      glVertex3f(0, -.5, 0);
      glVertex3f(-.3536, -.3536, 0);
      glVertex3f(-.5, 0, 0);
      glVertex3f(-.3536, .3536, 0);
   glEnd();
   glPopMatrix();
}
void Joint::setDiameter(float dia) {
   diameter = dia;
}
void Joint::setPosition(float x, float y) {
   posX = x;
   posY = y;
}
void Joint::setColor(float r, float g, float b) {
   red = r;
   green = g;
   blue = b;
}
float Joint::getDiameter() {
   return diameter;
}
float Joint::getPosX() {
   return posX;
}
float Joint::getPosY() {
   return posY;
}
Joint::~Joint(void) {
}
