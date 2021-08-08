static s32 scaleX = 1;
static s32 scaleY = 1;

void scale(s32 valX, s32 valY)
{
    scaleX = valX;
    scaleY = valY;
}

static void clearScreen(u32 color)
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

static void drawRect(s32 x0, s32 y0, s32 x1, s32 y1, u32 color)
{
    x0 *= scaleX;
    x1 *= scaleX;
    y0 *= scaleY;
    y1 *= scaleY;

    x0 = clamp(x0, 0, globalVariables.screenWidth);
    x1 = clamp(x1, 0, globalVariables.screenWidth);
    y0 = clamp(y0, 0, globalVariables.screenHeight);
    y1 = clamp(y1, 0, globalVariables.screenHeight);

    for (s32 x = x0; x < x1; x++)
    {
        for (s32 y = y0; y < y1; y++)
        {
            u32 *pixel = (u32 *)globalVariables.memory + _idx(x, y);
            *pixel = color;
        }
    }
}

static void fillRectWithoutScale(s32 x, s32 y, s32 w, s32 h, u32 color)
{
    x = clamp(x, 0, globalVariables.screenWidth);
    y = clamp(y, 0, globalVariables.screenHeight);
    w = clamp(w, 0, globalVariables.screenWidth - x);
    h = clamp(h, 0, globalVariables.screenHeight - y);
    for (s32 i = 0; i < w; i++)
    {
        for (s32 j = 0; j < h; j++)
        {
            s32 curX = x + i;
            s32 curY = y + j;
            s32 idx = curX + (globalVariables.screenHeight - (curY + 1)) * globalVariables.screenWidth;

            u32 *pixel = (u32 *)globalVariables.memory + idx;
            *pixel = color;
        }
    }
}

static void fillRect(s32 x, s32 y, s32 width, s32 height, u32 color)
{
    x *= scaleX;
    y *= scaleY;
    width *= scaleX;
    height *= scaleY;

    for (s32 i = 0; i < width; i++)
    {
        for (s32 j = 0; j < height; j++)
        {
            s32 curX = x + i;
            s32 curY = y + j;
            s32 idx = curX + (globalVariables.screenHeight - (curY + 1)) * globalVariables.screenWidth;

            u32 *pixel = (u32 *)globalVariables.memory + idx;
            *pixel = color;
        }
    }
}

static void fillRect(s32 idx, s32 width, s32 height, u32 color)
{
    s32 x = idx % globalVariables.gridAmount;
    s32 y = (idx - x) / globalVariables.gridAmount;
    x *= scaleX;
    y *= scaleY;
    width *= scaleX;
    height *= scaleY;

    for (s32 i = 0; i < width; i++)
    {
        for (s32 j = 0; j < height; j++)
        {
            s32 curX = x + i;
            s32 curY = y + j;
            s32 idx = curX + (globalVariables.screenHeight - (curY + 1)) * globalVariables.screenWidth;

            u32 *pixel = (u32 *)globalVariables.memory + idx;
            *pixel = color;
        }
    }
}

static void strokeRect(s32 x, s32 y, s32 w, s32 h, u32 color)
{
    x *= scaleX;
    y *= scaleY;
    w *= scaleX;
    h *= scaleY;

    x = clamp(x, 0, globalVariables.screenWidth);
    y = clamp(y, 0, globalVariables.screenHeight);
    w = clamp(w, 0, globalVariables.screenWidth - w);
    h = clamp(h, 0, globalVariables.screenHeight - y);

    s32 curX;
    s32 curY;
    for (s32 i = 0; i < w; i++)
    {
        curX = x + i;
        curY = y + 0;
        s32 idx_1 = curX + (globalVariables.screenHeight - (curY + 1)) * globalVariables.screenWidth;
        u32 *pixel_1 = (u32 *)globalVariables.memory + idx_1;
        *pixel_1 = color;

        curX = x + i;
        curY = y + (h - 1);
        s32 idx_2 = curX + (globalVariables.screenHeight - (curY + 1)) * globalVariables.screenWidth;
        u32 *pixel_2 = (u32 *)globalVariables.memory + idx_2;
        *pixel_2 = color;
    }

    for (s32 i = 0; i < h; i++)
    {
        curX = x + 0;
        curY = y + i;
        s32 idx_1 = curX + (globalVariables.screenHeight - (curY + 1)) * globalVariables.screenWidth;
        u32 *pixel_1 = (u32 *)globalVariables.memory + idx_1;
        *pixel_1 = color;

        curX = x + (w - 1);
        curY = y + i;
        s32 idx_2 = curX + (globalVariables.screenHeight - (curY + 1)) * globalVariables.screenWidth;
        u32 *pixel_2 = (u32 *)globalVariables.memory + idx_2;
        *pixel_2 = color;
    }
}

static void fillPixel(s32 x, s32 y, u32 color)
{
    fillRect(x, y, 1, 1, color);
}

static void fillPixel(s32 idx, u32 color)
{
    fillRect(idx, 1, 1, color);
}

static POINT mouseToTile(POINT point)
{
    POINT final = {};

    int x = point.x;
    int y = point.y;

    int w = globalVariables.screenWidth;
    int h = globalVariables.screenHeight;

    x = clamp(x, 0, w);
    y = clamp(y, 0, h);

    final.x = x / scaleX;
    final.y = y / scaleY;

    return final;
}