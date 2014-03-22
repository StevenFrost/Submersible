#include <cmath>
#include "Waves.h"
#include "header.h"
#include "Terrain.h"
#include "JPGImage.h"
#include "StatusBar.h"
#include "Submarine.h"
#include "DialogBox.h"
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

#define FOREGROUND_TERRAIN_SPEED  70.0
#define BACKGROUND_TERRAIN_SPEED  20.0
#define WAVE_SPEED                80.0

MyProjectMain::MyProjectMain() : BaseEngine(MAX_OBJECTS),
	m_fpsTarget(60),
	m_gameState(MENU),
	m_pausedTime(0),
	m_startTime(0),
	m_paused(false) {}

MyProjectMain::~MyProjectMain() {
	delete m_objectManager;
}

int MyProjectMain::GameInit() {
	InitialiseObjects();
	updateDisplayableObjectArray();
	SetupBackgroundBuffer();

	return 0;
}

int MyProjectMain::InitialiseObjects() {
	DestroyOldObjects();

	/* Displayable object array initialisation */
	m_ppDisplayableObjects = new DisplayableObject*[MAX_OBJECTS];
	m_objectManager = new GameObjectManager(this, MAX_OBJECTS - PERMINANT_OBJECTS);

	/* Background terrain initialisation */
	m_backgroundTerrain = new Terrain(this, GetScreenWidth(), 600, BACKGROUND_TERRAIN_COLOUR);
	m_backgroundTerrain->initialise();
	m_backgroundTerrain->setSpeed(BACKGROUND_TERRAIN_SPEED);

	/* Foreground terrain initialisation */
	m_foregroundTerrain = new Terrain(this, GetScreenWidth(), 400, FOREGROUND_TERRAIN_COLOUR);
	m_foregroundTerrain->initialise();
	m_foregroundTerrain->setSpeed(FOREGROUND_TERRAIN_SPEED);

	/* Submarine, waves and the status bar */
	m_sub = new Submarine(this, 100, 250);
	m_waves = new Waves(this, WAVE_SPEED);
	m_statusBar = new StatusBar(this);
	m_statusBar->initialise();

	/* Dialog boxes */
	m_mainBox = new MainDialogBox(this, 550, 300);
	m_crashBox = new CrashedDialogBox(this, m_statusBar, 550, 300);
	m_pausedBox = new PausedDialogBox(this, 550, 300);
	m_helpBox = new HelpDialogBox(this, 650, 420);

	/* Observables */
	m_foregroundTerrain->attach(m_objectManager);

	return 0;
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

void MyProjectMain::CleanUp(void) {}

void MyProjectMain::GameAction() {
	static int lastFrameTime = GetModifiedTime();
	int thisFrameTime = GetModifiedTime();
	int elapsedTime = thisFrameTime - lastFrameTime;
	lastFrameTime = thisFrameTime;

	switch (m_gameState) {
	case PLAYING:
		playingAction(elapsedTime);
		break;
	}

	/* Update and redraw all displayable objects */
	UpdateAllObjects(elapsedTime);
	Redraw(false);
	//SDL_Delay(1);
}

void MyProjectMain::playingAction(int elapsedTime) {
	/* Update the status bar */
	m_statusBar->incrementDistance(m_foregroundTerrain->getSpeed() * PIXELS_TO_M * (elapsedTime / 1000.0));
	m_statusBar->incrementTime(elapsedTime / 1000.0);

	if (m_foregroundTerrain->getJustGenerated()) {
		updateDisplayableObjectArray();
	}

	/* Collision detection */
	bool fatalCollision = false;
	if (Collision::boundingBox(m_sub, m_foregroundTerrain)) {
		fatalCollision |= Collision::surface(2, m_sub->getSurface(), m_foregroundTerrain->getMainSurface(),
			m_sub->getXPosition(), m_sub->getYPosition(), m_foregroundTerrain->getOffset(), 350);
		fatalCollision |= Collision::surface(2, m_sub->getSurface(), m_foregroundTerrain->getBufferSurface(),
			m_sub->getXPosition(), m_sub->getYPosition(), 1270 + m_foregroundTerrain->getOffset(), 350);
	}

	/* If a fatal collision occurred, we need to stop the game */
	if (fatalCollision) {
		m_crashBox->setTitle("CRASHED");
		changeGameState(CRASHED);
	} else if (m_sub->getFuel() <= 0.1) {
		m_crashBox->setTitle("OUT OF FUEL");
		changeGameState(CRASHED);
	}
}

void MyProjectMain::KeyDown(int keyCode) {
	/* Key events common in all states */
	switch (keyCode) {
	case SDLK_ESCAPE:
		SetExitWithCode(0);
		break;
	}

	/* Game state specific key events */
	switch (m_gameState) {
	case PLAYING:
		playingKeyEvent(keyCode);
		break;
	case CRASHED:
		crashedKeyEvent(keyCode);
		break;
	case PAUSED:
		pausedKeyEvent(keyCode);
		break;
	case HELP:
		helpKeyEvent(keyCode);
		break;
	case MENU:
		menuKeyEvent(keyCode);
		break;
	}
}

void MyProjectMain::menuKeyEvent(int keyCode) {
	switch (keyCode) {
	case SDLK_p:
		changeGameState(PLAYING);
		m_sub->setFuelLevel(100);
		m_statusBar->resetTime();
		m_statusBar->resetDistance();
		m_statusBar->resetPoints();
		m_sub->setSubPosition(100, 250);
		m_sub->setXVelocity(0);
		m_sub->setYVelocity(0);
		break;
	case SDLK_h:
		changeGameState(HELP);
		break;
	}
}

void MyProjectMain::pausedKeyEvent(int keyCode) {
	switch (keyCode) {
	case SDLK_p:
		changeGameState(PLAYING);
		unpauseTimer();
		m_sub->setImmobilised(false);
		break;
	}
}

void MyProjectMain::helpKeyEvent(int keyCode) {
	switch (keyCode) {
	case SDLK_h:
		changeGameState(MENU);
		break;
	}
}

void MyProjectMain::crashedKeyEvent(int keyCode) {
	switch (keyCode) {
	case SDLK_r:
		changeGameState(PLAYING);
		m_sub->setFuelLevel(100);
		m_statusBar->resetTime();
		m_statusBar->resetDistance();
		m_statusBar->resetPoints();
		m_sub->setSubPosition(100, 250);
		m_sub->setXVelocity(0);
		m_sub->setYVelocity(0);
		break;
	case SDLK_m:
		changeGameState(MENU);
		m_statusBar->resetTime();
		m_statusBar->resetDistance();
		m_sub->setFuelLevel(100);
		break;
	}
}

void MyProjectMain::playingKeyEvent(int keyCode) {
	switch (keyCode) {
	case SDLK_h:
		changeGameState(HELP);
		break;
	case SDLK_p:
		changeGameState(PAUSED);
		pauseTimer();
		break;
	}
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
	DisplayableObject **gameObjects;
	m_ppDisplayableObjects[objID++] = m_backgroundTerrain;
	
	/* Add all game objects for the first terrain buffer */
	gameObjects = m_objectManager->getWaveObjectsBuffer1();
	for (int i = 0; i < m_objectManager->getNumWaveObjectsBuffer1(); i++) {
		if (gameObjects[i] != NULL) {
			m_ppDisplayableObjects[objID++] = gameObjects[i];
		}
	}

	/* Add all game objects for the second terrain buffer */
	gameObjects = m_objectManager->getWaveObjectsBuffer2();
	for (int i = 0; i < m_objectManager->getNumWaveObjectsBuffer2(); i++) {
		if (gameObjects[i] != NULL) {
			m_ppDisplayableObjects[objID++] = gameObjects[i];
		}
	}

	m_ppDisplayableObjects[objID++] = m_foregroundTerrain;
	m_ppDisplayableObjects[objID++] = m_waves;
	m_ppDisplayableObjects[objID++] = m_statusBar;

	/* State depentant objects */
	switch (m_gameState) {
	case PLAYING:
		m_ppDisplayableObjects[objID++] = m_sub;
		break;
	case PAUSED:
		m_ppDisplayableObjects[objID++] = m_sub;
		m_ppDisplayableObjects[objID++] = m_pausedBox;
		break;
	case CRASHED:
		m_ppDisplayableObjects[objID++] = m_sub;
		m_ppDisplayableObjects[objID++] = m_crashBox;
		break;
	case HELP:
		m_ppDisplayableObjects[objID++] = m_helpBox;
		break;
	case MENU:
		m_ppDisplayableObjects[objID++] = m_mainBox;
		break;
	}

	/* End of array */
	m_ppDisplayableObjects[objID++] = NULL;
	DrawableObjectsChanged();
}

void MyProjectMain::changeGameState(GameState state) {
	m_gameState = state;
	updateDisplayableObjectArray();

	switch (state) {
	case PLAYING:
		m_foregroundTerrain->setSpeed(FOREGROUND_TERRAIN_SPEED);
		m_backgroundTerrain->setSpeed(BACKGROUND_TERRAIN_SPEED);
		m_waves->setSpeed(WAVE_SPEED);
		m_sub->setImmobilised(false);
		break;
	case MENU:
	case HELP:
		m_foregroundTerrain->setSpeed(FOREGROUND_TERRAIN_SPEED);
		m_backgroundTerrain->setSpeed(BACKGROUND_TERRAIN_SPEED);
		m_waves->setSpeed(WAVE_SPEED);
		m_sub->setImmobilised(true);
		break;
	case PAUSED:
	case CRASHED:
		m_foregroundTerrain->setSpeed(0);
		m_backgroundTerrain->setSpeed(0);
		m_sub->setImmobilised(true);
		m_waves->setSpeed(0);
		break;
	}

	/* Instruct the engine to redraw the entire screen */
	Redraw(true);
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

int MyProjectMain::getStringWidth(const char *str, int size) {
	int width, height;
	TTF_Font *fnt = GetFont("../resources/Segoe UI.ttf", size)->GetFont();
	TTF_SizeText(fnt, str, &width, &height);

	return width;
}

int MyProjectMain::getStringHeight(const char *str, int size) {
	int width, height;
	TTF_Font *fnt = GetFont("../resources/Segoe UI.ttf", size)->GetFont();
	TTF_SizeText(fnt, str, &width, &height);

	return height;
}

int MyProjectMain::GetModifiedTime() {
	if (m_paused) {
		return m_pausedTime;
	} else {
		return GetTime() - m_startTime;
	}
}

void MyProjectMain::pauseTimer() {
	if (!m_paused) {
		m_paused = true;
		m_pausedTime = GetTime() - m_startTime;
	}
}

void MyProjectMain::unpauseTimer() {
	if (m_paused) {
		m_paused = false;
		m_startTime = GetTime() - m_pausedTime;
		m_pausedTime = 0;
	}
}