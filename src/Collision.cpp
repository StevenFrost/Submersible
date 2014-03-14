#include "Collision.h"

bool Collision::boundingBox(DisplayableObject *a, DisplayableObject *b) {
	static unsigned int count = 0;

	SDL_Rect aRect, bRect;
	
	a->GetRedrawRect(&aRect);
	b->GetRedrawRect(&bRect);

	int aRight  = aRect.x + aRect.w;
	int aLeft   = aRect.x;
	int aTop    = aRect.y;
	int aBottom = aRect.y + aRect.h;

	int bRight  = bRect.x + bRect.w;
	int bLeft   = bRect.x;
	int bTop    = bRect.y;
	int bBottom = bRect.y + bRect.h;

	if ((aLeft <= bRight && aRight >= bLeft) || (aLeft >= bRight && aRight <= bLeft)) {
		if (aTop <= bBottom && aBottom >= bTop) {
			printf("COLLISION {%d}\r\n", count++);
			return true;
		}
	}

	return false;
}

bool Collision::surface(SDL_Surface *a, SDL_Surface *b) {
	for (int x = 0; x < a->w; x++) {
		for (int y = 0; x < a->h; y++) {

		}
	}

	//unsigned int *aPixel = ((unsigned int *)a->pixels)[iX + iY * pTarget->pitch / sizeof(unsigned int)]
	return false;
}