#ifndef NAVAL_MINE_H
#define NAVAL_MINE_H

#include "Image.h"
#include "header.h"
#include "GameObject.h"

class NavalMine : public GameObject {
public:
	NavalMine(BaseEngine *engine, int x, int y);
	virtual ~NavalMine();

	/* Overrides from DisplayableObject */
	virtual void Draw();
	virtual void DoUpdate(int elapsedTime);
	virtual void GetRedrawRect(SDL_Rect *rectangle);

	virtual SDL_Surface *getCollidableSurface()  const { return m_mine->getSurface();        }
	virtual int          getCollidableSurfaceX() const { return m_currentScreenXPrecise + 6; }
	virtual int          getCollidableSurfaceY() const { return m_currentScreenYPrecise + 6; }
protected:
	static Image *m_mine;
	static Image *m_chain;
	static Image *m_lights;

	void animateLights(int elapsedTime);
private:
	int m_lightSpriteOffset;
	int m_totalTimeElapsed;
};

class RisingMine : public NavalMine {
public:
	RisingMine(BaseEngine *engine, int x, int y) : NavalMine(engine, x, y) {}
	virtual ~RisingMine() {}

	virtual void DoUpdate(int elapsedTime);
};

#endif // !NAVAL_MINE_H