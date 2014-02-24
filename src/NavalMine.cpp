#include "NavalMine.h"

ImageSurface *NavalMine::m_mine = new ImageSurface();
ImageSurface *NavalMine::m_chain = new ImageSurface();
ImageSurface *NavalMine::m_lights = new ImageSurface();

NavalMine::NavalMine(MyProjectMain *engine) : DisplayableObject(engine), m_offset(0.0) {
	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;
	m_iCurrentScreenX = 900;
	m_iCurrentScreenY = 300;
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
}

NavalMine::~NavalMine() {
	delete m_mine, m_chain;
}

void NavalMine::Draw() {
	m_mine->RenderImage(m_pEngine->GetForeground(), 0, 0, m_iCurrentScreenX + m_offset, m_iCurrentScreenY, m_mine->GetWidth(), m_mine->GetHeight());
	m_lights->RenderImage(m_pEngine->GetForeground(), 0, 0, (m_iCurrentScreenX - 6) + m_offset, m_iCurrentScreenY - 6, m_lights->GetWidth(), m_lights->GetHeight());
	for (int i = m_iCurrentScreenY + 80; i < m_pEngine->GetScreenHeight(); i += m_chain->GetHeight()) {
		m_chain->RenderImage(m_pEngine->GetForeground(), 0, 0, (m_iCurrentScreenX + 41) + m_offset, i, m_chain->GetWidth(), m_chain->GetHeight());
	}
}

void NavalMine::DoUpdate(int elapsedTime) {
	/* We can't use m_iCurrentScreenX as it's too inaccurate and we can't sync mines with the terrain */
	m_offset -= (70.0 * elapsedTime / 1000.0);
}

void NavalMine::GetRedrawRect(SDL_Rect *rectangle) {
	rectangle->x = (m_iCurrentScreenX - 6) + m_offset;
	rectangle->y = (m_iCurrentScreenY - 6);
	rectangle->w = m_lights->GetWidth() + 1;
	rectangle->h = m_pEngine->GetScreenHeight() - rectangle->y;
}

void NavalMine::RedrawBackground() {
	SDL_Rect *rect = new SDL_Rect();
	GetRedrawRect(rect);
	m_pEngine->CopyBackgroundPixels(rect->x, rect->y, rect->w, rect->h);
}