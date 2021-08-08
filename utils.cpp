typedef char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef bool b32;
typedef float f32;
typedef int s32;
typedef unsigned int u32;
typedef long long s64;
typedef unsigned long long u64;

typedef struct vec_t
{
    f32 x = 0;
    f32 y = 0;
} vec_t;

typedef struct gui_t
{
    s32 id = 0;
    u32 color = 0x000000;
    RECT rect;
} gui_t;

typedef struct chunk_t
{
    RECT bounds;
    RECT dirtyRect;
    u32 particleAmount = 0;
} chunk_t;

typedef struct particle_t
{
    s32 id;
    // f32 life_time;
    // vec_t vel;
    u32 color;
    b32 updatedThisFrame;
} particle_t;

typedef struct particle_shower
{
    s32 id = -1;
    POINT pos;
} particle_shower;

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