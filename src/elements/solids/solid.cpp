#ifndef SOLID_CPP
#define SOLID_CPP

#include "../element.cpp"

class Solid : public Element
{
public:
    Solid(u32 id, u32 color) : Element(id, color) {}
};

#endif // SOLID_CPP