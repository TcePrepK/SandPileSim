#ifndef ELEMENT_CPP
#define ELEMENT_CPP

#include "../utils/vector.cpp"
#include "../utils/types.cpp"

class Element
{
public:
    u32 id = 0;
    u32 color = 0;
    b32 updatedThisFrame = false;

    Vector vel;

    Element()
    {
        this->id = 0;
        this->color = 0x000000;
    }

    Element(u32 id, u32 color)
    {
        this->id = id;
        this->color = color;
    }

    virtual u32 getColor()
    {
        return 0xff0000;
    }

    virtual POINT update(s32 x, s32 y)
    {
        cout << "Updating Something";
        return {x, y};
    }
};

#endif // ELEMENT_CPP
