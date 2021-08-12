#ifndef WORLD_CPP
#define WORLD_CPP

class World
{
public:
    s32 chunkWidth, chunkHeight;
    unordered_map<string, Chunk *> chunks;
    vector<Chunk *> chunkList;

    World(s32 chunkWidth, s32 chunkHeight) : chunkWidth(chunkWidth), chunkHeight(chunkHeight) {}

    void prepareUpdate()
    {
        for (Chunk *chunk : chunkList)
        {
            chunk->prepareUpdate();
        }
    }

    void update()
    {
        prepareUpdate();
        for (Chunk *chunk : chunkList)
        {
            for (s32 x = chunk->tileX; x < chunk->tileX + chunkWidth; x++)
            {
                for (s32 y = chunk->tileY; y < chunk->tileY + chunkHeight; y++)
                {
                    Element *e = getPixel(x, y);
                    if (e->updatedThisFrame || e->id == mat_id_empty)
                        continue;

                    e->updatedThisFrame = true;

                    POINT newPos = e->update(x, y);

                    if (newPos.x == x && newPos.y == y)
                        continue;

                    chunk->setPixel(x % chunkWidth, y % chunkHeight, new Element());
                    setPixel(newPos.x, newPos.y, e);
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
            RECT pos = gui->pos;
            fillRectWithoutScale(pos.left, pos.top, pos.right - pos.left, pos.bottom - pos.top, gui->color);
        }
    }

    // HELPERS

    Chunk *getChunk(s32 x, s32 y)
    {
        string key = to_string(x) + "|" + to_string(y);
        if (chunks.find(key) == chunks.end())
        {
            Chunk *c = new Chunk(x, y, chunkWidth, chunkHeight);

            chunks[key] = c;
            chunkList.push_back(c);
        }

        return chunks[key];
    }

    Chunk *getChunkWithTiles(s32 x, s32 y)
    {
        Chunk *chunk = getChunk(x / chunkWidth, y / chunkHeight);
    }

    Element *getPixel(s32 x, s32 y)
    {
        return getChunkWithTiles(x, y)->getPixel(x % chunkWidth, y % chunkHeight);
    }

    void setPixel(s32 x, s32 y, Element *element)
    {
        getChunkWithTiles(x, y)->setPixel(x % chunkWidth, y % chunkHeight, element);
    }

    void setCirclePixel(s32 x, s32 y, s32 r, Element *e, b32 random)
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

                Element *d = getPixel(x + offX, y + offY);
                if (d->id != mat_id_empty)
                    continue;

                setPixel(x + offX, y + offY, e);
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
                    setPixel(x + offX, y + offY, e);
                }
            }
        }
    }
};

World sandWorld = World(globalVariables.chunkSize, globalVariables.chunkSize);

#endif // WORLD_CPP