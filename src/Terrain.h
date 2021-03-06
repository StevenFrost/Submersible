#ifndef TERRAIN_H
#define TERRAIN_H

#include "header.h"
#include "BaseEngine.h"
#include "IObservable.h"
#include "DisplayableObject.h"

/**
* This class handles the generation, drawing and updating functions for some
* terrain. Terrain is generated procedurally using the midpoint displacement
* algorithm.
* 
* Two buffers are maintained, each a little wider than the width of the screen
* so we can swap off-screen for a seamless transition between buffers.
*
* Buffer layout for terrain surface arrays:
*   +-------------------+---------------------------+--------------------+
*   | Bottom-Left Point | All terrain suface points | Bottom-Right point |
*   +-------------------+---------------------------+--------------------+
*   |         0         |   1 .. (m_bufSize - 2)    |  (m_bufSize - 1)   |
*   +-------------------+---------------------------+--------------------+
*
* The Y-Buffers contain height values that describe the terrain surface. The
* X-Buffers describe the respective horizontal dispacement of each point in
* the Y-Buffer.
*
* The terrain never actually moves physically, the effect is achieved by
* initially drawing the polygon to a temporary surface, then blitting the
* surface to the foreground, displacing it by an framerate-independant offset
* each time the class gets updated. When the main buffer runs off-screen,
* the pointers are swaped and we continue to scroll the terrain as usual.
* This approach allows us to have seamless random terrain of infinite length.
*/
class Terrain : public DisplayableObject, public IObservable {
public:
	Terrain(BaseEngine *engine, unsigned int width, unsigned int height, unsigned int colour);
	virtual ~Terrain();

	/* Overrides from DisplayableObject */
	virtual void Draw();
	virtual void DoUpdate(int elapsedTime);
	virtual void GetRedrawRect(SDL_Rect *rectangle);
	virtual void RedrawBackground();

	/* Setters */
	void setSpeed(double speed)               { m_speed = speed;               }
	void setRoughness(double roughness)       { m_roughness = roughness;       }
	void setDisplacement(double displacement) { m_displacement = displacement; }
	void setOffset(double offset)             { m_offset = offset;             }

	/* Getters */
	SDL_Surface *getMainSurface()   const     { return m_terrainMain;          }
	SDL_Surface *getBufferSurface() const     { return m_terrainBuffer;        }
	double       getSpeed()         const     { return m_speed;                }
	double       getWidth()         const     { return m_polygonWidth * 2;     }
	int          getHighestPoint()  const     { return m_terrainMaxHeight;     }
	double       getOffset()        const     { return m_offset;               }
	double      *getMainArray()     const     { return m_terrainMainY;         }
	double      *getBufferArray()   const     { return m_terrainBufferY;       }
	int          getBufferSize()    const     { return m_bufSize;              }

	/* Initialises core terrain objects, such as the surfaces onto which to the
	   terrain is drawn. */
	bool initialise();
protected:
	/* Generic terrain properties */
	unsigned int m_colour;			// The colour of the terrain (32-bit ARGB)
	unsigned int m_width;			// The width of the terrain, scaled by SCALING_FACTOR
	unsigned int m_height;			// The maximum height of the terrain

	virtual void generateTerrain(double *buffer, int &max, double leftSeed, double rightSeed);
private:
	/* Class-contruction members */
	bool m_initialised;				// True if the terrain class has been correctly initialised

	/* SDL Members */
	SDL_Surface *m_terrainMain;		// Primary terrain surface
	SDL_Surface *m_terrainBuffer;	// Secondary terrain surface for drawing off-screen polygons
	SDL_Rect *m_terrainMainRect;	// Rectangle representing the primary terrain surface bounds
	SDL_Rect *m_terrainBufferRect;	// Rectangle representing the buffer terrain surface bounds

	/* Maximum hight values */
	int m_terrainMainMaxHeight;		// The maximum height of the main surface points
	int m_terrainBufferMaxHeight;	// The maximum height of the buffer surface points
	int m_terrainMaxHeight;			// The maximum height of both the main and buffer surface points

	/* Buffer limits */
	int m_numPoints;				// The number of terrain surface points
	int m_bufSize;					// The number of elements in each buffer
	double m_polygonWidth;			// The width of the terrain polygon in pixels

	/* Terrain buffers */
	double *m_terrainMainX;			// Array containing X-coordinates of each terrain surface point
	double *m_terrainMainY;			// Array containing Y-coordinates for the main terrain section
	double *m_terrainBufferY;		// Array containing Y-coordinates for the buffer terrain section

	/* General properties */
	double m_offset;				// Current pixel offset of the terrain surface
	double m_displacement;			// Maximum displacement from the terrain seed
	double m_roughness;				// Roughness of the terrain
	double m_speed;					// Speed of terrain movement, in pixels/s
	double m_seed;					// The rightmost point of the previously generated terrain surface

	/* Initialises the specified surface with a new RGB SDL Surface, with alpha. */
	bool createTerrainSurface(SDL_Surface *&surface);

	/* Draws the specified buffer (containing height values) to the surface. This
	   function assumes that the buffer contains exactly m_bufSize elements and
	   that the SDL Surface has been correctly initialised. */
	void drawTerrainSurface(SDL_Surface *surface, double *buffer);

	/*  Sets the m_terrainMaxHeight value to the highest point between the primary
	    and buffer terrain surfaces */
	inline void calculateTerrainMaxHeight() {
		m_terrainMaxHeight = (m_terrainMainMaxHeight < m_terrainBufferMaxHeight) ?
			m_terrainMainMaxHeight : m_terrainBufferMaxHeight;
	}
};

#endif // !TERRAIN_H