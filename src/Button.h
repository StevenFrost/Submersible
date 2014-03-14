#ifndef BUTTON_H
#define BUTTON_H

#include "header.h"
#include "Image.h"
#include "MyProjectMain.h"
#include "DisplayableObject.h"

/** 
 * This class defines a button used in the top level menu system. A button has
 * the appearance of a moored mine with text in the centre describing its
 * purpose.
 * 
 * Buttons have two states; Selected and Deselected. When in the selected state,
 * lights are visible around the mine to give positive feedback to the user that
 * something has changed.
 */
class Button : public DisplayableObject {
public:
	Button(MyProjectMain *engine, int x, int y, const char *text, Font *font, bool selected);
	~Button();

	/* Overrides from DisplayableObject */
	virtual void Draw();
	virtual void DoUpdate(int elapsedTime);
	virtual void RedrawBackground();

	void setSelected(bool selected) { m_selected = selected; }
protected:
	static Image *m_buttonBase;
	static Image *m_buttonLights;
	static Image *m_chain;

	Font *m_font;
	bool m_selected;
	const char *m_text;
};

#endif // !BUTTON_H