#ifndef DIALOG_BOX_H
#define DIALOG_BOX_H

#include "MyProjectMain.h"
#include "DisplayableObject.h"
#include "StatusBar.h"

class DialogBox : public DisplayableObject {
public:
	typedef enum Type {
		CRASH	= 0xBC0D0D,
		MESSAGE = 0x2D4452
	} Type;

	DialogBox(MyProjectMain *engine, int width, int height, Type boxType);
	virtual ~DialogBox();

	/* Overrides from DisplayableObject */
	virtual void Draw();
	virtual void DoUpdate(int elapsedTime);
	virtual void GetRedrawRect(SDL_Rect *rectangle);

	/* Getters */
	int getCurrentX() { return m_iCurrentScreenX; }
	int getCurrentY() { return m_iCurrentScreenY; }
protected:
	SDL_Surface *m_boxBase;
	int m_width;
	int m_height;
};

/**
* Specialised version of DialogBox, which shows the user's distance, score and
* total time at the point the dialog was updated
*/
class CrashedDialogBox : public DialogBox {
public:
	CrashedDialogBox(MyProjectMain *engine, StatusBar *status, int width, int height) : DialogBox(engine, width, height, Type::CRASH), m_statusBar(status) {}
	virtual ~CrashedDialogBox() {}

	virtual void Draw();
private:
	StatusBar *m_statusBar;
};

#endif // !DIALOG_BOX_H