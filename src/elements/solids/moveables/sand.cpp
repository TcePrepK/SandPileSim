#include "../solid.cpp"

class Sand : public Solid
{
public:
    Sand() : Solid(mat_id_sand, createColor()) {}

    u32 createColor()
    {
        return 0xff0000;
    }

    POINT simulateSand(s32 x, s32 y, s32 id)
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

    POINT update(s32 x, s32 y)
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
};