#ifndef STATUS_BAR_H
#define STATUS_BAR_H

#include "header.h"
#include "IObserver.h"
#include "BaseEngine.h"
#include "DisplayableObject.h"

class MyProjectMain;

class StatusBar : public DisplayableObject, public IObserver {
public:
	StatusBar(BaseEngine *engine);
	virtual ~StatusBar();

	/* Overrides from DisplayableObject */
	virtual void Draw();
	virtual void DoUpdate(int elapsedTime);
	virtual void GetRedrawRect(SDL_Rect *rectangle);

	/* Observer implementation */
	virtual void update(IObservable *observable);

	void initialise();
	void drawBackground();

	inline void incrementTime(double delta)     { m_seconds += delta;  }
	inline void incrementPoints()               { m_points++;          }
	inline void incrementDistance(double delta) { m_distance += delta; }
	
	/* Reset functions */
	inline void resetTime()     { m_seconds = 0;  }
	inline void resetPoints()   { m_points = 0;   }
	inline void resetDistance() { m_distance = 0; }

	/* Getters */
	inline double       getDistance() const { return m_distance; }
	inline unsigned int getPoints()   const { return m_points;   }
	inline double       getSeconds()  const { return m_seconds;  }
protected:
	MyProjectMain *m_pEngine;

	double m_distance;
	unsigned int m_points;
	double m_seconds;
private:
	unsigned int m_alpha;
	unsigned int m_height;
	unsigned int m_fuelColour;

	double m_fuel;
};

#endif // !STATUS_BAR_H