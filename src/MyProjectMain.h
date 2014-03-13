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
class GameObjectManager;

class MyProjectMain : public BaseEngine {
public:
	typedef enum GameState {
		MENU,
		PLAYING,
		CRASHED,
		PAUSED,
		HELP
	} GameState;

	typedef enum StaticGameObject {
		BACKGROUND_TERRAIN,
		GAME_OBJECTS,
		FOREGROUND_TERRAIN,
		SUBMARINE,
		WAVES,
		STATUS_BAR,
		NONE
	} StaticGameObject;

	MyProjectMain();
	~MyProjectMain();

	virtual int GameInit();
	virtual void GameAction();
	virtual void CleanUp();

	virtual int InitialiseObjects();
	virtual void SetupBackgroundBuffer();

	virtual void KeyDown(int keyCode);

	/* Getters */
	inline GameObjectManager *getObjectManager()                       const { return m_objectManager; }
	DisplayableObject *       getStaticObject(StaticGameObject object) const;
protected:
	/* Game objects */
	GameObjectManager *m_objectManager;
	Terrain *m_backgroundTerrain;
	Terrain *m_foregroundTerrain;
	StatusBar *m_statusBar;
	Submarine *m_sub;
	Waves *m_waves;

	/* Game properties */
	const int m_fpsTarget;

	void updateDisplayableObjectArray();
private:
	GameState m_state;

	void printDebugInformation();
};

#endif // !MYPROJECTMAIN_H