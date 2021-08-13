#ifndef WORLD_CPP
#define WORLD_CPP

class World
{
public:
    s32 chunkWidth = 64;
    s32 chunkHeight = 64;
    unordered_map<string, Chunk *> chunks;
    vector<Chunk *> chunkList;

    World() {}

    void prepareUpdate()
    {
        for (s32 i = 0; i < chunkList.size(); i++)
        {
            Chunk *chunk = chunkList.at(i);

            if (chunk->filledPixelAmount == 0)
            {
                chunks.erase(chunk->name);
                chunkList.erase(chunkList.begin() + i);
                i--;

                delete chunk;
                continue;
            }

            chunk->prepareUpdate();
        }
    }

    void update()
    {
        prepareUpdate();
        b32 reverse = globalVariables.currentFrame % 2;
        for (Chunk *chunk : chunkList)
        {
            for (s32 offX = reverse ? 0 : chunkWidth - 1; reverse ? offX < chunkWidth : offX >= 0; reverse ? offX++ : offX--)
            {
                for (s32 offY = reverse ? 0 : chunkHeight - 1; reverse ? offY < chunkHeight : offY >= 0; reverse ? offY++ : offY--)
                {
                    s32 x = chunk->tileX + offX;
                    s32 y = chunk->tileY + offY;

                    Element *e = chunk->getPixel(offX, offY);
                    if (e == nullptr || e->updatedThisFrame)
                        continue;

                    e->updatedThisFrame = true;

                    Vector newPos = e->update(x, y);

                    if (newPos.x == x && newPos.y == y)
                        continue;

                    setPixel(newPos.x, newPos.y, e);
                    chunk->setPixel(offX, offY, nullptr);
                }
            }
        }
    }

    void render()
    {
        for (Chunk *chunk : chunkList)
        {
            chunk->render();
        }

        for (GUI *gui : GUIs)
        {
            Rect pos = gui->pos;
            fillRectWithoutScale(pos.left, pos.top, pos.right - pos.left, pos.bottom - pos.top, gui->color);
        }
    }

    // HELPERS

    Chunk *getChunk(s32 x, s32 y)
    {
        string name = to_string(x) + "|" + to_string(y);
        if (chunks.find(name) == chunks.end())
        {
            Chunk *c = new Chunk(x, y, chunkWidth, chunkHeight, name);

            chunks[name] = c;
            chunkList.push_back(c);

            return c;
        }

        return chunks[name];
    }

    Chunk *getChunkWithTiles(s32 x, s32 y)
    {
        s32 ix = x / chunkWidth;
        s32 iy = y / chunkHeight;

        if (x < 0)
            ix--;
        if (y < 0)
            iy--;

        return getChunk(ix, iy);
    }

    Element *getPixel(s32 x, s32 y)
    {
        Chunk *chunk = getChunkWithTiles(x, y);

        x = x % chunkWidth;
        y = y % chunkHeight;

        if (x < 0)
            x = chunkWidth - (abs(x) % chunkWidth);
        if (y < 0)
            y = chunkHeight - (abs(y) % chunkHeight);
        return chunk->getPixel(x, y);
    }

    void setPixel(s32 x, s32 y, Element *element)
    {
        Chunk *chunk = getChunkWithTiles(x, y);

        x = x % chunkWidth;
        y = y % chunkHeight;

        if (x < 0)
            x = chunkWidth - (abs(x) % chunkWidth);
        if (y < 0)
            y = chunkHeight - (abs(y) % chunkHeight);
        chunk->setPixel(x, y, element);
    }

    void setCirclePixel(s32 x, s32 y, s32 r, Element *e)
    {
        s32 sqrR = r * r;
        for (s32 offX = -r; offX < r; offX++)
        {
            for (s32 offY = -r; offY < r; offY++)
            {
                s32 sqrLen = offX * offX + offY * offY;
                if (sqrLen > sqrR)
                    continue;
                setPixel(x + offX, y + offY, e->clone());
            }
        }

        delete e;
    }

    void setCircleLine(s32 fromX, s32 fromY, s32 toX, s32 toY, s32 r, Element *e)
    {
        if (toX < fromX)
        {
            s32 temp = toX;
            toX = fromX;
            fromX = temp;
        }
        if (toY < fromY)
        {
            s32 temp = toY;
            toY = fromY;
            fromY = temp;
        }

        s32 dx = toX - fromX;
        s32 dy = toY - fromY;

        if (dx == 0 && dy == 0)
        {
            setCirclePixel(fromX, fromY, r, e->clone());
            return;
        }

        s32 step = (dx > dy) ? dx : dy;

        s32 xin = dx / step;
        s32 yin = dy / step;

        s32 x = fromX;
        s32 y = fromY;
        for (s32 i = 0; i < step; i++)
        {
            x += xin;
            y += yin;

            setCirclePixel(x, y, r, e->clone());
        }

        delete e;
    }
};

World sandWorld = World();

#endif // WORLD_CPP