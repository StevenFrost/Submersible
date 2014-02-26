#include <cmath>
#include "Waves.h"
#include "header.h"
#include "Terrain.h"
#include "JPGImage.h"
#include "StatusBar.h"
#include "Submarine.h"
#include "NavalMine.h"
#include "BaseEngine.h"
#include "TileManager.h"
#include "MyProjectMain.h"
#include "DisplayableObject.h"

#include "Collision.h"

/* General private properties */
static const unsigned int SKY_COLOUR = 0x3990C6;
static const unsigned int WATER_COLOUR = 0x2F76A2;
static const unsigned int FOREGROUND_TERRAIN_COLOUR = 0xFF28485D;
static const unsigned int BACKGROUND_TERRAIN_COLOUR = 0xFF2E6D94;

#define PIXELS_TO_M 0.153
#define MAX_OBECTS 30

MyProjectMain::MyProjectMain() : BaseEngine(MAX_OBECTS), m_fpsTarget(60) {}

void MyProjectMain::SetupBackgroundBuffer() {
	/* Start by drawing the sky background */
	DrawRectangle(0, 0, GetScreenWidth(), 120, SKY_COLOUR, GetBackground());

	/* Now we draw the sun and help effect */
	m_sun->RenderImage(GetBackground(), 0, 0, GetScreenWidth() - m_sun->GetWidth(), 0, m_sun->GetWidth(), m_sun->GetHeight());

	/* Draw the status bar background */
	m_statusBar->drawBackground();

	/* Finally, draw the water background */
	DrawRectangle(0, 120, GetScreenWidth(), GetScreenHeight(), WATER_COLOUR, GetBackground());
}

int MyProjectMain::InitialiseObjects() {
	DrawableObjectsChanged();
	DestroyOldObjects();

	m_ppDisplayableObjects = new DisplayableObject*[MAX_OBECTS];

	/* Terrain initialisation */
	m_backgroundTerrain = new Terrain(this, GetScreenWidth(), 600, BACKGROUND_TERRAIN_COLOUR);
	m_backgroundTerrain->initialise();
	m_backgroundTerrain->setSpeed(20.0);
	m_ppDisplayableObjects[0] = m_backgroundTerrain;

	m_mine = new NavalMine(this);
	m_ppDisplayableObjects[1] = m_mine;

	m_foregroundTerrain = new Terrain(this, GetScreenWidth(), 400, FOREGROUND_TERRAIN_COLOUR);
	m_foregroundTerrain->initialise();
	m_ppDisplayableObjects[2] = m_foregroundTerrain;

	/* Load the sun graphic */
	m_sun = new ImageSurface();
	m_sun->LoadImage("../resources/sun.png");

	/* Load the submarine */
	m_sub = new Submarine(this, 100, 250);
	m_ppDisplayableObjects[3] = m_sub;

	/* Load the waves */
	m_waves = new Waves(this);
	m_ppDisplayableObjects[4] = m_waves;

	/* Initialise the status bar */
	m_statusBar = new StatusBar(this);
	m_ppDisplayableObjects[5] = m_statusBar;

	m_ppDisplayableObjects[6] = NULL;

	return 0;
}

void MyProjectMain::GameAction() {
	static int lastFrameTime = GetTime();
	int thisFrameTime = GetTime();
	int elapsedTime = thisFrameTime - lastFrameTime;
	lastFrameTime = thisFrameTime;

	/* Update the travelled distance */
	m_statusBar->incrementDistance(m_foregroundTerrain->getSpeed() * PIXELS_TO_M * (elapsedTime / 1000.0));

	/* Collision detection */
	Collision::boundingBox(m_sub, m_foregroundTerrain);

	UpdateAllObjects(elapsedTime);
	Redraw(false);

	SDL_Delay(1);
}

void MyProjectMain::GetUpdateRectanglesForChangingObjects() {
	// TODO: Override this to take into account that some objects do not
	//       inherit from DisplayableObject, so we need to manually set the
	//       rectangles for these
	SDL_Rect* pRect;

	if (m_ppDisplayableObjects != NULL) {
		for (int i = 0; m_ppDisplayableObjects[i] != NULL; i++) {
			pRect = GetNextUpdateRect();
			m_ppDisplayableObjects[i]->GetRedrawRect(pRect);
		}
	}
}

void MyProjectMain::KeyDown(int iKeyCode) {
	switch (iKeyCode) {
	case SDLK_ESCAPE:
		SetExitWithCode(0);
		break;
	case SDLK_p:
		m_statusBar->incrementPoints();
		break;
	}
}

void MyProjectMain::printDebugInformation() {

}