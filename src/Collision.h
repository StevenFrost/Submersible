#ifndef COLLISION_H
#define COLLISION_H

#include "header.h"
#include "DisplayableObject.h"

class Collision {
public:
	/* Performs a fast bounding box collision test on two displayable objects.
	   This function can be used for approximation of collisions, before a more
	   CPU-intensive collision test. */
	static bool boundingBox(DisplayableObject *a, DisplayableObject *b);
	static bool boundingBox(int aX, int aY, int aW, int aH, int bX, int bY, int bW, int bH);
	
	/* Performs a slower collision test on two SDL surfaces. The test speed can be
	   increased by using a less precise check (higher resolution parameter value). */
	static bool surface(int resolution, SDL_Surface *a, SDL_Surface *b, int aX, int aY, int bX, int bY);
	static bool surface(int resolution, SDL_Surface *a, SDL_Surface *b, int aX, int aY, int aW, int bX, int bY, int bW);
};

#endif // !COLLISION_H