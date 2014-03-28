#ifndef FUEL_H
#define FUEL_H

#include "Image.h"
#include "header.h"
#include "GameObject.h"
#include "MyProjectMain.h"

class Fuel : public GameObject {
public:
	Fuel(MyProjectMain *engine, int x, int y);
	virtual ~Fuel();

	/* Overrides from DisplayableObject */
	virtual void Draw();
	virtual void DoUpdate(int elapsedTime);
	virtual void GetRedrawRect(SDL_Rect *rectangle);
	virtual void RedrawBackground();

	virtual SDL_Surface *getCollidableSurface() const { return m_fuel->getSurface(); }
protected:
	static Image *m_fuel;
};

#endif // !FUEL_H