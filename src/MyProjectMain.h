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
class Button;
class DialogBox;
class CrashedDialogBox;

class MyProjectMain : public BaseEngine {
public:
	typedef enum GameState {
		MENU,
		PLAYING,
		RESTART,
		CRASHED,
		PAUSED,
		HELP
	} GameState;

	typedef enum MenuState {
		MENU_PLAY,
		MENU_HELP
	} MenuState;

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

	void menuAction(int elapsedTime);
	void playingAction(int elapsedTime);
	void crashedAction(int elapsedTIme);

	virtual int InitialiseObjects();
	virtual void SetupBackgroundBuffer();

	virtual void KeyDown(int keyCode);

	void changeGameState(GameState state);

	/* Getters */
	inline GameObjectManager *getObjectManager()                       const { return m_objectManager; }
	DisplayableObject *       getStaticObject(StaticGameObject object) const;
	int getStringWidth (const char *str, int size);
	int getStringHeight(const char *str, int size);
protected:
	/* Game objects */
	GameObjectManager *m_objectManager;
	Terrain *m_backgroundTerrain;
	Terrain *m_foregroundTerrain;
	StatusBar *m_statusBar;
	Submarine *m_sub;
	Waves *m_waves;

	/* Menu objects */
	Button *m_menuPlay;
	Button *m_menuHelp;

	/* Dialog boxes */
	CrashedDialogBox *m_crashBox;

	/* Game properties */
	const int m_fpsTarget;

	void updateDisplayableObjectArray();
private:
	GameState m_gameState;
	MenuState m_menuState;

	void printDebugInformation();
};

#endif // !MYPROJECTMAIN_H