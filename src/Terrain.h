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
* and a colour of 0x232A2F.
*
* Background: 0xD3D4D5
*
* Y-Buffer layout: | Bottom-Left Point | All terrain suface points | Bottom-Right point |
*                  |         0         |   1 .. (m_bufSize - 2)    |  (m_bufSize - 1)   |
*/
class Terrain : public DisplayableObject {
public:
	Terrain(MyProjectMain *engine, unsigned int width, unsigned int height);
	virtual ~Terrain();

	/* Overrides from DisplayableObject */
	virtual void Draw();
	virtual void DoUpdate(int iCurrentTime);

	/* Terrain configuration */
	void setMaxDisplacement(double displacement) { m_displacement = displacement; }
	void setRoughness(double roughness)          { m_roughness = roughness;       }
	void setColour(unsigned int colour)          { m_colour = colour;             }
	void setSpeed(double speed)                  { m_speed = speed;               }
protected:
	unsigned int m_width;
	unsigned int m_height;

	void generate(double leftSeed, double rightSeed);
private:
	int m_numPoints;
	int m_bufSize;

	/* Buffers containing height values for the terrain (y-axis) */
	double *m_yPoints;
	double *m_yBuffer;

	/* Buffers used for drawing polygons */
	double *m_xPoints;
	double *m_xBuffer;

	/* General internal terrain properties */
	double m_offset;
	double m_displacement;
	double m_roughness;
	double m_colour;

	double m_speed;
};

#endif // !TERRAIN_H