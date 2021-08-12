#ifndef VECTOR_CPP
#define VECTOR_CPP

class Vector
{
public:
    s32 x;
    s32 y;

    Vector()
    {
        this->x = 0;
        this->y = 0;
    }

    Vector(s32 x, s32 y)
    {
        this->x = x;
        this->y = y;
    }

    Vector operator+(const Vector &vec)
    {
        return Vector(x + vec.x, y + vec.y);
    }

    Vector operator-(const Vector &vec)
    {
        return Vector(x - vec.x, y - vec.y);
    }

    Vector operator*(const s32 &num)
    {
        return Vector(x * num, y * num);
    }

    Vector operator/(const s32 &num)
    {
        return Vector(x / num, y / num);
    }

    Vector operator*(const Vector &vec)
    {
        return Vector(x * vec.x, y * vec.y);
    }

    Vector operator/(const Vector &vec)
    {
        return Vector(x / vec.x, y / vec.y);
    }

    s32 dot(const Vector &vec)
    {
        return x * vec.x + y * vec.y;
    }

    // Vector cross(const Vector &vec)
    // {
    //     return Vector(y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x);
    // }

    Vector normalize()
    {
        Vector vec;
        s32 len = sqrt(x * x + y * y);
        if (len != 0)
        {
            vec.x = x / len;
            vec.y = y / len;
        }
        return vec;
    }
};

#endif // VECTOR_CPP