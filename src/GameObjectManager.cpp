#include "GameObjectManager.h"

GameObjectManager::GameObjectManager(MyProjectMain *engine, int maxObjects) : m_pEngine(engine), m_numWaveObjects(0) {
	
}

GameObjectManager::~GameObjectManager() {
	//delete[] m_waveObjects;
}

void GameObjectManager::update(IObservable *observerable) {
	generateWave();
}

void GameObjectManager::generateWave() {
	
}