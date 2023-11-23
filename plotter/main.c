#include <stdio.h>
#include <stdlib.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720

enum image_index {
    PRESS,
    RIGHT,
    LEFT,
    UP,
    DOWN,

    MAX
};

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
} App;

App *init_app() {
    App *app = (App *)malloc(sizeof(App));

    int renderer_flags, window_flags;

    renderer_flags = SDL_RENDERER_ACCELERATED;

    window_flags = 0;

    if (TTF_Init() < 0) {
        printf("Couldn't initialize TTF: %s\n", TTF_GetError());

        return NULL;
    }

    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("Couldn't initialize SDL: %s\n", SDL_GetError());

        return NULL;
    }

    app->window = SDL_CreateWindow("Plotter", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
                                   WINDOW_WIDTH, WINDOW_HEIGHT, window_flags);

    if (!app->window) {
        printf("Failed to open %d x %d window: %s\n", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_GetError());

        return NULL;
    }

    app->renderer = SDL_CreateRenderer(app->window, -1, SDL_RENDERER_ACCELERATED);

    if (!app->renderer) {
        printf("Failed to create renderer: %s\n", SDL_GetError());

        SDL_DestroyWindow(app->window);

        return NULL;
    }

    SDL_SetRenderDrawColor(app->renderer, 0xFF, 0xFF, 0xFF, 0xFF);

    int img_flags = IMG_INIT_PNG;

    if ((IMG_Init(img_flags) & img_flags) != img_flags) {
        printf("SDL_image could not be initialized! SDL_image Error: %s\n", IMG_GetError());

        SDL_DestroyRenderer(app->renderer);
        SDL_DestroyWindow(app->window);

        return NULL;
    }

    return app;
}

SDL_Texture *load_texture(App *app, const char *file) {
    SDL_Texture *texture = NULL;

    texture = IMG_LoadTexture(app->renderer, file);

    if (texture == NULL) {
        printf("Unable to create texture from %s! SDL Error: %s\n", file, SDL_GetError());

        return NULL;
    }

    return texture;
}

// SDL_Surface *load_and_optimize_image(App *app, const char *file) {
//     SDL_Surface *surface = IMG_Load(file);
//     SDL_Surface *optimized = NULL;
//
//     if (surface == NULL) {
//         printf("Unalbe to load image %s! SDL Error: %s\n", file, SDL_GetError());
//
//         return NULL;
//     }
//
//     optimized = SDL_ConvertSurface(surface, app->surface->format, 0);
//
//     if (optimized == NULL) {
//         printf("Unable to optimize image %s! SDL ERror: %s\n", file, SDL_GetError());
//
//         SDL_FreeSurface(surface);
//
//         return NULL;
//     }
//
//     return optimized;
// }

void destroy_app(App *app) {
    SDL_DestroyRenderer(app->renderer);
    SDL_DestroyWindow(app->window);
    SDL_Quit();

    free(app);
}

void draw_axes(App *app, int padding, int segment_separator_length) {
    // Abscissa
    SDL_RenderDrawLine(app->renderer, WINDOW_WIDTH / 2, 0, WINDOW_WIDTH / 2, WINDOW_HEIGHT);

    for (int x = WINDOW_WIDTH / 2 - padding; x >= 0; x -= padding) {
        SDL_RenderDrawLine(app->renderer, x, WINDOW_HEIGHT / 2 - segment_separator_length, x,
                           WINDOW_HEIGHT / 2 + segment_separator_length);
    }

    for (int x = WINDOW_WIDTH / 2 + padding; x <= WINDOW_WIDTH; x += padding) {
        SDL_RenderDrawLine(app->renderer, x, WINDOW_HEIGHT / 2 - segment_separator_length, x,
                           WINDOW_HEIGHT / 2 + segment_separator_length);
    }

    // Ordinate
    SDL_RenderDrawLine(app->renderer, 0, WINDOW_HEIGHT / 2, WINDOW_WIDTH, WINDOW_HEIGHT / 2);

    for (int y = WINDOW_HEIGHT / 2 - padding; y >= 0; y -= padding) {
        SDL_RenderDrawLine(app->renderer, WINDOW_WIDTH / 2 - segment_separator_length, y,
                           WINDOW_WIDTH / 2 + segment_separator_length, y);
    }

    for (int y = WINDOW_HEIGHT / 2 + padding; y <= WINDOW_HEIGHT; y += padding) {
        SDL_RenderDrawLine(app->renderer, WINDOW_WIDTH / 2 - segment_separator_length, y,
                           WINDOW_WIDTH / 2 + segment_separator_length, y);
    }
}

// void draw_axes2(App *app, TTF_Font *font, int scale) {
//     SDL_RenderDrawLine(app->renderer, WINDOW_WIDTH / 2, 0, WINDOW_WIDTH / 2, WINDOW_HEIGHT);
//     SDL_RenderDrawLine(app->renderer, 0, WINDOW_HEIGHT / 2, WINDOW_WIDTH, WINDOW_HEIGHT / 2);
//
//     int step = 50 * scale;
//
//     SDL_Color fg;
//
//     fg.r = 0xFF;
//     fg.g = 0xFF;
//     fg.b = 0xFF;
//     fg.a = 0xFF;
//
//     for (int x = WINDOW_WIDTH / 2, i = 0; x < WINDOW_WIDTH; x += step, i++) {
//         SDL_RenderDrawLine(app->renderer, x, WINDOW_HEIGHT / 2 - 5, x, WINDOW_HEIGHT / 2 + 5);
//         char label[10];
//         sprintf(label, "%d", i * step);
//         SDL_Surface *textSurface = TTF_RenderText_Solid(font, label, fg);
//         SDL_Texture *text = SDL_CreateTextureFromSurface(app->renderer, textSurface);
//         int textW, textH;
//         SDL_QueryTexture(text, NULL, NULL, &textW, &textH);
//         SDL_Rect renderQuad = {x - (textW / 2), WINDOW_HEIGHT / 2 + 10, textW, textH};
//         SDL_RenderCopy(app->renderer, text, NULL, &renderQuad);
//         SDL_FreeSurface(textSurface);
//         SDL_DestroyTexture(text);
//     }
//
//     for (int x = WINDOW_WIDTH / 2, i = 0; x > 0; x -= step, i--) {
//         SDL_RenderDrawLine(app->renderer, x, WINDOW_HEIGHT / 2 - 5, x, WINDOW_HEIGHT / 2 + 5);
//         char label[10];
//         sprintf(label, "%d", i * step);
//         SDL_Surface *textSurface = TTF_RenderText_Solid(font, label, fg);
//         SDL_Texture *text = SDL_CreateTextureFromSurface(app->renderer, textSurface);
//         int textW, textH;
//         SDL_QueryTexture(text, NULL, NULL, &textW, &textH);
//         SDL_Rect renderQuad = {x - (textW / 2), WINDOW_HEIGHT / 2 + 10, textW, textH};
//         SDL_RenderCopy(app->renderer, text, NULL, &renderQuad);
//         SDL_FreeSurface(textSurface);
//         SDL_DestroyTexture(text);
//     }
//
//     for (int y = WINDOW_HEIGHT / 2, j = 0; y < WINDOW_HEIGHT; y += step, j++) {
//         SDL_RenderDrawLine(app->renderer, WINDOW_WIDTH / 2 - 5, y, WINDOW_WIDTH / 2 + 5, y);
//         char label[10];
//         sprintf(label, "%d", j * step);
//         SDL_Surface *textSurface = TTF_RenderText_Solid(font, label, fg);
//         SDL_Texture *text = SDL_CreateTextureFromSurface(app->renderer, textSurface);
//         int textW, textH;
//         SDL_QueryTexture(text, NULL, NULL, &textW, &textH);
//         SDL_Rect renderQuad = {WINDOW_WIDTH / 2 + 10, y - (textH / 2), textW, textH};
//         SDL_RenderCopy(app->renderer, text, NULL, &renderQuad);
//         SDL_FreeSurface(textSurface);
//         SDL_DestroyTexture(text);
//     }
//
//     for (int y = WINDOW_HEIGHT / 2, j = 0; y > 0; y -= step, j--) {
//         SDL_RenderDrawLine(app->renderer, WINDOW_WIDTH / 2 - 5, y, WINDOW_WIDTH / 2 + 5, y);
//         char label[10];
//         sprintf(label, "%d", j * step);
//         SDL_Surface *textSurface = TTF_RenderText_Solid(font, label, fg);
//         SDL_Texture *text = SDL_CreateTextureFromSurface(app->renderer, textSurface);
//         int textW, textH;
//         SDL_QueryTexture(text, NULL, NULL, &textW, &textH);
//         SDL_Rect renderQuad = {WINDOW_WIDTH / 2 + 10, y - (textH / 2), textW, textH};
//         SDL_RenderCopy(app->renderer, text, NULL, &renderQuad);
//         SDL_FreeSurface(textSurface);
//         SDL_DestroyTexture(text);
//     }
// }

int main() {
    App *app = init_app();

    if (app == NULL) {
        return -1;
    }

    SDL_Texture *texture = load_texture(app, "texture.png");

    if (texture == NULL) {
        destroy_app(app);

        return -1;
    }

    SDL_Event e;

    int quit = 0;

    long long int i = 0;
    long long int j = 0;

    int mode = 0;

    TTF_Font *font = TTF_OpenFont("/Users/aloysobek/Downloads/Roboto/Roboto-Regular.ttf",
                                  12); // Users/aloysobek/Downloads/Roboto/Roboto-Regular.ttf
    if (!font) {
        printf("TTF Font load error: %s\n", TTF_GetError());
        return 1;
    }

    int scale = 1;

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT) {
                quit = 1;
            }
            if (e.type == SDL_MOUSEWHEEL) {
                if (e.wheel.y > 0) // scroll up
                {
                    ++scale;
                } else if (e.wheel.y < 0) // scroll down
                {
                    --scale;

                    if (scale < 1) {
                        scale = 1;
                    }
                }
            }
        }

        // SDL_Rect viewport;

        // viewport.x = j;
        // viewport.y = 0;

        // viewport.w = WINDOW_WIDTH / 2;
        // viewport.h = WINDOW_HEIGHT / 2;

        // SDL_SetRenderDrawColor(app->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        // SDL_RenderClear(app->renderer);

        // SDL_Rect rect = {WINDOW_WIDTH / 4, WINDOW_HEIGHT / 4, WINDOW_WIDTH / 2, WINDOW_HEIGHT /
        // 2}; SDL_SetRenderDrawColor(app->renderer, 0xFF, 0x00, 0x00, 0xFF);
        // SDL_RenderFillRect(app->renderer, &rect);

        // SDL_Rect rect2 = {WINDOW_WIDTH / 6, WINDOW_HEIGHT / 6, WINDOW_WIDTH * 2 / 3,
        //                   WINDOW_HEIGHT * 2 / 3};
        // SDL_SetRenderDrawColor(app->renderer, 0x00, 0xFF, 0x00, 0xFF);
        // SDL_RenderFillRect(app->renderer, &rect2);

        // SDL_SetRenderDrawColor(app->renderer, 0x00, 0x00, 0xFF, 0xFF);
        // SDL_RenderDrawLine(app->renderer, 0, WINDOW_HEIGHT / 2, WINDOW_WIDTH, WINDOW_HEIGHT / 2);

        // // Draw vertical line of yellow dots
        // SDL_SetRenderDrawColor(app->renderer, 0xFF, 0xFF, 0x00, 0xFF);
        // for (int i = 0; i < WINDOW_HEIGHT; i += 4) {
        //     SDL_RenderDrawPoint(app->renderer, WINDOW_WIDTH / 2, i);
        // }

        // SDL_RenderSetViewport(app->renderer, &viewport);

        // SDL_RenderCopy(app->renderer, texture, NULL, NULL);

        // SDL_RenderPresent(app->renderer);

        SDL_SetRenderDrawColor(app->renderer, 0x00, 0x00, 0x00, 0xFF);
        SDL_RenderClear(app->renderer);

        SDL_SetRenderDrawColor(app->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
        draw_axes(app, scale, 10);

        SDL_RenderPresent(app->renderer);
    }

    SDL_DestroyTexture(texture);

    TTF_CloseFont(font);
    TTF_Quit();

    destroy_app(app);

    return 0;
}
