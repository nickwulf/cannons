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

const int boundX = 700;
const int boundY = 475;


Bullet::Bullet(float pX, float pY, float vX, float vY, float dia, bool act) {
   posX = pX;
   posY = pY;
   velX = vX;
   velY = vY;
   diameter = dia;
   active = act;
}
void Bullet::move(float accX, float accY) {      // moves the bullet according to the applied forces
   velX += accX;
   velY += accY;
   posX += velX;
   posY += velY;
}
bool Bullet::update() {      // this method takes care of collisions
   bool done = false;
   if (active) {
      float accX=0, accY=0;
      if (posX<=-boundX) active = false;
      if (posX>=boundX) active = false;
      if (posY<=-boundY) active = false;
      accY = -.02;
      move(accX,accY);
      if (!active) done = true;
   }
   return done;
}
void Bullet::draw() {
   if (active) {
      glPushMatrix();
      glColor3f(0, 0, 0);
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
}
void Bullet::setVelocity(float vX, float vY) {
   velX = vX;
   velY = vY;
}
void Bullet::setPosition(float pX, float pY) {   
   posX = pX;
   posY = pY;
}
void Bullet::setDiameter(float dia) {
   diameter = dia;
}
void Bullet::setActive(bool act) {
   active = act;
}
float Bullet::getPosX() {
   return posX;
}
float Bullet::getPosY() {
   return posY;
}
float Bullet::getVelX() {
   return velX;
}
float Bullet::getVelY() {
   return velY;
}
float Bullet::getDiameter() {
   return diameter;
}
bool Bullet::getActive() {
   return active;
}
Bullet::~Bullet(void) {
}
