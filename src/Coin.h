#ifndef COIN_H
#define COIN_H

#include "Image.h"
#include "header.h"
#include "GameObject.h"
#include "MyProjectMain.h"

class Coin : public GameObject {
public:
	Coin(MyProjectMain *engine, int x, int y);
	virtual ~Coin();

	/* Overrides from DisplayableObject */
	virtual void Draw();
	virtual void DoUpdate(int elapsedTime);
	virtual void GetRedrawRect(SDL_Rect *rectangle);
	virtual void RedrawBackground();

	virtual SDL_Surface *getCollidableSurface()  { return m_coin->getSurface();    }
	virtual int          getCollidableSurfaceX() { return m_currentScreenXPrecise; }
	virtual int          getCollidableSurfaceY() { return m_currentScreenYPrecise; }
protected:
	static Image *m_coin;

	double m_currentScreenXPrecise;		// A more precise version of m_iCurrentScreenX
	double m_currentScreenYPrecise;		// A More precise version of m_iCurrentScreenY
};

#endif // !COIN_H