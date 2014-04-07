#ifndef SHARK_H
#define SHARK_H

#include "Image.h"
#include "header.h"
#include "BaseEngine.h"
#include "DisplayableObject.h"

class Shark : public DisplayableObject {
public:
	Shark(BaseEngine *engine, int x);
	virtual ~Shark() {}

	/* Overrides from DisplayableObject */
	virtual void Draw();
	virtual void DoUpdate(int elapsedTime);
protected:
	static Image m_shark;			// The shark image

	int m_resetPoint;				// The x coordinate of the shark reset point
	double m_currentScreenXPrecise;	// Precise version of the current x position
	double m_currentScreenYPrecise;	// Precise verison of the current y position

private:
	inline int generateResetPoint() const { return (-100 - (rand() % (500 - 300) + 300)); }
};

#endif // !SHARK_H