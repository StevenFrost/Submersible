#ifndef STATUS_BAR_H
#define STATUS_BAR_H

#include "header.h"
#include "IObserver.h"
#include "BaseEngine.h"
#include "DisplayableObject.h"

class MyProjectMain;

class StatusBar : public DisplayableObject, public IObserver {
public:
	StatusBar(BaseEngine *engine);
	virtual ~StatusBar();

	/* Overrides from DisplayableObject */
	virtual void Draw();
	virtual void GetRedrawRect(SDL_Rect *rectangle);

	/* Observer implementation */
	virtual void update(IObservable *observable);

	void initialise();
	void drawBackground();

	/* Setters */
	void		 incrementTime(double delta)     { m_seconds += delta;   }
	void		 incrementPoints()               { m_points++;           }
	void		 incrementDistance(double delta) { m_distance += delta;  }
	void         setHighscore(int val)           { m_highPoints = val;   }
	
	/* Reset functions */
	void		 resetTime()					 { m_seconds = 0;		 }
	void		 resetPoints()					 { m_points = 0;		 }
	void		 resetDistance()				 { m_distance = 0;		 }

	/* Getters */
	double       getDistance()  const			 { return m_distance;	 }
	unsigned int getPoints()    const			 { return m_points;		 }
	double       getSeconds()   const			 { return m_seconds;	 }
	unsigned int getHighscore() const            { return m_highPoints;  }
protected:
	MyProjectMain *m_pEngine;

	double m_distance;
	unsigned int m_points;
	double m_seconds;
	unsigned int m_highPoints;
private:
	unsigned int m_alpha;
	unsigned int m_height;
	unsigned int m_fuelColour;

	double m_fuel;
};

#endif // !STATUS_BAR_H