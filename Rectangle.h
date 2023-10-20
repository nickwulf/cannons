#pragma once

class Rectangle {
public:
	Rectangle(float w=.1, float h=.1, float rX=0, float rY=0, float ang=0, float pX=0, float pY=0, float r=1, float g=1, float b=1);
	bool testCollision(float bX, float bY, float rad);
	void draw();
	void setDimensions(float w, float h);
	void setRotationPt(float rX, float rY);
	void setAngle(float ang);
	void setPosition(float pX, float pY);
	void setColor(float r, float g, float b);
	float getWidth();
	float getHeight();
	float getRotX();
	float getRotY();
	float getAngle();
	float getPosX();
	float getPosY();
	virtual ~Rectangle(void);
private:
	bool testSideClose(float p1X, float p1Y, float p2X, float p2Y, float bX, float bY, float rad);
	bool testSideInside(float p1X, float p1Y, float p2X, float p2Y, float testX, float testY, float cenX, float cenY);
	float width;
	float height;
	float rotX;
	float rotY;
	float angle;
	float posX;
	float posY;
	float red;
	float green;
	float blue;
};
