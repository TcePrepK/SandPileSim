#ifndef SOLID_CPP
#define SOLID_CPP

#include "../element.cpp"
#include "../../../utils/ids.cpp"
#include "../../../utils/types.cpp"

class Solid : public Element
{
public:
    Solid(u32 id, u32 color)
    {
        this->id = id;
        this->color = color;
    }

    void update(double dt)
    {
        // do nothing
    }
};

#endif // SOLID_CPP