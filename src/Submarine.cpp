#include "Submarine.h"

ImageSurface *Submarine::m_body = new ImageSurface();

Submarine::Submarine(MyProjectMain *engine, unsigned int x, unsigned int y) : DisplayableObject(engine), m_friction(0.90) {
	/* Set the initial position */
	m_iCurrentScreenX = x;
	m_iCurrentScreenY = y;

	m_iStartDrawPosX = 0;
	m_iPreviousScreenX = m_iCurrentScreenX;
	m_iStartDrawPosY = 0;
	m_iPreviousScreenY = m_iCurrentScreenY;

	/* Load the graphic */
	if (!m_body->IsLoaded()) {
		m_body->LoadImage("../resources/sub.png");
	}

	m_iDrawWidth = m_body->GetWidth();
	m_iDrawHeight = m_body->GetHeight();
}

Submarine::~Submarine() {
	delete(m_body);
}

void Submarine::Draw() {
	/* Draw the main submarine body */
	m_body->RenderImage(m_pEngine->GetForeground(), 0, 0, m_iCurrentScreenX, m_iCurrentScreenY, m_body->GetWidth(), m_body->GetHeight());

	StoreLastScreenPositionAndUpdateRect();

	/* Draw the lights if required */
	if (m_lightsOn) {
		// ...
	}

	/* Draw the jet at the rear of the sub */
	// ...
}

void Submarine::DoUpdate(int elapsedTime) {
	if (m_iCurrentScreenY < 100) {
		m_iCurrentScreenY = 100;
		m_yDelta = 0;
		return;
	}
	
	if (m_iCurrentScreenY > 500) {
		m_iCurrentScreenY = 500;
		m_yDelta = 0;
		return;
	}
	
	if (m_iCurrentScreenX < 50) {
		m_iCurrentScreenX = 50;
		m_xDelta = 0;
	}

	if (m_iCurrentScreenX > 1050) {
		m_iCurrentScreenX = 1050;
		m_xDelta = 0;
	}

	m_yDelta *= m_friction;
	m_iCurrentScreenY += (m_yDelta * elapsedTime / 1000.0);

	m_xDelta *= m_friction;
	m_iCurrentScreenX += (m_xDelta * elapsedTime / 1000.0);
}