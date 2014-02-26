#include "Submarine.h"

Image *Submarine::m_body = new Image();

Submarine::Submarine(MyProjectMain *engine, unsigned int x, unsigned int y) : DisplayableObject(engine), m_friction(0.96), m_fuel(100.0) {
	/* Set the initial position */
	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;
	m_iCurrentScreenX = x;
	m_iCurrentScreenY = y;
	m_iPreviousScreenX = m_iCurrentScreenX;
	m_iPreviousScreenY = m_iCurrentScreenY;

	/* Load the graphic */
	if (!m_body->IsLoaded()) {
		m_body->LoadImage("../resources/sub.png");
	}

	/* Set the width and height of the rectangle to draw */
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
}

void Submarine::DoUpdate(int elapsedTime) {
	controlSub();

	/* Limitations */
	if (m_iCurrentScreenY < 125) {
		m_iCurrentScreenY = 125;
		m_yDelta = 0;
		return;
	}

	/* Apply a frictional force */
	m_yDelta *= m_yDelta < 0.0 ? (1.0 - m_friction) : m_friction;
	m_xDelta *= m_xDelta < 0.0 ? (1.0 - m_friction) : m_friction;

	/* Update the submarine position */
	m_iCurrentScreenY += (m_yDelta * elapsedTime / 1000.0);
	m_iCurrentScreenX += (m_xDelta * elapsedTime / 1000.0);

	/* Calculate the quantity of fuel remaining */
	m_fuel -= (0.2 * (elapsedTime / 100.0));

	/* Notify any observers that something changed */
	notify();
}

void Submarine::controlSub() {
	if (m_pEngine->IsKeyPressed(SDLK_RIGHT)) {
		setXDelta(100);
	} else if (m_pEngine->IsKeyPressed(SDLK_LEFT)) {
		setXDelta(-100);
	}

	if (m_pEngine->IsKeyPressed(SDLK_UP)) {
		setYDelta(-100);
	} else if (m_pEngine->IsKeyPressed(SDLK_DOWN)) {
		setYDelta(100);
	}
}