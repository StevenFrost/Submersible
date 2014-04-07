#ifndef FLARE_H
#define FLARE_H

#include "Image.h"
#include "header.h"
#include "GameObject.h"
#include "BaseEngine.h"

class Flare : public GameObject {
public:
	Flare(BaseEngine *engine, int x, int y);
	virtual ~Flare() {}

	/* Overrides from GameObject */
	virtual void		 Draw();
	virtual void		 DoUpdate(int elapsedTime);
	virtual SDL_Surface *getCollidableSurface() const { return m_flare.getSurface(); }

	void reset();

	/* Getters */
	bool isActive() { return m_active; }

	/* Setters */
	void setX(int x)		 { m_currentScreenXPrecise = m_iCurrentScreenX = x; }
	void setY(int y)		 { m_currentScreenYPrecise = m_iCurrentScreenY = y; }
	void setActive(bool val) { m_active = val;									}
protected:
	static Image m_flare;

	void animate(int elapsedTime);
private:
	bool m_active;				// True if the flare is currently active
	double m_velocity;			// Flare velocity, decreases with time

	int m_flareSpriteOffset;	// Sprite sheet offset for flare animation
	int m_totalTimeElapsed;		// Total time elapsed since the flare was fired
};

#endif // !FLARE_H