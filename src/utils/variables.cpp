#ifndef VARIABLES_CPP
#define VARIABLES_CPP

#include <windows.h>

#include "./types.cpp"

static struct globalVariables
{
    b32 gameIsRunning = true;
    RECT screenRect;
    s32 screenWidth = 0;
    s32 screenHeight = 0;

    s32 gridAmount = 256;

    s32 chunkSize = 64;
    s32 chunkAmount = gridAmount / chunkSize;

    s32 worldSize = gridAmount * gridAmount;

    s32 totalChunkSize = chunkAmount * chunkAmount;
    chunk_t *chunks = new chunk_t[totalChunkSize];

    s32 currentMatID = 1;
    s32 totalParticleTypes = 3;
    gui_t *matGUIs = new gui_t[totalParticleTypes];

    POINT mouse;
    b32 holdingLeft = false;

    particle_shower shower;

    void *worldBuffer;
    void *memory;

    BITMAPINFO bitmap_info;

    s32 currentFrame = 0;
} globalVariables;

#endif // VARIABLES_CPP