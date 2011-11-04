#include "PhisEntity.h"

PhisEntity :: PhisEntity( Material new_material )
{
    material = new_material;
}

Material PhisEntity :: GetMaterial() const
{
    return material;
}

