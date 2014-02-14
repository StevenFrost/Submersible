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

void MyProjectMain::SetupBackgroundBuffer(unsigned int colour) {
	FillBackground(colour);
}

int MyProjectMain::InitialiseObjects() {
	m_sub = new Submarine(this, 100, 250);
	m_backgroundTerrain = new Terrain(this, 110, 300);
	m_foregroundTerrain = new Terrain(this, 110, 500);

	m_backgroundTerrain->setColour(0x2E6D94);
	m_backgroundTerrain->setSpeed(2.0);

	// Record the fact that we are about to change the array - so it doesn't get used elsewhere without reloading it
	DrawableObjectsChanged();

	// Destroy any existing objects
	DestroyOldObjects();

	// Create an array one element larger than the number of objects that you want.
	m_ppDisplayableObjects = new DisplayableObject*[4];

	// You MUST set the array entry after the last one that you create to NULL, so that the system knows when to stop.
	// i.e. The LAST entry has to be NULL. The fact that it is NULL is used in order to work out where the end of the array is.
	m_ppDisplayableObjects[0] = m_sub;
	m_ppDisplayableObjects[1] = m_backgroundTerrain;
	m_ppDisplayableObjects[2] = m_foregroundTerrain;
	m_ppDisplayableObjects[3] = NULL;

	return 0;
}

void MyProjectMain::DrawStrings() {
	// Build the string to print
	char buf[128];
	sprintf(buf, "Changing text %6d %6d", rand(), rand());
	// Clear the top of the screen, since we about to draw text on it.
	CopyBackgroundPixels(0, 0, GetScreenWidth(), 35);
	// Then draw the strings
	DrawScreenString(150, 10, buf, 0xffffff, NULL);
	// And mark that area of the screen as having been changed, so it gets redrawn
	SetNextUpdateRect(0/*X*/, 0/*Y*/, GetScreenWidth(), 35/*Height*/);
}

void MyProjectMain::GameAction() {
	if (!TimeToAct()) {
		return;
	}

	static int lastFrameTime = GetTime();
	int thisFrameTime = GetTime();
	int elapsedTime = thisFrameTime - lastFrameTime;
	lastFrameTime = thisFrameTime;

	SetupBackgroundBuffer(0x4295C8);
	
	m_backgroundTerrain->DoUpdate(NULL);
	m_backgroundTerrain->Draw();

	m_sub->Draw();

	m_foregroundTerrain->DoUpdate(GetTime());
	m_foregroundTerrain->Draw();

	//ImageSurface water;
	//water.LoadImage("../resources/water.png");
	//water.RenderImage(this->GetBackground(), 0, 0, 0, 0, water.GetWidth(), water.GetHeight());

	Redraw(true);
	SetTimeToAct(1);
	UpdateAllObjects(elapsedTime);

	if (1000 / m_fpsTarget > GetTime() - thisFrameTime) {
		SDL_Delay(1000 / m_fpsTarget - (GetTime() - thisFrameTime));
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