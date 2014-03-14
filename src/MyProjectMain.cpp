#include <cmath>
#include "Waves.h"
#include "Button.h"
#include "header.h"
#include "Terrain.h"
#include "JPGImage.h"
#include "StatusBar.h"
#include "Submarine.h"
#include "NavalMine.h"
#include "Collision.h"
#include "BaseEngine.h"
#include "MyProjectMain.h"
#include "DisplayableObject.h"
#include "GameObjectManager.h"

/* General properties */
#define SKY_COLOUR                0x3990C6
#define WATER_COLOUR              0x2F76A2
#define FOREGROUND_TERRAIN_COLOUR 0xFF28485D
#define BACKGROUND_TERRAIN_COLOUR 0xFF2E6D94
#define PIXELS_TO_M               0.153
#define MAX_OBJECTS               30
#define PERMINANT_OBJECTS         6

MyProjectMain::MyProjectMain() : BaseEngine(MAX_OBJECTS), m_fpsTarget(60), m_gameState(MENU), m_menuState(MENU_PLAY) {}
MyProjectMain::~MyProjectMain() {
	delete m_objectManager;
}

void MyProjectMain::SetupBackgroundBuffer() {
	/* Start by drawing the sky background */
	DrawRectangle(0, 0, GetScreenWidth(), 120, SKY_COLOUR, GetBackground());

	/* Now we draw the sun and helo effect */
	ImageSurface *sun = new ImageSurface();
	sun->LoadImage("../resources/sun.png");
	sun->RenderImage(GetBackground(), 0, 0, GetScreenWidth() - sun->GetWidth(), 0, sun->GetWidth(), sun->GetHeight());
	delete sun;

	/* Draw the status bar background */
	m_statusBar->drawBackground();

	/* Finally, draw the water background */
	DrawRectangle(0, 120, GetScreenWidth(), GetScreenHeight(), WATER_COLOUR, GetBackground());
}

int MyProjectMain::InitialiseObjects() {
	DestroyOldObjects();

	/* Displayable object array initialisation */
	m_ppDisplayableObjects = new DisplayableObject*[MAX_OBJECTS];
	m_objectManager = new GameObjectManager(this, MAX_OBJECTS - PERMINANT_OBJECTS);

	/* Background terrain initialisation */
	m_backgroundTerrain = new Terrain(this, GetScreenWidth(), 600, BACKGROUND_TERRAIN_COLOUR);
	m_backgroundTerrain->initialise();
	m_backgroundTerrain->setSpeed(20.0);
	
	/* Foreground terrain initialisation */
	m_foregroundTerrain = new Terrain(this, GetScreenWidth(), 400, FOREGROUND_TERRAIN_COLOUR);
	m_foregroundTerrain->initialise();
	m_foregroundTerrain->setSpeed(70.0);

	/* Submarine, waves and the status bar */
	m_sub = new Submarine(this, 100, 250);
	m_waves = new Waves(this);
	m_statusBar = new StatusBar(this);
	m_statusBar->initialise();
	
	/* Menu objects */
	m_menuPlay = new Button(this, 300, 200, "Play", GetFont("../resources/Segoe UI.ttf", 42), true);
	m_menuHelp = new Button(this, 667, 200, "Help", GetFont("../resources/Segoe UI.ttf", 42), false);
	return 0;
}

int MyProjectMain::GameInit() {
	InitialiseObjects();
	updateDisplayableObjectArray();
	SetupBackgroundBuffer();

	return 0;
}

void MyProjectMain::CleanUp(void) {

}

void MyProjectMain::GameAction() {
	static int lastFrameTime = GetTime();
	int thisFrameTime = GetTime();
	int elapsedTime = thisFrameTime - lastFrameTime;
	lastFrameTime = thisFrameTime;

	switch (m_gameState) {
	case PLAYING:
		playingAction(elapsedTime);
		break;
	case MENU:
		menuAction(elapsedTime);
		break;
	}

	/* Update and redraw all displayable objects */
	UpdateAllObjects(elapsedTime);
	Redraw(false);
	//SDL_Delay(1);
}

void MyProjectMain::menuAction(int elapsedTime) {
	switch (m_menuState) {
	case MENU_PLAY:
		m_menuPlay->setSelected(true);
		m_menuHelp->setSelected(false);
		break;
	case MENU_HELP:
		m_menuHelp->setSelected(true);
		m_menuPlay->setSelected(false);
		break;
	}
}

void MyProjectMain::playingAction(int elapsedTime) {
	/* Update the travelled distance */
	m_statusBar->incrementDistance(m_foregroundTerrain->getSpeed() * PIXELS_TO_M * (elapsedTime / 1000.0));

	/* Collision detection */
	Collision::boundingBox(m_sub, m_foregroundTerrain);
}

void MyProjectMain::KeyDown(int keyCode) {
	switch (keyCode) {
	case SDLK_ESCAPE:
		SetExitWithCode(0);
		break;
	case SDLK_RETURN:
		if (m_gameState == MENU) {
			if (m_menuState == MENU_PLAY) {
				m_gameState = PLAYING;
				updateDisplayableObjectArray();
				Redraw(true);
			} else if (m_menuState == MENU_HELP) {
				m_gameState = HELP;
			}
		}
		break;
	case SDLK_h:
		m_gameState = HELP;
		break;
	case SDLK_p:
		m_gameState = PAUSED;
		break;
	case SDLK_LEFT:
		if (m_menuState == MENU_HELP) {
			m_menuState = MENU_PLAY;
		}
		break;
	case SDLK_RIGHT:
		if (m_menuState == MENU_PLAY) {
			m_menuState = MENU_HELP;
		}
		break;
	}
}

void MyProjectMain::printDebugInformation() {
	
}

void MyProjectMain::updateDisplayableObjectArray() {
	/* There are several displayable objects that are perminant and visible in all
	 * game states. These include the background and foreground terrain, waves and
	 * the status bar.
	 * Game objects are inserted after the background terrain and before the
	 * foreground terrain to avoid drawing issues. Any state-dependant displayable
	 * objects (other than core game objects) are inserted at the end of the array.
	 */
	int objID = 0;
	m_ppDisplayableObjects[objID++] = m_backgroundTerrain;
	DisplayableObject **gameObjects = m_objectManager->getWaveObjects();
	for (objID = 1; objID < m_objectManager->getNumWaveObjects(); objID++) {
		if (gameObjects[objID] != NULL) {
			m_ppDisplayableObjects[objID] = gameObjects[objID - 1];
		}
	}

	m_ppDisplayableObjects[objID++] = m_foregroundTerrain;
	m_ppDisplayableObjects[objID++] = m_waves;
	m_ppDisplayableObjects[objID++] = m_statusBar;

	/* State depentant objects */
	switch (m_gameState) {
	case PLAYING:
	case PAUSED:
	case CRASHED:
	case HELP:
		m_ppDisplayableObjects[objID++] = m_sub;
		break;
	case MENU:
		m_ppDisplayableObjects[objID++] = m_menuPlay;
		m_ppDisplayableObjects[objID++] = m_menuHelp;
		break;
	}

	/* End of array */
	m_ppDisplayableObjects[objID++] = NULL;
	DrawableObjectsChanged();
}

DisplayableObject *MyProjectMain::getStaticObject(StaticGameObject object) const {
	switch (object) {
	case BACKGROUND_TERRAIN:
		return m_backgroundTerrain;
	case FOREGROUND_TERRAIN:
		return m_foregroundTerrain;
	case SUBMARINE:
		return m_sub;
	case WAVES:
		return m_waves;
	case STATUS_BAR:
		return m_statusBar;
	default:
		return NULL;
	}
}