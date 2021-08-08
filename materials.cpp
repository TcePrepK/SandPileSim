// IDs
#define mat_id_empty (s32)0
#define mat_id_sand (s32)1
#define mat_id_water (s32)2
#define mat_id_brick (s32)3

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
static void setData(s32 idx, particle_t p);
static void setData(s32 x, s32 y, particle_t p);
static particle_t *getDataPointer(s32 idx);
static particle_t getData(s32 idx);
static chunk_t *getChunkPointer(s32 idx);
static chunk_t getChunk(s32 idx);
static chunk_t *getChunkPointerWithTiles(s32 tileX, s32 tileY);
static chunk_t getChunkWithTiles(s32 tileX, s32 tileY);
static void resetChunk(s32 x, s32 y, b32 tile);
static b32 swapData(POINT from, POINT to);

// World Shaping
static void placeParticle(s32 x, s32 y, s32 id);
static void placeCircleParticle(s32 x, s32 y, s32 r, s32 id, b32 random);

// GUIs
static gui_t create_gui(s32 id);

// Particles
static particle_t create_empty();
static particle_t create_sand();
static particle_t create_water();
static particle_t create_brick();
static particle_t create_particle(s32 id);

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
    particle_t p = getData(_idx(x, y));
    return (p.id == mat_id_water);
}

static b32 isFluid(s32 id)
{
    return (id == mat_id_water);
}

static b32 isDynamic(s32 id)
{
    return (id != mat_id_brick);
}

static b32 isSolid(s32 id)
{
    return !isFluid(id);
}

static b32 isMoveFree(s32 x, s32 y, s32 id)
{
    if (!inBounds(x, y))
        return false;
    particle_t p = getData(_idx(x, y));
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

static void setData(s32 idx, particle_t p)
{
    particle_t *particle = (particle_t *)globalVariables.worldBuffer + idx;
    *particle = p;
    fillPixel(idx, p.color);
}

static void setData(s32 x, s32 y, particle_t p)
{
    s32 idx = _idx(x, y);

    particle_t *particle = (particle_t *)globalVariables.worldBuffer + idx;
    *particle = p;
    fillPixel(idx, p.color);

    chunk_t *chunk = getChunkPointerWithTiles(x, y);
    if (p.id == mat_id_empty)
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

static particle_t *getDataPointer(s32 idx)
{
    return (particle_t *)globalVariables.worldBuffer + idx;
}

static particle_t getData(s32 idx)
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

    particle_t particle1 = getData(idx1);
    particle_t particle2 = getData(idx2);

    chunk_t *chunk1 = getChunkPointerWithTiles(from.x, from.y);
    chunk_t *chunk2 = getChunkPointerWithTiles(to.x, to.y);

    setData(from.x, from.y, particle2);
    setData(to.x, to.y, particle1);

    if (chunk1 == chunk2)
        return 1;

    if (particle1.id == mat_id_empty && particle2.id == mat_id_empty)
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
    particle_t p = create_particle(id);
    g.id = p.id;
    g.color = p.color;
    return g;
}

// Particles
static particle_t create_empty()
{
    particle_t p = {};

    // ID
    p.id = mat_id_empty;

    // Color
    p.color = 0x00000;

    return p;
}

static particle_t create_sand()
{
    particle_t p = {};

    // ID
    p.id = mat_id_sand;

    // Color
    f32 r = (f32)(randomVal(0, 10)) / 10.f;
    s32 red = (s32)(lerp(0.8f, 1.f, r) * 255.f);
    s32 green = (s32)(lerp(0.5f, 0.6f, r) * 255.f);
    s32 blue = (s32)(lerp(0.2f, 0.25f, r) * 255.f);
    s32 alpha = 255;
    u32 color = rgbaToHex(red, green, blue, alpha);
    p.color = color;

    return p;
}

static particle_t create_water()
{
    particle_t p = {};

    // ID
    p.id = mat_id_water;

    // Color
    f32 r = (f32)(randomVal(0, 1)) / 2.f;
    s32 red = (s32)(lerp(0.1f, 0.15f, r) * 255.f);
    s32 green = (s32)(lerp(0.3f, 0.35f, r) * 255.f);
    s32 blue = (s32)(lerp(0.7f, 0.8f, r) * 255.f);
    s32 alpha = 255;
    u32 color = rgbaToHex(red, green, blue, alpha);
    p.color = color;

    return p;
}

static particle_t create_brick()
{
    particle_t p = {};

    // ID
    p.id = mat_id_brick;

    // Color
    f32 r = (f32)(randomVal(0, 1)) / 2.f;
    s32 red = (s32)(lerp(0.5f, 0.65f, r) * 255.f);
    s32 green = (s32)(lerp(0.5f, 0.65f, r) * 255.f);
    s32 blue = (s32)(lerp(0.5f, 0.65f, r) * 255.f);
    s32 alpha = 255;
    u32 color = rgbaToHex(red, green, blue, alpha);
    p.color = color;

    return p;
}

// Particle By ID
static particle_t create_particle(s32 id)
{
    switch (id)
    {
    case mat_id_empty:
        return create_empty();
    case mat_id_sand:
        return create_sand();
    case mat_id_water:
        return create_water();
    case mat_id_brick:
        return create_brick();
    default:
        return create_empty();
    }
}