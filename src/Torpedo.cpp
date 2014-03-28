#include "Torpedo.h"
#include "MyProjectMain.h"

Image *Torpedo::m_torpedo = new Image();

Torpedo::Torpedo(MyProjectMain *engine, DisplayableObject *target) : GameObject(engine, Type::TORPEDO), m_target(target) {
	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;
	m_currentScreenXPrecise = m_iCurrentScreenX = m_iPreviousScreenX = -(rand() % (400 - 100) + 100);
	m_currentScreenYPrecise = m_iCurrentScreenY = m_iPreviousScreenY = m_target->GetYCentre();

	if (!m_torpedo->IsLoaded()) {
		m_torpedo->LoadImage("../resources/torpedo.png");
	}

	m_iDrawWidth = m_torpedo->GetWidth() + 2;
	m_iDrawHeight = m_torpedo->GetHeight() + 2;
	m_bVisible = true;
}

Torpedo::~Torpedo() {}

void Torpedo::Draw() {
	if (!m_bVisible) return;

	m_torpedo->RenderImage(m_pEngine->GetForeground(), 0, 0, m_currentScreenXPrecise + 1, m_currentScreenYPrecise + 1, m_torpedo->GetWidth(), m_torpedo->GetHeight());
}

void Torpedo::DoUpdate(int elapsedTime) {
	m_currentScreenXPrecise += (50.0 * elapsedTime / 1000.0);

	if (m_currentScreenYPrecise < m_target->GetYCentre() - 0.1) {
		m_currentScreenYPrecise += (40.0 * elapsedTime / 1000.0);
	} else if (m_currentScreenYPrecise > m_target->GetYCentre() + 0.1) {
		m_currentScreenYPrecise -= (40.0 * elapsedTime / 1000.0);
	}

	/* Keep the less precise version updated, we need it for redrawing */
	m_iCurrentScreenX = static_cast<int>(m_currentScreenXPrecise);
	m_iCurrentScreenY = static_cast<int>(m_currentScreenYPrecise);

	/* Update the redraw rectangles */
	StoreLastScreenPositionAndUpdateRect();
}

void Torpedo::GetRedrawRect(SDL_Rect *rectangle) {
	rectangle->x = m_iCurrentScreenX;
	rectangle->y = m_iCurrentScreenY;
	rectangle->w = m_iDrawWidth;
	rectangle->h = m_iDrawHeight;
}

void Torpedo::RedrawBackground() {
	SDL_Rect *rect = new SDL_Rect();
	GetRedrawRect(rect);
	m_pEngine->CopyBackgroundPixels(rect->x, rect->y, rect->w, rect->h);
}