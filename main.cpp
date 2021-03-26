#include <stdlib.h>
#include <math.h>
#include "TXLib.h"

const int SIZE_X 			= 1000;
const int SIZE_Y 			= 1000;

const double scale			= 0.01;

const size_t infiniteNum 	= 256;

const double radius			= 2;

size_t CountLungeNum (double x, double y);

int main () {


	txCreateWindow (SIZE_X, SIZE_Y);

	txLock ();

	RGBQUAD* videoMem = txVideoMemory ();
	
	for (int y = 1; y < SIZE_Y; y++) {

		for (int x = 1; x < SIZE_X; x++) {

			size_t numLunge = CountLungeNum ((double)x - (double)SIZE_X / 2.0, (-1.0) * ( (double)y - (double)SIZE_Y / 2.0 ));

			RGBQUAD color = numLunge == infiniteNum ? 	(RGBQUAD) {(BYTE)(0),(BYTE)(0),(BYTE)(0)} : 
														(RGBQUAD) {(BYTE)(255),(BYTE)(255),(BYTE)(255)};
			*(videoMem + x + (SIZE_Y - y) * SIZE_X) = color;

		}

	}

	txUnlock ();

	return 0;

}

size_t CountLungeNum (double x, double y) {

	size_t toCountLunge = 0;

	double nextX = x * scale;
	double nextY = x * scale;

	const double x_0   = x * scale;
	const double y_0   = y * scale;
 
	double tempX = 0;

	while (toCountLunge != infiniteNum) {

		double rho = nextX * nextX + nextY * nextY;

		if ( rho > radius * radius ) {

			return toCountLunge;

		}

		tempX = nextX * nextX - nextY * nextY + x_0;
		nextY = 2. * nextX * nextY + y_0;
		nextX = tempX;

		toCountLunge++;

	}

	return toCountLunge;

}
