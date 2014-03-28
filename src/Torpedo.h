#ifndef TORPEDO_H
#define TORPEDO_H

#include "Image.h"
#include "header.h"
#include "GameObject.h"
#include "MyProjectMain.h"

class Torpedo : public GameObject {
public:
	Torpedo(MyProjectMain *engine, DisplayableObject *target);
	virtual ~Torpedo();

	/* Overrides from DisplayableObject */
	virtual void Draw();
	virtual void DoUpdate(int elapsedTime);
	virtual void GetRedrawRect(SDL_Rect *rectangle);
	virtual void RedrawBackground();

	virtual SDL_Surface *getCollidableSurface() const { return m_torpedo->getSurface(); }

	void retarget(DisplayableObject *target)          { m_target = target;              }
protected:
	static Image *m_torpedo;
	DisplayableObject *m_target;
};

#endif // !TORPEDO_H