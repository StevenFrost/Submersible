#ifndef COLLISION_H
#define COLLISION_H

#include "DisplayableObject.h"

class Collision {
public:
	static bool boundingBox(DisplayableObject *a, DisplayableObject *b);
	static bool surface(SDL_Surface *a, SDL_Surface *b);
};

#endif // !COLLISION_H