#ifndef MATHS_CPP
#define MATHS_CPP

static f32 lerp(f32 v0, f32 v1, f32 t)
{
    return (1.f - t) * v0 + t * v1;
}

static s32 clamp(s32 val, s32 min, s32 max)
{
    if (val < min)
        return min;
    if (val > max)
        return max;
    return val;
}

static u32 rgbaToHex(s32 r, s32 g, s32 b, s32 a)
{
    u32 color = ((a & 0xff) << 24) + ((r & 0xff) << 16) + ((g & 0xff) << 8) + (b & 0xff);
    return color;
}

static s32 randomVal(s32 lower, s32 upper)
{
    if (upper < lower)
    {
        s32 tmp = lower;
        lower = upper;
        upper = tmp;
    }
    return (rand() % (upper - lower + 1) + lower);
}

#endif // MATHS_CPP