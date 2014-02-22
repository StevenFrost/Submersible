#ifndef COLLISION_H
#define COLLISION_H

#include "DisplayableObject.h"

class Collision {
public:
	static bool boundingBox(DisplayableObject *a, DisplayableObject *b);
};

#endif // !COLLISION_H