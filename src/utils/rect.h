#ifndef RECT_CPP
#define RECT_CPP

class Rect
{
public:
    s32 left, top, right, bottom;

    Rect() : left(0), top(0), right(0), bottom(0) {}
    Rect(s32 left, s32 top, s32 right, s32 bottom) : left(left), top(top), right(right), bottom(bottom) {}
};

#endif // RECT_CPP