#include "Flare.h"

Image *Flare::m_flare = new Image();

Flare::Flare(MyProjectMain *engine, int x, int y) : DisplayableObject(engine), m_flareSpriteOffset(0), m_totalTimeElapsed(0) {
	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;
	m_currentScreenXPrecise = m_iCurrentScreenX = m_iPreviousScreenX = x;
	m_currentScreenYPrecise = m_iCurrentScreenY = m_iPreviousScreenY = y;

	if (!m_flare->IsLoaded()) {
		m_flare->LoadImage("../resources/flare.png");
	}

	m_iDrawWidth = m_flare->GetWidth() + 1;
	m_iDrawHeight = m_flare->GetHeight() + 1;

	m_bVisible = false;
}

Flare::~Flare() {}

void Flare::Draw() {
	if (!m_bVisible) return;

	/* Draw the coin */
	m_flare->RenderImage(m_pEngine->GetForeground(), m_flareSpriteOffset, 0, m_currentScreenXPrecise, m_currentScreenYPrecise, 26, 26);
}

void Flare::DoUpdate(int elapsedTime) {
	if (!m_bVisible) return;

	/* Update the flare position */
	m_currentScreenXPrecise -= (m_velocity * elapsedTime / 1000.0);
	m_currentScreenYPrecise += (20.0 * elapsedTime / 1000.0);

	if (m_velocity < 70.0) {
		m_velocity += (20.0 * elapsedTime / 1000.0);
	}

	/* Animate the flare */
	animate(elapsedTime);

	/* Keep the less precise version updated, we need it for redrawing */
	m_iCurrentScreenX = static_cast<int>(m_currentScreenXPrecise);
	m_iCurrentScreenY = static_cast<int>(m_currentScreenYPrecise);

	/* Update the redraw rectangles */
	StoreLastScreenPositionAndUpdateRect();
}

void Flare::animate(int elapsedTime) {
	m_totalTimeElapsed += elapsedTime;
	
	/* The flare has a lifetime of ~5 seconds */
	if (m_totalTimeElapsed < 3000.0) {
		return;
	} else if (m_totalTimeElapsed > 4000.0) {
		m_flareSpriteOffset = 0;
		m_totalTimeElapsed = 0;
		m_bVisible = false;
		return;
	}

	int mod = m_totalTimeElapsed % 1000;
	if (mod < 100) {
		m_flareSpriteOffset = 0;
	} else if (mod < 200) {
		m_flareSpriteOffset = 26;
	} else if (mod < 300) {
		m_flareSpriteOffset = 52;
	} else if (mod < 400) {
		m_flareSpriteOffset = 78;
	} else if (mod < 500) {
		m_flareSpriteOffset = 104;
	} else if (mod < 600) {
		m_flareSpriteOffset = 130;
	} else if (mod < 700) {
		m_flareSpriteOffset = 156;
	} else if (mod < 800) {
		m_flareSpriteOffset = 182;
	} else if (mod < 900) {
		m_flareSpriteOffset = 208;
	} else {
		m_flareSpriteOffset = 234;
	}
}

void Flare::RedrawBackground() {
	SDL_Rect *rect = new SDL_Rect();
	GetRedrawRect(rect);
	m_pEngine->CopyBackgroundPixels(rect->x, rect->y, rect->w, rect->h);
}

void Flare::GetRedrawRect(SDL_Rect *rectangle) {
	rectangle->x = m_iCurrentScreenX;
	rectangle->y = m_iCurrentScreenY;
	rectangle->w = m_iDrawWidth;
	rectangle->h = m_iDrawHeight;
}