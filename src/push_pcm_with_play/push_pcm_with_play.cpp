#include <iostream>
#include <SDL.h>

using namespace std;

#ifdef PROJECT_DIR_PATH
string pcm_path = string(PROJECT_DIR_PATH) + "/resource/musics/s16le_44100.pcm";
#endif

const int width = 640;
const int height = 426;

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

    // audio spec
    SDL_AudioSpec audioSpec;
    audioSpec.freq = 44100;
    audioSpec.format = AUDIO_S16SYS;
    audioSpec.channels = 2;
    audioSpec.silence = 0;
    audioSpec.samples = 1024;
    audioSpec.callback = nullptr; // push 模式不用配置

    // open audio device
    SDL_AudioDeviceID deviceID;
    if ((deviceID = SDL_OpenAudioDevice(nullptr, 0, &audioSpec, nullptr, SDL_AUDIO_ALLOW_ANY_CHANGE)) < 2) {
        cout << " SDL_OpenAudioDevice with error deviceID : " << deviceID << endl;
        return -1;
    }

    // write buffer
    FILE *pcmFile = fopen(pcm_path.c_str(), "rb");
    if (pcmFile == nullptr) {
        cout << "cannot open file: " << pcm_path << endl;
        return -1;
    }

    int bufferSize = 4096;
    char *buffer = (char *)malloc(bufferSize);
    SDL_PauseAudioDevice(deviceID, 0);


    // event
    bool bQuit = false;
    SDL_Event windowEvent;
    while (!bQuit) {
        int num = fread(buffer, 1, bufferSize, pcmFile);
        if (num > 0) {
            SDL_QueueAudio(deviceID, buffer, bufferSize);
        }

        while (SDL_PollEvent(&windowEvent)) {
            switch (windowEvent.type) {
                case SDL_QUIT:
                    bQuit = true;
                    break;
                default:
                    break;
            }
        }
    }

    SDL_CloseAudio();
    SDL_DestroyWindow(pWindow);
    SDL_Quit();
    free(buffer);
    fclose(pcmFile);

    return 0;
}