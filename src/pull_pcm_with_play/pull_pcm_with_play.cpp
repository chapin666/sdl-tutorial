#include <iostream>
#include <SDL.h>

using namespace std;

#ifdef PROJECT_DIR_PATH
string pcm_path = string(PROJECT_DIR_PATH) + "/resource/musics/s16le_44100.pcm";
#endif

const int width = 640;
const int height = 426;

// 读取出pcm数据的长度
int bufferLen;
// 读取出pcm的数据
char *bufferData;


void fill_audio(void *udata, Uint8 *stream, int len) {
    SDL_memset4(stream, 0, len);

    if (bufferLen <= 0) {
        return;
    }

    len = len > bufferLen ? bufferLen : len;

    memcpy(stream, (Uint8 *)bufferData, len);
    // SDL_MixAudio(stream,(Uint8 *) bufferData, len,SDL_MIX_MAXVOLUME);

    bufferData += len;
    bufferLen -= len;
}


int main() {

    // sdl init
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
        return -1;
    }

    // create window
    SDL_Window *pWindow = SDL_CreateWindow("pull pcm with play",
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
    audioSpec.callback = fill_audio; // pull 模式设置回调

    // open audio device
    SDL_AudioDeviceID deviceID;
    if ((deviceID = SDL_OpenAudioDevice(nullptr, 0, &audioSpec, nullptr, SDL_AUDIO_ALLOW_ANY_CHANGE)) < 2) {
        cout << "open audio device failed " << endl;
        return -1;
    }

    // write buffer
    FILE *pcmFile = fopen(pcm_path.c_str(), "rb");
    if (!pcmFile) {
        return -1;
    }
    int bufferSize = 4096;
    char *buffer = (char *)malloc(bufferSize);


    SDL_PauseAudioDevice(deviceID, 0);

    // event
    bool bQuit = false;
    SDL_Event windowEvent;
    while (!bQuit) {

        //只要从文件中读取的音频数据，还没有填充完毕，就跳过
        if(bufferLen > 0) continue;
        bufferLen = fread(buffer, 1, bufferSize, pcmFile);
        //读取到了文件数据
        if(bufferLen > 0) {
            bufferData = buffer;
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