#if defined(_MSC_VER)

#include "SDL.h"
#include <SDL_ttf.h>

#else

#include "SDL/SDL.h"
#include <SDL_ttf.h>

#endif

#undef  TERRAIN_BUFFER_OUTPUT
#define UNLIMITED_FUEL
#undef  NO_CRASH