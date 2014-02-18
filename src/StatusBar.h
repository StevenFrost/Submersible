#ifndef STATUS_BAR_H
#define STATUS_BAR_H

#include "header.h"
#include "MyProjectMain.h"
#include "DisplayableObject.h"

class StatusBar : public DisplayableObject {
public:
	StatusBar(MyProjectMain *engine);
	virtual ~StatusBar();

	/* Overrides from DisplayableObject */
	virtual void Draw();
	virtual void DoUpdate(int elapsedTime);
	virtual void GetRedrawRect(SDL_Rect *rectangle);

	void drawBackground();

	void incrementDistance()             { m_distance++;    }
	void incrementPoints()               { m_points++;      }
	void fuelQuantityDelta(double delta) { m_fuel += delta; }
protected:
	unsigned int m_distance;
	unsigned int m_points;
	double m_seconds;
	double m_fuel;
private:
	unsigned int m_alpha;
	unsigned int m_height;
	unsigned int m_fuelColour;
};

#endif // !STATUS_BAR_H