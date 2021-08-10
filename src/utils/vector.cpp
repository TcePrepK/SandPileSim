#ifndef VECTOR_CPP
#define VECTOR_CPP

class Vector
{
public:
    s32 x;
    s32 y;
    s32 z;

    Vector()
    {
        this->x = 0;
        this->y = 0;
        this->z = 0;
    }

    Vector(s32 x, s32 y, s32 z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }

    Vector(const Vector &vec)
    {
        x = vec.x;
        y = vec.y;
        z = vec.z;
    }

    Vector operator+(const Vector &vec)
    {
        return Vector(x + vec.x, y + vec.y, z + vec.z);
    }

    Vector operator-(const Vector &vec)
    {
        return Vector(x - vec.x, y - vec.y, z - vec.z);
    }

    Vector operator*(const s32 &num)
    {
        return Vector(x * num, y * num, z * num);
    }

    Vector operator/(const s32 &num)
    {
        return Vector(x / num, y / num, z / num);
    }

    Vector operator*(const Vector &vec)
    {
        return Vector(x * vec.x, y * vec.y, z * vec.z);
    }

    Vector operator/(const Vector &vec)
    {
        return Vector(x / vec.x, y / vec.y, z / vec.z);
    }

    s32 dot(const Vector &vec)
    {
        return x * vec.x + y * vec.y + z * vec.z;
    }

    Vector cross(const Vector &vec)
    {
        return Vector(y * vec.z - z * vec.y, z * vec.x - x * vec.z, x * vec.y - y * vec.x);
    }

    Vector normalize()
    {
        Vector vec;
        s32 len = sqrt(x * x + y * y + z * z);
        if (len != 0)
        {
            vec.x = x / len;
            vec.y = y / len;
            vec.z = z / len;
        }
        return vec;
    }
};

#endif // VECTOR_CPP