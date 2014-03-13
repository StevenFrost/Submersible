#ifndef GAME_OBJECT_MANAGER_H
#define GAME_OBJECT_MANAGER_H

#include "IObserver.h"
#include "MyProjectMain.h"

/**
* This class manages the current wave of game objects scrolling along the
* game window. The manager observes a terrain class in order to be notified
* when a new surface is generated and needs populating with game objects
*
* Game objects include the following:
*    + Moored mines
*    + Rising mines
*    + Torpedoes
*    + Powerup items
*    + Coins
*/
class GameObjectManager : public IObserver {
public:
	GameObjectManager(MyProjectMain *engine, int maxObjects);
	virtual ~GameObjectManager();

	virtual void update(IObservable *observerable);

	/* Getters */
	inline DisplayableObject **getWaveObjects()    { return m_waveObjects;    }
	inline int                 getNumWaveObjects() { return m_numWaveObjects; }
protected:
	virtual void generateWave();
private:
	MyProjectMain *m_pEngine;

	int m_numWaveObjects;
	DisplayableObject **m_waveObjects;
};

#endif // !GAME_OBJECT_MANAGER