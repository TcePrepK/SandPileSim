#ifndef TYPES_CPP
#define TYPES_CPP

#include <windows.h>

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

#endif // TYPES_CPP