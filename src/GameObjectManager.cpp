#include "GameObjectManager.h"
#include "NavalMine.h"
#include "Coin.h"

GameObjectManager::GameObjectManager(MyProjectMain *engine, int maxObjects) : m_pEngine(engine), m_numWaveObjectsBuffer1(0), m_numWaveObjectsBuffer2(0), m_maxObjects(maxObjects) {
	reset();
}

GameObjectManager::~GameObjectManager() {
	delete[] m_waveObjectsBuffer1;
	delete[] m_waveObjectsBuffer2;
}

void GameObjectManager::update(IObservable *observerable) {
	/* Delete the first buffer */
	delete[] m_waveObjectsBuffer1;

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
	int count = 0;

	m_waveObjectsBuffer2[count++] = new NavalMine(m_pEngine, 1270 + 300, 360);
	m_waveObjectsBuffer2[count++] = new Coin(m_pEngine, 1270 + 0,    250);
	m_waveObjectsBuffer2[count++] = new Coin(m_pEngine, 1270 + 100,  250);
	m_waveObjectsBuffer2[count++] = new Coin(m_pEngine, 1270 + 200,  250);
	m_waveObjectsBuffer2[count++] = new Coin(m_pEngine, 1270 + 300,  250);
	m_waveObjectsBuffer2[count++] = new Coin(m_pEngine, 1270 + 400,  250);
	m_waveObjectsBuffer2[count++] = new Coin(m_pEngine, 1270 + 500,  250);
	m_waveObjectsBuffer2[count++] = new Coin(m_pEngine, 1270 + 600,  250);
	m_waveObjectsBuffer2[count++] = new Coin(m_pEngine, 1270 + 700,  250);
	m_waveObjectsBuffer2[count++] = new Coin(m_pEngine, 1270 + 800,  250);
	m_waveObjectsBuffer2[count++] = new Coin(m_pEngine, 1270 + 900,  250);
	m_waveObjectsBuffer2[count++] = new Coin(m_pEngine, 1270 + 1000, 250);
	m_waveObjectsBuffer2[count++] = new Coin(m_pEngine, 1270 + 1100, 250);
	m_waveObjectsBuffer2[count++] = new Coin(m_pEngine, 1270 + 1200, 250);
	m_waveObjectsBuffer2[count++] = NULL;

	m_numWaveObjectsBuffer2 = count;
}