#include "Flare.h"
#include <cmath>

Image Flare::m_flare = Image();

Flare::Flare(BaseEngine*engine, int x, int y) : GameObject(engine, FLARE), m_flareSpriteOffset(0), m_totalTimeElapsed(0), m_active(false), m_velocity(50) {
	/* GameObject member initialisation */
	m_bVisible = false;
	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;
	m_currentScreenXPrecise = m_iCurrentScreenX = m_iPreviousScreenX = x;
	m_currentScreenYPrecise = m_iCurrentScreenY = m_iPreviousScreenY = y;
	
	/* Load the flare sprite if needed */
	if (!m_flare.IsLoaded()) {
		m_flare.LoadImage("../resources/flare.png");
	}

	/* Redraw rectangle width/height */
	m_iDrawWidth = 28;
	m_iDrawHeight = 28;
}

void Flare::reset() {
	m_bVisible = false;
	m_active = false;
	m_flareSpriteOffset = 0;
	m_totalTimeElapsed = 0;
	m_velocity = 50.0;
}

void Flare::Draw() {
	if (!m_bVisible) return;

	/* Draw the flare at the current calculated offset */
	m_flare.RenderImage(m_pEngine->GetForeground(), m_flareSpriteOffset, 0, m_currentScreenXPrecise, m_currentScreenYPrecise, 26, 26);
}

void Flare::DoUpdate(int elapsedTime) {
	if (!m_bVisible) return;

	/* Store the previous flare position */
	m_iPreviousScreenX = m_iCurrentScreenX;
	m_iPreviousScreenY = m_iCurrentScreenY;

	/* Animate the flare */
	animate(elapsedTime);

	/* Accelerate the flare towards the terrain unless it has reached max velocity */
	if (m_velocity < 70.0) {
		m_velocity += (20.0 * elapsedTime / 1000.0);
	}

	/* Update the flare position */
	m_currentScreenXPrecise -= (m_velocity * elapsedTime / 1000.0);
	m_iCurrentScreenX = static_cast<int>(m_currentScreenXPrecise);

	/* If the flare hasn't hit the terrain, allow it to drop */
	if (m_active) {
		m_currentScreenYPrecise += (20.0 * elapsedTime / 1000.0);
		m_iCurrentScreenY = static_cast<int>(m_currentScreenYPrecise);
	}

	/* Update the redraw rectangle */
	StoreLastScreenPositionAndUpdateRect();
}

void Flare::animate(int elapsedTime) {
	m_totalTimeElapsed += elapsedTime;
	
	// The flare has a lifetime of our seconds, three of which the
	// flare is at full strength
	if (m_totalTimeElapsed < 3000.0) {
		return;
	} else if (m_totalTimeElapsed > 3900.0) {
		reset();
		return;
	}

	/* Calculate the sprite sheet offset */
	int mod = m_totalTimeElapsed % 1000;
	m_flareSpriteOffset = floor(mod / 100.0) * 26;
}