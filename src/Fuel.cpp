#include "Fuel.h"

Image Fuel::m_fuel = Image();

Fuel::Fuel(BaseEngine *engine, int x, int y) : GameObject(engine, Type::FUEL) {
	/* GameObject member initialisation */
	m_bVisible = true;
	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;
	m_currentScreenXPrecise = m_iCurrentScreenX = m_iPreviousScreenX = x;
	m_currentScreenYPrecise = m_iCurrentScreenY = m_iPreviousScreenY = y;

	/* Load any images that need loading */
	if (!m_fuel.IsLoaded()) {
		m_fuel.LoadImage("../resources/fuel-can.png");
	}

	/* Redraw rectangle width/height */
	m_iDrawWidth = m_fuel.GetWidth() + 1;
	m_iDrawHeight = m_fuel.GetHeight() + 1;
}

void Fuel::Draw() {
	if (!m_bVisible) return;

	/* Draw the fuel can to the foreground surface */
	m_fuel.RenderImage(m_pEngine->GetForeground(), 0, 0, m_currentScreenXPrecise, m_currentScreenYPrecise, m_fuel.GetWidth(), m_fuel.GetHeight());
}

void Fuel::DoUpdate(int elapsedTime) {
	/* Store the previous x-position */
	m_iPreviousScreenX = m_iCurrentScreenX;

	/* Update the fuel can x-position */
	m_currentScreenXPrecise -= (70.0 * elapsedTime / 1000.0);
	m_iCurrentScreenX = static_cast<int>(m_currentScreenXPrecise);

	/* Update the redraw rectangle */
	StoreLastScreenPositionAndUpdateRect();
}