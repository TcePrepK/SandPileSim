static void prepareBoard();
static void updateBoard();
static POINT updatePixel(s32 x, s32 y);

// World Shaping
static void placeParticle(s32 x, s32 y, s32 id);
static void placeCircleParticle(s32 x, s32 y, s32 r, s32 id, b32 random);

// Particles
static Element *create_particle(s32 id);

// GUIs
static gui_t create_gui(s32 id);

//////////////////////////////////////////////

static void prepareBoard()
{
    s32 w = globalVariables.gridAmount;
    for (s32 i = 0; i < w; i++)
    {
        for (s32 j = 0; j < w; j++)
        {
            Element *p = getDataPointer(_idx(i, j));
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
            Element *p = getDataPointer(_idx(i, j));
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
    Element *p = getDataPointer(_idx(x, y));
    s32 id = p->id;

    if (id == mat_id_empty || p->updatedThisFrame || !isDynamic(id))
        return {x, y};
    p->updatedThisFrame = 1;

    return p->update(x, y);
}

//////////////////////////////////////////////

static Element *create_particle(s32 id)
{
    switch (id)
    {
    case mat_id_sand:
        return new Sand();
    case mat_id_water:
        return new Water();
    case mat_id_stone:
        return new Stone();
    default:
        return new Element();
    }
}

static void placeParticle(s32 x, s32 y, s32 id)
{
    if (!isMoveFree(x, y, id))
        return;
    setData(x, y, create_particle(id));
    if (!isDynamic(id))
        return;

    chunk_t *chunk = getChunkPointerWithTiles(x, y);
    resetChunk(x, y, 1);
}

static void placeCircleParticle(s32 x, s32 y, s32 r, s32 id, b32 random)
{
    s32 sqrR = r * r;
    if (random)
    {
        for (s32 i = 0; i < sqrR; i++)
        {
            s32 offX = randomVal(-r, r);
            s32 offY = randomVal(-r, r);

            s32 sqrLen = offX * offX + offY * offY;
            if (sqrLen > sqrR)
                continue;
            placeParticle(x + offX, y + offY, id);
        }
    }
    else
    {
        for (s32 offX = 0; offX < r; offX++)
        {
            for (s32 offY = 0; offY < r; offY++)
            {
                s32 sqrLen = offX * offX + offY * offY;
                if (sqrLen > sqrR)
                    continue;
                placeParticle(x + offX, y + offY, id);
            }
        }
    }
}

static void fillWorld(s32 id)
{
    for (s32 i = 0; i < globalVariables.gridAmount; i++)
    {
        for (s32 j = 0; j < globalVariables.gridAmount; j++)
        {
            setData(i, j, create_particle(id));
        }
    }
}

// GUI
static gui_t create_gui(s32 id)
{
    gui_t g = {};
    Element *p = create_particle(id);
    g.id = p->id;
    g.color = p->color;
    return g;
}