#ifndef SHARK_H
#define SHARK_H

#include "Image.h"
#include "header.h"
#include "MyProjectMain.h"
#include "DisplayableObject.h"

class Shark : public DisplayableObject {
public:
	Shark(MyProjectMain *engine, int x);
	virtual ~Shark();

	/* Overrides from DisplayableObject */
	virtual void Draw();
	virtual void DoUpdate(int elapsedTime);
protected:
	static Image *m_shark;

	double m_currentScreenXPrecise;
	double m_currentScreenYPrecise;
};

#endif // !SHARK_H