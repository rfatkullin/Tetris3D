
#ifndef MATERIAL_H
#define MATERIAL_H

class Material
{
private :
        float mAmbient[ 4 ];
        float mDiffuse[ 4 ];
        float mSpecular[ 4 ];
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

const int	MATERIALS_CNT = 8;

const Material	materials[ MATERIALS_CNT ] = {
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
