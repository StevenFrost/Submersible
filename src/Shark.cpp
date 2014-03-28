#include "Shark.h"

Image *Shark::m_shark = new Image();

Shark::Shark(MyProjectMain *engine, int x) : DisplayableObject(engine) {
	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;
	m_currentScreenXPrecise = m_iCurrentScreenX = m_iPreviousScreenX = x;
	m_currentScreenYPrecise = m_iCurrentScreenY = m_iPreviousScreenY = 120 + (rand() % 200);

	if (!m_shark->IsLoaded()) {
		m_shark->LoadImage("../resources/shark.png");
	}

	m_iDrawWidth = m_shark->GetWidth();
	m_iDrawHeight = m_shark->GetHeight();
}

Shark::~Shark() {}

void Shark::Draw() {
	m_shark->RenderImage(m_pEngine->GetForeground(), 0, 0, m_currentScreenXPrecise, m_currentScreenYPrecise, m_shark->GetWidth(), m_shark->GetHeight());
}

void Shark::DoUpdate(int elapsedTime) {
	static int resetPoint = 1100 + (rand() % (1000 - 300) + 300);

	// We can't use m_iCurrentScreenX as it's too inaccurate and we can't sync
	// mines with the terrain. Instead, we use the more accurate member
	// m_currentScreenXPrecise
	m_currentScreenXPrecise += (10.0 * elapsedTime / 1000.0);

	if (m_currentScreenXPrecise > resetPoint) {
		resetPoint = (1100 + rand() % (1000 - 300) + 300);
		m_currentScreenXPrecise = 0 - m_shark->GetWidth();
		m_currentScreenYPrecise = 120 + (rand() % 200);
	}

	/* Keep the less precise version updated, we need it for redrawing */
	m_iCurrentScreenX = static_cast<int>(m_currentScreenXPrecise);

	/* Update the redraw rectangles */
	StoreLastScreenPositionAndUpdateRect();
}