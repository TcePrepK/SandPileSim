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

    virtual u32 getColor() { return 0xff0000; }

    virtual POINT update(s32 x, s32 y) { return {x, y}; }

    virtual b32 isDynamic() { return true; }
};

#endif // ELEMENT_CPP
