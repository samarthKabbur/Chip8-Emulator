#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "SDL.h"

typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
} sdl_t;

typedef struct {
    uint32_t window_width;
    uint32_t window_height;

    // colors
    uint32_t fg_color; //foreground clr RGBA888
    uint32_t bg_color; //background clr RGBA888
} config_t;

bool init_sdl(sdl_t *sdl, const config_t config){
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0){
        SDL_Log("Unable to initialize SDL: %s\n", SDL_GetError());
        return false;
    }

    sdl->window = SDL_CreateWindow("Chip-8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, config.window_width, config.window_height, 0);

    if (!sdl->window){
        SDL_Log("Unable to create window: %s\n", SDL_GetError());
        return false;
    }

    sdl->renderer = SDL_CreateRenderer(sdl->window, -1, SDL_RENDERER_ACCELERATED);
    if (!sdl->renderer){
        SDL_Log("Unable to create renderer: %s\n", SDL_GetError());
        return false;
    }

    return true; //success
}

// Set up initial emulator config from command line arguments
bool set_config_from_args(config_t *config, const int argc, char **argv){
    (void)argc;
    (void)argv;

    //Defaults
    *config = (config_t){
        .window_width = 640,
        .window_height = 320,
        .fg_color = 0xFFFF00FF, //yellow
        .bg_color = 0x00000000, //black
    };

    //Override defaults from cmd line
    for (int i = 1; i < argc; i++){
        (void)argv[i]; //prevevnt unused variable warning
    }

    return true; //success
}

void final_cleanup(const sdl_t sdl){
    SDL_DestroyRenderer(sdl.renderer); //Destroy the renderer
    SDL_DestroyWindow(sdl.window); //Destroy the window
    SDL_Quit(); //Turn off SDL`
}

//Clear screen and sdl window to the bg color
void clear_screen(const sdl_t sdl, const config_t config){
    const uint8_t r = (config.bg_color >> 24) & 0xFF;
    const uint8_t g = (config.bg_color >> 16) & 0xFF;
    const uint8_t b = (config.bg_color >>  8) & 0xFF;
    const uint8_t a = (config.bg_color >>  0) & 0xFF;



    SDL_SetRenderDrawColor(sdl.renderer, r, g, b, a);
    SDL_RenderClear(sdl.renderer);
}

// Update the screen with any changes
void update_screen(const sdl_t sdl){
    SDL_RenderPresent(sdl.renderer);
}

int main(int argc, char **argv){

    // Initialize Emulator Components
    config_t config = {0};
    if (!set_config_from_args(&config, argc, argv)) exit(EXIT_FAILURE);

    // Initialize SDL
    sdl_t sdl = {0};
    if (!init_sdl(&sdl, config)) exit(EXIT_FAILURE);

    // Initial screen clear to the bg color
    clear_screen(sdl, config);

    //Big emulator while loop
    while (true){
        //Get_time();
        // Emulate chip8 instructions
        // Get_time(); elasped since last get_time(); then subtract from 16.
        // Delay for 60hz
        SDL_Delay(16);
        // Update window with changes
        update_screen(sdl);
    }



    // Final Cleanup Functions
    final_cleanup(sdl);




    puts("test");

    exit(EXIT_SUCCESS);
}