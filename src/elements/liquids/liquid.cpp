#ifndef LIQUID_CPP
#define LIQUID_CPP

#include "../element.cpp"

class Liquid : public Element
{
public:
    Liquid(u32 id, u32 color) : Element(id, color) {}
};

#endif // LIQUID_CPP