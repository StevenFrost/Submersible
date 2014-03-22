#ifndef MYPROJECTMAIN_H
#define MYPROJECTMAIN_H

#include "header.h"
#include "JPGImage.h"
#include "BaseEngine.h"

class Submarine;
class Terrain;
class StatusBar;
class Waves;
class NavalMine;
class GameObjectManager;
class Button;
class DialogBox;
class CrashedDialogBox;
class PausedDialogBox;
class HelpDialogBox;
class MainDialogBox;

class MyProjectMain : public BaseEngine {
public:
	typedef enum GameState {
		MENU,
		PLAYING,
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

	friend class Terrain;

	/* Overrides from BaseEngine */
	virtual int GameInit();
	virtual void CleanUp();
	virtual void GameAction();
	virtual int GetModifiedTime();
	virtual int InitialiseObjects();
	virtual void KeyDown(int keyCode);
	virtual void SetupBackgroundBuffer();

	/* Action functions for different game states */
	void playingAction(int elapsedTime);

	/* Key event functions for different game states */
	void menuKeyEvent(int keyCode);
	void pausedKeyEvent(int keyCode);
	void helpKeyEvent(int keyCode);
	void crashedKeyEvent(int keyCode);
	void playingKeyEvent(int keyCode);

	/* Timer functions for tracking time while paused */
	void pauseTimer();
	void unpauseTimer();

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

	/* Dialog boxes */
	MainDialogBox *m_mainBox;
	CrashedDialogBox *m_crashBox;
	PausedDialogBox *m_pausedBox;
	HelpDialogBox *m_helpBox;

	/* Game properties */
	const int m_fpsTarget;
	bool m_paused;
	unsigned int m_pausedTime;
	unsigned int m_startTime;

	void updateDisplayableObjectArray();
private:
	GameState m_gameState;

	bool gameObjectCollisionTest(DisplayableObject **objects, int size);
};

#endif // !MYPROJECTMAIN_H