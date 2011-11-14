
#ifndef MATERIAL_H
#define MATERIAL_H

class Material
{
private :
	float material_for_ambient[ 4 ];
	float material_for_diffuse[ 4 ];
	float material_for_specular[ 4 ];
public :
        Material();
        Material( float a_r, float a_g, float a_b, float a_a,
		  float d_r, float d_g, float d_b, float d_a,
		  float s_r, float s_g, float s_b, float s_a
		);
	const float* GetMaterialForAmbient();
	const float* GetMaterialForDiffuse();
	const float* GetMaterialForSpecular();
};

const int	MaterialsCount = 8;

const Material	materials[ MaterialsCount ] = {
    Material(  0.0f, 1.0f, 0.0f, 1.0f,
               0.0f, 1.0f, 0.0f, 1.0f,
               0.0f, 0.0f, 0.0f, 1.0f
            ),
    Material(  1.0f, 0.0f, 0.0f, 1.0f,
               1.0f, 0.0f, 0.0f, 1.0f,
	       0.0f, 0.0f, 0.0f, 1.0f
            ),
    Material(  0.0f, 0.0f, 1.0f, 1.0f,
               0.0f, 0.0f, 1.0f, 1.0f,
	       0.0f, 0.0f, 0.0f, 1.0f
            ),
    Material(  1.0f, 0.0f, 1.0f, 1.0f,
               1.0f, 0.0f, 1.0f, 1.0f,
	       0.0f, 0.0f, 0.0f, 1.0f
            ),
    Material(  1.0f, 1.0f, 0.0f, 1.0f,
               1.0f, 1.0f, 0.0f, 1.0f,
	       0.0f, 0.0f, 0.0f, 1.0f
            ),
    Material(  0.0f, 1.0f, 1.0f, 1.0f,
               0.0f, 1.0f, 1.0f, 1.0f,
	       0.0f, 0.0f, 0.0f, 1.0f
            ),
    Material(  1.0f, 1.0f, 1.0f, 1.0f, //For select blocks
               1.0f, 1.0f, 1.0f, 1.0f,
               0.0f, 0.0f, 0.0f, 1.0f
            ),
    Material(  0.2f, 0.2f, 0.2f, 1.0f, //For bottom platform
               0.2f, 0.2f, 0.2f, 1.0f,
               0.0f, 0.0f, 0.0f, 1.0f
        )
    };

#endif
