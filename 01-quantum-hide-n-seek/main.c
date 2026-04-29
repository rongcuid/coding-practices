#define SDL_MAIN_USE_CALLBACKS 1 /* use the callbacks instead of main() */
#include <SDL3/SDL.h>
#include <SDL3/SDL_main.h>

#include <msc.h>

#include "nk_config.h"
#include "nuklear_sdl3.h"

typedef struct {
  SDL_Window *window;
  SDL_Renderer *renderer;
  struct nk_context *ctx;
  struct nk_colorf bg;
  enum nk_anti_aliasing AA;
} app_t;

static SDL_AppResult nk_sdl_fail() {
  SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "Error: %s", SDL_GetError());
  return SDL_APP_FAILURE;
}

static void *sdl_msc_malloc(void *context, ptrdiff_t size, ptrdiff_t align) {
  (void)context;
  (void)align;
  return SDL_malloc(size);
}

static void *sdl_msc_realloc(void *context, void *ptr, ptrdiff_t old_size,
                             ptrdiff_t old_align, ptrdiff_t new_size,
                             ptrdiff_t new_align) {
  (void)context;
  (void)old_size;
  (void)old_align;
  (void)new_align;
  return SDL_realloc(ptr, new_size);
}

static void sdl_msc_free(void *context, void *ptr, ptrdiff_t size,
                         ptrdiff_t align) {
  (void)context;
  (void)size;
  (void)align;
  SDL_free(ptr);
}

static const msc_allocator_t sdl_alloc = {
    .malloc = sdl_msc_malloc,
    .realloc = sdl_msc_realloc,
    .free = sdl_msc_free,
    .context = nullptr,
};

/* This function runs once at startup. */
SDL_AppResult SDL_AppInit(void **appstate, int argc, char *argv[]) {
  app_t *app;
  struct nk_context *ctx;
  float font_scale;
  (void)argc;
  (void)argv;

  SDL_SetAppMetadata("Example Renderer Primitives", "1.0",
                     "com.example.renderer-primitives");

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    SDL_Log("Couldn't initialize SDL: %s", SDL_GetError());
    return SDL_APP_FAILURE;
  }

  app = msc_malloc(&sdl_alloc, sizeof(*app), _Alignof(app_t));
  if (app == nullptr) {
    return nk_sdl_fail();
  }

  if (!SDL_CreateWindowAndRenderer("Nuklear: SDL3 Renderer", 1280, 720,
                                   SDL_WINDOW_RESIZABLE, &app->window,
                                   &app->renderer)) {
    msc_free(&sdl_alloc, app, sizeof(*app), _Alignof(app_t));
    return nk_sdl_fail();
  }

  *appstate = app;

  if (!SDL_SetRenderVSync(app->renderer, 1)) {
    SDL_LogError(SDL_LOG_CATEGORY_CUSTOM, "SDL_SetRenderVSync failed: %s",
                 SDL_GetError());
  }

  app->bg.r = 0.10f;
  app->bg.g = 0.18f;
  app->bg.b = 0.24f;
  app->bg.a = 1.0f;

  font_scale = 1;
  {
    /* This scaling logic was kept simple for the demo purpose.
     * On some platforms, this might not be the exact scale
     * that you want to use. For more information, see:
     * https://wiki.libsdl.org/SDL3/README-highdpi */
    const float scale = SDL_GetWindowDisplayScale(app->window);
    SDL_SetRenderScale(app->renderer, scale, scale);
    font_scale = scale;
  }

  ctx = nk_sdl_init(app->window, app->renderer, nk_sdl_allocator());
  app->ctx = ctx;

  {
    struct nk_font_atlas *atlas;
    struct nk_font_config config = nk_font_config(0);
    struct nk_font *font;

    /* set up the font atlas and add desired font; note that font sizes are
     * multiplied by font_scale to produce better results at higher DPIs */
    atlas = nk_sdl_font_stash_begin(ctx);
    font = nk_font_atlas_add_default(atlas, 13 * font_scale, &config);
    nk_sdl_font_stash_end(ctx);

    /* this hack makes the font appear to be scaled down to the desired
     * size and is only necessary when font_scale > 1 */
    font->handle.height /= font_scale;
    /*nk_style_load_all_cursors(ctx, atlas->cursors);*/
    nk_style_set_font(ctx, &font->handle);

    nk_input_begin(ctx);
    app->AA = NK_ANTI_ALIASING_ON;
  }

  return SDL_APP_CONTINUE; /* carry on with the program! */
}

/* This function runs when a new event (mouse input, keypresses, etc) occurs. */
SDL_AppResult SDL_AppEvent(void *appstate, SDL_Event *event) {
  app_t *app = appstate;

  switch (event->type) {
  case SDL_EVENT_QUIT:
    return SDL_APP_SUCCESS;
  case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED:
    /* You may wish to rescale the renderer and Nuklear during this event.
     * Without this the UI and Font could appear too small or too big.
     * This is not handled by the demo in order to keep it simple,
     * but you may wish to re-bake the Font whenever this happens. */
    SDL_Log("Unhandled scale event! Nuklear may appear blurry");
    return SDL_APP_CONTINUE;
  }

  /* Remember to always rescale the event coordinates,
   * if your renderer uses custom scale. */
  SDL_ConvertEventToRenderCoordinates(app->renderer, event);

  nk_sdl_handle_event(app->ctx, event);

  return SDL_APP_CONTINUE;
}

/* This function runs once per frame, and is the heart of the program. */
SDL_AppResult SDL_AppIterate(void *appstate) {
  app_t *app = appstate;
  struct nk_context *ctx = app->ctx;

  nk_input_end(ctx);

  /* GUI */
  if (nk_begin(ctx, "Demo", nk_rect(50, 50, 230, 250),
               NK_WINDOW_BORDER | NK_WINDOW_MOVABLE | NK_WINDOW_SCALABLE |
                   NK_WINDOW_MINIMIZABLE | NK_WINDOW_TITLE)) {
    enum { EASY, HARD };
    static int op = EASY;
    static int property = 20;

    nk_layout_row_static(ctx, 30, 80, 1);
    if (nk_button_label(ctx, "button")) {
      SDL_Log("button pressed");
    }
    nk_layout_row_dynamic(ctx, 30, 2);
    if (nk_option_label(ctx, "easy", op == EASY))
      op = EASY;
    if (nk_option_label(ctx, "hard", op == HARD))
      op = HARD;
    nk_layout_row_dynamic(ctx, 25, 1);
    nk_property_int(ctx, "Compression:", 0, &property, 1000, 1, 1);

    nk_layout_row_dynamic(ctx, 20, 1);
    nk_label(ctx, "background:", NK_TEXT_LEFT);
    nk_layout_row_dynamic(ctx, 25, 1);
    if (nk_combo_begin_color(ctx, nk_rgb_cf(app->bg),
                             nk_vec2(nk_widget_width(ctx), 400))) {
      nk_layout_row_dynamic(ctx, 120, 1);
      app->bg = nk_color_picker(ctx, app->bg, NK_RGBA);
      nk_layout_row_dynamic(ctx, 25, 1);
      app->bg.r = nk_propertyf(ctx, "#R:", 0, app->bg.r, 1.0f, 0.01f, 0.005f);
      app->bg.g = nk_propertyf(ctx, "#G:", 0, app->bg.g, 1.0f, 0.01f, 0.005f);
      app->bg.b = nk_propertyf(ctx, "#B:", 0, app->bg.b, 1.0f, 0.01f, 0.005f);
      app->bg.a = nk_propertyf(ctx, "#A:", 0, app->bg.a, 1.0f, 0.01f, 0.005f);
      nk_combo_end(ctx);
    }
  }
  nk_end(ctx);

  SDL_SetRenderDrawColorFloat(app->renderer, app->bg.r, app->bg.g, app->bg.b,
                              app->bg.a);
  SDL_RenderClear(app->renderer);

  nk_sdl_render(ctx, app->AA);
  nk_sdl_update_TextInput(ctx);

  /* show if TextInput is active for debug purpose. Feel free to remove this. */
  SDL_SetRenderDrawColor(app->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
  SDL_RenderDebugTextFormat(app->renderer, 10, 10, "TextInputActive? %s",
                            SDL_TextInputActive(app->window) ? "Yes" : "No");

  SDL_RenderPresent(app->renderer);

  nk_input_begin(ctx);
  return SDL_APP_CONTINUE;
}

/* This function runs once at shutdown. */
void SDL_AppQuit(void *appstate, SDL_AppResult result) {
  /* SDL will clean up the window/renderer for us. */
}
