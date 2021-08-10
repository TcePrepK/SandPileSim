// World Testing
static b32 inBounds(s32 x, s32 y);
static b32 isAir(s32 x, s32 y);
static b32 isAir(POINT p);
static b32 isLiquid(s32 x, s32 y);
static b32 isFluid(s32 id);
static b32 isSolid(s32 id);
static b32 isMoveFree(s32 x, s32 y, s32 id);
static b32 isMoveFree(POINT p, s32 id);

// Data Manipulation
static void setData(s32 idx, Element *p);
static void setData(s32 x, s32 y, Element *p);
static Element *getDataPointer(s32 idx);
static Element getData(s32 idx);
static chunk_t *getChunkPointer(s32 idx);
static chunk_t getChunk(s32 idx);
static chunk_t *getChunkPointerWithTiles(s32 tileX, s32 tileY);
static chunk_t getChunkWithTiles(s32 tileX, s32 tileY);
static void resetChunk(s32 x, s32 y, b32 tile);
static b32 swapData(POINT from, POINT to);

//////////////////////////////////////////////////////////////////////////////////////////////////

// Functions
static b32 inBounds(s32 x, s32 y)
{
    return (!(x < 0 || x >= globalVariables.gridAmount || y < 0 || y >= globalVariables.gridAmount));
}

static b32 isAir(s32 x, s32 y)
{
    return (inBounds(x, y) && getDataPointer(_idx(x, y))->id == mat_id_empty);
}

static b32 isAir(POINT p)
{
    return isAir(p.x, p.y);
}

static b32 isLiquid(s32 x, s32 y)
{
    if (!inBounds(x, y))
        return false;
    Element p = getData(_idx(x, y));
    return (p.id == mat_id_water);
}

static b32 isFluid(s32 id)
{
    return (id == mat_id_water);
}

static b32 isDynamic(s32 id)
{
    return (id != mat_id_stone);
}

static b32 isSolid(s32 id)
{
    return !isFluid(id);
}

static b32 isMoveFree(s32 x, s32 y, s32 id)
{
    if (!inBounds(x, y))
        return false;
    Element p = getData(_idx(x, y));
    if (p.id == id)
        return false;
    if (isSolid(id) && isFluid(p.id))
        return true;
    if (p.id != mat_id_empty)
        return false;
    return true;
}

static b32 isMoveFree(POINT p, s32 id)
{
    return isMoveFree(p.x, p.y, id);
}

static b32 isSurrounded(s32 x, s32 y, s32 id)
{
    return (!isAir(x + 1, y) && !isAir(x - 1, y) && !isAir(x, y + 1));
}

static void setData(s32 idx, Element *p)
{
    // Element *particle = (Element *)globalVariables.worldBuffer + idx;
    // *particle = p;
    globalVariables.grid[idx] = p;
    fillPixel(idx, p->color);
}

static void setData(s32 x, s32 y, Element *p)
{
    s32 idx = _idx(x, y);

    globalVariables.grid[idx] = p;

    // Element *particle = (Element *)globalVariables.worldBuffer + idx;
    // *particle = p;
    fillPixel(idx, p->color);

    chunk_t *chunk = getChunkPointerWithTiles(x, y);
    if (p->id == mat_id_empty)
    {
        if (chunk->particleAmount > 0)
        {
            chunk->particleAmount--;
        }
    }
    else
    {
        if (chunk->particleAmount == 0)
        {
            resetChunk(x, y, 1);
        }

        chunk->particleAmount++;
    }
}

static Element *getDataPointer(s32 idx)
{
    return globalVariables.grid[idx];
}

static Element getData(s32 idx)
{
    return *getDataPointer(idx);
}

static chunk_t *getChunkPointer(s32 idx)
{
    return (chunk_t *)globalVariables.chunks + idx;
}

static chunk_t getChunk(s32 idx)
{
    return *getChunkPointer(idx);
}

static chunk_t *getChunkPointerWithTiles(s32 tileX, s32 tileY)
{
    s32 chunkX = tileX / globalVariables.chunkSize;
    s32 chunkY = tileY / globalVariables.chunkSize;
    return getChunkPointer(_idxC(chunkX, chunkY));
}

static chunk_t getChunkWithTiles(s32 tileX, s32 tileY)
{
    return *getChunkPointerWithTiles(tileX, tileY);
}

static void resetChunk(s32 x, s32 y, b32 tile)
{
    chunk_t *chunk;
    if (tile)
    {
        chunk = getChunkPointerWithTiles(x, y);
    }
    else
    {
        chunk = getChunkPointer(x + y * globalVariables.chunkAmount);
    }

    RECT rect = chunk->bounds;
    chunk->dirtyRect = {rect.left, rect.top, rect.right, rect.bottom};
}

static b32 swapData(POINT from, POINT to)
{
    if (from.x == to.x && from.y == to.y)
        return 0;

    s32 idx1 = _idx(from);
    s32 idx2 = _idx(to);

    Element *particle1 = getDataPointer(idx1);
    Element *particle2 = getDataPointer(idx2);

    chunk_t *chunk1 = getChunkPointerWithTiles(from.x, from.y);
    chunk_t *chunk2 = getChunkPointerWithTiles(to.x, to.y);

    setData(from.x, from.y, particle2);
    setData(to.x, to.y, particle1);

    if (chunk1 == chunk2)
        return 1;

    if (particle1->id == mat_id_empty && particle2->id == mat_id_empty)
        return 1;

    RECT dirty = chunk2->dirtyRect;
    s32 xMin = min((s32)dirty.left, (s32)to.x);
    s32 yMin = min((s32)dirty.top, (s32)to.y);
    s32 xMax = max((s32)dirty.right, (s32)to.x);
    s32 yMax = max((s32)dirty.bottom, (s32)to.y);

    RECT rect = chunk2->bounds;
    s32 left = clamp(xMin - 2, rect.left, rect.right);
    s32 top = clamp(yMin - 2, rect.top, rect.bottom);
    s32 right = clamp(xMax + 2, rect.left, rect.right);
    s32 bottom = clamp(yMax + 2, rect.top, rect.bottom);

    chunk2->dirtyRect = {left, top, right, bottom};

    return 1;
}

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