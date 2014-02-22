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
	/* Game objects */
	Terrain *m_backgroundTerrain;
	Terrain *m_foregroundTerrain;
	ImageSurface *m_sun;
	Waves *m_waves;
	StatusBar *m_statusBar;
	Submarine *m_sub;

	const int m_fpsTarget;

private:
	void controlSub();

	bool pixelCollideTest(SDL_Surface *surface, int x, int y);
	bool collide(SDL_Surface *a, SDL_Surface *b, int aX, int aY, int bX, int bY);
};

#endif // !MYPROJECTMAIN_H