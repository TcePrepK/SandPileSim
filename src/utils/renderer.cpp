void scale(s32 valX, s32 valY);
void clearScreen(u32 color);

void fillPixel(s32 x, s32 y, u32 color);
void fillRect(s32 x, s32 y, s32 w, s32 h, u32 color);
void strokeRect(s32 x, s32 y, s32 w, s32 h, u32 color);

void fillELement(s32 x, s32 y, u32 color);

POINT mouseToTile(s32 x, s32 y);
b32 inBounds(s32 x, s32 y);

s32 scaleX = 1;
s32 scaleY = 1;

void scale(s32 valX, s32 valY)
{
    scaleX = valX;
    scaleY = valY;
}

void clearScreen(u32 color)
{
    u32 *pixel = (u32 *)globalVariables.memory;
    for (s32 x = 0; x < globalVariables.screenWidth; x++)
    {
        for (s32 y = 0; y < globalVariables.screenHeight; y++)
        {
            *pixel++ = color;
        }
    }
}

void fillPixel(s32 x, s32 y, u32 color)
{
    s32 idx = x + (globalVariables.screenHeight - (y + 1)) * globalVariables.screenWidth;
    u32 *pixel = (u32 *)globalVariables.memory + idx;
    *pixel = color;
}

void fillRect(s32 x, s32 y, s32 w, s32 h, u32 color)
{
    x *= scaleX;
    y *= scaleY;
    w *= scaleX;
    h *= scaleY;

    if (!inBounds(x, y))
        return;

    w = clamp(w, 0, globalVariables.screenWidth - x);
    h = clamp(h, 0, globalVariables.screenHeight - y);

    for (s32 i = 0; i < w; i++)
    {
        for (s32 j = 0; j < h; j++)
        {
            s32 curX = x + i;
            s32 curY = y + j;
            fillPixel(curX, curY, color);
        }
    }
}

void fillRectWithoutScale(s32 x, s32 y, s32 w, s32 h, u32 color)
{
    if (!inBounds(x, y))
        return;

    w = clamp(w, 0, globalVariables.screenWidth - x);
    h = clamp(h, 0, globalVariables.screenHeight - y);

    for (s32 i = 0; i < w; i++)
    {
        for (s32 j = 0; j < h; j++)
        {
            fillPixel(x + i, y + j, color);
        }
    }
}

void strokeRect(s32 x, s32 y, s32 w, s32 h, u32 color)
{
    x *= scaleX;
    y *= scaleY;
    w *= scaleX;
    h *= scaleY;

    if (!inBounds(x, y))
        return;

    w = clamp(w, 0, globalVariables.screenWidth - x);
    h = clamp(h, 0, globalVariables.screenHeight - y);

    for (s32 i = 0; i < w; i++)
    {
        fillPixel(x + i, y, color);
        fillPixel(x + i, y + (h - 1), color);
    }

    for (s32 i = 0; i < h; i++)
    {
        fillPixel(x, y + i, color);
        fillPixel(x + (w - 1), y + i, color);
    }
}

void fillElement(s32 x, s32 y, u32 color)
{
    x *= scaleX;
    y *= scaleY;

    if (!inBounds(x, y))
        return;

    s32 w = scaleX;
    s32 h = scaleY;

    w = clamp(w, 0, globalVariables.screenWidth - x);
    h = clamp(h, 0, globalVariables.screenHeight - y);

    for (s32 i = 0; i < w; i++)
    {
        for (s32 j = 0; j < h; j++)
        {
            s32 curX = x + i;
            s32 curY = y + j;
            fillPixel(curX, curY, color);
        }
    }
}

POINT mouseToTile(POINT pos32)
{
    POINT final = {};

    s32 x = pos32.x;
    s32 y = pos32.y;

    s32 w = globalVariables.screenWidth;
    s32 h = globalVariables.screenHeight;

    x = clamp(x, 0, w);
    y = clamp(y, 0, h);

    final.x = x / scaleX;
    final.y = y / scaleY;

    return final;
}

b32 inBounds(s32 x, s32 y)
{
    return (x >= 0 && x < globalVariables.screenWidth && y >= 0 && y < globalVariables.screenHeight);
}