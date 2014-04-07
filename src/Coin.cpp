#include "Coin.h"

Image Coin::m_coin = Image();

Coin::Coin(BaseEngine *engine, int x, int y) : GameObject(engine, Type::COIN) {
	/* GameObject member initialisation */
	m_bVisible = true;
	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;
	m_currentScreenXPrecise = m_iCurrentScreenX = m_iPreviousScreenX = x;
	m_currentScreenYPrecise = m_iCurrentScreenY = m_iPreviousScreenY = y;

	/* Load any non-loaded images */
	if (!m_coin.IsLoaded()) {
		m_coin.LoadImage("../resources/coin.png");
	}

	/* Coin draw width/height */
	m_iDrawWidth = m_coin.GetWidth() + 1;
	m_iDrawHeight = m_coin.GetHeight() + 1;
}

void Coin::Draw() {
	if (!m_bVisible) return;

	/* Draw the coin */
	m_coin.RenderImage(m_pEngine->GetForeground(), 0, 0, m_currentScreenXPrecise, m_currentScreenYPrecise, m_coin.GetWidth(), m_coin.GetHeight());
}

void Coin::DoUpdate(int elapsedTime) {
	m_iPreviousScreenX = m_iCurrentScreenX;

	/* Update the horizontal coin position */
	m_currentScreenXPrecise -= (70.0 * elapsedTime / 1000.0);
	m_iCurrentScreenX = static_cast<int>(m_currentScreenXPrecise);

	/* Update the redraw rectangle */
	StoreLastScreenPositionAndUpdateRect();
}