#include "Submarine.h"

Image *Submarine::m_body = new Image();

Submarine::Submarine(MyProjectMain *engine, unsigned int x, unsigned int y) : DisplayableObject(engine),
	m_fuel(100.0),
	m_maxVelocityX(0.25),
	m_maxVelocityY(0.4),
	m_acceleration(0.6),
	m_xVelocity(0.0),
	m_yVelocity(0.0)
{
	/* Load the graphic */
	if (!m_body->IsLoaded()) {
		m_body->LoadImage("../resources/sub.png");
	}
	
	/* Set the initial position */
	m_iStartDrawPosX = 0;
	m_iStartDrawPosY = 0;
	m_iCurrentScreenX = x;
	m_iCurrentScreenY = y;
	m_iDrawWidth = m_body->GetWidth();
	m_iDrawHeight = m_body->GetHeight();
	m_iPreviousScreenX = m_iCurrentScreenX;
	m_iPreviousScreenY = m_iCurrentScreenY;
	m_currentScreenXPrecise = m_iCurrentScreenX;
	m_currentScreenYPrecise = m_iCurrentScreenY;
}

Submarine::~Submarine() {
	delete(m_body);
}

void Submarine::Draw() {
	/* Draw the main submarine body */
	m_body->RenderImage(m_pEngine->GetForeground(), 0, 0, m_currentScreenXPrecise, m_currentScreenYPrecise, m_body->GetWidth(), m_body->GetHeight());

	/* Update the submarine position cache */
	StoreLastScreenPositionAndUpdateRect();
}

void Submarine::DoUpdate(int elapsedTime) {
	if (m_immobilised) return;

	/* Update the submarine position */
	controlSub(elapsedTime);

	/* Calculate the quantity of fuel remaining */
	m_fuel -= (0.2 * (elapsedTime / 100.0));

	/* Notify any observers that something changed */
	notify();
}

void Submarine::controlSub(int elapsedTime) {
	double secondsThisFrame = elapsedTime / 1000.0;
	double delta = m_acceleration * secondsThisFrame;
	static bool subRight = true;
	static bool subUp = true;

	/* Handle Right, Left, Up and Down array key presses for submarine movement */
	if (m_pEngine->IsKeyPressed(SDLK_RIGHT)) {
		subRight = true;
		m_xVelocity += (m_xVelocity < m_maxVelocityX ? delta : 0);
	} else if (m_pEngine->IsKeyPressed(SDLK_LEFT)) {
		subRight = false;
		m_xVelocity -= (m_xVelocity > -m_maxVelocityX ? delta : 0);
	} else {
		if (subRight) {
			m_xVelocity -= (m_xVelocity > 0.0 ? delta : 0);
		} else {
			m_xVelocity += (m_xVelocity < -0.0 ? delta : 0);
		}

		if (abs(m_xVelocity) < 0.01) {
			m_xVelocity = 0.0;
		}
	}
	if (m_pEngine->IsKeyPressed(SDLK_UP)) {
		subUp = true;
		m_yVelocity -= (m_yVelocity > -m_maxVelocityY ? delta : 0);
	} else if (m_pEngine->IsKeyPressed(SDLK_DOWN)) {
		subUp = false;
		m_yVelocity += (m_yVelocity < m_maxVelocityY ? delta : 0);
	} else {
		if (subUp) {
			m_yVelocity += (m_yVelocity < -0.0 ? delta : 0);
		} else {
			m_yVelocity -= (m_yVelocity > 0.0 ? delta : 0);
		}

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
	if (m_currentScreenYPrecise >(m_pEngine->GetScreenHeight() - m_body->GetHeight())) {
		m_iCurrentScreenY = (m_pEngine->GetScreenHeight() - m_body->GetHeight());
		m_currentScreenYPrecise = m_iCurrentScreenY;
		m_yVelocity = 0.0;
	}
	if (m_currentScreenXPrecise < 0) {
		m_iCurrentScreenX = 0;
		m_currentScreenXPrecise = m_iCurrentScreenX;
		m_xVelocity = 0;
	}
	if (m_currentScreenXPrecise >(m_pEngine->GetScreenWidth() - m_body->GetWidth())) {
		m_iCurrentScreenX = (m_pEngine->GetScreenWidth() - m_body->GetWidth());
		m_currentScreenXPrecise = m_iCurrentScreenX;
		m_xVelocity = 0;
	}
}

void Submarine::setSubPosition(int x, int y) {
	m_currentScreenXPrecise = m_iCurrentScreenX = x;
	m_currentScreenYPrecise = m_iCurrentScreenY = y;
}