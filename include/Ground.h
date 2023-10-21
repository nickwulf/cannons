#pragma once

class Ground {
public:
	Ground(float pLX=0, float pLY=0, float pRX=0, float pRY=0);
	bool testCollision(float bX, float bY, float rad);
	void draw();
	void setPositions(float pLX, float pLY, float pRX, float pRY);
	float getPosLeftX();
	float getPosLeftY();
	float getPosRightX();
	float getPosRightY();
	virtual ~Ground(void);
private:
	float posLeftX;
	float posLeftY;
	float posRightX;
	float posRightY;
};
