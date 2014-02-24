#ifndef NAVAL_MINE_H
#define NAVAL_MINE_H

#include "header.h"
#include "JPGImage.h"
#include "MyProjectMain.h"
#include "DisplayableObject.h"

class NavalMine : public DisplayableObject {
public:
	NavalMine(MyProjectMain *engine);
	virtual ~NavalMine();

	/* Overrides from DisplayableObject */
	virtual void Draw();
	virtual void DoUpdate(int elapsedTime);
	virtual void GetRedrawRect(SDL_Rect *rectangle);
	virtual void RedrawBackground();
protected:
	static ImageSurface *m_mine;
	static ImageSurface *m_chain;
	static ImageSurface *m_lights;

	double m_offset;
};

#endif // !NAVAL_MINE_H