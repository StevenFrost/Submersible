#ifndef WAVES_H
#define WAVES_H

#include "header.h"
#include "JPGImage.h"
#include "MyProjectMain.h"
#include "DisplayableObject.h"

class Waves : public DisplayableObject {
public:
	Waves(MyProjectMain *engine);
	virtual ~Waves();

	/* Overrides from DisplayableObject */
	virtual void Draw();
	virtual void DoUpdate(int elapsedTime);
	virtual void GetRedrawRect(SDL_Rect *rectangle);
	virtual void RedrawBackground();
private:
	static ImageSurface *m_base;

	double m_offset;
	const double m_speed;
};

#endif // !WAVES_H