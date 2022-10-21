#include <iostream>
#include <SDL.h>

#define SDL_CUSTOM_EVENT (SDL_USEREVENT + 1)

using namespace std;

// custom event
int sdl_thread_custom_event(void *) {
    SDL_Delay(5000);
    SDL_Event sdlEvent;
    sdlEvent.type = SDL_CUSTOM_EVENT;

    SDL_PushEvent(&sdlEvent);
}

int main() {

    // init
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return -1;
    }

    // create window
    int width = 400;
    int height = 400;
    SDL_Window *window = SDL_CreateWindow("Hello SDL",
                                          SDL_WINDOWPOS_CENTERED,
                                          SDL_WINDOWPOS_CENTERED,
                                          width,
                                          height,
                                          SDL_WINDOW_ALLOW_HIGHDPI);


    // render and present
    SDL_Renderer *pRenderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawColor(pRenderer, 0, 155, 100, 155);
    SDL_RenderClear(pRenderer);
    SDL_RenderPresent(pRenderer);


    // run custom event
    SDL_CreateThread(sdl_thread_custom_event, "custom_event", nullptr);


    // exit event
    SDL_Event windowEvent;
    bool bQuit = false;
    while (!bQuit) {
        // SDL_WaitEvent(&windowEvent);
        while (SDL_PollEvent(&windowEvent)) {
            switch (windowEvent.type) {
                case SDL_QUIT:
                    bQuit = true;
                    break;
                case SDL_KEYDOWN:
                    if (windowEvent.key.keysym.sym == SDLK_SPACE) {
                        cout << "user click space " << endl;
                    }
                    break;
                case SDL_MOUSEBUTTONDOWN:
                    cout << "button index is " << windowEvent.button.button << endl;
                    break;
                case SDL_CUSTOM_EVENT:
                    cout << "receive user custom event" << endl;
                    break;
                default:
                    break;
            }
        }
    }

    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(pRenderer);
    SDL_Quit();

    return 0;
}
