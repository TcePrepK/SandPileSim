#ifndef VARIABLES_CPP
#define VARIABLES_CPP

static struct globalVariables
{
    b32 gameIsRunning = true;

    s32 screenWidth = 0;
    s32 screenHeight = 0;

    s32 currentMatID = 1;

    particle_shower shower;

    Vector oldMouse;
    Vector mouse;
    b32 holdingLeft = false;

    void *memory;
    BITMAPINFO bitmap_info;

    s32 currentFrame = 0;
} globalVariables;

#endif // VARIABLES_CPP