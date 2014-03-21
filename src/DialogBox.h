#ifndef DIALOG_BOX_H
#define DIALOG_BOX_H

#include "MyProjectMain.h"
#include "DisplayableObject.h"
#include "StatusBar.h"

class DialogBox : public DisplayableObject {
public:
	typedef enum Type {
		CRASH	= 0xBC0D0D,
		MESSAGE = 0x21323C
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

class CrashedDialogBox : public DialogBox {
public:
	CrashedDialogBox(MyProjectMain *engine, StatusBar *status, int width, int height) : DialogBox(engine, width, height, Type::CRASH), m_statusBar(status), m_str("CRASHED") {}
	virtual ~CrashedDialogBox() {}

	virtual void Draw();
	void setTitle(const char *str) { m_str = str; }
private:
	StatusBar *m_statusBar;
	const char *m_str;
};

class PausedDialogBox : public DialogBox {
public:
	PausedDialogBox(MyProjectMain *engine, int width, int height) : DialogBox(engine, width, height, Type::MESSAGE) {}
	virtual ~PausedDialogBox() {}

	virtual void Draw();
};

class HelpDialogBox : public DialogBox {
public:
	HelpDialogBox(MyProjectMain *engine, int width, int height) : DialogBox(engine, width, height, Type::MESSAGE) {}
	virtual ~HelpDialogBox() {}

	virtual void Draw();
};

class MainDialogBox : public DialogBox {
public:
	MainDialogBox(MyProjectMain *engine, int width, int height) : DialogBox(engine, width, height, Type::MESSAGE) {}
	virtual ~MainDialogBox() {}

	virtual void Draw();
};

#endif // !DIALOG_BOX_H