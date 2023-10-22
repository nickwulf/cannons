#pragma once

class Joint {
public:
   Joint(float dia=.1, float x=0, float y=0, float r=1, float g=1, float b=1);
   bool testCollision(float bX, float bY, float rad);
   void draw();
   void setDiameter(float dia);
   void setPosition(float x, float y);
   void setColor(float r, float g, float b);
   float getDiameter();
   float getPosX();
   float getPosY();
    virtual ~Joint(void);
private:
   float diameter;
   float posX;
   float posY;
   float red;
   float green;
   float blue;
};
