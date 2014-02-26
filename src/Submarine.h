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
	Image       *getSubBody()   const  { return m_body;               }
	SDL_Surface *getSurface()   const  { return m_body->getSurface(); }
	double       getFuel()      const  { return m_fuel;               }
	double       getXVelocity() const  { return m_xVelocity;          }
	double       getYVelocity() const  { return m_yVelocity;          }

	/* Setters */
	void setXVelocity(double velocity) { m_xVelocity = velocity;      }
	void setYVelocity(double velocity) { m_yVelocity = velocity;      }
protected:
	static Image *m_body;
private:
	double m_fuel;

	double m_xVelocity;
	double m_yVelocity;
	const double m_maxVelocityX;
	const double m_maxVelocityY;
	const double m_acceleration;

	double m_currentScreenXPrecise;		// A more precise version of m_iCurrentScreenX
	double m_currentScreenYPrecise;		// A More precise version of m_iCurrentScreenY

	/**
	* Handles the submarine movement when the user holds down
	* keys that
	*/
	void controlSub(int elapsedTime);
};

#endif // !SUBMARINE_H