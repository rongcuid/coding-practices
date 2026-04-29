
#define NK_IMPLEMENTATION
#include "nk_config.h"
#define NK_SDL3_RENDERER_IMPLEMENTATION
#include "nuklear_sdl3.h"

char *nk_sdl_dtoa(char *str, double d) {
  NK_ASSERT(str);
  if (!str)
    return NULL;
  (void)SDL_snprintf(str, 99999, "%.17g", d);
  return str;
}
