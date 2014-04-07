#include "Submarine.h"
#include "GameObject.h"

Image Submarine::m_body = Image();

Submarine::Submarine(BaseEngine *engine, unsigned int x, unsigned int y) : GameObject(engine, SUBMARINE), m_flare(new Flare(engine, x, y)), m_fuel(100.0), m_maxVelocityX(0.3), m_maxVelocityY(0.4), m_acceleration(0.6), m_xVelocity(0.0), m_yVelocity(0.0) {
	/* GameObject member initialisation */
	m_bVisible = true;
	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;
	m_currentScreenXPrecise = m_iCurrentScreenX = m_iPreviousScreenX = x;
	m_currentScreenYPrecise = m_iCurrentScreenY = m_iPreviousScreenY = y;

	/* Load the graphic */
	if (!m_body.IsLoaded()) {
		m_body.LoadImage("../resources/sub.png");
	}
	
	/* Redraw rectangle width/height */
	m_iDrawWidth = m_body.GetWidth() + 2;
	m_iDrawHeight = m_body.GetHeight() + 2;
}

void Submarine::Draw() {
	if (!m_bVisible) return;

	/* Draw the main submarine body */
	m_body.RenderImage(m_pEngine->GetForeground(), 0, 0, m_currentScreenXPrecise + 1, m_currentScreenYPrecise + 1, m_body.GetWidth(), m_body.GetHeight());
}

void Submarine::DoUpdate(int elapsedTime) {
	if (m_immobilised) return;

	/* Save the previous position */
	m_iPreviousScreenX = m_iCurrentScreenX;
	m_iPreviousScreenY = m_iCurrentScreenY;

	/* Update the submarine position */
	controlSub(elapsedTime);

	/* Calculate the quantity of fuel remaining */
	m_fuel -= (0.2 * (elapsedTime / 100.0));
	notify();

	/* Update the redraw rectangle position */
	StoreLastScreenPositionAndUpdateRect();
}

void Submarine::controlSub(int elapsedTime) {
	double secondsThisFrame = elapsedTime / 1000.0;
	double delta = m_acceleration * secondsThisFrame;

	/* Right and Left movement */
	if (m_pEngine->IsKeyPressed(SDLK_RIGHT)) {
		m_xVelocity += (m_xVelocity < m_maxVelocityX ? delta : 0);
	} else if (m_pEngine->IsKeyPressed(SDLK_LEFT)) {
		m_xVelocity -= (m_xVelocity > -m_maxVelocityX ? delta : 0);
	} else {
		if (m_xVelocity > 0.0) {
			m_xVelocity -= (m_xVelocity > 0.0 ? delta : 0);
		} else {
			m_xVelocity += (m_xVelocity < -0.0 ? delta : 0);
		}

		/* Solves floating point 'sliding' issues */
		if (abs(m_xVelocity) < 0.01) {
			m_xVelocity = 0.0;
		}
	}

	/* Up and down movement */
	if (m_pEngine->IsKeyPressed(SDLK_UP)) {
		m_yVelocity -= (m_yVelocity > -m_maxVelocityY ? delta : 0);
	} else if (m_pEngine->IsKeyPressed(SDLK_DOWN)) {
		m_yVelocity += (m_yVelocity < m_maxVelocityY ? delta : 0);
	} else {
		if (m_yVelocity < 0.0) {
			m_yVelocity += (m_yVelocity < -0.0 ? delta : 0);
		} else {
			m_yVelocity -= (m_yVelocity > 0.0 ? delta : 0);
		}

		/* Solves floating point 'sliding' issues */
		if (abs(m_yVelocity) < 0.01) {
			m_yVelocity = 0.0;
		}
	}

	/* Update the precise submarine location */
	m_currentScreenXPrecise += m_xVelocity;
	m_currentScreenYPrecise += m_yVelocity;

	/* Keep the less-precise coordinates so we can stil redraw easily */
	m_iCurrentScreenX = static_cast<int>(m_currentScreenXPrecise);
	m_iCurrentScreenY = static_cast<int>(m_currentScreenYPrecise);

	/* Limit the range of the submarine */
	if (m_currentScreenYPrecise < 125) {
		m_iCurrentScreenY = 125;
		m_currentScreenYPrecise = m_iCurrentScreenY;
		m_yVelocity = 0.0;
	}
	if (m_currentScreenYPrecise >(m_pEngine->GetScreenHeight() - m_body.GetHeight())) {
		m_iCurrentScreenY = (m_pEngine->GetScreenHeight() - m_body.GetHeight());
		m_currentScreenYPrecise = m_iCurrentScreenY;
		m_yVelocity = 0.0;
	}
	if (m_currentScreenXPrecise < 0) {
		m_iCurrentScreenX = 0;
		m_currentScreenXPrecise = m_iCurrentScreenX;
		m_xVelocity = 0;
	}
	if (m_currentScreenXPrecise >(m_pEngine->GetScreenWidth() - m_body.GetWidth())) {
		m_iCurrentScreenX = (m_pEngine->GetScreenWidth() - m_body.GetWidth());
		m_currentScreenXPrecise = m_iCurrentScreenX;
		m_xVelocity = 0;
	}
}

void Submarine::setSubPosition(int x, int y) {
	/* Store the old position */
	m_iPreviousScreenX = m_iCurrentScreenX;
	m_iPreviousScreenY = m_iCurrentScreenY;

	/* Set the new position */
	m_currentScreenXPrecise = m_iCurrentScreenX = x;
	m_currentScreenYPrecise = m_iCurrentScreenY = y;

	StoreLastScreenPositionAndUpdateRect();
}