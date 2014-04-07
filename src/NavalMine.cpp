#include <cmath>
#include "Terrain.h"
#include "NavalMine.h"
#include "MyProjectMain.h"

Image NavalMine::m_mine      = Image();
Image NavalMine::m_chain     = Image();
Image NavalMine::m_lights    = Image();
Image NavalMine::m_explosion = Image();

NavalMine::NavalMine(BaseEngine *engine, int x, int y) : GameObject(engine, Type::NAVAL_MINE), m_totalTimeElapsed(0), m_exploding(false), m_hit(false), m_explosionSpriteOffset(0), m_explosionTime(0) {
	/* GameObject member initialisation */
	m_bVisible = true;
	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;
	m_currentScreenXPrecise = m_iCurrentScreenX = m_iPreviousScreenX = x;
	m_currentScreenYPrecise = m_iCurrentScreenY = m_iPreviousScreenY = y;
	m_chainY = m_currentScreenYPrecise + 97;

	/* Load any images that need loading */
	if (!m_mine.IsLoaded()) {
		m_mine.LoadImage("../resources/mine.png");
	}
	if (!m_chain.IsLoaded()) {
		m_chain.LoadImage("../resources/chain.png");
	}
	if (!m_lights.IsLoaded()) {
		m_lights.LoadImage("../resources/lights-red.png");
	}
	if (!m_explosion.IsLoaded()) {
		m_explosion.LoadImage("../resources/explosion.png");
	}

	/* Draw width wider than mine body to accomodate the explosion animation */
	m_iDrawWidth = m_explosion.GetWidth() + 1;
	m_iDrawHeight = m_pEngine->GetScreenHeight() - m_iCurrentScreenY;
}

void NavalMine::Draw() {
	/* Draw the chain to the bottom of the screen */
	for (int i = m_chainY; i < m_pEngine->GetScreenHeight(); i += m_chain.GetHeight()) {
		m_chain.RenderImage(m_pEngine->GetForeground(), 0, 0, m_currentScreenXPrecise + 59, i, m_chain.GetWidth(), m_chain.GetHeight());
	}

	if (m_bVisible) {
		if (m_exploding) {
			/* Draw the current explosion frame */
			m_explosion.RenderImage(m_pEngine->GetForeground(), m_explosionSpriteOffset, 0, m_currentScreenXPrecise, m_currentScreenYPrecise, 128, 128);
		} else {
			/* Draw the mine body */
			m_mine.RenderImage(m_pEngine->GetForeground(), 0, 0, m_currentScreenXPrecise + 18, m_currentScreenYPrecise + 18, m_mine.GetWidth(), m_mine.GetHeight());

			/* Draw the individual prong lights */
			m_lights.RenderImage(m_pEngine->GetForeground(), m_lightSpriteOffset, 0, m_currentScreenXPrecise + 54, m_currentScreenYPrecise + 12, 15, 14);
			m_lights.RenderImage(m_pEngine->GetForeground(), m_lightSpriteOffset, 0, m_currentScreenXPrecise + 24, m_currentScreenYPrecise + 23, 15, 14);
			m_lights.RenderImage(m_pEngine->GetForeground(), m_lightSpriteOffset, 0, m_currentScreenXPrecise + 96, m_currentScreenYPrecise + 52, 15, 14);
			m_lights.RenderImage(m_pEngine->GetForeground(), m_lightSpriteOffset, 0, m_currentScreenXPrecise + 85, m_currentScreenYPrecise + 80, 15, 14);
			m_lights.RenderImage(m_pEngine->GetForeground(), m_lightSpriteOffset, 0, m_currentScreenXPrecise + 23, m_currentScreenYPrecise + 80, 15, 14);
			m_lights.RenderImage(m_pEngine->GetForeground(), m_lightSpriteOffset, 0, m_currentScreenXPrecise + 12, m_currentScreenYPrecise + 52, 15, 14);
			m_lights.RenderImage(m_pEngine->GetForeground(), m_lightSpriteOffset, 0, m_currentScreenXPrecise + 83, m_currentScreenYPrecise + 23, 15, 14);
		}
	}
}

void NavalMine::DoUpdate(int elapsedTime) {
	m_iPreviousScreenX = m_iCurrentScreenX;
	m_iPreviousScreenY = m_iCurrentScreenY;

	/* Make the mine invisible when off-screen to avoid hidden collisions */
	if (m_bVisible && m_iCurrentScreenX < -m_iDrawWidth) {
		m_bVisible = false;
	}

	/* Update the mine position */
	m_currentScreenXPrecise -= (70.0 * elapsedTime / 1000.0);
	m_iCurrentScreenX = static_cast<int>(m_currentScreenXPrecise);

	/* Animate the mine lights */
	animateLights(elapsedTime);
	if (m_exploding) animateExplosion(elapsedTime);

	/* Once the mine has been hit we drop the chain */
	if (m_iCurrentScreenY < m_pEngine->GetScreenHeight() && m_hit) {
		m_chainY += (120.0 * elapsedTime / 1000.0);
	}

	/* Update the redraw rectangles */
	StoreLastScreenPositionAndUpdateRect();
}

void NavalMine::animateLights(int elapsedTime) {
	m_totalTimeElapsed += elapsedTime;
	int mod = m_totalTimeElapsed % 1000;

	m_lightSpriteOffset = (floor(mod / 100.0)) * 15;
}

void NavalMine::animateExplosion(int elapsedTime) {
	m_explosionTime += elapsedTime;
	int mod = m_explosionTime % 1000;

	if (mod > 950) {
		m_exploding = false;
		m_bVisible = false;
	}

	m_explosionSpriteOffset = (floor(mod / 25.0)) * 128;
}

void RisingMine::DoUpdate(int elapsedTime) {
	m_iPreviousScreenX = m_iCurrentScreenX;
	m_iPreviousScreenY = m_iCurrentScreenY;

	/* Update the mine position */
	m_currentScreenXPrecise -= (70.0 * elapsedTime / 1000.0);
	m_iCurrentScreenX = static_cast<int>(m_currentScreenXPrecise);

	/* While the mine hasn't been hit, it rises to the surface */
	if (m_iCurrentScreenX <= 1100 && m_iCurrentScreenY > 130 && !m_hit) {
		m_chainY = m_currentScreenYPrecise + 98;
		m_currentScreenYPrecise -= (10.0 * elapsedTime / 1000.0);
		m_iCurrentScreenY = static_cast<int>(m_currentScreenYPrecise);
	}

	/* Animate the mine lights */
	animateLights(elapsedTime);
	if (m_exploding) animateExplosion(elapsedTime);

	/* Once the mine has been hit we drop the chain */
	if (m_iCurrentScreenY < m_pEngine->GetScreenHeight() && m_hit) {
		m_chainY += (120.0 * elapsedTime / 1000.0);
	}

	/* Update the redraw rectangles */
	StoreLastScreenPositionAndUpdateRect();
}