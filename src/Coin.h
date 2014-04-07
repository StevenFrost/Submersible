#ifndef COIN_H
#define COIN_H

#include "Image.h"
#include "header.h"
#include "BaseEngine.h"
#include "GameObject.h"

class Coin : public GameObject {
public:
	Coin(BaseEngine *engine, int x, int y);
	virtual ~Coin() {}

	/* Overrides from GameObject */
	virtual void Draw();
	virtual void DoUpdate(int elapsedTime);
	virtual SDL_Surface *getCollidableSurface() const { return m_coin.getSurface(); }
protected:
	static Image m_coin;
};

#endif // !COIN_H