#ifndef TYPES_CPP
#define TYPES_CPP

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

typedef struct particle_shower
{
    s32 id = -1;
    POINT pos;
} particle_shower;

#endif // TYPES_CPP