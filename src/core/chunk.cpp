#ifndef CHUNK_CPP
#define CHUNK_CPP

class Chunk
{
public:
    s32 x, y, tileX, tileY;
    s32 width, height;

    s32 filledPixelAmount = 0;

    Element **grid;

    Chunk(s32 x, s32 y, s32 width, s32 height) : x(x), y(y), tileX(x * width), tileY(y * height), width(width), height(height)
    {
        grid = new Element *[width * height]
        { nullptr };
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
                if (e->id != mat_id_empty)
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
                u32 color = e->color;

                fillElement(i + tileX, j + tileY, color);
            }
        }

        strokeRect(tileX, tileY, width, height, 0xff0000);
    }

    // HELPERS

    Element *getPixel(s32 x, s32 y)
    {
        Element *e = grid[x + y * width];
        if (e == nullptr)
        {
            e = new Element();
            grid[x + y * width] = e;
        }
        return e;
    }

    void setPixel(s32 x, s32 y, Element *element)
    {
        Element *dest = getPixel(x, y);

        grid[x + y * width] = element;

        if ((dest->id == mat_id_empty || !element->isDynamic()) && element->id != mat_id_empty)
        {
            filledPixelAmount++;
        }
        else if (dest->id != mat_id_empty && (element->id == mat_id_empty || !element->isDynamic()))
        {
            filledPixelAmount--;
        }

        delete dest;
    }
};

#endif // CHUNK_CPP