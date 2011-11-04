#include "Material.h"

const float* Material :: GetMaterialForAmbient()
{
    return material_for_ambient;
}

const float* Material :: GetMaterialForDiffuse()
{
    return material_for_diffuse;
}

const float* Material :: GetMaterialForSpecular()
{
    return material_for_specular;
}
