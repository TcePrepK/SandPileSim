#define WATER 2

class Water : public Liquid
{
public:
    Water() : Liquid(WATER, getColor()) {}

    u32 getColor()
    {
        f32 r = (f32)(randomVal(0, 1)) / 2.f;
        s32 red = (s32)(lerp(0.1f, 0.15f, r) * 255.f);
        s32 green = (s32)(lerp(0.3f, 0.35f, r) * 255.f);
        s32 blue = (s32)(lerp(0.7f, 0.8f, r) * 255.f);
        s32 alpha = 255;
        return rgbaToHex(red, green, blue, alpha);
    }

    Element *clone() { return new Water(); }

    // Vector simulateWater(s32 x, s32 y, s32 id)
    // {
    //     s32 dir = randomVal(0, 1) ? 1 : -1;
    //     Vector pos = {x, y};
    //     Vector B = {x, y + 1};
    //     Vector L = {x + dir, y};
    //     Vector R = {x - dir, y};
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
    //     else if (isMoveFree(L, id))
    //     {
    //         return L;
    //     }
    //     else if (isMoveFree(R, id))
    //     {
    //         return R;
    //     }

    //     return pos;
    // }

    // Vector update(s32 x, s32 y)
    // {
    //     s32 id = mat_id_water;
    //     s32 fallRate = 1;   // 5
    //     s32 spreadRate = 1; // 50

    //     s32 currentFall = 0;
    //     s32 currentSpread = 0;

    //     Vector pos = {x, y};
    //     if (isSurrounded(x, y, id))
    //         return {x, y}; // It can't move

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
    //         Vector nextP = simulateWater(currentP.x, currentP.y, id);
    //         currentSpread = nextP.x - x;
    //         currentFall = nextP.y - y;

    //         if ((nextP.x == currentP.x && nextP.y == currentP.y) || abs(currentSpread) > spreadRate || currentFall > fallRate)
    //         {
    //             swapData(pos, currentP);
    //             return currentP;
    //         }
    //         currentP = nextP;
    //     }

    //     Vector lastPos = {x + currentSpread, y + currentFall};
    //     swapData(pos, lastPos);
    //     return lastPos;
    // }
};