#include "Waves.h"

Image Waves::m_base = Image();

Waves::Waves(BaseEngine *engine, double speed) : DisplayableObject(engine), m_speed(speed), m_offset(0.0) {
	/* Load the image if needed */
	if (!m_base.IsLoaded()) {
		m_base.LoadImage("../resources/waves.png");
	}
}

void Waves::Draw() {
	for (int i = 0; i < m_pEngine->GetScreenWidth() + 250; i += 202) {
		m_base.RenderImage(m_pEngine->GetForeground(), 0, 0, m_offset + i, 100, 202, 20);
	}
}

void Waves::DoUpdate(int elapsedTime) {
	if (m_offset < -202) m_offset = 0;
	m_offset -= m_speed * (elapsedTime / 1000.0);
}

void Waves::GetRedrawRect(SDL_Rect *rectangle) {
	rectangle->x = 0;
	rectangle->y = 100;
	rectangle->w = m_pEngine->GetScreenWidth();
	rectangle->h = 20;
}

void Waves::RedrawBackground() {
	SDL_Rect rect;
	GetRedrawRect(&rect);
	m_pEngine->CopyBackgroundPixels(rect.x, rect.y, rect.w, rect.h);
}