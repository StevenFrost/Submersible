#include "Terrain.h"

static const double SCALING_FACTOR = 0.1;

Terrain::Terrain(MyProjectMain *engine, unsigned int width, unsigned int height, unsigned int colour) : DisplayableObject(engine),
	m_initialised(false),
	m_width(width * SCALING_FACTOR),
	m_height(height),
	m_colour(colour),
	m_numPoints(0),
	m_bufSize(0),
	m_offset(-10.0),
	m_displacement(60),
	m_roughness(0.77),
	m_speed(70.0),
	m_terrainMainRect(new SDL_Rect),
	m_terrainBufferRect(new SDL_Rect) {}

Terrain::~Terrain() {
	/* Delete the terrain buffers */
	delete[] m_terrainMainX, m_terrainMainY, m_terrainBufferY;

	/* Free the SDL surfaces */
	SDL_FreeSurface(m_terrainMain);
	SDL_FreeSurface(m_terrainBuffer);
}

bool Terrain::initialise() {
	/* Get the next-largest power of two to the width of the terrain */
	m_numPoints = pow(2, ceil(log10(m_width) / log10(2)));
	m_bufSize = m_numPoints + 2;

	/* Initialise the buffers */
	m_terrainMainX = new double[m_bufSize];
	m_terrainMainY = new double[m_bufSize];
	m_terrainBufferY = new double[m_bufSize];

	/* Calculate the X positions for each terrain point */
	for (int i = 0; i < m_bufSize; i++) {
		m_terrainMainX[i] = ((i - 1) * 100 * SCALING_FACTOR);
	}
	
	/* Set the end points */
	m_terrainMainX[0] = m_terrainMainX[1];
	m_terrainMainX[m_bufSize - 1] = m_terrainMainX[m_numPoints];
	m_polygonWidth = m_terrainMainX[m_numPoints];

	/* Generate and draw some terrain */
	generateTerrain(m_terrainMainY, m_terrainMainMaxHeight, 0, 0);
	generateTerrain(m_terrainBufferY, m_terrainBufferMaxHeight, m_seed, 0);

	printf("m_terrainMainMaxHeight: %d\r\nm_terrainbufferMaxHeight: %d\r\n", m_terrainMainMaxHeight, m_terrainBufferMaxHeight);

	/* Build the rectangles required for blitting the terrain */
	GetRedrawRect(m_terrainMainRect);
	GetRedrawRect(m_terrainBufferRect);

	/* Create the initial terrain surfaces */
	createTerrainSurface(m_terrainMain);
	createTerrainSurface(m_terrainBuffer);

	/* Draw the generated terrain on the surface */
	drawTerrainSurface(m_terrainMain, m_terrainMainY);
	drawTerrainSurface(m_terrainBuffer, m_terrainBufferY);

	m_initialised = true;
	return m_initialised;
}

void Terrain::generateTerrain(double *buffer, int &max, double leftSeed, double rightSeed) {
	/* Generate a seed for the left and right elements if needed */
	if (!leftSeed)  { leftSeed  = (m_height / 2.0) + (((double)rand() / RAND_MAX) * m_displacement * 2) - m_displacement; }
	if (!rightSeed) { rightSeed = (m_height / 2.0) + (((double)rand() / RAND_MAX) * m_displacement * 2) - m_displacement; }

	/* Set the left and right points */
	m_seed = rightSeed;
	buffer[0] = leftSeed;
	buffer[m_numPoints] = rightSeed;

	/* Make a copy of the displacement member so we don't mutate the original value */
	double displacement = m_displacement;
	max = m_height;

	/* Run the midpoint displacement algorithm */
	for (int i = 1; i < m_numPoints; i *= 2) {
		displacement *= m_roughness;
		for (int j = (m_numPoints / i) / 2; j < m_numPoints; j += (m_numPoints / i)) {
			buffer[j] = ((buffer[j - (m_numPoints / i) / 2] + buffer[j + (m_numPoints / i) / 2]) / 2.0f);
			buffer[j] += (((double)rand() / RAND_MAX) * displacement * 3) - displacement;
		}
	}

	for (int i = 0; i < m_bufSize - 1; i++) {
		if (buffer[i] < max) {
			max = buffer[i];
		}
	}

	/* Set the first and last values */
	buffer[0] = m_height;
	buffer[1] = leftSeed;
	buffer[m_bufSize - 1] = buffer[0];
}

bool Terrain::createTerrainSurface(SDL_Surface *&surface) {
	// We build a new ARGB surface of size height:m_height and width:m_terrainMainX[m_bufSize - 1].
	// The height is not the size of the redraw rectangle because this will force the terrain
	// off-screen. Instead, we deal with a large surface but only consider a smaller area
	// when drawing
	surface = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_ANYFORMAT,
		m_polygonWidth, m_height,
		m_pEngine->GetForeground()->format->BitsPerPixel, 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
	return (surface != NULL);
}

void Terrain::drawTerrainSurface(SDL_Surface *surface, double *buffer) {
	// Make a copy of the original terrain X-buffer to avoid the
	// DrawBackgroundPolygon function mutating our primary array
	double *terrainMainX = new double[m_bufSize];
	memcpy(terrainMainX, m_terrainMainX, m_bufSize * sizeof(double));

	/* Draw the polygon on the specified surface */
	GetEngine()->DrawPolygon(m_bufSize, terrainMainX, buffer, m_colour, surface);

	/* Now free the temporary terrain points, we don't need them */
	delete[] terrainMainX;
}

void Terrain::Draw() {
	if (!m_initialised) return;

	double tmp = (m_terrainMainMaxHeight < m_terrainBufferMaxHeight) ? m_terrainMainMaxHeight : m_terrainBufferMaxHeight;
	SDL_Rect mainRectSource = {(Sint16)0, tmp, m_polygonWidth, m_height - tmp};
	SDL_Rect bufferRectSource = {(Sint16)0, tmp, m_polygonWidth, m_height - tmp};

	/* Blit the terrain surfaces to the foreground surface */
	SDL_BlitSurface(m_terrainMain, &mainRectSource, m_pEngine->GetForeground(), m_terrainMainRect);
	SDL_BlitSurface(m_terrainBuffer, &bufferRectSource, m_pEngine->GetForeground(), m_terrainBufferRect);
}

void Terrain::DoUpdate(int elapsedTime) {
	if (!m_initialised) return;

	if (m_polygonWidth + m_offset <= 0) {
		/* Delete the old terrain points and free the surface */
		delete[] m_terrainMainY;
		delete m_terrainMainRect;
		SDL_FreeSurface(m_terrainMain);

		/* Update the main terrain pointer to point to the buffer */
		m_terrainMain = m_terrainBuffer;
		m_terrainMainY = m_terrainBufferY;
		m_terrainMainMaxHeight = m_terrainBufferMaxHeight;
		m_terrainMainRect = m_terrainBufferRect;

		/* Assign new memory */
		m_terrainBufferY = new double[m_bufSize];
		m_terrainBufferRect = new SDL_Rect;

		/* Generate some new terrain in the buffer */
		generateTerrain(m_terrainBufferY, m_terrainBufferMaxHeight, m_seed, 0);
		GetRedrawRect(m_terrainMainRect);
		GetRedrawRect(m_terrainBufferRect);
		createTerrainSurface(m_terrainBuffer);
		drawTerrainSurface(m_terrainBuffer, m_terrainBufferY);

		/* Reset the offset */
		m_offset = 0.0;
	}

	/* Move the terrain backwards. This gives the impression that the user is moving forwards */
	m_offset -= (m_speed * elapsedTime / 1000.0);

	/* Calculate the new terrain positions */
	m_terrainMainRect->x = (Sint16)(0 + static_cast<int>(m_offset));
	m_terrainBufferRect->x = (Sint16)(m_polygonWidth + static_cast<int>(m_offset));
}

void Terrain::GetRedrawRect(SDL_Rect *rectangle) {
	rectangle->x = 0;
	rectangle->h = (Sint16)(m_height - ((m_terrainMainMaxHeight < m_terrainBufferMaxHeight) ? m_terrainMainMaxHeight : m_terrainBufferMaxHeight));
	rectangle->y = (Sint16)m_pEngine->GetScreenHeight() - rectangle->h;
	rectangle->w = (Sint16)m_polygonWidth;
}

void Terrain::RedrawBackground() {
	SDL_Rect rect;
	GetRedrawRect(&rect);
	m_pEngine->CopyBackgroundPixels(rect.x, rect.y, rect.w, rect.h);
}