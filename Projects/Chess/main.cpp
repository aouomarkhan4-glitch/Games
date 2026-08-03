#define WIN32_LEAN_AND_MEAN
#include "game.hpp"
#include <Windows.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
    game obj;
    obj.run();
    return 0;
}
