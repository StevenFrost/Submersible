#include <cmath>
#include <fstream>
#include <sstream>
#include <iostream>
#include "header.h"
#include "Torpedo.h"
#include "JPGImage.h"
#include "NavalMine.h"
#include "Collision.h"
#include "BaseEngine.h"
#include "MyProjectMain.h"
#include "DisplayableObject.h"

using std::string;
using std::getline;
using std::ofstream;
using std::ifstream;
using std::istringstream;

/* General properties */
#define SKY_COLOUR                0x3990C6
#define WATER_COLOUR              0x2F76A2
#define FOREGROUND_TERRAIN_COLOUR 0xFF28485D
#define BACKGROUND_TERRAIN_COLOUR 0xFF2E6D94
#define PIXELS_TO_M               0.00276190476
#define MAX_OBJECTS               50
#define COLLISION_RESOLUTION	  2

#define FOREGROUND_TERRAIN_SPEED  70.0
#define BACKGROUND_TERRAIN_SPEED  20.0
#define WAVE_SPEED                80.0

MyProjectMain::MyProjectMain() : BaseEngine(MAX_OBJECTS), m_fpsTarget(60), m_gameState(MENU), m_pausedTime(0), m_startTime(0), m_paused(false) {}

MyProjectMain::~MyProjectMain() {
	delete m_objectManager;
}

int MyProjectMain::GameInit() {
	InitialiseObjects();
	updateDisplayableObjectArray();
	updateHighscoreInformation();
	SetupBackgroundBuffer();

	return 0;
}

int MyProjectMain::InitialiseObjects() {
	DestroyOldObjects();

	/* Displayable object array initialisation */
	m_ppDisplayableObjects = new DisplayableObject*[MAX_OBJECTS];
	m_objectManager = new GameObjectManager(this, MAX_OBJECTS);

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
	m_shark1 = new Shark(this, 1150);
	m_shark2 = new Shark(this, 400);

	/* Configure the status bar */
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

void MyProjectMain::GameAction() {
	static int lastFrameTime = GetModifiedTime();
	static int frame = 0;
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
}

void MyProjectMain::playingAction(int elapsedTime) {
	static int nextDistanceIncrement = 100;

	/* Update the status bar */
	m_statusBar->incrementDistance((m_sub->getXVelocity() + m_sub->getMaxVelocityX()) * m_foregroundTerrain->getSpeed() * PIXELS_TO_M * (elapsedTime / 1000.0));
	m_statusBar->incrementTime(elapsedTime / 1000.0);

	if (m_statusBar->getPoints() > m_statusBar->getHighscore()) {
		m_statusBar->setHighscore(m_statusBar->getPoints());
	}

	/* The user gets 1 point per 100m travelled */
	if (static_cast<int>(m_statusBar->getDistance()) == nextDistanceIncrement) {
		m_statusBar->incrementPoints();
		nextDistanceIncrement += 100;
	}

	/* Check submarine-based collisions */
	subCollisionTest();

	/* The torpedo might not be present, so we need to check that */
	if (m_objectManager->getTorpedo() != NULL) {
		Torpedo *torpedo = dynamic_cast<Torpedo *>(m_objectManager->getTorpedo());

		/* Check if we need to retarget */
		if (torpedo->needsNewTarget()) {
			torpedo->retarget(m_sub);
		}

		/* Perform the collision test */
		torpedoCollisionTest();
	}

	/* The flare should sit on the terrain when a collision is detected */
	Flare *flare = m_sub->getFlare();
	if (flare->IsVisible() && flare->isActive()) {
		if (Collision::boundingBox(flare, m_foregroundTerrain)) {
			bool collision = false;
			collision |= Collision::surface(COLLISION_RESOLUTION, flare->getCollidableSurface(), m_foregroundTerrain->getMainSurface(),
				flare->getCollidableSurfaceX(), flare->getCollidableSurfaceY(), 26, m_foregroundTerrain->getOffset(), 350, 1270);
			collision |= Collision::surface(COLLISION_RESOLUTION, flare->getCollidableSurface(), m_foregroundTerrain->getBufferSurface(),
				flare->getCollidableSurfaceX(), flare->getCollidableSurfaceY(), 26, 1270 + m_foregroundTerrain->getOffset(), 350, 1270);

			if (collision) {
				flare->setActive(false);
			}
		}
	}

	/* Fuel quantity enforcement */
	if (m_sub->getFuel() <= 1.0) {
#ifndef UNLIMITED_FUEL
		m_crashBox->setTitle("OUT OF FUEL");
		changeGameState(CRASHED);
		pauseTimer();
#endif // UNLIMITED_FUEL
	}
}

void MyProjectMain::subCollisionTest() {
	bool fatalCollision = false;

	/* Terrain collision test */
	if (Collision::boundingBox(m_sub, m_foregroundTerrain)) {
		fatalCollision |= Collision::surface(COLLISION_RESOLUTION, m_sub->getCollidableSurface(), m_foregroundTerrain->getMainSurface(),
			m_sub->getXPosition(), m_sub->getYPosition(), m_foregroundTerrain->getOffset(), 350);
		fatalCollision |= Collision::surface(COLLISION_RESOLUTION, m_sub->getCollidableSurface(), m_foregroundTerrain->getBufferSurface(),
			m_sub->getXPosition(), m_sub->getYPosition(), 1270 + m_foregroundTerrain->getOffset(), 350);
	}

	/* Game object collision test */
	fatalCollision |= gameObjectArrayCollisionTest(m_sub, m_objectManager->getWaveObjectsBuffer1(), m_objectManager->getNumWaveObjectsBuffer1());
	fatalCollision |= gameObjectArrayCollisionTest(m_sub, m_objectManager->getWaveObjectsBuffer2(), m_objectManager->getNumWaveObjectsBuffer2());

	if (fatalCollision) {
#ifndef NO_CRASH
		m_crashBox->setTitle("CRASHED");
		changeGameState(CRASHED);
		pauseTimer();
#endif // NO_CRASH
	}
}

void MyProjectMain::torpedoCollisionTest() {
	GameObject *torpedo = m_objectManager->getTorpedo();
	bool fatalCollision = false;

	/* If the torpedo is invisible, we shouldn't collide with anything */
	if (!torpedo->IsVisible()) return;

	/* Terrain collision test */
	if (Collision::boundingBox(torpedo, m_foregroundTerrain)) {
		fatalCollision |= Collision::surface(COLLISION_RESOLUTION, torpedo->getCollidableSurface(), m_foregroundTerrain->getMainSurface(),
			torpedo->getCollidableSurfaceX(), torpedo->getCollidableSurfaceY(), m_foregroundTerrain->getOffset(), 350);
		fatalCollision |= Collision::surface(COLLISION_RESOLUTION, torpedo->getCollidableSurface(), m_foregroundTerrain->getBufferSurface(),
			torpedo->getCollidableSurfaceX(), torpedo->getCollidableSurfaceY(), 1270 + m_foregroundTerrain->getOffset(), 350);
	}

	/* Game object collision test */
	fatalCollision |= gameObjectArrayCollisionTest(torpedo, m_objectManager->getWaveObjectsBuffer1(), m_objectManager->getNumWaveObjectsBuffer1());
	fatalCollision |= gameObjectArrayCollisionTest(torpedo, m_objectManager->getWaveObjectsBuffer2(), m_objectManager->getNumWaveObjectsBuffer2());

	/* Submarine collision test */
	if (Collision::boundingBox(torpedo, m_sub)) {
		bool collide = gameObjectCollisionTest(torpedo, m_sub);
		if (collide) {
#ifndef NO_CRASH
			m_crashBox->setTitle("CRASHED");
			changeGameState(CRASHED);
			pauseTimer();
#endif // NO_CRASH
		}
	}
	
	/* If a fatal collision was detected, we make the torpedo invisible */
	if (fatalCollision) {
		torpedo->SetVisible(false);
	}
}

bool MyProjectMain::subCollisionAction(GameObject *src, GameObject *obj) {
	switch (obj->getType()) {
	case GameObject::TORPEDO:
	case GameObject::NAVAL_MINE:
	case GameObject::RISING_MINE:
		return (obj->IsVisible());
	case GameObject::COIN:
		if (obj->IsVisible()) {
			m_statusBar->incrementPoints();
			obj->SetVisible(false);
		}
		break;
	case GameObject::FUEL:
		if (obj->IsVisible()) {
			m_sub->setFuelLevel(100.0);
			obj->SetVisible(false);
		}
		break;
	}

	return false;
}

bool MyProjectMain::torpedoCollisionAction(GameObject *src, GameObject *obj) {
	switch (obj->getType()) {
	case GameObject::NAVAL_MINE:
		if (src->IsVisible() && obj->IsVisible()) {
			dynamic_cast<NavalMine *>(obj)->explode();
		}
	case GameObject::TORPEDO:
	case GameObject::RISING_MINE:
		if (src->IsVisible() && obj->IsVisible()) {
			src->SetVisible(false);
			//obj->SetVisible(false);
			return true;
		}
	case GameObject::COIN:
	case GameObject::FUEL:
		if (src->IsVisible() && obj->IsVisible()) {
			obj->SetVisible(false);
		}
		break;
	}

	return false;
}

bool MyProjectMain::gameObjectArrayCollisionTest(GameObject *source, GameObject **objects, int size) {
	bool fatalCollision = false;
	for (int i = 0; i < size; i++) {
		if (objects[i] != NULL && Collision::boundingBox(source, objects[i])) {
			GameObject *object = dynamic_cast<GameObject *>(objects[i]);

			/* If we have a pixel-perfect collision, we can do something */
			if (gameObjectCollisionTest(source, object)) {
				if (source->getType() == GameObject::SUBMARINE) {
					fatalCollision |= subCollisionAction(source, object);
				} else if (source->getType() == GameObject::TORPEDO) {
					fatalCollision |= torpedoCollisionAction(source, object);
				}
			}
		}
	}

	return fatalCollision;
}

bool MyProjectMain::gameObjectCollisionTest(GameObject *src, GameObject *obj) {
	return Collision::surface(COLLISION_RESOLUTION, src->getCollidableSurface(), obj->getCollidableSurface(),
		src->getCollidableSurfaceX(), src->getCollidableSurfaceY(), obj->getCollidableSurfaceX(), obj->getCollidableSurfaceY());
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
		m_currentHighscorePoints = m_statusBar->getHighscore();
		m_sub->setSubPosition(100, 250);
		m_sub->setXVelocity(0);
		m_sub->setYVelocity(0);
		m_foregroundTerrain->setOffset(-1260);
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
		m_currentHighscorePoints = m_statusBar->getHighscore();
		m_sub->setSubPosition(100, 250);
		m_sub->setXVelocity(0);
		m_sub->setYVelocity(0);
		m_sub->getFlare()->reset();
		m_objectManager->reset();
		updateDisplayableObjectArray();
		m_foregroundTerrain->setOffset(-1260);
		unpauseTimer();
		break;
	case SDLK_m:
		changeGameState(MENU);
		m_sub->setFuelLevel(100);
		m_statusBar->resetTime();
		m_statusBar->resetDistance();
		m_statusBar->resetPoints();
		m_currentHighscorePoints = m_statusBar->getHighscore();
		m_sub->getFlare()->reset();
		m_objectManager->reset();
		updateDisplayableObjectArray();
		unpauseTimer();
		break;
	}
}

void MyProjectMain::playingKeyEvent(int keyCode) {
	Flare *flare = m_sub->getFlare();
	switch (keyCode) {
	case SDLK_f:
		if (!flare->IsVisible()) {
			flare->setX(m_sub->getXPosition());
			flare->setY(m_sub->getYPosition() + m_sub->getCollidableSurface()->h);
			flare->SetVisible(true);
			flare->setActive(true);

			if (m_objectManager->getTorpedo() != NULL) {
				dynamic_cast<Torpedo *>(m_objectManager->getTorpedo())->retarget(flare);
			}
		}
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
	GameObject **gameObjects;
	m_ppDisplayableObjects[objID++] = m_backgroundTerrain;
	m_ppDisplayableObjects[objID++] = m_shark1;
	m_ppDisplayableObjects[objID++] = m_shark2;

	if (m_gameState == PLAYING || CRASHED || PAUSED ) {
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

		/* Add the torpedo */
		if (m_objectManager->getTorpedo() != NULL) {
			m_ppDisplayableObjects[objID++] = m_objectManager->getTorpedo();
		}
	}
	
	m_ppDisplayableObjects[objID++] = m_foregroundTerrain;
	m_ppDisplayableObjects[objID++] = m_waves;
	m_ppDisplayableObjects[objID++] = m_statusBar;

	/* State depentant objects */
	switch (m_gameState) {
	case PLAYING:
		m_ppDisplayableObjects[objID++] = m_sub;
		m_ppDisplayableObjects[objID++] = m_sub->getFlare();
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

		/* Write the highscore if there is one */
		if (m_statusBar->getHighscore() > m_currentHighscorePoints) {
			char buf[50];
			sprintf(buf, "%02.0f:%02.0f", floor(m_statusBar->getSeconds() / 60.0), fmod(floor(m_statusBar->getSeconds()), 60));
			writeHighscore(m_statusBar->getDistance(), m_statusBar->getPoints(), buf);
		}
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

void MyProjectMain::updateHighscoreInformation() {
	/* Read the current highscore */
	int distance; string cha;
	getHighscore(distance, m_currentHighscorePoints, cha);
	m_statusBar->setHighscore(m_currentHighscorePoints);
}

void MyProjectMain::getHighscore(int &distance, int &points, string &time) {
	enum FileState { UNK, HS };
	FileState state = UNK;
	ifstream file;
	string line;

	file.open("../resources/highscore.log");
	if (file.is_open()) {
		while (getline(file, line)) {
			switch (state) {
			case UNK:
				state = (line == "[HIGHSCORE]") ? HS : UNK;
				break;
			case HS:
				istringstream tokenizer(line);
				string token;
				string value;
				getline(tokenizer, token, '=');
				getline(tokenizer, value, '=');
				istringstream intStream(value);

				if (token == "distance") {
					intStream >> distance;
				} else if (token == "points") {
					intStream >> points;
				} else if (token == "time") {
					time = string(value);
				}
				break;
			}
		}
		file.close();
	}
}

void MyProjectMain::writeHighscore(int distance, int points, const char *time) {
	ofstream file;
	file.open("../resources/highscore.log");
	file << "[HIGHSCORE]"	<< std::endl <<
			"distance="		<< distance << std::endl <<
			"points="		<< points << std::endl <<
			"time="			<< time << std::endl;
	file.close();
}