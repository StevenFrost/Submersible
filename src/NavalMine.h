#ifndef NAVAL_MINE_H
#define NAVAL_MINE_H

#include "header.h"
#include "Image.h"
#include "IObserver.h"
#include "MyProjectMain.h"
#include "DisplayableObject.h"

class NavalMine : public DisplayableObject, public IObserver {
public:
	NavalMine(MyProjectMain *engine);
	virtual ~NavalMine();

	/* Overrides from DisplayableObject */
	virtual void Draw();
	virtual void DoUpdate(int elapsedTime);
	virtual void RedrawBackground();

	virtual void update(IObservable *observerable);

	void initialise();
protected:
	static Image *m_mine;
	static Image *m_chain;
	static Image *m_lights;

	double m_currentScreenXPrecise;		// A more precise version of m_iCurrentScreenX
	double m_currentScreenYPrecise;		// A More precise version of m_iCurrentScreenY

private:
	int m_lightSpriteOffset;
	double m_speed;

	void animateLights(int elapsedTime);
};

#endif // !NAVAL_MINE_H