#include "Shark.h"

Image Shark::m_shark = Image();

Shark::Shark(BaseEngine *engine, int x) : DisplayableObject(engine) {
	/* DisplayableObject member initialisation */
	m_bVisible = true;
	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;
	m_currentScreenXPrecise = m_iCurrentScreenX = m_iPreviousScreenX = x;
	m_currentScreenYPrecise = m_iCurrentScreenY = m_iPreviousScreenY = 120 + (rand() % 200);

	/* Load the shark image if needed */
	if (!m_shark.IsLoaded()) {
		m_shark.LoadImage("../resources/shark.png");
	}

	/* Redraw rectangle width/height */
	m_iDrawWidth = m_shark.GetWidth() + 1;
	m_iDrawHeight = m_shark.GetHeight() + 1;

	/* Generate an initial reset point */
	m_resetPoint = generateResetPoint();
}

void Shark::Draw() {
	if (!m_bVisible) return;

	/* Draw the shark to the foreground */
	m_shark.RenderImage(m_pEngine->GetForeground(), 0, 0, m_currentScreenXPrecise, m_currentScreenYPrecise, m_iDrawWidth, m_iDrawHeight);
}

void Shark::DoUpdate(int elapsedTime) {
	m_iPreviousScreenX = m_iCurrentScreenX;
	m_iPreviousScreenY = m_iCurrentScreenY;

	/* When the shark reaches the reset point, we send it back to a new x and y position */
	if (m_currentScreenXPrecise < m_resetPoint) {
		m_resetPoint = generateResetPoint();
		m_currentScreenXPrecise = 1150;
		m_currentScreenYPrecise = 120 + (rand() % 200);
		m_iCurrentScreenY = static_cast<int>(m_currentScreenYPrecise);
	}

	/* Update the shark x position */
	m_currentScreenXPrecise -= (30.0 * elapsedTime / 1000.0);
	m_iCurrentScreenX = static_cast<int>(m_currentScreenXPrecise);

	/* Update the redraw rectangles */
	StoreLastScreenPositionAndUpdateRect();
}