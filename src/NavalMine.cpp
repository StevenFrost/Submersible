#include "NavalMine.h"
#include "Terrain.h"
#include "MyProjectMain.h"

Image *NavalMine::m_mine = new Image();
Image *NavalMine::m_chain = new Image();
Image *NavalMine::m_lights = new Image();

NavalMine::NavalMine(MyProjectMain *engine, int x, int y, double speed) : DisplayableObject(engine), m_totalTimeElapsed(0), m_speed(speed){
	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;
	m_iCurrentScreenX = x;
	m_iCurrentScreenY = y;
	m_currentScreenXPrecise = m_iCurrentScreenX;
	m_currentScreenYPrecise = m_iCurrentScreenY;
	m_iPreviousScreenX = m_iCurrentScreenX;
	m_iPreviousScreenY = m_iCurrentScreenY;

	if (!m_mine->IsLoaded()) {
		m_mine->LoadImage("../resources/mine.png");
	}
	if (!m_chain->IsLoaded()) {
		m_chain->LoadImage("../resources/chain.png");
	}
	if (!m_lights->IsLoaded()) {
		m_lights->LoadImage("../resources/lights-red.png");
	}

	m_iDrawWidth = 98;
	m_iDrawHeight = m_pEngine->GetScreenHeight() - m_iCurrentScreenY;
}

NavalMine::~NavalMine() {}

void NavalMine::Draw() {
	/* Draw the mine body */
	m_mine->RenderImage(m_pEngine->GetForeground(), 0, 0, m_currentScreenXPrecise + 6, m_currentScreenYPrecise + 6, m_mine->GetWidth(), m_mine->GetHeight());

	/* Draw the lights */
	m_lights->RenderImage(m_pEngine->GetForeground(), m_lightSpriteOffset, 0, m_currentScreenXPrecise + 42, m_currentScreenYPrecise, 15, 14);
	m_lights->RenderImage(m_pEngine->GetForeground(), m_lightSpriteOffset, 0, m_currentScreenXPrecise + 12, m_currentScreenYPrecise + 11, 15, 14);
	m_lights->RenderImage(m_pEngine->GetForeground(), m_lightSpriteOffset, 0, m_currentScreenXPrecise + 84, m_currentScreenYPrecise + 40, 15, 14);
	m_lights->RenderImage(m_pEngine->GetForeground(), m_lightSpriteOffset, 0, m_currentScreenXPrecise + 73, m_currentScreenYPrecise + 68, 15, 14);
	m_lights->RenderImage(m_pEngine->GetForeground(), m_lightSpriteOffset, 0, m_currentScreenXPrecise + 11, m_currentScreenYPrecise + 68, 15, 14);
	m_lights->RenderImage(m_pEngine->GetForeground(), m_lightSpriteOffset, 0, m_currentScreenXPrecise + 0, m_currentScreenYPrecise + 40, 15, 14);
	m_lights->RenderImage(m_pEngine->GetForeground(), m_lightSpriteOffset, 0, m_currentScreenXPrecise + 71, m_currentScreenYPrecise + 11, 15, 14);

	/* Draw the chain to the bottom of the screen */
	for (int i = m_iCurrentScreenY + 86; i < m_pEngine->GetScreenHeight(); i += m_chain->GetHeight()) {
		m_chain->RenderImage(m_pEngine->GetForeground(), 0, 0, m_currentScreenXPrecise + 47, i, m_chain->GetWidth(), m_chain->GetHeight());
	}
}

void NavalMine::DoUpdate(int elapsedTime) {
	// We can't use m_iCurrentScreenX as it's too inaccurate and we can't sync
	// mines with the terrain. Instead, we use the more accurate member
	// m_currentScreenXPrecise
	m_currentScreenXPrecise -= (70.0 * elapsedTime / 1000.0);
	m_currentScreenYPrecise -= (0.0 * elapsedTime / 1000.0);

	if (m_currentScreenXPrecise < (0 - m_mine->GetWidth() - 6)) {
		m_currentScreenXPrecise = m_pEngine->GetScreenWidth() + 6;
		m_currentScreenYPrecise = (rand() % (500 - 200)) + 200;
	}

	/* Animate the mine lights */
	animateLights(elapsedTime);

	/* Keep the less precise version updated, we need it for redrawing */
	m_iCurrentScreenX = static_cast<int>(m_currentScreenXPrecise);
	m_iCurrentScreenY = static_cast<int>(m_currentScreenYPrecise);

	/* Update the redraw rectangles */
	StoreLastScreenPositionAndUpdateRect();
}

void NavalMine::animateLights(int elapsedTime) {
	m_totalTimeElapsed += elapsedTime;

	int mod = m_totalTimeElapsed % 1000;

	if (mod < 100) {
		m_lightSpriteOffset = 0;
	} else if (mod < 200) {
		m_lightSpriteOffset = 15;
	} else if (mod < 300) {
		m_lightSpriteOffset = 30;
	} else if (mod < 400) {
		m_lightSpriteOffset = 45;
	} else if (mod < 500) {
		m_lightSpriteOffset = 60;
	} else if (mod < 600) {
		m_lightSpriteOffset = 75;
	} else if (mod < 700) {
		m_lightSpriteOffset = 90;
	} else if (mod < 800) {
		m_lightSpriteOffset = 105;
	} else if (mod < 900) {
		m_lightSpriteOffset = 120;
	} else {
		m_lightSpriteOffset = 135;
	}
}

void NavalMine::RedrawBackground() {
	SDL_Rect *rect = new SDL_Rect();
	GetRedrawRect(rect);
	m_pEngine->CopyBackgroundPixels(rect->x, rect->y, rect->w, rect->h);
}