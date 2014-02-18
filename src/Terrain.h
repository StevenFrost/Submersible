#ifndef TERRAIN_H
#define TERRAIN_H

#include "header.h"
#include "MyProjectMain.h"
#include "DisplayableObject.h"

/**
* This class handles the generation, drawing and updating functions for some
* terrain. Terrain is generated procedurally using the midpoint displacement
* algorithm.
*
* Default terrain configuration has a displacement of 150, roughness of 0.82
* and a colour of 0x28485D.
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
* regenerating the X-Buffer and redrawing the polygon which gives the 
* appearance that the terrain is moving.
*/
class Terrain : public DisplayableObject {
public:
	Terrain(MyProjectMain *engine, unsigned int width, unsigned int height);
	virtual ~Terrain();

	/* Overrides from DisplayableObject */
	virtual void Draw();
	virtual void DoUpdate(int elapsedTime);
	virtual void GetRedrawRect(SDL_Rect *rectangle);
	virtual void RedrawBackground();

	/* Setters */
	void setMaxDisplacement(double displacement) { m_displacement = displacement; }
	void setRoughness(double roughness)          { m_roughness = roughness;       }
	void setColour(unsigned int colour)          { m_colour = colour;             }
	void setSpeed(double speed)                  { m_speed = speed;               }
protected:
	/* Generic terrain properties */
	unsigned int m_width;
	unsigned int m_height;
	double m_colour;

	/**
	* Generates a new terrain surface and puts the resulting points into
	* the m_yBuffer array.
	*
	* @param leftSeed  the left-most height value. If zero then a random point
	*                  will be generated.
	* @param rightSeed the right-most height value. If zero then a random point
	*                  will be generated.
	*/
	virtual void generate(double leftSeed, double rightSeed);
private:
	int m_numPoints;
	int m_bufSize;

	/* Primary terrain arrays */
	double *m_xPoints;
	double *m_yPoints;

	/* Terrain buffers */
	double *m_xBuffer;
	double *m_yBuffer;

	/* Highest points on the terrain */
	double m_yPointsMin;
	double m_yBufferMin;

	/* General internal terrain properties */
	double m_offset;
	double m_displacement;
	double m_roughness;
	double m_speed;
};

#endif // !TERRAIN_H