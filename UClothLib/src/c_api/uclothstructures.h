#ifndef UCLOTH_STRUCTURE_H_
#define UCLOTH_STRUCTURE_H_

struct UClothVector3f
{
    // constructors
    UClothVector3f()
    {

    }

    UClothVector3f(const float x, const float y, const float z)
    {
        x_ = x;
        y_ = y;
        z_ = z;
    }

    // set
    void set (const UClothVector3f& v)
    {
        x_ = v.x_;
        y_ = v.y_;
        z_ = v.z_;
    }

    // setCast
    template <class T_VECTOR_FLOAT_3>
    void setCast (const T_VECTOR_FLOAT_3& vectorFloat3)
    {
        *this = *((UClothVector3f*)&vectorFloat3);
    }

    // data_members
    float x_;
    float y_;
    float z_;

};

#endif // UCLOTH_STRUCTURE_H_