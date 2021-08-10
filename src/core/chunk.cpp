#ifndef CHUNK_CPP
#define CHUNK_CPP

class Chunk
{
public:
    s32 x, y;
    s32 width = 64;
    s32 height = 64;

    Element *grid;

    Chunk(s32 x, s32 y) : x(x), y(y)
    {
        grid = new Element[width * height];
    }

    ~Chunk()
    {
        delete[] grid;
    }
};

#endif // CHUNK_CPP