#include "Material.h"

Material :: Material( float a_r, float a_g, float a_b, float a_a,
		      float d_r, float d_g, float d_b, float d_a,
		      float s_r, float s_g, float s_b, float s_a
		    )
{
    material_for_ambient[ 0 ] = a_r;
    material_for_ambient[ 1 ] = a_g;
    material_for_ambient[ 2 ] = a_b;
    material_for_ambient[ 3 ] = a_a;

    material_for_diffuse[ 0 ] = d_r;
    material_for_diffuse[ 1 ] = d_g;
    material_for_diffuse[ 2 ] = d_b;
    material_for_diffuse[ 3 ] = d_a;

    material_for_specular[ 0 ] = s_r;
    material_for_specular[ 1 ] = s_g;
    material_for_specular[ 2 ] = s_b;
    material_for_specular[ 3 ] = s_a;
}

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

Material :: Material()
{
    for ( int i = 0; i < 4; i++ )
        material_for_ambient[ i ] = material_for_diffuse[ i ] = material_for_specular[ i ] = 0.0f;
}
