#ifndef ELEMENT_CPP
#define ELEMENT_CPP

#include "../utils/vector.cpp"

class Element
{
public:
    u32 id = 0;
    u32 color = 0;
    b32 updatedThisFrame = false;

    Vector vel;
};

#endif // ELEMENT_CPP
