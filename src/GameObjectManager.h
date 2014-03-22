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
*    + Coins
*/
class GameObjectManager : public IObserver {
public:
	GameObjectManager(MyProjectMain *engine, int maxObjects);
	~GameObjectManager();

	void update(IObservable *observerable);
	void reset();

	/* Getters */
	inline DisplayableObject **getWaveObjectsBuffer1()    { return m_waveObjectsBuffer1;    }
	inline DisplayableObject **getWaveObjectsBuffer2()    { return m_waveObjectsBuffer2;    }
	inline int                 getNumWaveObjectsBuffer1() { return m_numWaveObjectsBuffer1; }
	inline int                 getNumWaveObjectsBuffer2() { return m_numWaveObjectsBuffer2; }

	void generateWave();
	void generateWave(DisplayableObject **&buffer, int &size);
private:
	MyProjectMain *m_pEngine;
	int m_maxObjects;

	int m_numWaveObjectsBuffer1;
	int m_numWaveObjectsBuffer2;
	DisplayableObject **m_waveObjectsBuffer1;
	DisplayableObject **m_waveObjectsBuffer2;
};

#endif // !GAME_OBJECT_MANAGER