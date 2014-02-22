#ifndef STATUS_BAR_H
#define STATUS_BAR_H

#include "header.h"
#include "IObserver.h"
#include "MyProjectMain.h"
#include "DisplayableObject.h"

class StatusBar : public DisplayableObject, public IObserver {
public:
	StatusBar(MyProjectMain *engine);
	virtual ~StatusBar();

	/* Overrides from DisplayableObject */
	virtual void Draw();
	virtual void DoUpdate(int elapsedTime);
	virtual void GetRedrawRect(SDL_Rect *rectangle);

	/* Observer implementation */
	virtual void update(IObservable *observable);

	void drawBackground();

	inline void incrementDistance(double delta) { m_distance += delta; }
	inline void incrementPoints()               { m_points++;          }
protected:
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