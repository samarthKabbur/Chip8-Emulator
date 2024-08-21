#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>

#include "SDL.h"

// SDL Container Object
typedef struct {
    SDL_Window *window;
    SDL_Renderer *renderer;
} sdl_t;

// SDL emulator configuration
typedef struct {
    // window size
    uint32_t window_width;
    uint32_t window_height;

    // colors
    uint32_t fg_color; //foreground clr RGBA888
    uint32_t bg_color; //background clr RGBA888

    uint32_t scale_factor; //integer pixel scaling factor
} config_t;

// Emulator state
typedef enum {
    QUIT,
    RUNNING,
    PAUSED,
} emulator_state_t;

// chip8 machine object
typedef struct {
    emulator_state_t state;
} chip8_t;

bool init_sdl(sdl_t *sdl, const config_t config){
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER) != 0){
        SDL_Log("Unable to initialize SDL: %s\n", SDL_GetError());
        return false;
    }

    sdl->window = SDL_CreateWindow("Chip-8 Emulator", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
                                    config.window_width * config.scale_factor, 
                                    config.window_height * config.scale_factor, 
                                    0);
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

    //Defaults
    *config = (config_t){
        .window_width = 64,
        .window_height = 32,
        .fg_color = 0xFFFFFFFF, //white
        .bg_color = 0XFFFF00FF, //yellow
        .scale_factor = 20, //default res will be 1280x640
    };

    //Override defaults from cmd line
    for (int i = 1; i < argc; i++){
        (void)argv[i]; //prevevnt unused variable warning
    }

    return true; //success
}

// Initialize the chip8 machine
bool init_chip8(chip8_t *chip8){
    chip8->state = RUNNING; //Default state of the machine is running
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

void handle_input(chip8_t *chip8){
    SDL_Event event;
    while (SDL_PollEvent(&event)){
        switch (event.type){
            case SDL_QUIT:
                chip8->state = QUIT; //exits the main emulator while loop
                return;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym){
                    case SDLK_ESCAPE:
                        chip8->state = QUIT; //escape will close window and end program
                        return;
                    default:
                        break;
                }
                break;

            case SDL_KEYUP:
                break;

            default:
                break;

        }
    }
}

int main(int argc, char **argv){

    // Initialize Emulator Components
    config_t config = {0};
    if (!set_config_from_args(&config, argc, argv)) exit(EXIT_FAILURE);

    // Initialize SDL
    sdl_t sdl = {0};
    if (!init_sdl(&sdl, config)) exit(EXIT_FAILURE);

    // Initialize CHIP8 machine
    chip8_t chip8 = {0};
    if(!init_chip8(&chip8)) exit(EXIT_FAILURE);

    // Initial screen clear to the bg color
    clear_screen(sdl, config);

    //Big emulator while loop
    while (chip8.state != QUIT){

        //Handle user input
        handle_input(&chip8); //Can change the state of the machine

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