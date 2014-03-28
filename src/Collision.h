#ifndef COLLISION_H
#define COLLISION_H

#include "DisplayableObject.h"

class Collision {
public:
	/**
	* Performs a fast bounding box collision test on two displayable objects.
	* This function can be used for approximation of collisions, before a more
	* CPU-intensive collision test.
	*
	* @param a	the first surface
	* @param b	the second surface
	* @return	true if the boxes intersect
	*/
	static bool boundingBox(DisplayableObject *a, DisplayableObject *b);

	static bool boundingBox(int aX, int aY, int aW, int aH, int bX, int bY, int bW, int bH);
	
	/**
	* Performs a slower collision test on two SDL surfaces. The test speed can
	* be increased by using a less precise check (higher resolution parameter
	* value).
	*
	* @precondition		the two surfaces are overlapping
	* @precondition		the surfaces are normalised
	*
	* @param resolution	the resolution of the collision check, in pixels
	* @param a			the first surface
	* @param b			the second surface
	* @param aX			first surface x-coordinate (world space)
	* @param aY			first surface y-coordinate (world space)
	* @param bX			second surface x-coordinate (world space)
	* @param bY			second surface y-coordinate (world space)
	* 
	* @return			true if a collisioin occured at the pixel resolution level
	*/
	static bool surface(int resolution, SDL_Surface *a, SDL_Surface *b, int aX, int aY, int bX, int bY);
};

#endif // !COLLISION_H