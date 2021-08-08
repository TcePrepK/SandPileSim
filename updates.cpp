static void prepareBoard();
static void updateBoard();
static POINT updatePixel(s32 x, s32 y);

// Updates
static POINT simulateSand(s32 x, s32 y, s32 id);
static POINT simulateWater(s32 x, s32 y, s32 id);

static POINT updateSand(s32 x, s32 y);
static POINT updateWater(s32 x, s32 y);

//////////////////////////////////////////////

static void prepareBoard()
{
    s32 w = globalVariables.gridAmount;
    for (s32 i = 0; i < w; i++)
    {
        for (s32 j = 0; j < w; j++)
        {
            particle_t *p = getDataPointer(_idx(i, j));
            p->updatedThisFrame = 0;
        }
    }
}

static void renderBoard()
{
    s32 w = globalVariables.gridAmount;
    for (s32 i = 0; i < w; i++)
    {
        for (s32 j = 0; j < w; j++)
        {
            particle_t *p = getDataPointer(_idx(i, j));
            fillPixel(i, j, p->color);
        }
    }
}

static void updateBoard()
{
    b32 rev = (globalVariables.currentFrame + 1) % 2;
    s32 w = globalVariables.chunkAmount;
    s32 startingPos = rev ? w - 1 : 0;
    s32 dir = rev ? -1 : 1;
    for (s32 i = startingPos; rev ? i >= 0 : i < w; i += dir)
    {
        for (s32 j = startingPos; rev ? j >= 0 : j < w; j += dir)
        {
            chunk_t *chunk = (chunk_t *)globalVariables.chunks + (i + j * w);
            if (chunk->particleAmount == 0)
                continue;

            RECT rect = chunk->bounds;
            RECT dirty = chunk->dirtyRect;

            s32 x0 = dirty.left;
            s32 y0 = dirty.top;
            s32 x1 = dirty.right;
            s32 y1 = dirty.bottom;

            s32 xMin = globalVariables.gridAmount;
            s32 yMin = globalVariables.gridAmount;
            s32 xMax = -1;
            s32 yMax = -1;

            for (s32 y = rev ? y1 - 1 : y0; rev ? y >= y0 : y < y1; y += dir)
            {
                for (s32 x = rev ? x1 - 1 : x0; rev ? x >= x0 : x < x1; x += dir)
                {
                    POINT newPos = updatePixel(x, y);
                    if (newPos.x == x && newPos.y == y)
                        continue;

                    s32 pingX = 0;
                    s32 pingY = 0;
                    if (x == x0)
                        pingX = -1;
                    else if (x == x1 - 1)
                        pingX = 1;
                    if (y == y0)
                        pingY = -1;
                    else if (y == y1 - 1)
                        pingY = 1;

                    if (pingX != 0)
                        resetChunk(x + pingX, y, 1);
                    if (pingY != 0)
                        resetChunk(x, y + pingY, 1);
                    if (pingX != 0 || pingY != 0)
                        resetChunk(x + pingX, y + pingY, 1);

                    xMin = min(xMin, (s32)newPos.x);
                    yMin = min(yMin, (s32)newPos.y);
                    xMax = max(xMax, (s32)newPos.x);
                    yMax = max(yMax, (s32)newPos.y);
                }
            }

            s32 left = clamp(xMin - 2, rect.left, rect.right);
            s32 top = clamp(yMin - 2, rect.top, rect.bottom);
            s32 right = clamp(xMax + 2, rect.left, rect.right);
            s32 bottom = clamp(yMax + 2, rect.top, rect.bottom);

            chunk->dirtyRect = {left, top, right, bottom};
        }
    }
}

static POINT updatePixel(s32 x, s32 y)
{
    particle_t *p = getDataPointer(_idx(x, y));
    s32 id = p->id;

    if (id == mat_id_empty || p->updatedThisFrame || !isDynamic(id))
        return {x, y};
    p->updatedThisFrame = 1;

    switch (id)
    {
    case mat_id_sand:
        return updateSand(x, y);
    case mat_id_water:
        return updateWater(x, y);
    default:
        return {x, y};
    }
}

//////////////////////////////////////////////

static POINT simulateSand(s32 x, s32 y, s32 id)
{
    s32 dir = randomVal(0, 1) ? 1 : -1;
    POINT pos = {x, y};
    POINT B = {x, y + 1};
    POINT BL = {x + dir, y + 1};
    POINT BR = {x - dir, y + 1};
    if (isMoveFree(B, id))
    {
        return B;
    }
    else if (isMoveFree(BL, id))
    {
        return BL;
    }
    else if (isMoveFree(BR, id))
    {
        return BR;
    }

    return pos;
}

static POINT simulateWater(s32 x, s32 y, s32 id)
{
    s32 dir = randomVal(0, 1) ? 1 : -1;
    POINT pos = {x, y};
    POINT B = {x, y + 1};
    POINT L = {x + dir, y};
    POINT R = {x - dir, y};
    POINT BL = {x + dir, y + 1};
    POINT BR = {x - dir, y + 1};
    if (isMoveFree(B, id))
    {
        return B;
    }
    else if (isMoveFree(BL, id))
    {
        return BL;
    }
    else if (isMoveFree(BR, id))
    {
        return BR;
    }
    else if (isMoveFree(L, id))
    {
        return L;
    }
    else if (isMoveFree(R, id))
    {
        return R;
    }

    return pos;
}

static POINT updateSand(s32 x, s32 y)
{
    s32 id = mat_id_sand;
    if (isSurrounded(x, y, id))
        return {x, y}; // It can't move

    s32 fallRate = 4;   // 4
    s32 spreadRate = 5; // 5

    s32 currentFall = 0;
    s32 currentSpread = 0;

    POINT pos = {x, y};

    for (s32 j = 1; j <= fallRate; j++)
    {
        POINT B = {x, y + j};
        if (!isAir(B))
            break;
        currentFall = j;
    }

    if (currentFall == fallRate)
    {
        POINT g = {x, y + fallRate};
        swapData(pos, g);
        return g;
    }

    POINT currentP = {x, y + currentFall};
    s32 currentLoop = 0;
    while (currentLoop++ < spreadRate + fallRate)
    {
        POINT nextP = simulateSand(currentP.x, currentP.y, id);
        currentSpread = nextP.x - x;
        currentFall = nextP.y - y;

        if ((nextP.x == currentP.x && nextP.y == currentP.y) || abs(currentSpread) > spreadRate || currentFall > fallRate)
        {
            swapData(pos, currentP);
            return currentP;
        }
        currentP = nextP;
    }

    POINT lastP = {x + currentSpread, y + currentFall};
    swapData(pos, lastP);
    return lastP;
}

static POINT updateWater(s32 x, s32 y)
{
    s32 id = mat_id_water;
    s32 fallRate = 1;   // 5
    s32 spreadRate = 1; // 50

    s32 currentFall = 0;
    s32 currentSpread = 0;

    POINT pos = {x, y};
    if (isSurrounded(x, y, id))
        return {x, y}; // It can't move

    for (s32 j = 1; j <= fallRate; j++)
    {
        POINT B = {x, y + j};
        if (!isAir(B))
            break;
        currentFall = j;
    }

    if (currentFall == fallRate)
    {
        POINT g = {x, y + fallRate};
        swapData(pos, g);
        return g;
    }

    POINT currentP = {x, y + currentFall};
    s32 currentLoop = 0;
    while (currentLoop++ < spreadRate + fallRate)
    {
        POINT nextP = simulateWater(currentP.x, currentP.y, id);
        currentSpread = nextP.x - x;
        currentFall = nextP.y - y;

        if ((nextP.x == currentP.x && nextP.y == currentP.y) || abs(currentSpread) > spreadRate || currentFall > fallRate)
        {
            swapData(pos, currentP);
            return currentP;
        }
        currentP = nextP;
    }

    POINT lastPos = {x + currentSpread, y + currentFall};
    swapData(pos, lastPos);
    return lastPos;
}