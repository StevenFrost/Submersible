#include "StatusBar.h"
#include "Submarine.h"
#include "MyProjectMain.h"

/* File-scoped constants */
#define BACKGROUND_COLOUR     0x2D4452
#define BORDER_COLOUR         0x292929
#define FONT_COLOUR           0xEEEEEE

#define DISTANCE_LABEL_X       10
#define DISTANCE_LABEL_Y       5
#define DISTANCE_LABEL_HEIGHT  35
#define DISTANCE_LABEL_WIDTH   170
							   
#define POINTS_LABEL_X         200
#define POINTS_LABEL_Y         5
#define POINTS_LABEL_HEIGHT    35
#define POINTS_LABEL_WIDTH     200

#define HIGHSCORE_LABEL_X      350
#define HIGHSCORE_LABEL_Y      5
#define HIGHSCORE_LABEL_HEIGHT 35
#define HIGHSCORE_LABEL_WIDTH  200

#define TIME_LABEL_X           526
#define TIME_LABEL_Y           5
#define TIME_LABEL_HEIGHT      35
#define TIME_LABEL_WIDTH       150

StatusBar::StatusBar(BaseEngine *engine) : DisplayableObject(engine), m_alpha(0xBD), m_height(40), m_distance(0), m_points(0), m_seconds(0.0), m_highPoints(0), m_fuel(100) {
	m_pEngine = dynamic_cast<MyProjectMain *>(engine);
}

StatusBar::~StatusBar() {
}

void StatusBar::initialise() {
	dynamic_cast<Submarine *>(m_pEngine->getStaticObject(MyProjectMain::SUBMARINE))->attach(this);
}

void StatusBar::drawBackground() {
	/* Create a temporary surface for drawing the semi-transparent background */
	SDL_Surface *surface = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_ANYFORMAT,
		m_pEngine->GetScreenWidth(), m_height,
		m_pEngine->GetBackground()->format->BitsPerPixel, 0, 0, 0, 0);

	/* Fill the surface with the solid colour, then apply the alpha */
	SDL_FillRect(surface, NULL, BACKGROUND_COLOUR);
	SDL_SetAlpha(surface, SDL_SRCALPHA, m_alpha);

	/* Blit the temporary surface to the background */
	SDL_Rect rect;
	GetRedrawRect(&rect);
	SDL_BlitSurface(surface, NULL, m_pEngine->GetBackground(), &rect);

	/* Free the temporary surface */
	SDL_FreeSurface(surface);

	/* Draw other static status bar elements */
	m_pEngine->DrawLine(0, m_height, m_pEngine->GetScreenWidth(), 40, BORDER_COLOUR, m_pEngine->GetBackground());
	m_pEngine->DrawLine(m_pEngine->GetScreenWidth() - (138 + 9), 9, m_pEngine->GetScreenWidth() - 9, 9, BORDER_COLOUR, m_pEngine->GetBackground());
	m_pEngine->DrawLine(m_pEngine->GetScreenWidth() - (138 + 9), 30, m_pEngine->GetScreenWidth() - 9, 30, BORDER_COLOUR, m_pEngine->GetBackground());
	m_pEngine->DrawLine(m_pEngine->GetScreenWidth() - (138 + 9), 9, m_pEngine->GetScreenWidth() - (138 + 9), 30, BORDER_COLOUR, m_pEngine->GetBackground());
	m_pEngine->DrawLine(m_pEngine->GetScreenWidth() - 9, 9, m_pEngine->GetScreenWidth() - 9, 30, BORDER_COLOUR, m_pEngine->GetBackground());

	/* Static text */
	m_pEngine->DrawString(900, 5, "Fuel: ", FONT_COLOUR, NULL, m_pEngine->GetBackground());
}

void StatusBar::Draw() {
	char buf[56];
	
	/* Distance */
	sprintf(buf, "Distance: %.0fm", m_distance);
	m_pEngine->CopyBackgroundPixels(DISTANCE_LABEL_X, DISTANCE_LABEL_Y, DISTANCE_LABEL_WIDTH, DISTANCE_LABEL_HEIGHT);
	m_pEngine->DrawString(DISTANCE_LABEL_X, DISTANCE_LABEL_Y, buf, FONT_COLOUR, NULL, m_pEngine->GetForeground());
	m_pEngine->SetNextUpdateRect(DISTANCE_LABEL_X, DISTANCE_LABEL_Y, DISTANCE_LABEL_WIDTH, DISTANCE_LABEL_HEIGHT);

	/* Points */
	sprintf(buf, "Points: %d", m_points);
	m_pEngine->CopyBackgroundPixels(POINTS_LABEL_X, POINTS_LABEL_Y, POINTS_LABEL_WIDTH, POINTS_LABEL_HEIGHT);
	m_pEngine->DrawString(POINTS_LABEL_X, POINTS_LABEL_Y, buf, FONT_COLOUR, NULL, m_pEngine->GetForeground());
	m_pEngine->SetNextUpdateRect(POINTS_LABEL_X, POINTS_LABEL_Y, POINTS_LABEL_WIDTH, POINTS_LABEL_HEIGHT);

	/* Highscore */
	sprintf(buf, "Highscore: %d", m_highPoints);
	m_pEngine->CopyBackgroundPixels(HIGHSCORE_LABEL_X, HIGHSCORE_LABEL_Y, HIGHSCORE_LABEL_WIDTH, HIGHSCORE_LABEL_HEIGHT);
	m_pEngine->DrawString(HIGHSCORE_LABEL_X, HIGHSCORE_LABEL_Y, buf, FONT_COLOUR, NULL, m_pEngine->GetForeground());
	m_pEngine->SetNextUpdateRect(HIGHSCORE_LABEL_X, HIGHSCORE_LABEL_Y, HIGHSCORE_LABEL_WIDTH, HIGHSCORE_LABEL_HEIGHT);

	/* Time */
	sprintf(buf, "%02.0f:%02.0f", floor(m_seconds / 60.0), fmod(floor(m_seconds), 60));
	m_pEngine->CopyBackgroundPixels(TIME_LABEL_X, TIME_LABEL_Y, TIME_LABEL_WIDTH, TIME_LABEL_HEIGHT);
	m_pEngine->DrawString(TIME_LABEL_X, TIME_LABEL_Y, buf, FONT_COLOUR, NULL, m_pEngine->GetForeground());
	m_pEngine->SetNextUpdateRect(TIME_LABEL_X, TIME_LABEL_Y, TIME_LABEL_WIDTH, TIME_LABEL_HEIGHT);

	/* Fuel */
	m_pEngine->CopyBackgroundPixels(m_pEngine->GetScreenWidth() - (138 + 9), 10, 138, 20);
	if (m_fuel > 2.0) {
		unsigned int colour = (((static_cast<int>(fabs(floor(100 - m_fuel) * 2.5)) << 8) + 200) << 8) + 70;
		m_pEngine->DrawRectangle(m_pEngine->GetScreenWidth() - (138 + 8), 10, m_pEngine->GetScreenWidth() - ((fabs(100 - m_fuel) * 1.38) + 10), 29, colour, m_pEngine->GetForeground());
	}
	m_pEngine->SetNextUpdateRect(m_pEngine->GetScreenWidth() - (138 + 9), 10, 138, 20);
}

void StatusBar::update(IObservable *observable) {
	m_fuel = dynamic_cast<Submarine *>(m_pEngine->getStaticObject(MyProjectMain::SUBMARINE))->getFuel();
}

void StatusBar::GetRedrawRect(SDL_Rect *rectangle) {
	rectangle->x = 0;
	rectangle->y = 0;
	rectangle->w = m_pEngine->GetScreenWidth();
	rectangle->h = m_height;
}