#include ".\bullet.h"
#include ".\joint.h"
#include ".\rectangle.h"
#include ".\cannon.h"
#include ".\ground.h"
#include ".\text.h"
#using <mscorlib.dll>

#include "glut.h"
#include <stdlib.h>
#include <stdarg.h>
#include <iostream>
#include <cmath>
#include <memory.h>
#include <time.h>
#include <fstream>
using namespace std;

#define LOW_Y -350.0
#define DEG_TO_RAD .0174532925
#define RAD_TO_DEG 57.29577951

Ground::Ground(float pLX, float pLY, float pRX, float pRY) {
	posLeftX = pLX;
	posLeftY = pLY;
	posRightX = pRX;
	posRightY = pRY;
}
bool Ground::testCollision(float bX, float bY, float rad) {
	bool hit = false;
	// is it within the endpoints?
	if (pow(rad,2) > pow(posLeftX-bX,2)+pow(posLeftY-bY,2)) hit = true;
	if (pow(rad,2) > pow(posRightX-bX,2)+pow(posRightY-bY,2)) hit = true;
	// is its center within the rectangle/ground?
	float slope = (posRightY-posLeftY)/(posRightX-posLeftX);
	float yIntercept = posLeftY - slope*posLeftX;
	if (bX>=posLeftX && bX<=posRightX && bY<(slope*bX + yIntercept)) hit = true;
	// is the ball within the left & right boundaries perpendicular to the ground and close to the ground?
	if (slope == 0) {
		if ((bY<posLeftY+rad) && (bX>=posLeftX) && (bX<=posRightX)) hit = true;
	}
	else {
		float slopePerp = -1/slope;				// find slope and Y-intercept of line perpendicular to ground
		float yInterPerp = bY - slopePerp*bX;	// and that goes through the point (bX,bY)
		float meetX = (yInterPerp-yIntercept)/(slope-slopePerp);	// find where the point on the ground where
		float meetY = meetX*slope + yIntercept;						// the perpendicular line intersects it
		if (meetX>=posLeftX && meetX<=posRightX && pow(rad,2)>pow(meetX-bX,2)+pow(meetY-bY,2)) hit = true;
	}
	return hit;
}
void Ground::draw() {
	glPushMatrix();
	glBegin(GL_POLYGON);
		glColor3f(0, .6, 0);
		glVertex3f(posLeftX, posLeftY, 0);
		glVertex3f(posRightX, posRightY, 0);
		glColor3f(.5, .25, 0);
		glVertex3f(posRightX, LOW_Y, 0);
		glVertex3f(posLeftX, LOW_Y, 0);
	glEnd();
	glPopMatrix();
}
void Ground::setPositions(float pLX, float pLY, float pRX, float pRY) {
	posLeftX = pLX;
	posLeftY = pLY;
	posRightX = pRX;
	posRightY = pRY;
}
float Ground::getPosLeftX() {
	return posLeftX;
}
float Ground::getPosLeftY() {
	return posLeftY;
}
float Ground::getPosRightX() {
	return posRightX;
}
float Ground::getPosRightY() {
	return posRightY;
}
Ground::~Ground(void) {
}
