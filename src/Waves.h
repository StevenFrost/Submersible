#ifndef WAVES_H
#define WAVES_H

#include "header.h"
#include "JPGImage.h"
#include "BaseEngine.h"
#include "DisplayableObject.h"

class Waves : public DisplayableObject {
public:
	Waves(BaseEngine *engine, double speed);
	virtual ~Waves();

	/* Overrides from DisplayableObject */
	virtual void Draw();
	virtual void DoUpdate(int elapsedTime);
	virtual void GetRedrawRect(SDL_Rect *rectangle);
	virtual void RedrawBackground();

	/* Setters */
	void setSpeed(double speed) { m_speed = speed; }
private:
	static ImageSurface * m_base;

	double m_offset;
	double m_speed;
};

#endif // !WAVES_H