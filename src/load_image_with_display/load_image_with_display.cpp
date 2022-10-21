#include <iostream>
#include <SDL.h>
#include <SDL_image.h>

using namespace std;

#ifdef PROJECT_DIR_PATH
string image_path = string(PROJECT_DIR_PATH) + "/resource/images/image-jpg-640x426.jpeg";
#endif

const int width = 640;
const int height = 426;

int main() {

    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return -1;
    }
    SDL_Window *pWindow = SDL_CreateWindow("HELLO SDL",
                                           SDL_WINDOWPOS_CENTERED,
                                           SDL_WINDOWPOS_CENTERED,
                                           width,
                                           height,
                                           SDL_WINDOW_ALLOW_HIGHDPI);

    // 得到 SDL_Window 的 SDL_Surface
    SDL_Surface *pWindowSurface = SDL_GetWindowSurface(pWindow);
    // 根据图片创建一个 SDL_Surface
    SDL_Surface *pImageSurface = IMG_Load(image_path.c_str());

    bool bQuit = false;
    SDL_Event windowEvent;
    while (!bQuit) {
        while (SDL_PollEvent(&windowEvent)) {
            switch (windowEvent.type) {
                case SDL_QUIT:
                    bQuit = true;
                    break;
                default:
                    break;
            }
        }
        // 将 pImageSurface 的内容传输到 pWindowSurface 上
        SDL_BlitSurface(pImageSurface, nullptr, pWindowSurface, nullptr);
        // 将 Surface 内容刷新显示到屏幕上
        SDL_UpdateWindowSurface(pWindow);
    }

    pWindowSurface = nullptr;
    pImageSurface = nullptr;
    SDL_DestroyWindow(pWindow);
    SDL_Quit();

    return 0;
}