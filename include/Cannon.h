#pragma once

class Cannon {
public:
	Cannon(float pXB=0, float pYB=0, float aA=0, float r=1, float g=1, float b=1);
	Bullet shoot();
	void update(int KeyDown[256]);
	bool testCollision(float bX, float bY, float rad);
	void draw();
	void setPosition(float pXB, float pYB);
	void setAngle(float aA);
	void setPower(float p);
	void reBuild();
	void setColor(float r, float g, float b);
	void setActive(bool act);
	void setScore(int scr);
	void setSpecialScale(float s);
	void setSpecialRotate(float r);
	float getPosX();
	float getPosY();
	float getAngle();
	float getPower();
	bool getActive();
	int getScore();
	float getSpecialScale();
	float getSpecialRotate();
	virtual ~Cannon(void);
private:
	Rect rectStand;
	Joint jointBase;
	Rect rectBase;
	Joint jointArm;
	Rect rectArm;
	float posXBase;
	float posYBase;
	float angleBase;
	float posXArm;
	float posYArm;
	float angleArm;
	float omega;
	float power;
	float deltaPower;
	float red;
	float green;
	float blue;
	bool active;
	int score;
	float specialScale;
	float specialRotate;
};
