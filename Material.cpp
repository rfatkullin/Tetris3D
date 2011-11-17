#include "Material.h"

Material :: Material( float a_r, float a_g, float a_b, float a_a,
		      float d_r, float d_g, float d_b, float d_a,
		      float s_r, float s_g, float s_b, float s_a
		    )
{
    mAmbient[ 0 ] = a_r;
    mAmbient[ 1 ] = a_g;
    mAmbient[ 2 ] = a_b;
    mAmbient[ 3 ] = a_a;

    mDiffuse[ 0 ] = d_r;
    mDiffuse[ 1 ] = d_g;
    mDiffuse[ 2 ] = d_b;
    mDiffuse[ 3 ] = d_a;

    mSpecular[ 0 ] = s_r;
    mSpecular[ 1 ] = s_g;
    mSpecular[ 2 ] = s_b;
    mSpecular[ 3 ] = s_a;
}

const float* Material :: GetMaterialForAmbient()
{
    return mAmbient;
}

const float* Material :: GetMaterialForDiffuse()
{
    return mDiffuse;
}

const float* Material :: GetMaterialForSpecular()
{
    return mSpecular;
}

Material :: Material()
{
    for ( int i = 0; i < 4; i++ )
        mAmbient[ i ] = mDiffuse[ i ] = mSpecular[ i ] = 0.0f;
}
