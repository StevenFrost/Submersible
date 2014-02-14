#ifndef SUBMARINE_H
#define SUBMARINE_H

#include "header.h"
#include "JPGImage.h"
#include "MyProjectMain.h"
#include "DisplayableObject.h"

class Submarine : public DisplayableObject {
public:
	Submarine(MyProjectMain *engine, unsigned int x, unsigned int y);
	virtual ~Submarine();

	/* Overrides from DisplayableObject */
	virtual void Draw();
	virtual void DoUpdate(int elapsedTime);

	/* Getters */
	ImageSurface *getSubBody()  { return m_body;     }
	bool          getLightsOn() { return m_lightsOn; }
	double        getXDelta()   { return m_xDelta;   }
	double        getYDelta()   { return m_yDelta;   }

	/* Setters */
	void setLightsOn(bool lightsOn) { m_lightsOn = lightsOn; }
	void setXDelta(double delta)    { m_xDelta = delta;      }
	void setYDelta(double delta)    { m_yDelta = delta;      }
protected:
	static ImageSurface *m_body;
	bool m_lightsOn;

	double m_friction;
	double m_xDelta;
	double m_yDelta;
};

#endif // !SUBMARINE_H