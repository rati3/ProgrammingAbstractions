#include <iostream>
#include <cmath>
#include "gwindow.h"
#include "gtypes.h"
#include "simpio.h"
using namespace std;

const double theta = 60;


/* Method gets order of Sierpinski triangle from user */
int getOrder();

/* Method gets length of the largest triangle */
double getLength();

/* Method returns left point of center triangle */
GPoint findMidTriangle(double length, GWindow gw);

/* Method draws sierpinski triangle with left point & length */
void drawSierpinski(int order, double length, GPoint &point, GWindow gw);

/* Method draws triangle by its point & length and after that point will be top of triangle */
void drawTriangle(GPoint &point, double length, GWindow gw);


int main() {	
	GWindow gw;
	int order = getOrder();
	double length = getLength();

	GPoint point = findMidTriangle(length,gw); // left point of mid triangle
	drawSierpinski(order, length, point, gw);

    return 0;
}

int getOrder() {
	int order;
	while (true) {
		order = getInteger("order of Sierpinski triangle: ");
		if (order >= 0)
			return order;
	}
}

double getLength() {
	double length;
	while (true) {
		length = getReal("length of triangle: ");
		if (length > 0)
			return length;
	}
}

GPoint findMidTriangle(double length, GWindow gw) {
	double centerX = gw.getWidth()  / 2;
	double centerY = gw.getHeight() / 2;
	GPoint point(centerX - length / 2, centerY + length / (2 * sqrt(3.0))); // left point of triangle
	return point;
}

void drawSierpinski(int order, double length, GPoint &point, GWindow gw) { 
	if (order == 0) {                   // Base Case 
		drawTriangle(point,length,gw); // draw Triangle
		return;
	}
	// if order > 0  find mid point of n triangle's base
	GPoint newPoint(point.getX() + length / 2, point.getY());

	drawSierpinski(order - 1,length / 2,newPoint,gw);  // draw right n-1 triangle 

	drawSierpinski(order - 1,length / 2,point,gw);   // draw left n-1 
	// now point is mid point of left side of n sierpinski triangle
	drawSierpinski(order - 1,length / 2,point,gw); // draw top n-1 
	
}

void drawTriangle(GPoint &point, double length, GWindow gw) {
	gw.drawPolarLine(point,length,theta);                  // left side
	point = gw.drawPolarLine(point,length,0);             // base
	point = gw.drawPolarLine(point,length,180 - theta);  // right side
}

