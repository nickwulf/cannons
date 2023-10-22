#pragma once

class Bullet {
public:
   Bullet(float pX=0, float pY=0, float vX=0, float vY=0, float dia=1, bool act=false);
   bool update();
   void draw();
   void setVelocity(float vX=0, float vY=0);
   void setPosition(float pX=0, float pY=0);
   void setDiameter(float dia);
   void setActive(bool act);
   float getPosX();
   float getPosY();
   float getVelX();
   float getVelY();
   float getDiameter();
   bool getActive();
   virtual ~Bullet(void);
private:
   void move(float accX=0, float accY=0);
   float posX;
   float posY;
   float velX;
   float velY;
   float diameter;
   bool active;
};
