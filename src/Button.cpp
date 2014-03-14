#include "Button.h"

#define TEXT_COLOUR 0x2F76A2

Image *Button::m_buttonBase = new Image();
Image *Button::m_chain = new Image();
Image *Button::m_buttonLights = new Image();

Button::Button(MyProjectMain *engine, int x, int y, const char *text, Font *font, bool selected) : DisplayableObject(engine), m_text(text), m_font(font), m_selected(selected) {
	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;
	m_iCurrentScreenX = m_iPreviousScreenX = x;
	m_iCurrentScreenY = m_iPreviousScreenY = y;

	if (!m_buttonBase->IsLoaded()) {
		m_buttonBase->LoadImage("../resources/button-base.png");
	}
	if (!m_chain->IsLoaded()) {
		m_chain->LoadImage("../resources/chain.png");
	}
	if (!m_buttonLights->IsLoaded()) {
		m_buttonLights->LoadImage("../resources/button-lights.png");
	}

	m_iDrawWidth = 181;
	m_iDrawHeight = m_pEngine->GetScreenHeight() - m_iCurrentScreenY;
}

Button::~Button() {}

void Button::Draw() {
	/* Draw the button base */
	m_buttonBase->RenderImage(m_pEngine->GetForeground(), 0, 0, m_iCurrentScreenX, m_iCurrentScreenY, m_buttonBase->GetWidth(), m_buttonBase->GetHeight());
	
	/* Draw the text on the button*/
	m_pEngine->DrawString(m_iCurrentScreenX + 52, m_iCurrentScreenY + 54, m_text, TEXT_COLOUR, m_font, m_pEngine->GetForeground());
	
	if (m_selected) {
		m_buttonLights->RenderImage(m_pEngine->GetForeground(), 0, 0, m_iCurrentScreenX, m_iCurrentScreenY, m_buttonLights->GetWidth(), m_buttonLights->GetHeight());
	}

	/* Draw the chain to the bottom of the screen */
	for (int i = m_iCurrentScreenY + 159; i < m_pEngine->GetScreenHeight(); i += m_chain->GetHeight()) {
		m_chain->RenderImage(m_pEngine->GetForeground(), 0, 0, m_iCurrentScreenX + 89, i, m_chain->GetWidth(), m_chain->GetHeight());
	}
}

void Button::DoUpdate(int elapsedTime) {
	StoreLastScreenPositionAndUpdateRect();
}

void Button::RedrawBackground() {
	SDL_Rect *rect = new SDL_Rect();
	GetRedrawRect(rect);
	m_pEngine->CopyBackgroundPixels(rect->x, rect->y, rect->w, rect->h);
}