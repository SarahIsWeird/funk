# FunkPlayer

FunkPlayer is a minimalistic music player that hopefully one day supports playing directly from YouTube!

Highly WIP, won't work for you yet unless you change the path to the music file in code.

## Building FunkPlayer

### Dependencies

- Discord Game SDK
- SFML 2.5.1

### Compiling

FunkPlayer must be compiled with at least C++17. (C++20 preferred, later probably needed.) Both the Discord Game SDK and SFML need to either be in your path or added as a flag to the compiler/linker. The GameSDK needs to be pre-compiled. (Just run a simple ``g++ *.cpp`` in the source folder.) Currently, only Windows is supported, but that will change soon. I'll write a Makefile when I can be arsed.

Sample MinGW command:

```bash
g++ -o funk.exe src/funk.cpp src/assets.cpp path/to/discord/sdk/*.o \
  -I"path/to/sfml/include" -I"path/to/discord/sdk/cpp/" -L"path/to/sfml/lib" \
  -L"path/to/discord/sdk/lib/x86" -lsfml-system -lsfml-window -lsfml-graphics \
  -lsfml-audio -ldiscord_game_sdk -mwindows -std=c++2a
```
