#ifndef SUBMARINE_H
#define SUBMARINE_H

#include "Image.h"
#include "header.h"
#include "IObservable.h"
#include "MyProjectMain.h"
#include "DisplayableObject.h"

class Submarine : public DisplayableObject, public IObservable {
public:
	Submarine(MyProjectMain *engine, unsigned int x, unsigned int y);
	virtual ~Submarine();

	/* Overrides from DisplayableObject */
	virtual void Draw();
	virtual void DoUpdate(int elapsedTime);

	/* Getters */
	Image       *getSubBody()  const { return m_body;               }
	SDL_Surface *getSurface()  const { return m_body->getSurface(); }
	double       getFuel()     const { return m_fuel;               }
	double       getXDelta()   const { return m_xDelta;             }
	double       getYDelta()   const { return m_yDelta;             }

	/* Setters */
	void setXDelta(double delta)      { m_xDelta = delta;           }
	void setYDelta(double delta)      { m_yDelta = delta;           }
protected:
	static Image *m_body;
private:
	double m_fuel;
	double m_friction;
	double m_xDelta;
	double m_yDelta;

	void controlSub();
};

#endif // !SUBMARINE_H