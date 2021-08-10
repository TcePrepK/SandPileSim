#ifndef WORLD_CPP
#define WORLD_CPP

class World
{
public:
    unordered_map<string, Chunk> chunks;

    World() {}

    Chunk getChunk(s32 x, s32 y)
    {
        string key = to_string(x) + "|" + to_string(y);
        if (chunks.find(key) == chunks.end())
        {
            chunks[key] = Chunk(x, y);
        }

        return chunks[key];
    }

    // ~World()
    // {
    //     delete grid;
    // }
};

#endif // WORLD_CPP