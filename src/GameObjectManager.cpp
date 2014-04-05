#include "GameObjectManager.h"
#include "NavalMine.h"
#include "Coin.h"
#include "Fuel.h"
#include "Submarine.h"
#include "Terrain.h"
#include "Torpedo.h"
#include "Collision.h"
#include "MyProjectMain.h"

GameObjectManager::GameObjectManager(BaseEngine *engine, int maxObjects) : m_numWaveObjectsBuffer1(0), m_numWaveObjectsBuffer2(0), m_maxObjects(maxObjects), m_torpedo(NULL) {
	m_pEngine = dynamic_cast<MyProjectMain *>(engine);
	reset();
}

GameObjectManager::~GameObjectManager() {
	delete[] m_waveObjectsBuffer1;
	delete[] m_waveObjectsBuffer2;
}

void GameObjectManager::update(IObservable *observerable) {
	if (m_pEngine->m_gameState != MyProjectMain::PLAYING) return;

	/* Delete the first buffer */
	delete[] m_waveObjectsBuffer1;

	/* Swap the wave pointers */
	m_waveObjectsBuffer1 = m_waveObjectsBuffer2;
	m_numWaveObjectsBuffer1 = m_numWaveObjectsBuffer2;
	m_waveObjectsBuffer2 = new GameObject*[m_maxObjects];

	/* Generate a new wave for the second buffer */
	generateWave();
}

void GameObjectManager::reset() {
	if (m_numWaveObjectsBuffer1) delete[] m_waveObjectsBuffer1;
	if (m_numWaveObjectsBuffer2) delete[] m_waveObjectsBuffer2;
	if (m_torpedo) delete m_torpedo; m_torpedo = NULL;

	m_waveObjectsBuffer1 = new GameObject*[m_maxObjects];
	m_waveObjectsBuffer2 = new GameObject*[m_maxObjects];

	m_numWaveObjectsBuffer1 = 0;
	m_numWaveObjectsBuffer2 = 0;

	m_waveObjectsBuffer1[0] = NULL;
	m_waveObjectsBuffer2[0] = NULL;
}

void GameObjectManager::generateWave() {
	generateWave(m_waveObjectsBuffer2, m_numWaveObjectsBuffer2);
}

void GameObjectManager::generateWave(GameObject **&buffer, int &size) {
	/* Get terrain properties from the foreground terrain object */
	const Terrain *const terrain  = dynamic_cast<Terrain *>(m_pEngine->getStaticObject(MyProjectMain::FOREGROUND_TERRAIN));
	const double  *const terrainY = terrain->getBufferArray();
	const int terrainBufferSize   = terrain->getBufferSize();
	
	int count = 0;
	bool fuelGenerated = false;

	for (int i = 1; i < terrainBufferSize; i += 5) {
		int lowY  = 130;
		int highY = 750;
		int lowX  = (1270 + (i * 10)) - 20;
		int highX = (1270 + (i * 10)) + 20;

		// We need to look at the terrain points either side of the potential
		// object location to make sure we don't collide with the terrain surface
		for (int j = i - 4; j < i + 4; j++) {
			if (j < 0 || j >= terrainBufferSize) { continue;            }
			if (terrainY[j] < highY)             { highY = terrainY[j]; }
		}

		// We add the height of the terrain surface to convert the height into
		// screen coordinates
		highY = 750 - (400 - highY) - 100;

		/* Naval mines */
		if (i % 40 == 1) {
			int mineX = rand() % (highX - lowX) + lowX;
			int mineY = rand() % (highY - lowY) + lowY;

			if (mineX + 98 < 2400) {
				if ((double)rand() / RAND_MAX < 0.6) {
					buffer[count++] = new NavalMine(m_pEngine, mineX, mineY);
				} else {
					buffer[count++] = new RisingMine(m_pEngine, mineX, mineY);
				}
				i += 10;
			}
		}

		/* Coins */
		if (i % 15 == 1 && i % 45 != 1) {
			int coinX = rand() % ((highX - 10) - (lowX + 10)) + (lowX + 10);
			int coinY = rand() % ((highY - 10) - (lowY + 10)) + (lowY + 10);

			if (coinX + 53 < 2400) {
				buffer[count++] = new Coin(m_pEngine, coinX, coinY);
			}

			if (!fuelGenerated) {
				Submarine *sub = dynamic_cast<Submarine *>(m_pEngine->getStaticObject(MyProjectMain::SUBMARINE));
				if (sub->getFuel() < (rand() % (100 - 80) + 80)) {
					buffer[count++] = new Fuel(m_pEngine, coinX, highY - coinY + lowY);
				}
				fuelGenerated = true;
			}
		}
	}

	/* Torpedo */
	if (m_torpedo != NULL && m_torpedo->GetXCentre() > 1200) {
		m_torpedo = NULL;
	}
	if (m_torpedo == NULL && (double)rand() / RAND_MAX > 0.1) {
		m_torpedo = new Torpedo(m_pEngine, m_pEngine->getStaticObject(MyProjectMain::SUBMARINE));
	}
	
	buffer[count++] = NULL;
	size = count;
}