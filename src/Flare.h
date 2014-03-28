#ifndef FLARE_H
#define FLARE_H

#include "Image.h"
#include "header.h"
#include "MyProjectMain.h"
#include "DisplayableObject.h"

class Flare : public DisplayableObject {
public:
	Flare(MyProjectMain *engine, int x, int y);
	virtual ~Flare();

	/* Overrides from DisplayableObject */
	virtual void Draw();
	virtual void DoUpdate(int elapsedTime);
	virtual void GetRedrawRect(SDL_Rect *rectangle);
	virtual void RedrawBackground();

	/* Setters */
	void setX(int x) { m_currentScreenXPrecise = m_iCurrentScreenX = x; }
	void setY(int y) { m_currentScreenYPrecise = m_iCurrentScreenY = y; }
protected:
	static Image *m_flare;

	void animate(int elapsedTime);
private:
	bool m_active;				// True if the flare is currently active
	double m_velocity;			// Flare velocity, decreases with time

	int m_flareSpriteOffset;	// Sprite sheet offset for flare animation
	int m_totalTimeElapsed;		// Total time elapsed since the flare was fired

	double m_currentScreenXPrecise;
	double m_currentScreenYPrecise;
};

#endif // !FLARE_H