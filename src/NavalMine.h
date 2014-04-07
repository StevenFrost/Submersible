#ifndef NAVAL_MINE_H
#define NAVAL_MINE_H

#include "Image.h"
#include "header.h"
#include "GameObject.h"

class NavalMine : public GameObject {
public:
	NavalMine(BaseEngine *engine, int x, int y);
	virtual ~NavalMine() {}

	/* Overrides from GameObject */
	virtual void Draw();
	virtual void DoUpdate(int elapsedTime);
	virtual SDL_Surface *getCollidableSurface()  const { return m_mine.getSurface();          }
	virtual int          getCollidableSurfaceX() const { return m_currentScreenXPrecise + 18; }
	virtual int          getCollidableSurfaceY() const { return m_currentScreenYPrecise + 18; }

	void explode() { m_exploding = true; m_hit = true; }
protected:
	static Image m_mine;			// Mine body image
	static Image m_chain;			// Chain segment image
	static Image m_lights;			// Light sprite sheet
	static Image m_explosion;		// Explosion sprite sheet

	bool m_exploding;				// True if the mine is currently exploding
	bool m_hit;						// True if the mine has been hit
	double m_chainY;				// The y-coordinate of the top of the chain

	void animateLights(int elapsedTime);
	void animateExplosion(int elapsedTime);
private:
	int m_lightSpriteOffset;		// Current frame offset for the mine lights
	int m_totalTimeElapsed;			// Total time the mine has been active

	int m_explosionSpriteOffset;	// Current frame offset for the explosion animation
	int m_explosionTime;			// Total explosion time
};

class RisingMine : public NavalMine {
public:
	RisingMine(BaseEngine *engine, int x, int y) : NavalMine(engine, x, y) {}
	virtual ~RisingMine() {}

	virtual void DoUpdate(int elapsedTime);
};

#endif // !NAVAL_MINE_H