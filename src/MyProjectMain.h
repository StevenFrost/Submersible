#ifndef MYPROJECTMAIN_H
#define MYPROJECTMAIN_H

#include "header.h"
#include "JPGImage.h"
#include "BaseEngine.h"
#include <cmath>

class Submarine;
class Terrain;
class StatusBar;
class Waves;
class NavalMine;

class MyProjectMain : public BaseEngine {
public:
	MyProjectMain();

	/**
	* Builds the background buffer prior to locking the screen buffer. The
	* resulting background will be copied to the screen as and when it is
	* needed.
	*/
	virtual void SetupBackgroundBuffer();

	/**
	* Creates moving objects. Subclasses need to implement this method
	*/
	int InitialiseObjects();

	/**
	* The main game logic method. Moves objects and changes state if necessary.
	* This method must call redraw if any visible object changes.
	*/
	virtual void GameAction();

	virtual void GetUpdateRectanglesForChangingObjects();

	/**
	* Handles the key-down event
	*/
	virtual void KeyDown(int iKeyCode);
protected:
	/* Game objects */
	Terrain *m_backgroundTerrain;
	Terrain *m_foregroundTerrain;
	ImageSurface *m_sun;
	Waves *m_waves;
	StatusBar *m_statusBar;
	Submarine *m_sub;
	NavalMine *m_mine;

	const int m_fpsTarget;
};

#endif // !MYPROJECTMAIN_H