#ifndef IMAGE_H
#define IMAGE_H

#include "header.h"
#include "JPGImage.h"

class Image : public ImageSurface {
public:
	Image() {}
	virtual ~Image() {}

	/* Gets the raw SDL image surface */
	SDL_Surface *getSurface() { return m_pSurface; }
};

#endif // !IMAGE_H