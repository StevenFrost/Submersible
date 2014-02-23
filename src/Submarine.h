#ifndef SUBMARINE_H
#define SUBMARINE_H

#include "header.h"
#include "JPGImage.h"
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
	ImageSurface *getSubBody()  const { return m_body;     }
	bool          getLightsOn() const { return m_lightsOn; }
	double        getFuel()     const { return m_fuel;     }
	double        getXDelta()   const { return m_xDelta;   }
	double        getYDelta()   const { return m_yDelta;   }

	/* Setters */
	void setLightsOn(bool lightsOn) { m_lightsOn = lightsOn; }
	void setXDelta(double delta)    { m_xDelta = delta;      }
	void setYDelta(double delta)    { m_yDelta = delta;      }
protected:
	static ImageSurface *m_body;
	bool m_lightsOn;

	double m_fuel;

	double m_friction;
	double m_xDelta;
	double m_yDelta;
};

#endif // !SUBMARINE_H