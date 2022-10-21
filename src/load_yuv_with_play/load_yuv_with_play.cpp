#include <iostream>
#include <SDL.h>

using namespace std;

#ifdef PROJECT_DIR_PATH
string yuv_path = string(PROJECT_DIR_PATH) + "/resource/videos/video-yuv420p-280x120.yuv";
#endif

const int width = 280;
const int height = 120;

int main() {

    // sdl init
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return -1;
    }

    // create window
    SDL_Window *pWindow = SDL_CreateWindow("HELLO SDL",
                                           SDL_WINDOWPOS_CENTERED,
                                           SDL_WINDOWPOS_CENTERED,
                                           width,
                                           height,
                                           SDL_WINDOW_RESIZABLE);
    if (!pWindow) {
        return -1;
    }

    SDL_Renderer *pRenderer = SDL_CreateRenderer(pWindow, -1, 0);
    if (!pRenderer) {
        return -1;
    }

    SDL_Texture *texture = SDL_CreateTexture(pRenderer,
                                             SDL_PIXELFORMAT_IYUV,
                                             SDL_TEXTUREACCESS_STREAMING,
                                             width,
                                             height
    );
    if (!texture) {
        return -1;
    }


    // read file
    FILE *yuvFile = fopen(yuv_path.c_str(), "rb");
    if (!yuvFile) {
        return -1;
    }
    // yuv420p 格式的文件大小
    int frameSize = width * height * 3 / 2;
    auto *yuvData = static_cast<unsigned char *>(malloc(frameSize));

    // event
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

        if (fread(yuvData, sizeof(char), frameSize, yuvFile) != frameSize) {
            fseek(yuvFile, 0, SEEK_SET);
            fread(yuvData, sizeof(char), frameSize, yuvFile);
        }


        // 更新纹理内容，把读取的yuv数据转换成纹理
        SDL_UpdateTexture(texture, nullptr, yuvData, width);
        // 清屏操作
        SDL_RenderClear(pRenderer);
        // 将指定纹理复制到要渲染的地方
        SDL_RenderCopy(pRenderer, texture, nullptr, nullptr);
        // 上屏操作
        SDL_RenderPresent(pRenderer);
    }

    fclose(yuvFile);
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(pRenderer);
    SDL_DestroyWindow(pWindow);
    SDL_Quit();

    return 0;
}