#include "NavalMine.h"

Image *NavalMine::m_mine = new Image();
Image *NavalMine::m_chain = new Image();
Image *NavalMine::m_lights = new Image();

NavalMine::NavalMine(MyProjectMain *engine) : DisplayableObject(engine) {
	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;
	m_iCurrentScreenX = 900;
	m_iCurrentScreenY = 400;
	m_currentScreenXPrecise = m_iCurrentScreenX;
	m_currentScreenYPrecise = m_iCurrentScreenY;
	m_iPreviousScreenX = m_iCurrentScreenX;
	m_iPreviousScreenY = m_iCurrentScreenY;

	if (!m_mine->IsLoaded()) {
		m_mine->LoadImage("../resources/mine.png");
	}
	if (!m_chain->IsLoaded()) {
		m_chain->LoadImage("../resources/chain.png");
	}
	if (!m_lights->IsLoaded()) {
		m_lights->LoadImage("../resources/mine-lights.png");
	}

	m_iDrawWidth = m_lights->GetWidth();
	m_iDrawHeight = m_pEngine->GetScreenHeight() - m_iCurrentScreenY;
}

NavalMine::~NavalMine() {
	delete m_mine, m_chain, m_lights;
}

void NavalMine::Draw() {
	/* Draw the mine and lights */
	m_mine->RenderImage(m_pEngine->GetForeground(), 0, 0, m_currentScreenXPrecise + 6, m_currentScreenYPrecise + 6, m_mine->GetWidth(), m_mine->GetHeight());
	m_lights->RenderImage(m_pEngine->GetForeground(), 0, 0, m_currentScreenXPrecise, m_currentScreenYPrecise, m_lights->GetWidth(), m_lights->GetHeight());
	
	/* Draw the terrain to the bottom of the screen */
	for (int i = m_iCurrentScreenY + 86; i < m_pEngine->GetScreenHeight(); i += m_chain->GetHeight()) {
		m_chain->RenderImage(m_pEngine->GetForeground(), 0, 0, m_currentScreenXPrecise + 47, i, m_chain->GetWidth(), m_chain->GetHeight());
	}
}

void NavalMine::DoUpdate(int elapsedTime) {
	// We can't use m_iCurrentScreenX as it's too inaccurate and we can't sync
	// mines with the terrain. Instead, we use the more accurate member
	// m_currentScreenXPrecise
	m_currentScreenXPrecise -= (70.0 * elapsedTime / 1000.0);
	m_currentScreenYPrecise -= (17.0 * elapsedTime / 1000.0);

	/* Keep the less precise version updated, we need it for redrawing */
	m_iCurrentScreenX = static_cast<int>(m_currentScreenXPrecise);
	m_iCurrentScreenY = static_cast<int>(m_currentScreenYPrecise);

	/* Update the redraw rectangles */
	StoreLastScreenPositionAndUpdateRect();
}

void NavalMine::RedrawBackground() {
	SDL_Rect *rect = new SDL_Rect();
	GetRedrawRect(rect);
	m_pEngine->CopyBackgroundPixels(rect->x, rect->y, rect->w, rect->h);
}