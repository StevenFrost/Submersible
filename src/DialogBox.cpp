#include <cmath>
#include "DialogBox.h"
#include "MyProjectMain.h"

#define BOX_ALPHA 140

DialogBox::DialogBox(BaseEngine *engine, int width, int height, Type boxType) : DisplayableObject(engine) {
	/* DisplayableObject member initialisation */
	m_iCurrentScreenX = m_iPreviousScreenX = static_cast<int>((m_pEngine->GetScreenWidth() - width) / 2);
	m_iCurrentScreenY = m_iPreviousScreenY = static_cast<int>((m_pEngine->GetScreenHeight() - height) / 2);
	m_iDrawHeight = height;
	m_iDrawWidth = width;
	m_bVisible = true;

	/* Create the dialog RGB surface */
	m_boxBase = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_ANYFORMAT,
		m_iDrawWidth, m_iDrawHeight, m_pEngine->GetForeground()->format->BitsPerPixel,
		m_pEngine->GetForeground()->format->Rmask, m_pEngine->GetForeground()->format->Gmask,
		m_pEngine->GetForeground()->format->Bmask, m_pEngine->GetForeground()->format->Amask);
	
	/* Fill the surface with the solid colour, then apply the alpha */
	if (boxType == Type::CRASH) {
		SDL_FillRect(m_boxBase, NULL, static_cast<Uint32>(boxType) - 0x430000);
	} else {
		SDL_FillRect(m_boxBase, NULL, static_cast<Uint32>(boxType) - 0x131C22);
	}

	/* Draw the base dialog box rectangle */
	m_pEngine->DrawRectangle(10, 10, m_iDrawWidth - 10, m_iDrawHeight - 10, static_cast<Uint32>(boxType), m_boxBase);
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

void DialogBox::DoUpdate(int elapsedTime) {
	/* Update the redraw rectangle */
	StoreLastScreenPositionAndUpdateRect();
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
	Font *font = m_pEngine->GetFont("../resources/NotoSans-Regular.ttf", 50);

	int titleX = static_cast<int>((m_iDrawWidth - engine->getStringWidth(m_str, 50)) / 2) + m_iCurrentScreenX;
	m_pEngine->DrawString(titleX, 20 + m_iCurrentScreenY, m_str, 0xFFFFFF, font, m_pEngine->GetForeground());

	/* Draw the body containing score information */
	char buf[56];
	double seconds = m_statusBar->getSeconds();
	font = m_pEngine->GetFont("../resources/NotoSans-Regular.ttf", 18);

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
	m_pEngine->DrawString(xPos, m_iDrawHeight - 50 + m_iCurrentScreenY, restart, 0xFFFFFF, font, m_pEngine->GetForeground());

	/* Menu label */
	xPos = m_iDrawWidth - 30 - engine->getStringWidth(menu, 18) + m_iCurrentScreenX;
	m_pEngine->DrawString(xPos, m_iDrawHeight - 50 + m_iCurrentScreenY, menu, 0xFFFFFF, font, m_pEngine->GetForeground());
}

void PausedDialogBox::Draw() {
	/* First draw the base dialog box */
	DialogBox::Draw();

	/* Text to be drawn */
	static const char *title = "PAUSED";
	static const char *message = "Press 'P' to continue";

	/* Draw the box title */
	MyProjectMain *engine = dynamic_cast<MyProjectMain *>(m_pEngine);
	Font *font = m_pEngine->GetFont("../resources/NotoSans-Regular.ttf", 50);

	/* Title */
	int xPos = static_cast<int>((m_iDrawWidth - engine->getStringWidth(title, 50)) / 2) + m_iCurrentScreenX;
	m_pEngine->DrawString(xPos, 20 + m_iCurrentScreenY, title, 0xFFFFFF, font, m_pEngine->GetForeground());

	/* Message */
	char buf[56];
	font = m_pEngine->GetFont("../resources/NotoSans-Regular.ttf", 18);
	xPos = static_cast<int>((m_iDrawWidth - engine->getStringWidth(message, 18)) / 2) + m_iCurrentScreenX;
	m_pEngine->DrawString(xPos, 145 + m_iCurrentScreenY, message, 0xFFFFFF, font, m_pEngine->GetForeground());
}

void HelpDialogBox::Draw() {
	/* First draw the base dialog box */
	DialogBox::Draw();

	/* Text to be drawn */
	static const char *title = "HELP";
	static const char *message = "Press 'H' to return to the menu";

	/* Draw the box title */
	MyProjectMain *engine = dynamic_cast<MyProjectMain *>(m_pEngine);
	Font *font = m_pEngine->GetFont("../resources/NotoSans-Regular.ttf", 50);

	/* Title */
	int xPos = static_cast<int>((m_iDrawWidth - engine->getStringWidth(title, 50)) / 2) + m_iCurrentScreenX;
	m_pEngine->DrawString(xPos, 20 + m_iCurrentScreenY, title, 0xFFFFFF, font, m_pEngine->GetForeground());

	/* Help text */
	font = m_pEngine->GetFont("../resources/NotoSans-Regular.ttf", 16);
	m_pEngine->DrawString(m_iCurrentScreenX + 30, m_iCurrentScreenY + 100, "Controls:", 0xFFFFFF, font, m_pEngine->GetForeground());
	m_pEngine->DrawString(m_iCurrentScreenX + 70, m_iCurrentScreenY + 122, "- Press and hold the UP, DOWN, LEFT and RIGHT arrow keys to control", 0xFFFFFF, font, m_pEngine->GetForeground());
	m_pEngine->DrawString(m_iCurrentScreenX + 80, m_iCurrentScreenY + 144, "the submarine position.", 0xFFFFFF, font, m_pEngine->GetForeground());
	m_pEngine->DrawString(m_iCurrentScreenX + 70, m_iCurrentScreenY + 166, "- Press F to release a flare, only one can be active at any time", 0xFFFFFF, font, m_pEngine->GetForeground());
	m_pEngine->DrawString(m_iCurrentScreenX + 70, m_iCurrentScreenY + 188, "- Press P to pause the game", 0xFFFFFF, font, m_pEngine->GetForeground());

	m_pEngine->DrawString(m_iCurrentScreenX + 30, m_iCurrentScreenY + 232, "Gameplay:", 0xFFFFFF, font, m_pEngine->GetForeground());
	m_pEngine->DrawString(m_iCurrentScreenX + 70, m_iCurrentScreenY + 254, "- Collect as many coins as possible while avoiding moored mines,", 0xFFFFFF, font, m_pEngine->GetForeground());
	m_pEngine->DrawString(m_iCurrentScreenX + 80, m_iCurrentScreenY + 276, "torpedoes and rising mines.", 0xFFFFFF, font, m_pEngine->GetForeground());
	m_pEngine->DrawString(m_iCurrentScreenX + 70, m_iCurrentScreenY + 298, "- Torpedoes are heat-seeking and can be diverted by releasing flares", 0xFFFFFF, font, m_pEngine->GetForeground());
	m_pEngine->DrawString(m_iCurrentScreenX + 70, m_iCurrentScreenY + 320, "- Hitting any part of the terrain will result in a crash", 0xFFFFFF, font, m_pEngine->GetForeground());

	/* Message */
	font = m_pEngine->GetFont("../resources/NotoSans-Regular.ttf", 18);
	xPos = static_cast<int>((m_iDrawWidth - engine->getStringWidth(message, 18)) / 2) + m_iCurrentScreenX;
	m_pEngine->DrawString(xPos, m_iDrawHeight - 50 + m_iCurrentScreenY, message, 0xFFFFFF, font, m_pEngine->GetForeground());
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
	Font *font = m_pEngine->GetFont("../resources/NotoSans-Regular.ttf", 50);

	/* Title */
	int xPos = static_cast<int>((m_iDrawWidth - engine->getStringWidth(title, 50)) / 2) + m_iCurrentScreenX;
	m_pEngine->DrawString(xPos, 20 + m_iCurrentScreenY, title, 0xFFFFFF, font, m_pEngine->GetForeground());

	/* Body */
	font = m_pEngine->GetFont("../resources/NotoSans-Regular.ttf", 16);
	xPos = static_cast<int>((m_iDrawWidth - engine->getStringWidth(body1, 16)) / 2) + m_iCurrentScreenX;
	m_pEngine->DrawString(xPos, 100 + m_iCurrentScreenY, body1, 0xFFFFFF, font, m_pEngine->GetForeground());
	xPos = static_cast<int>((m_iDrawWidth - engine->getStringWidth(body2, 16)) / 2) + m_iCurrentScreenX;
	m_pEngine->DrawString(xPos, 122 + m_iCurrentScreenY, body2, 0xFFFFFF, font, m_pEngine->GetForeground());
	xPos = static_cast<int>((m_iDrawWidth - engine->getStringWidth(body3, 16)) / 2) + m_iCurrentScreenX;
	m_pEngine->DrawString(xPos, 166 + m_iCurrentScreenY, body3, 0xFFFFFF, font, m_pEngine->GetForeground());
	xPos = static_cast<int>((m_iDrawWidth - engine->getStringWidth(body4, 16)) / 2) + m_iCurrentScreenX;
	m_pEngine->DrawString(xPos, 188 + m_iCurrentScreenY, body4, 0xFFFFFF, font, m_pEngine->GetForeground());

	/* Play label */
	font = m_pEngine->GetFont("../resources/NotoSans-Regular.ttf", 18);
	xPos = m_iCurrentScreenX + 30;
	m_pEngine->DrawString(xPos, m_iDrawHeight - 50 + m_iCurrentScreenY, play, 0xFFFFFF, font, m_pEngine->GetForeground());

	/* Help label */
	xPos = m_iDrawWidth - 30 - engine->getStringWidth(help, 18) + m_iCurrentScreenX;
	m_pEngine->DrawString(xPos, m_iDrawHeight - 50 + m_iCurrentScreenY, help, 0xFFFFFF, font, m_pEngine->GetForeground());
}