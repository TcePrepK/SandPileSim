#ifndef VARIABLES_CPP
#define VARIABLES_CPP

static struct globalVariables
{
    b32 gameIsRunning = true;

    RECT screenRect;
    s32 screenWidth = 0;
    s32 screenHeight = 0;

    s32 currentMatID = 1;
    s32 totalElements = 0;

    s32 chunkSize = 64;

    // World sandWorld = World(chunkSize, chunkSize);
    particle_shower shower;

    POINT mouse;
    b32 holdingLeft = false;

    void *memory;
    BITMAPINFO bitmap_info;

    s32 currentFrame = 0;
} globalVariables;

#endif // VARIABLES_CPP