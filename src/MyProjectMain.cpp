#include <cmath>
#include "header.h"
#include "Terrain.h"
#include "JPGImage.h"
#include "Submarine.h"
#include "BaseEngine.h"
#include "TileManager.h"
#include "MyProjectMain.h"
#include "DisplayableObject.h"

MyProjectMain::MyProjectMain() : BaseEngine(6), m_fpsTarget(60) {}

void MyProjectMain::SetupBackgroundBuffer() {
	// We want a two-tone background for water and sky. The seam will then be
	// covered up by the wave animation
	static const unsigned int SKY_COLOUR = 0x3990C6;
	static const unsigned int WATER_COLOUR = 0x3686B8;

	DrawRectangle(0, 0, GetScreenWidth(), 120, SKY_COLOUR, GetBackground());
	DrawRectangle(0, 120, GetScreenWidth(), GetScreenHeight(), WATER_COLOUR, GetBackground());
}

int MyProjectMain::InitialiseObjects() {
	m_backgroundTerrain = new Terrain(this, 110, 300);
	m_foregroundTerrain = new Terrain(this, 110, 500);
	m_sub = new Submarine(this, 100, 250);
	
	m_backgroundTerrain->setColour(0x2E6D94);
	m_backgroundTerrain->setSpeed(10.0);

	DrawableObjectsChanged();
	DestroyOldObjects();

	m_ppDisplayableObjects = new DisplayableObject*[4];
	m_ppDisplayableObjects[0] = m_backgroundTerrain;
	m_ppDisplayableObjects[1] = m_foregroundTerrain;
	m_ppDisplayableObjects[2] = m_sub;
	m_ppDisplayableObjects[3] = NULL;
	
	return 0;
}

void MyProjectMain::DrawStrings() {}

void MyProjectMain::GameAction() {
	static int lastFrameTime = GetTime();
	int thisFrameTime = GetTime();
	int elapsedTime = thisFrameTime - lastFrameTime;
	lastFrameTime = thisFrameTime;

	UpdateAllObjects(elapsedTime);
	Redraw(false);

	if (1000 / m_fpsTarget > GetTime() - thisFrameTime) {
		SDL_Delay(1000 / m_fpsTarget - (GetTime() - thisFrameTime));
	}
}

void MyProjectMain::controlSub() {
	if (IsKeyPressed(SDLK_RIGHT)) {
		m_sub->setXDelta(100);
	} else if (IsKeyPressed(SDLK_LEFT)) {
		m_sub->setXDelta(-100);
	}

	if (IsKeyPressed(SDLK_UP)) {
		m_sub->setYDelta(-100);
	} else if (IsKeyPressed(SDLK_DOWN)) {
		m_sub->setYDelta(100);
	}
}

void MyProjectMain::MouseDown(int iButton, int iX, int iY) {

}

void MyProjectMain::KeyDown(int iKeyCode) {
	switch (iKeyCode) {
	case SDLK_ESCAPE:
		SetExitWithCode(0);
		break;
	case SDLK_UP:
		m_sub->setYDelta(-100);
		break;
	case SDLK_DOWN:
		m_sub->setYDelta(100);
		break;
	case SDLK_LEFT:
		m_sub->setXDelta(-100);
		break;
	case SDLK_RIGHT:
		m_sub->setXDelta(100);
		break;
	}
}