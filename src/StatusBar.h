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
	virtual void RedrawBackground();

	void drawBackground();
protected:
	unsigned int m_distance;
	unsigned int m_points;
	unsigned int m_seconds;
	double m_fuel;
private:
	unsigned int m_alpha;
	unsigned int m_height;
};

#endif // !STATUS_BAR_H