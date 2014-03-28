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

	virtual SDL_Surface *getCollidableSurface() const { return m_coin->getSurface();    }
protected:
	static Image *m_coin;
};

#endif // !COIN_H