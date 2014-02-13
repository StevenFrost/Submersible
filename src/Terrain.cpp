#include "Terrain.h"

Terrain::Terrain(MyProjectMain *engine, unsigned int width, unsigned int height) : DisplayableObject(engine), m_width(width), m_height(height), m_offset(0), m_displacement(60), m_roughness(0.77), m_colour(0x232A2F), m_speed(6) {
	/* Get the next-largest power of two to the width of the terrain */
	m_numPoints = pow(2, ceil(log10(m_width) / log10(2)));
	m_bufSize = m_numPoints + 2;

	// Initialise the horizontal buffers
	m_xPoints = new double[m_bufSize];
	m_xBuffer = new double[m_bufSize];

	/* Generate some initial terrain */
	m_yBuffer = new double[m_bufSize];
	generate(0, 0);

	/* Point to the buffer initially */
	m_yPoints = m_yBuffer;

	/* Build a new buffer connected to the previously generated terrain */
	m_yBuffer = new double[m_bufSize];
	generate(m_yPoints[m_numPoints] - 270, 0);
}

Terrain::~Terrain() {
	delete[] m_yPoints, m_yBuffer, m_xPoints, m_xBuffer;
}

void Terrain::generate(double leftSeed, double rightSeed) {
	double displacement = m_displacement;

	/* Generate a seed for the left and right elements if needed */
	if (!leftSeed) {
		leftSeed = (m_height / 2.0) + (((double)rand() / RAND_MAX) * displacement * 2) - displacement;
	}

	if (!rightSeed) {
		rightSeed = (m_height / 2.0) + (((double)rand() / RAND_MAX) * displacement * 2) - displacement;
	}

	/* Set the left and right points */
	m_yBuffer[0] = leftSeed;
	m_yBuffer[m_numPoints] = rightSeed;

	/* Consider the roughness of the terrain */
	displacement *= m_roughness;
	
	for (int i = 1; i < m_numPoints; i *= 2) {
		for (int j = (m_numPoints / i) / 2; j < m_numPoints; j += (m_numPoints / i)) {
			m_yBuffer[j] = ((m_yBuffer[j - (m_numPoints / i) / 2] + m_yBuffer[j + (m_numPoints / i) / 2]) / 2.0f);
			m_yBuffer[j] += (((double)rand() / RAND_MAX) * displacement * 3) - displacement;
		}

		/* Reduce the range of the displacement */
		displacement *= m_roughness;
	}

	/* Decrease the depth of all terrain points, other than the two end points */
	for (int i = 1; i < m_bufSize - 1; i++) {
		m_yBuffer[i] += 270;
	}

	/* Set the first and last values */
	m_yBuffer[0] = 750;
	m_yBuffer[m_bufSize - 1] = 750;
}

void Terrain::Draw() {
	double *tmpYPoints = new double[m_bufSize];
	double *tmpYBuffer = new double[m_bufSize];

	/* Make a copy of the original terrain arrays to avoid the
	   DrawBackgroundPolygon function mutating our primary array */
	memcpy(tmpYPoints, m_yPoints, m_bufSize * sizeof(double));
	memcpy(tmpYBuffer, m_yBuffer, m_bufSize * sizeof(double));

	/* Draw the terrain */
	m_pEngine->DrawBackgroundPolygon(m_bufSize, m_xPoints, tmpYPoints, m_colour);
	m_pEngine->DrawBackgroundPolygon(m_bufSize, m_xBuffer, tmpYBuffer, m_colour);

	/* Now free the memory, we don't need it */
	delete[] tmpYPoints, tmpYBuffer;
}

void Terrain::DoUpdate(int iCurrentTime) {
	if (m_xPoints[m_bufSize - 1] <= 0) {
		/* Delete the old terrain points */
		delete[] m_yPoints;

		/* Update the main terrain pointer to point to the buffer */
		m_yPoints = m_yBuffer;
		m_yBuffer = new double[m_bufSize];

		/* Generate some new terrain for the buffer */
		generate(m_yPoints[m_numPoints] - 270, 0);
		m_offset = -m_speed - 2;
	}

	/* Move the terrain backwards. This gives the impression that the user
	   is moving forwards */
	m_offset -= m_speed;

	/* Build the horizontal buffers for the terrain polygon */
	for (int i = 0; i < m_bufSize; i++) {
		m_xPoints[i] = (i * 10) + m_offset;
		m_xBuffer[i] = m_xPoints[i] + 1270;
	}

	m_xPoints[0] = m_xPoints[1];
	m_xPoints[m_bufSize - 1] = m_xPoints[m_numPoints];
	m_xBuffer[0] = m_xPoints[m_bufSize - 1];
	m_xBuffer[m_bufSize - 1] = m_xBuffer[m_numPoints];
	m_yBuffer[1] = m_yPoints[m_numPoints];
}