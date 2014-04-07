#if defined(_MSC_VER)
#include "SDL.h"
#include <SDL_ttf.h>
#else  // _MSC_VER
#include "SDL/SDL.h"
#include <SDL_ttf.h>
#endif // _MSC_VER

#undef  TERRAIN_BUFFER_OUTPUT
#undef  UNLIMITED_FUEL
#undef  NO_CRASH