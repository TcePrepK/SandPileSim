#define SAND 1

class Sand : public Solid
{
public:
    Sand() : Solid(SAND, getColor()) {}

    u32 getColor() { return generateColor(); }

    u32 generateColor()
    {
        f32 r = (f32)(randomVal(0, 10)) / 10.f;
        s32 red = (s32)(lerp(0.8f, 1.f, r) * 255.f);
        s32 green = (s32)(lerp(0.5f, 0.6f, r) * 255.f);
        s32 blue = (s32)(lerp(0.2f, 0.25f, r) * 255.f);
        s32 alpha = 255;
        return rgbaToHex(red, green, blue, alpha);
    }

    Element *clone() { return new Sand(); }

    Vector update(s32 x, s32 y)
    {
        // Element *element = sandWorld.getPixel(x, y + 1);
        if (sandWorld.getPixel(x, y + 1) == nullptr)
            return Vector(x, y + 1);
        if (sandWorld.getPixel(x + 1, y + 1) == nullptr)
            return Vector(x + 1, y + 1);
        if (sandWorld.getPixel(x - 1, y + 1) == nullptr)
            return Vector(x - 1, y + 1);

        return Vector(x, y);
    }

    // Vector simulateSand(s32 x, s32 y, s32 id)
    // {
    //     s32 dir = randomVal(0, 1) ? 1 : -1;
    //     Vector pos = {x, y};
    //     Vector B = {x, y + 1};
    //     Vector BL = {x + dir, y + 1};
    //     Vector BR = {x - dir, y + 1};
    //     if (isMoveFree(B, id))
    //     {
    //         return B;
    //     }
    //     else if (isMoveFree(BL, id))
    //     {
    //         return BL;
    //     }
    //     else if (isMoveFree(BR, id))
    //     {
    //         return BR;
    //     }

    //     return pos;
    // }

    // Vector update(s32 x, s32 y)
    // {
    //     s32 id = mat_id_sand;
    //     if (isSurrounded(x, y, id))
    //         return {x, y}; // It can't move

    //     s32 fallRate = 4;   // 4
    //     s32 spreadRate = 5; // 5

    //     s32 currentFall = 0;
    //     s32 currentSpread = 0;

    //     Vector pos = {x, y};

    //     for (s32 j = 1; j <= fallRate; j++)
    //     {
    //         Vector B = {x, y + j};
    //         if (!isAir(B))
    //             break;
    //         currentFall = j;
    //     }

    //     if (currentFall == fallRate)
    //     {
    //         Vector g = {x, y + fallRate};
    //         swapData(pos, g);
    //         return g;
    //     }

    //     Vector currentP = {x, y + currentFall};
    //     s32 currentLoop = 0;
    //     while (currentLoop++ < spreadRate + fallRate)
    //     {
    //         Vector nextP = simulateSand(currentP.x, currentP.y, id);
    //         currentSpread = nextP.x - x;
    //         currentFall = nextP.y - y;

    //         if ((nextP.x == currentP.x && nextP.y == currentP.y) || abs(currentSpread) > spreadRate || currentFall > fallRate)
    //         {
    //             swapData(pos, currentP);
    //             return currentP;
    //         }
    //         currentP = nextP;
    //     }

    //     Vector lastP = {x + currentSpread, y + currentFall};
    //     swapData(pos, lastP);
    //     return lastP;
    // }
};