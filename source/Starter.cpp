#pragma execution_character_set( "utf-8" )

#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "assimp-vc143-mt.lib")

#include "windows.h"
#include "Program.hpp"

extern "C" {
    _declspec(dllexport) DWORD NvOptimusEnablement = 1;
    _declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
}

_CrtMemState sOld;
_CrtMemState sNew;
_CrtMemState sDiff;

int main(int argc, char** argv)
{
    SetConsoleOutputCP(65001); //utf-8 console
    _CrtMemCheckpoint(&sOld); //take a snapshot
    try
    {
        Program program;
    }
    catch (std::runtime_error& e)
    {
        SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "runtime_error", e.what(), NULL);
        std::cerr << e.what() << std::endl;

        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

struct AtExit
{
    ~AtExit() {
        _CrtMemCheckpoint(&sNew); //take a snapshot 
        if (_CrtMemDifference(&sDiff, &sOld, &sNew)) // if there is a difference
        {
            OutputDebugString(L"-----------_CrtMemDumpStatistics ---------");
            _CrtMemDumpStatistics(&sDiff);
            OutputDebugString(L"-----------_CrtMemDumpAllObjectsSince ---------");
            _CrtMemDumpAllObjectsSince(&sOld);
            OutputDebugString(L"-----------_CrtDumpMemoryLeaks ---------");
            _CrtDumpMemoryLeaks();
        }
    }
} doAtExit;