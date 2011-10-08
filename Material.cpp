#include "Material.h"

Material :: Material( float new_r, float new_g, float new_b )
{
	material[ 0 ] = new_r;
	material[ 1 ] = new_g;
	material[ 2 ] = new_b;
	material[ 3 ] = 1.0f;
	
}

const float* Material :: GetMaterial()
{
	return material;
}
