#ifndef SUBMARINE_H
#define SUBMARINE_H

#include "Image.h"
#include "header.h"
#include "IObservable.h"
#include "MyProjectMain.h"
#include "DisplayableObject.h"

class Submarine : public DisplayableObject, public IObservable {
public:
	Submarine(MyProjectMain *engine, unsigned int x, unsigned int y);
	virtual ~Submarine();

	/* Overrides from DisplayableObject */
	virtual void Draw();
	virtual void DoUpdate(int elapsedTime);

	/* Getters */
	Image       *getSubBody()   const   { return m_body;                  }
	SDL_Surface *getSurface()   const   { return m_body->getSurface();    }
	double       getFuel()      const   { return m_fuel;                  }
	double       getXVelocity() const   { return m_xVelocity;             }
	double       getYVelocity() const   { return m_yVelocity;             }
	double       getXPosition() const   { return m_currentScreenXPrecise; }
	double       getYPosition() const   { return m_currentScreenYPrecise; }
	double       getImmobilised() const { return m_immobilised;           }

	/* Setters */
	void setXVelocity(double velocity)  { m_xVelocity = velocity;         }
	void setYVelocity(double velocity)  { m_yVelocity = velocity;         }
	void setImmobilised(bool value)     { m_immobilised = value;          }
	void setSubPosition(int x, int y);
protected:
	static Image *m_body;
private:
	/* Generic properties */
	double m_fuel;						// The submarine fuel quantity

	/* Movement */
	double m_xVelocity;					// Submarine x-axis velocity
	double m_yVelocity;					// Submarine y-axis velocity
	const double m_maxVelocityX;		// The maximum allowable x-axis velocity
	const double m_maxVelocityY;		// The maximum allowable y-axis velocity
	const double m_acceleration;		// Acceleration constant
	bool m_immobilised;					// True if submarine movement is disabled

	/* Position */
	double m_currentScreenXPrecise;		// A more precise version of m_iCurrentScreenX
	double m_currentScreenYPrecise;		// A More precise version of m_iCurrentScreenY

	/**
	* Handles the submarine movement when the user holds down keys that affect
	* the submarine control surfaces
	*
	* @param elapsedTime	the elapsed time for the current frame
	*/
	void controlSub(int elapsedTime);
};

#endif // !SUBMARINE_H