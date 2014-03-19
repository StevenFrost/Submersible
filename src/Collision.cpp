#include "Collision.h"
#include <algorithm>

using std::max;
using std::min;

bool Collision::boundingBox(DisplayableObject *a, DisplayableObject *b) {
	/* Get the object rectangles */
	SDL_Rect aRect, bRect;
	a->GetRedrawRect(&aRect);
	b->GetRedrawRect(&bRect);

	/* Calculate their right, left and bottom positions */
	int aRight  = aRect.x + aRect.w;
	int aLeft   = aRect.x;
	int aBottom = aRect.y + aRect.h;
	int bRight  = bRect.x + bRect.w;
	int bLeft   = bRect.x;
	int bBottom = bRect.y + bRect.h;

	/* Check if any positions overlap */
	if ((aLeft <= bRight && aRight >= bLeft) || (aLeft >= bRight && aRight <= bLeft)) {
		if (aRect.y <= bBottom && aBottom >= bRect.y) {
			return true;
		}
	}

	return false;
}

bool Collision::surface(int resolution, SDL_Surface *a, SDL_Surface *b, int aX, int aY, int bX, int bY) {
	/* Calculate the intersection of the two surfaces */
	int left	= max(aX, bX);
	int top		= max(aY, bY);
	int right	= min(aX + a->w, bX + b->w);
	int bottom	= min(aY + a->h, bY + b->h);

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