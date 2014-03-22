#ifndef NAVAL_MINE_H
#define NAVAL_MINE_H

#include "Image.h"
#include "header.h"
#include "GameObject.h"
#include "MyProjectMain.h"

class NavalMine : public GameObject {
public:
	NavalMine(MyProjectMain *engine, int x, int y);
	virtual ~NavalMine();

	/* Overrides from DisplayableObject */
	virtual void Draw();
	virtual void DoUpdate(int elapsedTime);
	virtual void GetRedrawRect(SDL_Rect *rectangle);
	virtual void RedrawBackground();

	virtual SDL_Surface *getCollidableSurface()  { return m_mine->getSurface();        }
	virtual int          getCollidableSurfaceX() { return m_currentScreenXPrecise + 6; }
	virtual int          getCollidableSurfaceY() { return m_currentScreenYPrecise + 6; }
protected:
	static Image *m_mine;
	static Image *m_chain;
	static Image *m_lights;

	double m_currentScreenXPrecise;		// A more precise version of m_iCurrentScreenX
	double m_currentScreenYPrecise;		// A More precise version of m_iCurrentScreenY
private:
	int m_lightSpriteOffset;
	int m_totalTimeElapsed;

	void animateLights(int elapsedTime);
};

#endif // !NAVAL_MINE_H