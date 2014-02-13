#include "Submarine.h"

ImageSurface *Submarine::m_body = new ImageSurface();

Submarine::Submarine(MyProjectMain *pEngine, unsigned int x, unsigned int y) : DisplayableObject(pEngine) {
	/* Set the initial position */
	m_iCurrentScreenX = x;
	m_iCurrentScreenY = y;

	/* Load the graphic */
	if (!m_body->IsLoaded()) {
		m_body->LoadImage("../resources/sub.png");
	}
}

Submarine::~Submarine() {
	delete(m_body);
}

void Submarine::Draw() {
	/* Draw the main submarine body */
	m_body->RenderImage(m_pEngine->GetBackground(), 0, 0, m_iCurrentScreenX, m_iCurrentScreenY, m_body->GetWidth(), m_body->GetHeight());

	/* Draw the lights is required */
	if (m_lightsOn) {
		// ...
	}

	/* Draw the jet at the rear of the sub */
	// ...
}

void Submarine::DoUpdate(int iCurrentTime) {
}