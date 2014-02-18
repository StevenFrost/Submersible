#include "StatusBar.h"

/* File-scoped constants */
static const unsigned int BACKGROUND_COLOUR = 0x2D4452;
static const unsigned int BORDER_COLOUR     = 0x292929;
static const unsigned int FONT_COLOUR       = 0xEEEEEE;

StatusBar::StatusBar(MyProjectMain *engine) : DisplayableObject(engine), m_alpha(0xBD), m_height(40) {}

StatusBar::~StatusBar() {}

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
	m_pEngine->DrawRectangle(m_pEngine->GetScreenWidth() - (138 + 9), 9, m_pEngine->GetScreenWidth() - 9, 30, BORDER_COLOUR, m_pEngine->GetBackground());
	m_pEngine->DrawRectangle(m_pEngine->GetScreenWidth() - (138 + 8), 10, m_pEngine->GetScreenWidth() - 10, 29, 0x1CC839, m_pEngine->GetBackground());

	/* Static text */
	m_pEngine->DrawString(900, 5, "Fuel: ", FONT_COLOUR, NULL, m_pEngine->GetBackground());
	m_pEngine->DrawString(10, 5, "Distance: 0m", FONT_COLOUR, NULL, m_pEngine->GetBackground());
	m_pEngine->DrawString(200, 5, "Points: 0", FONT_COLOUR, NULL, m_pEngine->GetBackground());
	m_pEngine->DrawString(526, 5, "00:00", 0xEE0000, NULL, m_pEngine->GetBackground());
}

void StatusBar::Draw() {
	//char buf[128];
	//sprintf(buf, "Changing text %6d %6d", rand(), rand());
	//m_pEngine->CopyBackgroundPixels(0, 0, m_pEngine->GetScreenWidth(), 35);
	//m_pEngine->DrawScreenString(150, 10, buf, 0xffffff, NULL);
	//m_pEngine->SetNextUpdateRect(0, 0, m_pEngine->GetScreenWidth(), 35);
}

void StatusBar::DoUpdate(int elapsedTime) {

}

void StatusBar::GetRedrawRect(SDL_Rect *rectangle) {
	rectangle->x = 0;
	rectangle->y = 0;
	rectangle->w = m_pEngine->GetScreenWidth();
	rectangle->h = m_height;
}

void StatusBar::RedrawBackground() {

}