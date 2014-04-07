#include "Collision.h"
#include <algorithm>

using std::max;
using std::min;

bool Collision::boundingBox(DisplayableObject *a, DisplayableObject *b) {
	/* Get the object rectangles */
	SDL_Rect aRect, bRect;
	a->GetRedrawRect(&aRect);
	b->GetRedrawRect(&bRect);

	return boundingBox(aRect.x, aRect.y, aRect.w, aRect.h, bRect.x, bRect.y, bRect.w, bRect.h);
}

bool Collision::boundingBox(int aX, int aY, int aW, int aH, int bX, int bY, int bW, int bH) {
	int aRight  = aX + aW;
	int aBottom = aY + aH;
	int bRight  = bX + bW;
	int bBottom = bY + bH;

	return ((aX <= bRight && aRight >= bX) || (aX >= bRight && aRight <= bX)) && (aY <= bBottom && aBottom >= bY);
}

bool Collision::surface(int resolution, SDL_Surface *a, SDL_Surface *b, int aX, int aY, int bX, int bY) {
	return surface(resolution, a, b, aX, aY, a->w, bX, bY, b->w);
}

bool Collision::surface(int resolution, SDL_Surface *a, SDL_Surface *b, int aX, int aY, int aW, int bX, int bY, int bW) {
	/* Calculate the intersection of the two surfaces */
	int left   = max(aX, bX);
	int top    = max(aY, bY);
	int right  = min(aX + aW, bX + bW);
	int bottom = min(aY + a->h, bY + b->h);

	for (int x = left; x < right; x += resolution) {
		for (int y = top; y < bottom; y += resolution) {
			/* Calculate the array index of the pixel */
			int aIndex = (x - aX) + (y - aY) * a->pitch / sizeof(unsigned int);
			int bIndex = (x - bX) + (y - bY) * b->pitch / sizeof(unsigned int);

			/* Get the pixel alpha data */
			unsigned int aPixel = (static_cast<unsigned int *>(a->pixels)[aIndex]) & 0xFF000000;
			unsigned int bPixel = (static_cast<unsigned int *>(b->pixels)[bIndex]) & 0xFF000000;

			/* If both alphas are non-zero, we have a collision */
			if (aPixel != 0x00000000 && bPixel != 0x00000000) return true;
		}
	}

	return false;
}