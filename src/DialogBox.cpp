#include "DialogBox.h"
#include <cmath>

#define BOX_ALPHA 140

DialogBox::DialogBox(MyProjectMain *engine, int width, int height, Type boxType) : DisplayableObject(engine), m_width(width), m_height(height) {
	/* Calculate the box position*/
	m_iCurrentScreenX = m_iPreviousScreenX = static_cast<int>((m_pEngine->GetScreenWidth() - width) / 2);
	m_iCurrentScreenY = m_iPreviousScreenY = static_cast<int>((m_pEngine->GetScreenHeight() - height) / 2);
	m_iDrawHeight = height;
	m_iDrawWidth = width;

	/* Create the RGB surface */
	m_boxBase = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_ANYFORMAT,
				m_width, m_height, m_pEngine->GetForeground()->format->BitsPerPixel,
				m_pEngine->GetForeground()->format->Rmask, m_pEngine->GetForeground()->format->Gmask,
				m_pEngine->GetForeground()->format->Bmask, m_pEngine->GetForeground()->format->Amask);
	
	/* Fill the surface with the solid colour, then apply the alpha */
	if (boxType == Type::CRASH) {
		SDL_FillRect(m_boxBase, NULL, static_cast<Uint32>(boxType) - 0x430000);
	} else {
		SDL_FillRect(m_boxBase, NULL, static_cast<Uint32>(boxType) - 0x131C22);
	}

	m_pEngine->DrawRectangle(10, 10, m_width - 10, m_height - 10, static_cast<Uint32>(boxType), m_boxBase);
	SDL_SetAlpha(m_boxBase, SDL_SRCALPHA, BOX_ALPHA);
}

DialogBox::~DialogBox() {
	SDL_FreeSurface(m_boxBase);
}

void DialogBox::Draw() {
	SDL_Rect rect;
	GetRedrawRect(&rect);
	SDL_BlitSurface(m_boxBase, NULL, m_pEngine->GetForeground(), &rect);
}

void DialogBox::DoUpdate(int elapsedTime) {}

void DialogBox::GetRedrawRect(SDL_Rect *rectangle) {
	rectangle->x = m_iCurrentScreenX;
	rectangle->y = m_iCurrentScreenY;
	rectangle->w = m_width;
	rectangle->h = m_height;
}

void CrashedDialogBox::Draw() {
	/* First draw the base dialog box */
	DialogBox::Draw();

	/* Text to be drawn */
	static const char *distance = "Distance:";
	static const char *points   = "Points:";
	static const char *time     = "Time:";
	static const char *restart  = "R = Restart";
	static const char *menu     = "M = Menu";

	/* Draw the box title */
	MyProjectMain *engine = dynamic_cast<MyProjectMain *>(m_pEngine);
	Font *font = m_pEngine->GetFont("../resources/Segoe UI.ttf", 50);

	int titleX = static_cast<int>((m_width - engine->getStringWidth(m_str, 50)) / 2) + m_iCurrentScreenX;
	m_pEngine->DrawString(titleX, 20 + m_iCurrentScreenY, m_str, 0xFFFFFF, font, m_pEngine->GetForeground());

	/* Draw the body containing score information */
	char buf[56];
	double seconds = m_statusBar->getSeconds();
	font = m_pEngine->GetFont("../resources/Segoe UI.ttf", 18);

	/* Distance */
	m_pEngine->DrawString(m_iCurrentScreenX + 200, 100 + m_iCurrentScreenY, distance, 0xFFFFFF, font, m_pEngine->GetForeground());
	sprintf(buf, "%.0fm", m_statusBar->getDistance());
	m_pEngine->DrawString(m_iCurrentScreenX + 300, 100 + m_iCurrentScreenY, buf, 0xFFFFFF, font, m_pEngine->GetForeground());
	
	/* Points */
	m_pEngine->DrawString(m_iCurrentScreenX + 200, 130 + m_iCurrentScreenY, points, 0xFFFFFF, font, m_pEngine->GetForeground());
	sprintf(buf, "%d", m_statusBar->getPoints());
	m_pEngine->DrawString(m_iCurrentScreenX + 300, 130 + m_iCurrentScreenY, buf, 0xFFFFFF, font, m_pEngine->GetForeground());

	/* Time */
	m_pEngine->DrawString(m_iCurrentScreenX + 200, 160 + m_iCurrentScreenY, time, 0xFFFFFF, font, m_pEngine->GetForeground());
	sprintf(buf, "%02.0f:%02.0f", floor(seconds / 60.0), fmod(floor(seconds), 60));
	m_pEngine->DrawString(m_iCurrentScreenX + 300, 160 + m_iCurrentScreenY, buf, 0xFFFFFF, font, m_pEngine->GetForeground());

	/* Restart label */
	int xPos = m_iCurrentScreenX + 30;
	m_pEngine->DrawString(xPos, m_height - 50 + m_iCurrentScreenY, restart, 0xFFFFFF, font, m_pEngine->GetForeground());

	/* Menu label */
	xPos = m_width - 30 - engine->getStringWidth(menu, 18) + m_iCurrentScreenX;
	m_pEngine->DrawString(xPos, m_height - 50 + m_iCurrentScreenY, menu, 0xFFFFFF, font, m_pEngine->GetForeground());
}

void PausedDialogBox::Draw() {
	/* First draw the base dialog box */
	DialogBox::Draw();

	/* Text to be drawn */
	static const char *title = "PAUSED";
	static const char *message = "Press 'P' to continue";

	/* Draw the box title */
	MyProjectMain *engine = dynamic_cast<MyProjectMain *>(m_pEngine);
	Font *font = m_pEngine->GetFont("../resources/Segoe UI.ttf", 50);

	/* Title */
	int xPos = static_cast<int>((m_width - engine->getStringWidth(title, 50)) / 2) + m_iCurrentScreenX;
	m_pEngine->DrawString(xPos, 20 + m_iCurrentScreenY, title, 0xFFFFFF, font, m_pEngine->GetForeground());

	/* Message */
	char buf[56];
	font = m_pEngine->GetFont("../resources/Segoe UI.ttf", 18);
	xPos = static_cast<int>((m_width - engine->getStringWidth(message, 18)) / 2) + m_iCurrentScreenX;
	m_pEngine->DrawString(xPos, 145 + m_iCurrentScreenY, message, 0xFFFFFF, font, m_pEngine->GetForeground());
}

void HelpDialogBox::Draw() {
	/* First draw the base dialog box */
	DialogBox::Draw();

	/* Text to be drawn */
	static const char *title = "HELP";
	static const char *message = "Press 'H' to continue";

	/* Draw the box title */
	MyProjectMain *engine = dynamic_cast<MyProjectMain *>(m_pEngine);
	Font *font = m_pEngine->GetFont("../resources/Segoe UI.ttf", 50);

	/* Title */
	int xPos = static_cast<int>((m_width - engine->getStringWidth(title, 50)) / 2) + m_iCurrentScreenX;
	m_pEngine->DrawString(xPos, 20 + m_iCurrentScreenY, title, 0xFFFFFF, font, m_pEngine->GetForeground());

	/* Message */
	char buf[56];
	font = m_pEngine->GetFont("../resources/Segoe UI.ttf", 18);
	xPos = static_cast<int>((m_width - engine->getStringWidth(message, 18)) / 2) + m_iCurrentScreenX;
	m_pEngine->DrawString(xPos, 145 + m_iCurrentScreenY, message, 0xFFFFFF, font, m_pEngine->GetForeground());
}

void MainDialogBox::Draw() {
	/* First draw the base dialog box */
	DialogBox::Draw();

	/* Text to be drawn */
	static const char *title = "SUBMERSIBLE";
	static const char *body1 = "An underwater endurance-based submarine game built using";
	static const char *body2 = "C++ and SDL 1.2.";
	static const char *body3 = "This is a project for the G52CPP (C++ Programming) module";
	static const char *body4 = "at the University of Nottingham, 2013/14.";
	static const char *play = "P = Play";
	static const char *help = "H = Help";

	/* Draw the box title */
	MyProjectMain *engine = dynamic_cast<MyProjectMain *>(m_pEngine);
	Font *font = m_pEngine->GetFont("../resources/Segoe UI.ttf", 50);

	/* Title */
	int xPos = static_cast<int>((m_width - engine->getStringWidth(title, 50)) / 2) + m_iCurrentScreenX;
	m_pEngine->DrawString(xPos, 20 + m_iCurrentScreenY, title, 0xFFFFFF, font, m_pEngine->GetForeground());

	/* Body */
	char buf[56];
	font = m_pEngine->GetFont("../resources/Segoe UI.ttf", 18);
	xPos = static_cast<int>((m_width - engine->getStringWidth(body1, 18)) / 2) + m_iCurrentScreenX;
	m_pEngine->DrawString(xPos, 100 + m_iCurrentScreenY, body1, 0xFFFFFF, font, m_pEngine->GetForeground());
	xPos = static_cast<int>((m_width - engine->getStringWidth(body2, 18)) / 2) + m_iCurrentScreenX;
	m_pEngine->DrawString(xPos, 122 + m_iCurrentScreenY, body2, 0xFFFFFF, font, m_pEngine->GetForeground());
	xPos = static_cast<int>((m_width - engine->getStringWidth(body3, 18)) / 2) + m_iCurrentScreenX;
	m_pEngine->DrawString(xPos, 175 + m_iCurrentScreenY, body3, 0xFFFFFF, font, m_pEngine->GetForeground());
	xPos = static_cast<int>((m_width - engine->getStringWidth(body4, 18)) / 2) + m_iCurrentScreenX;
	m_pEngine->DrawString(xPos, 197 + m_iCurrentScreenY, body4, 0xFFFFFF, font, m_pEngine->GetForeground());

	/* Play label */
	xPos = m_iCurrentScreenX + 30;
	m_pEngine->DrawString(xPos, m_height - 50 + m_iCurrentScreenY, play, 0xFFFFFF, font, m_pEngine->GetForeground());

	/* Help label */
	xPos = m_width - 30 - engine->getStringWidth(help, 18) + m_iCurrentScreenX;
	m_pEngine->DrawString(xPos, m_height - 50 + m_iCurrentScreenY, help, 0xFFFFFF, font, m_pEngine->GetForeground());
}