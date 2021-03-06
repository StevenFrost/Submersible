#ifndef GAME_OBJECT_MANAGER_H
#define GAME_OBJECT_MANAGER_H

#include "IObserver.h"
#include "BaseEngine.h"
#include "GameObject.h"
#include "Torpedo.h"

class MyProjectMain;

/**
* This class manages the current wave of game objects scrolling along the
* game window. The manager observes a terrain class in order to be notified
* when a new surface is generated and needs populating with game objects
*
* Game objects include the following:
*    + Moored mines
*    + Rising mines
*    + Torpedoes
*    + Coins
*    + Fuel
*/
class GameObjectManager : public IObserver {
public:
	GameObjectManager(BaseEngine *engine, int maxObjects);
	~GameObjectManager();

	void update(int elapsedTime);
	void update(IObservable *observerable);
	void reset();

	/* Getters */
	GameObject **getWaveObjectsBuffer1()    { return m_waveObjectsBuffer1;    }
	GameObject **getWaveObjectsBuffer2()    { return m_waveObjectsBuffer2;    }
	int          getNumWaveObjectsBuffer1() { return m_numWaveObjectsBuffer1; }
	int          getNumWaveObjectsBuffer2() { return m_numWaveObjectsBuffer2; }
	GameObject  *getTorpedo()               { return m_torpedo;               }

	void generateWave();
	void generateWave(GameObject **&buffer, int &size);
private:
	MyProjectMain *m_pEngine;
	int m_maxObjects;

	int m_numWaveObjectsBuffer1;
	int m_numWaveObjectsBuffer2;
	GameObject **m_waveObjectsBuffer1;
	GameObject **m_waveObjectsBuffer2;

	Torpedo *m_torpedo;
};

#endif // !GAME_OBJECT_MANAGER