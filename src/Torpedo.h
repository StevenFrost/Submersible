#ifndef TORPEDO_H
#define TORPEDO_H

#include "Image.h"
#include "header.h"
#include "BaseEngine.h"
#include "GameObject.h"

class Torpedo : public GameObject {
public:
	Torpedo(BaseEngine *engine, DisplayableObject *target);
	virtual ~Torpedo();

	/* Overrides from DisplayableObject */
	virtual void Draw();
	virtual void DoUpdate(int elapsedTime);
	virtual void GetRedrawRect(SDL_Rect *rectangle);
	virtual void RedrawBackground();

	virtual SDL_Surface *getCollidableSurface() const { return m_torpedo->getSurface(); }

	void retarget(DisplayableObject *target) { m_target = target; m_needsRetarget = false; }

	bool needsNewTarget() { return m_needsRetarget; }
protected:
	static Image *m_torpedo;
	DisplayableObject *m_target;
private:
	bool m_needsRetarget;
};

#endif // !TORPEDO_H