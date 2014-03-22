#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "header.h"
#include "MyProjectMain.h"
#include "DisplayableObject.h"

class GameObject : public DisplayableObject {
public:
	typedef enum Type {
		RISING_MINE,
		NAVAL_MINE,
		TORPEDO,
		COIN
	} Type;

	GameObject(MyProjectMain *engine, Type type) : DisplayableObject(engine), m_type(type) {}
	virtual ~GameObject() {}

	Type getType() { return m_type; }

	virtual SDL_Surface *getCollidableSurface()  = 0;
	virtual int          getCollidableSurfaceX() = 0;
	virtual int          getCollidableSurfaceY() = 0;
protected:
	Type m_type;
};

#endif // !GAME_OBJECT_H