#include "Torpedo.h"

Image Torpedo::m_torpedo = Image();

Torpedo::Torpedo(BaseEngine *engine, DisplayableObject *target) : GameObject(engine, Type::TORPEDO), m_target(target), m_needsRetarget(false) {
	/* GameObject member initialisation */
	m_bVisible = true;
	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;
	m_currentScreenXPrecise = m_iCurrentScreenX = m_iPreviousScreenX = -(rand() % (400 - 100) + 100);
	m_currentScreenYPrecise = m_iCurrentScreenY = m_iPreviousScreenY = m_target->GetYCentre();

	/* Load the image if needed */
	if (!m_torpedo.IsLoaded()) {
		m_torpedo.LoadImage("../resources/torpedo.png");
	}

	/* Redraw rectangle width/height */
	m_iDrawWidth = m_torpedo.GetWidth() + 2;
	m_iDrawHeight = m_torpedo.GetHeight() + 2;
}

void Torpedo::Draw() {
	if (!m_bVisible) return;

	/* Draw the torpedo to the foreground */
	m_torpedo.RenderImage(m_pEngine->GetForeground(), 0, 0, m_currentScreenXPrecise + 1, m_currentScreenYPrecise + 1, m_torpedo.GetWidth(), m_torpedo.GetHeight());
}

void Torpedo::DoUpdate(int elapsedTime) {
	m_iPreviousScreenX = m_iCurrentScreenX;
	m_iPreviousScreenY = m_iCurrentScreenY;

	/* Horizontal torpedo movement */
	m_currentScreenXPrecise += (50.0 * elapsedTime / 1000.0);
	m_iCurrentScreenX = static_cast<int>(m_currentScreenXPrecise);

	/* Home in on the target provided we can see it */
	if (m_currentScreenXPrecise + m_torpedo.GetWidth() < m_target->GetXCentre()) {
		if (m_currentScreenYPrecise < m_target->GetYCentre() - 0.1) {
			m_currentScreenYPrecise += (40.0 * elapsedTime / 1000.0);
		} else if (m_currentScreenYPrecise > m_target->GetYCentre() + 0.1) {
			m_currentScreenYPrecise -= (40.0 * elapsedTime / 1000.0);
		}

		/* Save the y coordinate */
		m_iCurrentScreenY = static_cast<int>(m_currentScreenYPrecise);
	} else {
		m_needsRetarget = true;
	}

	/* Update the redraw rectangles */
	StoreLastScreenPositionAndUpdateRect();
}