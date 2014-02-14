#ifndef MYPROJECTMAIN_H
#define MYPROJECTMAIN_H

#include "header.h"
#include "BaseEngine.h"
#include <cmath>

class Submarine;
class Terrain;

class MyProjectMain : public BaseEngine {
public:
	MyProjectMain();

	/**
	* Builds the background buffer prior to locking the screen buffer. The
	* resulting background will be copied to the screen as and when it is
	* needed.
	*/
	virtual void SetupBackgroundBuffer(unsigned int colour);

	/**
	* Creates moving objects. Subclasses need to implement this method
	*/
	int InitialiseObjects();

	/**
	* Draws any text to the screen
	*/
	void DrawStrings();

	/**
	* The main game logic method. Moves objects and changes state if necessary.
	* This method must call redraw if any visible object changes.
	*/
	virtual void GameAction();

	/**
	* Handles the mouse-down event
	*/
	void MouseDown(int iButton, int iX, int iY);

	/**
	* Handles the key-down event
	*/
	virtual void KeyDown(int iKeyCode);
protected:
	Submarine *m_sub;
	Terrain *m_foregroundTerrain;
	Terrain *m_backgroundTerrain;

	const int m_fpsTarget;
};

#endif // !MYPROJECTMAIN_H