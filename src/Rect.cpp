#include "bullet.h"
#include "joint.h"
#include "rect.h"
#include "cannon.h"
#include "ground.h"
#include "text.h"

#include "GL/glut.h"
#include <stdlib.h>
#include <stdarg.h>

#include <iostream>
#include <cmath>
#include <memory.h>
#include <time.h>
#include <fstream>
using namespace std;

#define DEG_TO_RAD .0174532925

Rect::Rect(float w, float h, float rX, float rY, float ang, float pX, float pY, float r, float g, float b) {
   width = w;
   height = h;
   rotX = rX;
   rotY = rY;
   angle = ang;
   posX = pX;
   posY = pY;
   red = r;
   green = g;
   blue = b;
}
bool Rect::testCollision(float bX, float bY, float rad) {
   float ptX[5];
   float ptY[5];
   float cosine = cos(angle*DEG_TO_RAD);
   float sine = sin(angle*DEG_TO_RAD);
   float centerX = posX - rotX*cosine + rotY*sine;
   float centerY = posY - rotX*sine - rotY*cosine;
   ptX[0] = centerX - .5*width*cosine + .5*height*sine;
   ptY[0] = centerY - .5*width*sine - .5*height*cosine;
   ptX[1] = centerX - .5*width*cosine - .5*height*sine;
   ptY[1] = centerY - .5*width*sine + .5*height*cosine;
   ptX[2] = centerX + .5*width*cosine - .5*height*sine;
   ptY[2] = centerY + .5*width*sine + .5*height*cosine;
   ptX[3] = centerX + .5*width*cosine + .5*height*sine;
   ptY[3] = centerY + .5*width*sine - .5*height*cosine;
   ptX[4] = ptX[0];
   ptY[4] = ptY[0];
   bool hit = false;
   for (int i=0; i<4; i++){
      // is the bullet near a corner?
        if (pow(rad,2) > pow(ptX[i]-bX,2)+pow(ptY[i]-bY,2)) hit = true;
      // is the bullet near a side?
      if (testSideClose(ptX[i], ptY[i], ptX[i+1], ptY[i+1], bX, bY, rad)) hit = true; 
   }
   bool inside = true;
   for (int i=0; i<4; i++) {
   // is the bullet inside the rectangle?
      if (!testSideInside(ptX[i], ptY[i], ptX[i+1], ptY[i+1], bX, bY, centerX, centerY)) inside = false; 
   }
   if (inside) hit = true;
   return hit;

}
bool Rect::testSideClose(float p1X, float p1Y, float p2X, float p2Y, float bX, float bY, float rad) {
   bool hit = false;
   if (p1X == p2X) {
      if (((bX<p1X+rad)&&(bX>p1X-rad)) && ((bY<=p1Y)^(bY<=p2Y))) hit = true;
   }
   else if (p1Y == p2Y) {
      if (((bY<p1Y+rad)&&(bY>p1Y-rad)) && ((bX<=p1X)^(bX<=p2X))) hit = true;
   }
   else {
      float slope = (p2Y-p1Y)/(p2X-p1X);
      float yIntercept = p1Y - slope*p1X;
      float slopePerp = -1/slope;            // find slope and Y-intercept of line perpendicular to ground
      float yInterPerp = bY - slopePerp*bX;   // and that goes through the point (bX,bY)
      float meetX = (yInterPerp-yIntercept)/(slope-slopePerp);   // find where the point on the ground where
      float meetY = meetX*slope + yIntercept;                  // the perpendicular line intersects it
      if (((meetX<=p1X)^(meetX<=p2X)) && (pow(rad,2)>pow(meetX-bX,2)+pow(meetY-bY,2))) hit = true;
   }
   return hit;
}
bool Rect::testSideInside(float p1X, float p1Y, float p2X, float p2Y, float testX, float testY, float cenX, float cenY) {
   bool sameSide = false;
   if (p1X == p2X) {
      if ((testX<p1X) ^ !(cenX<p1X)) sameSide = true;
   }
   else {
      float slope = (p2Y-p1Y)/(p2X-p1X);
      float yIntercept = p1Y - slope*p1X;
      if ((testY<(slope*testX + yIntercept)) ^ !(cenY<(slope*cenX + yIntercept))) sameSide = true;
   }
   return sameSide;
}
void Rect::draw() {
   glPushMatrix();
   glColor3f(red, green, blue);
   glTranslatef(posX, posY, 0);
   glRotatef(angle, 0, 0, 1);
   glTranslatef(-rotX, -rotY, 0);
   glScalef(width, height, 1);
   glBegin(GL_POLYGON);
      glVertex3f(.5, .5, 0);
      glVertex3f(.5, -.5, 0);
      glVertex3f(-.5, -.5, 0);
      glVertex3f(-.5, .5, 0);
   glEnd();
   glPopMatrix();
}
void Rect::setDimensions(float w, float h) {
   width = w;
   height = h;
}
void Rect::setRotationPt(float rX, float rY) {
   rotX = rX;
   rotY = rY;
}
void Rect::setAngle(float ang) {
   angle = ang;
   if (angle>360) angle -= 360;
   if (angle<-360) angle += 360;
}
void Rect::setPosition(float pX, float pY) {
   posX = pX;
   posY = pY;
}
void Rect::setColor(float r, float g, float b) {
   red = r;
   green = g;
   blue = b;
}
float Rect::getWidth() {
   return width;
}
float Rect::getHeight() {
   return height;
}
float Rect::getRotX() {
   return rotX;
}
float Rect::getRotY() {
   return rotY;
}
float Rect::getAngle() {
   return angle;
}
float Rect::getPosX() {
   return posX;
}
float Rect::getPosY() {
   return posY;
}
Rect::~Rect(void) {
}
