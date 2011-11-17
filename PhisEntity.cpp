#include "PhisEntity.h"

PhisEntity :: PhisEntity( Material new_material )
{
    mMaterial = new_material;
}

Material PhisEntity :: GetMaterial() const
{
    return mMaterial;
}

void PhisEntity :: SetMaterial( Material new_material )
{
    mMaterial = new_material;
}
