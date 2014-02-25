#ifndef NAVAL_MINE_H
#define NAVAL_MINE_H

#include "header.h"
#include "Image.h"
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
	static Image *m_mine;
	static Image *m_chain;
	static Image *m_lights;

	double m_offset;
};

#endif // !NAVAL_MINE_H