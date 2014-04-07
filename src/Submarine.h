#ifndef SUBMARINE_H
#define SUBMARINE_H

#include "Image.h"
#include "Flare.h"
#include "header.h"
#include "BaseEngine.h"
#include "GameObject.h"
#include "IObservable.h"
#include "DisplayableObject.h"

class Submarine : public GameObject, public IObservable {
public:
	Submarine(BaseEngine *engine, unsigned int x, unsigned int y);
	virtual ~Submarine() {}

	/* Overrides from DisplayableObject */
	virtual void Draw();
	virtual void DoUpdate(int elapsedTime);

	/* Getters */
	Image       *getSubBody()           const { return &m_body;                 }
	SDL_Surface *getCollidableSurface() const { return m_body.getSurface();     }
	double       getFuel()			    const { return m_fuel;                  }
	double       getXVelocity()		    const { return m_xVelocity;             }
	double       getYVelocity()		    const { return m_yVelocity;             }
	double		 getMaxVelocityX()		const { return m_maxVelocityX;          }
	double       getXPosition()		    const { return m_currentScreenXPrecise; }
	double       getYPosition()		    const { return m_currentScreenYPrecise; }
	double       getImmobilised()	    const { return m_immobilised;           }
	Flare		*getFlare()             const { return m_flare;                 }

	/* Setters */
	void setXVelocity(double velocity)  { m_xVelocity = velocity;         }
	void setYVelocity(double velocity)  { m_yVelocity = velocity;         }
	void setImmobilised(bool value)     { m_immobilised = value;          }
	void setFuelLevel(double level)     { m_fuel = level; notify();       }
	void setSubPosition(int x, int y);
protected:
	static Image m_body;				// Main submarine body
	Flare *m_flare;						// Single flare associated with the submarine

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

	/**
	* Handles the submarine movement when the user holds down keys that affect
	* the submarine control surfaces
	*/
	void controlSub(int elapsedTime);
};

#endif // !SUBMARINE_H