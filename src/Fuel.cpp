#include "Fuel.h"

Image *Fuel::m_fuel = new Image();

Fuel::Fuel(MyProjectMain *engine, int x, int y) : GameObject(engine, Type::FUEL) {
	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;
	m_currentScreenXPrecise = m_iCurrentScreenX = m_iPreviousScreenX = x;
	m_currentScreenYPrecise = m_iCurrentScreenY = m_iPreviousScreenY = y;

	if (!m_fuel->IsLoaded()) {
		m_fuel->LoadImage("../resources/fuel-can.png");
	}

	m_iDrawWidth = m_fuel->GetWidth() + 1;
	m_iDrawHeight = m_fuel->GetHeight() + 1;
	m_bVisible = true;
}

Fuel::~Fuel() {}

void Fuel::Draw() {
	if (!m_bVisible) return;

	m_fuel->RenderImage(m_pEngine->GetForeground(), 0, 0, m_currentScreenXPrecise, m_currentScreenYPrecise, m_fuel->GetWidth(), m_fuel->GetHeight());
}

void Fuel::DoUpdate(int elapsedTime) {
	m_currentScreenXPrecise -= (70.0 * elapsedTime / 1000.0);
	m_iCurrentScreenX = static_cast<int>(m_currentScreenXPrecise);

	StoreLastScreenPositionAndUpdateRect();
}

void Fuel::RedrawBackground() {
	SDL_Rect *rect = new SDL_Rect();
	GetRedrawRect(rect);
	m_pEngine->CopyBackgroundPixels(rect->x, rect->y, rect->w, rect->h);
}

void Fuel::GetRedrawRect(SDL_Rect *rectangle) {
	rectangle->x = m_iCurrentScreenX;
	rectangle->y = m_iCurrentScreenY;
	rectangle->w = m_iDrawWidth;
	rectangle->h = m_iDrawHeight;
}