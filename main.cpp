#include <stdlib.h>
#include "TXLib.h"
#include <xmmintrin.h>
#include <immintrin.h>

const int SIZE_X 			= 800;
const int SIZE_Y 			= 800;
const __m256 scale			= {1./400, 1./400, 1./400, 1./400};
const size_t infiniteNum 		= 256;
const __m256 radius			= {4, 4, 4, 4};

int deltaX				= 0;
int dx					= 10;

int deltaY				= 0;
int dy					= 10;

const size_t mSize			= 8;
const float  eps			= 0.00001;

__m256 CountLungeNum (__m256 x, __m256 y);

inline bool mm_eq (__m256 a) {

	bool res = true;

	for (int i = 0; i < mSize; i++) {

		if ( a [i] > eps ) {

			res = false;
			break;

		} 

	}

	return res;

}

int main () {

	txCreateWindow (SIZE_X, SIZE_Y);

	txBegin ();

	RGBQUAD* videoMem = txVideoMemory ();

	while (!GetAsyncKeyState(VK_ESCAPE)) {

		if (GetAsyncKeyState (VK_RIGHT)) deltaX += dx;
		if (GetAsyncKeyState (VK_LEFT))  deltaX -= dx;
		if (GetAsyncKeyState (VK_UP)) 	 deltaY += dy;
		if (GetAsyncKeyState (VK_DOWN))  deltaY -= dy;

		for (int y = 1; y < SIZE_Y; y++) {

			for (int x = 1; x < SIZE_X; x += mSize) {	
				
				__m256 X = {};
				for (size_t i = 0; i < mSize; i++) X [i] = (float)x + (float)i - (float)SIZE_X / 2.0 + (float) deltaX;
				__m256 Y = {};
				for (size_t i = 0; i < mSize; i++) Y [i] = (float)SIZE_Y / 2.0 - (float)y + deltaY;

				__m256 numLunge = CountLungeNum (X, Y);
				for (int i = 0; i < mSize; i++) {

					RGBQUAD color = numLunge [i] < infiniteNum ? (RGBQUAD) {(BYTE)(255), (BYTE)(255), (BYTE)(255)} :
																 (RGBQUAD) {};

					if ((x + i) + (SIZE_Y - y) * SIZE_X >= 640000)
						break;

					if ((x + i) >= SIZE_X)
						break;	

					*(videoMem + (x + i) + (SIZE_Y - y) * SIZE_X) = color;											 	

				}

			}

		}

		printf ("%lg\n", txGetFPS ());
		txSleep ();

	}

	txEnd ();

	return 0;

}

__m256 CountLungeNum (__m256 x, __m256 y) {

	__m256 nextX = _mm256_mul_ps  (x, scale);
	__m256 nextY = _mm256_mul_ps  (y, scale);

	x = _mm256_mul_ps (x, scale);
	y = _mm256_mul_ps (y, scale);

	__m256 addit 		= {1, 1, 1, 1, 1, 1, 1, 1};
	__m256 toCountLunge = {0, 0, 0, 0, 0, 0, 0, 0}; 

	for (int j = 0; j < 256; j++) {

		__m256 x2    = _mm256_mul_ps   (nextX, nextX);
		__m256 y2    = _mm256_mul_ps   (nextY, nextY);
		__m256 xy    = _mm256_mul_ps   (nextX, nextY);
		__m256 rho   = _mm256_add_ps   (x2, y2);

		__m256 cmpr  = _mm256_cmp_ps   (rho, radius, _CMP_LE_OS);
		addit 	     = _mm256_and_ps   (addit, cmpr);

		toCountLunge = _mm256_add_ps   (addit, toCountLunge);

		if (mm_eq (addit)) 
			break;

		nextX = _mm256_add_ps (_mm256_add_ps (x, _mm256_sub_ps (x2, y2)), addit);
		nextY = _mm256_add_ps (_mm256_add_ps (_mm256_add_ps (xy, xy), y), addit);

	}

	return toCountLunge;

}
