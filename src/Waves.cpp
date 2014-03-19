#include "Waves.h"

ImageSurface *Waves::m_base = new ImageSurface();

Waves::Waves(MyProjectMain *engine, double speed) : DisplayableObject(engine), m_speed(speed), m_offset(0.0) {
	/* Load the graphic */
	if (!m_base->IsLoaded()) {
		m_base->LoadImage("../resources/waves.png");
	}
}

Waves::~Waves() {}

void Waves::Draw() {
	int offset = static_cast<int>(m_offset);

	m_base->RenderImage(m_pEngine->GetForeground(), 0, 0, offset + 0, 100, 202, 20);
	m_base->RenderImage(m_pEngine->GetForeground(), 0, 0, offset + 202, 100, 202, 20);
	m_base->RenderImage(m_pEngine->GetForeground(), 0, 0, offset + 404, 100, 202, 20);
	m_base->RenderImage(m_pEngine->GetForeground(), 0, 0, offset + 606, 100, 202, 20);
	m_base->RenderImage(m_pEngine->GetForeground(), 0, 0, offset + 808, 100, 202, 20);
	m_base->RenderImage(m_pEngine->GetForeground(), 0, 0, offset + 1010, 100, 202, 20);
	m_base->RenderImage(m_pEngine->GetForeground(), 0, 0, offset + 1212, 100, 202, 20);
}

void Waves::DoUpdate(int elapsedTime) {
	if (m_offset < -202) {
		m_offset = 0;
	}

	m_offset -= m_speed * (elapsedTime / 1000.0);
}

void Waves::GetRedrawRect(SDL_Rect *rectangle) {
	rectangle->x = 0;
	rectangle->y = 100;
	rectangle->w = m_pEngine->GetScreenWidth();
	rectangle->h = 20;
}

void Waves::RedrawBackground() {
	SDL_Rect *rect = new SDL_Rect();
	GetRedrawRect(rect);
	m_pEngine->CopyBackgroundPixels(rect->x, rect->y, rect->w, rect->h);
}