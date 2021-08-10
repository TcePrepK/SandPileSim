#ifndef ELEMENT_CPP
#define ELEMENT_CPP

class Element
{
public:
    u32 id;
    u32 color;
    b32 updatedThisFrame = false;

    Vector vel;

    Element() : id(0), color(0) {}

    Element(u32 id, u32 color) : id(id), color(color) {}

    virtual u32 getColor()
    {
        return 0xff0000;
    }

    virtual POINT update(s32 x, s32 y)
    {
        cout << "Update default element";
        return {x, y};
    }
};

#endif // ELEMENT_CPP
