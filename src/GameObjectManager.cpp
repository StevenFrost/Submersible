#include "GameObjectManager.h"
#include "NavalMine.h"

GameObjectManager::GameObjectManager(MyProjectMain *engine, int maxObjects) : m_pEngine(engine), m_numWaveObjectsBuffer1(0), m_numWaveObjectsBuffer2(0), m_maxObjects(maxObjects) {
	reset();
}

GameObjectManager::~GameObjectManager() {}

void GameObjectManager::update(IObservable *observerable) {
	/* Delete all objects from buffer 1, they're all off-screen now */
	for (int i = 0; i < m_numWaveObjectsBuffer1; i++) {
		if (m_waveObjectsBuffer1[i] != NULL) {
			delete m_waveObjectsBuffer1[i];
		}
	}
	
	/* Delete the first buffer */
	delete m_waveObjectsBuffer1;

	/* Swap the wave pointers */
	m_waveObjectsBuffer1 = m_waveObjectsBuffer2;
	m_numWaveObjectsBuffer1 = m_numWaveObjectsBuffer2;
	m_waveObjectsBuffer2 = new DisplayableObject*[m_maxObjects];

	/* Generate a new wave for the second buffer */
	generateWave();
}

void GameObjectManager::reset() {
	m_waveObjectsBuffer1 = new DisplayableObject*[m_maxObjects];
	m_waveObjectsBuffer2 = new DisplayableObject*[m_maxObjects];

	m_waveObjectsBuffer1[0] = NULL;
	m_waveObjectsBuffer2[0] = NULL;
}

void GameObjectManager::generateWave() {
	m_waveObjectsBuffer2[0] = new NavalMine(m_pEngine, 1270 + 300, 360, 70.0);
	m_waveObjectsBuffer2[1] = NULL;

	m_numWaveObjectsBuffer2 = 1;
}