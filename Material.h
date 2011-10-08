
#ifndef MATERIAL_H
#define MATERIAL_H

class Material
{
private :
	//float material_for_ambient[ 4 ];
	float material[ 4 ];
	//float material_for_specular[ 4 ];
public :
	Material( float new_r = 0.0f, float new_g = 0.0f, float new_b = 0.0f );
	const float* GetMaterial();
	//const float* GetMaterialForAmbient();
	//const float* GetMaterialForDiffuse();
	//const float* GetMaterialForSpecular();
};

#endif
