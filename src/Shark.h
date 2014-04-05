#ifndef SHARK_H
#define SHARK_H

#include "Image.h"
#include "header.h"
#include "BaseEngine.h"
#include "DisplayableObject.h"

class MyProjectMain;

class Shark : public DisplayableObject {
public:
	Shark(BaseEngine *engine, int x);
	virtual ~Shark();

	/* Overrides from DisplayableObject */
	virtual void Draw();
	virtual void DoUpdate(int elapsedTime);
	virtual void GetRedrawRect(SDL_Rect *rectangle);
protected:
	static Image *m_shark;

	double m_currentScreenXPrecise;
	double m_currentScreenYPrecise;
};

#endif // !SHARK_H