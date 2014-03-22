#include "Coin.h"

Image *Coin::m_coin = new Image();

Coin::Coin(MyProjectMain *engine, int x, int y) : GameObject(engine, Type::COIN) {
	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;
	m_currentScreenXPrecise = m_iCurrentScreenX = m_iPreviousScreenX = x;
	m_currentScreenYPrecise = m_iCurrentScreenY = m_iPreviousScreenY = y;

	if (!m_coin->IsLoaded()) {
		m_coin->LoadImage("../resources/coin.png");
	}

	m_iDrawWidth = m_coin->GetWidth() + 1;
	m_iDrawHeight = m_coin->GetHeight() + 1;
}

Coin::~Coin() {}

void Coin::Draw() {
	if (!m_bVisible) return;

	/* Draw the coin */
	m_coin->RenderImage(m_pEngine->GetForeground(), 0, 0, m_currentScreenXPrecise, m_currentScreenYPrecise, m_coin->GetWidth(), m_coin->GetHeight());
}

void Coin::DoUpdate(int elapsedTime) {
	// We can't use m_iCurrentScreenX as it's too inaccurate and we can't sync
	// mines with the terrain. Instead, we use the more accurate member
	// m_currentScreenXPrecise
	m_currentScreenXPrecise -= (70.0 * elapsedTime / 1000.0);

	/* Keep the less precise version updated, we need it for redrawing */
	m_iCurrentScreenX = static_cast<int>(m_currentScreenXPrecise);

	/* Update the redraw rectangles */
	StoreLastScreenPositionAndUpdateRect();
}

void Coin::RedrawBackground() {
	SDL_Rect *rect = new SDL_Rect();
	GetRedrawRect(rect);
	m_pEngine->CopyBackgroundPixels(rect->x, rect->y, rect->w, rect->h);
}

void Coin::GetRedrawRect(SDL_Rect *rectangle) {
	rectangle->x = m_iCurrentScreenX;
	rectangle->y = m_iCurrentScreenY;
	rectangle->w = m_iDrawWidth;
	rectangle->h = m_iDrawHeight;
}