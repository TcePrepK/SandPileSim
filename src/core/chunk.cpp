#ifndef CHUNK_CPP
#define CHUNK_CPP

class Chunk
{
public:
    s32 x, y, tileX, tileY;
    s32 width, height;
    string name;

    s32 filledPixelAmount = 0;

    Element **grid;

    Chunk(s32 x, s32 y, s32 width, s32 height, string name) : x(x), y(y), tileX(x * width), tileY(y * height), width(width), height(height), name(name)
    {
        grid = new Element *[width * height] {};
    }

    ~Chunk()
    {
        delete[] grid;
    }

    void prepareUpdate()
    {
        for (s32 i = 0; i < width; i++)
        {
            for (s32 j = 0; j < height; j++)
            {
                Element *e = getPixel(i, j);
                if (e != nullptr)
                {
                    e->updatedThisFrame = false;
                }
            }
        }
    }

    void render()
    {
        for (s32 i = 0; i < width; i++)
        {
            for (s32 j = 0; j < height; j++)
            {
                Element *e = getPixel(i, j);

                u32 color = 0x000000;
                if (e != nullptr)
                    color = e->color;

                fillElement(i + tileX, j + tileY, color);
            }
        }

        if (filledPixelAmount > 0)
        {
            strokeRect(tileX, tileY, width, height, 0x00ff00);
        }
        else
        {
            strokeRect(tileX, tileY, width, height, 0x0000000);
        }
    }

    // HELPERS

    Element *getPixel(s32 x, s32 y)
    {
        return grid[x + y * width];
    }

    void setPixel(s32 x, s32 y, Element *element)
    {
        Element *dest = getPixel(x, y);

        grid[x + y * width] = element;

        if (element != nullptr && dest == nullptr)
        {
            filledPixelAmount++;
        }
        else if (dest != nullptr && element == nullptr)
        {
            filledPixelAmount--;
        }
    }
};

#endif // CHUNK_CPP