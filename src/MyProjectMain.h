#ifndef MYPROJECTMAIN_H
#define MYPROJECTMAIN_H

#include <string>
#include "Flare.h"
#include "Shark.h"
#include "Waves.h"
#include "header.h"
#include "Terrain.h"
#include "JPGImage.h"
#include "DialogBox.h"
#include "StatusBar.h"
#include "Submarine.h"
#include "BaseEngine.h"
#include "GameObject.h"
#include "GameObjectManager.h"

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
	friend class GameObjectManager;

	/* Overrides from BaseEngine */
	virtual int GameInit();
	virtual void GameAction();
	virtual int GetModifiedTime();
	virtual int InitialiseObjects();
	virtual void KeyDown(int keyCode);
	virtual void SetupBackgroundBuffer();

	/* Timer functions for tracking time while paused */
	void pauseTimer();
	void unpauseTimer();

	void changeGameState(GameState state);

	/* Getters */
	inline GameObjectManager *getObjectManager() const { return m_objectManager; }
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
	Shark *m_shark1;
	Shark *m_shark2;

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
	int m_currentHighscorePoints;
	int m_nextDistanceIncrement;

	/* Action functions for different game states */
	void playingAction(int elapsedTime);

	/* Key event functions for different game states */
	void menuKeyEvent(int keyCode);
	void pausedKeyEvent(int keyCode);
	void helpKeyEvent(int keyCode);
	void crashedKeyEvent(int keyCode);
	void playingKeyEvent(int keyCode);

	void subCollisionTest();
	void torpedoCollisionTest();

	bool subCollisionAction(GameObject *src, GameObject *obj);
	bool torpedoCollisionAction(GameObject *src, GameObject *obj);

	bool gameObjectArrayCollisionTest(GameObject *source, GameObject **objects, int size);
	bool gameObjectCollisionTest(GameObject *src, GameObject *obj);

	void updateHighscoreInformation();

	void getHighscore(int &distance, int &points, std::string &time);
	void writeHighscore(int distance, int points, const char *time);
};

#endif // !MYPROJECTMAIN_H