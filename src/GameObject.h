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
		COIN,
		FUEL,
		SUBMARINE
	} Type;

	GameObject(MyProjectMain *engine, Type type) : DisplayableObject(engine), m_type(type) {}
	virtual ~GameObject() {}

	Type getType() { return m_type; }

	virtual SDL_Surface *getCollidableSurface()  const = 0;
	virtual int          getCollidableSurfaceX() const { return m_currentScreenXPrecise; }
	virtual int          getCollidableSurfaceY() const { return m_currentScreenYPrecise; }
protected:
	Type m_type;

	double m_currentScreenXPrecise;		// A more precise version of m_iCurrentScreenX
	double m_currentScreenYPrecise;		// A More precise version of m_iCurrentScreenY
};

#endif // !GAME_OBJECT_H