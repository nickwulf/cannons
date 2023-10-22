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

#define STAND_WIDTH 12.0
#define STAND_LENGTH 100.0
#define BASE_DIAMETER 14.0
#define BASE_WIDTH 8.0
#define BASE_LENGTH 20.0
#define ARM_DIAMETER 12.0
#define ARM_WIDTH 6.0
#define ARM_LENGTH 14.0
#define JOINT_DARK .7
#define DEG_TO_RAD .0174532925

Cannon::Cannon(float pXB, float pYB, float aA, float r, float g, float b) {
    posXBase = pXB;
   posYBase = pYB;
   angleArm = aA;
   omega = 0;
   power = .5;
   deltaPower = 0;
   red = r;
   green = g;
   blue = b;
   active = false;
   score = 0;
   specialScale = 1;
   specialRotate = 0;
   reBuild();
   rectStand = Rect(STAND_LENGTH, STAND_WIDTH, -STAND_LENGTH/2, 0, -90, posXBase, posYBase, red, green, blue);
   jointBase = Joint(BASE_DIAMETER, posXBase, posYBase, red*JOINT_DARK, green*JOINT_DARK, blue*JOINT_DARK);
   rectBase = Rect(BASE_LENGTH, BASE_WIDTH, -BASE_LENGTH/2, 0, angleBase, posXBase, posYBase, red, green, blue);
   jointArm = Joint(ARM_DIAMETER, posXArm, posYArm, red*JOINT_DARK, green*JOINT_DARK, blue*JOINT_DARK);
   rectArm = Rect(ARM_LENGTH, ARM_WIDTH, -ARM_LENGTH/2, 0, angleArm, posXArm, posYArm, red, green, blue);
}
Bullet Cannon::shoot() {
   Bullet bullet;
   if (active) {
      float bX = posXArm + (ARM_LENGTH+2.5)*cos(angleArm*DEG_TO_RAD);
      float bY = posYArm + (ARM_LENGTH+2.5)*sin(angleArm*DEG_TO_RAD);
      float vX = 6*power*cos(angleArm*DEG_TO_RAD);
      float vY = 6*power*sin(angleArm*DEG_TO_RAD);
      bullet = Bullet(bX, bY, vX, vY, 6, true);
      omega = 0;
      deltaPower = 0;
      active = false;
   }
   return bullet;
}
void Cannon::update(int KeyDown[256]) {
   if (active) {
      float alpha = 0;
      if (KeyDown['a'] && !KeyDown['d']) alpha = .002;
      if (KeyDown['d'] && !KeyDown['a']) alpha = -.002;
      omega += alpha;
      omega *= .998;
      if (alpha == 0 || (omega<0)^(alpha<0)) omega *= .85;
      setAngle(angleArm + omega);

      float movePower = 0;
      if (KeyDown['w'] && !KeyDown['s']) movePower = .00002;
      if (KeyDown['s'] && !KeyDown['w']) movePower = -.00002;
      deltaPower += movePower;
      deltaPower *= .998;
      if (movePower == 0 || (deltaPower<0)^(movePower<0)) deltaPower *= .85;
      setPower(power + deltaPower);
   }
}
bool Cannon::testCollision(float bX, float bY, float rad) {
   bool hit = false;
   if (rectStand.testCollision(bX, bY, rad)) hit = true;
   if (rectBase.testCollision(bX, bY, rad)) hit = true;
   if (rectArm.testCollision(bX, bY, rad)) hit = true;
   if (jointBase.testCollision(bX, bY, rad)) hit = true;
   if (jointArm.testCollision(bX, bY, rad)) hit = true;
   return hit;
}
void Cannon::draw() {
   rectStand.draw();
   rectBase.draw();
   rectArm.draw();   
   jointBase.draw();
   jointArm.draw();
}
void Cannon::setPosition(float pXB, float pYB) {
    posXBase = pXB;
   posYBase = pYB;
   reBuild();
}
void Cannon::setAngle(float aA) {
   angleArm = aA;
   if (angleArm>270) {
      angleArm = 270;
      omega = 0;
   }
   if (angleArm<-90) {
      angleArm = -90;
      omega = 0;
   }
   reBuild();
}
void Cannon::setPower(float p) {
   power = p;
   if (power>1) power = 1;
   if (power<.1) power = .1;
   reBuild();
}
void Cannon::reBuild() {
   rectStand.setPosition(posXBase, posYBase);
   jointBase.setPosition(posXBase, posYBase);
   angleBase = 45 + angleArm/2;
   float delta = angleBase - angleArm;
   angleBase = angleArm + 2*delta*(1-power);
   rectBase.setAngle(angleBase);
   rectBase.setPosition(posXBase, posYBase);
   posXArm = posXBase + BASE_LENGTH*cos(angleBase*DEG_TO_RAD);
   posYArm = posYBase + BASE_LENGTH*sin(angleBase*DEG_TO_RAD);
   jointArm.setPosition(posXArm, posYArm);
   rectArm.setAngle(angleArm);
   rectArm.setPosition(posXArm, posYArm);
}
void Cannon::setColor(float r, float g, float b) {
   red = r;
   green = g;
   blue = b;
   rectStand.setColor(red, green, blue);
   rectBase.setColor(red, green, blue);
   rectArm.setColor(red, green, blue);
   jointBase.setColor(red*JOINT_DARK, green*JOINT_DARK, blue*JOINT_DARK);
   jointArm.setColor(red*JOINT_DARK, green*JOINT_DARK, blue*JOINT_DARK);
}
void Cannon::setActive(bool act) {
   active = act;
}
void Cannon::setScore(int scr) {
   score = scr;
}
void Cannon::setSpecialScale(float s) {
   specialScale = s;
   rectArm.setDimensions(ARM_LENGTH*specialScale, ARM_WIDTH*specialScale);
   rectArm.setRotationPt(-ARM_LENGTH*specialScale/2, 0);
}
void Cannon::setSpecialRotate(float r) {
   specialRotate = r;
   rectArm.setAngle(angleArm + specialRotate);
}
float Cannon::getPosX() {
   return posXBase;
}
float Cannon::getPosY() {
   return posYBase;
}
float Cannon::getAngle() {
   return angleArm;
}
float Cannon::getPower() {
   return power;
}
bool Cannon::getActive() {
   return active;
}
int Cannon::getScore() {
   return score;
}
float Cannon::getSpecialScale() {
   return specialScale;
}
float Cannon::getSpecialRotate() {
   return specialRotate;
}
Cannon::~Cannon(void) {
}
