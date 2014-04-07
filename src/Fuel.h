#ifndef FUEL_H
#define FUEL_H

#include "Image.h"
#include "header.h"
#include "BaseEngine.h"
#include "GameObject.h"

class Fuel : public GameObject {
public:
	Fuel(BaseEngine *engine, int x, int y);
	virtual ~Fuel() {}

	/* Overrides from GameObject */
	virtual void		 Draw();
	virtual void		 DoUpdate(int elapsedTime);
	virtual SDL_Surface *getCollidableSurface() const { return m_fuel.getSurface(); }
protected:
	static Image m_fuel;
};

#endif // !FUEL_H