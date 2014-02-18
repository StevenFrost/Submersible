#ifndef STATUS_BAR_H
#define STATUS_BAR_H

#include "MyProjectMain.h"
#include "DisplayableObject.h"

class StatusBar : public DisplayableObject {
public:
	StatusBar(MyProjectMain *engine);
	~StatusBar();

protected:
	unsigned int m_distance;
	unsigned int m_points;
	unsigned int m_seconds;
	double m_fuel;
};

#endif // !STATUS_BAR_H