
#ifndef MATERIAL_H
#define MATERIAL_H

#include <QTextStream>

class Material
{
private :
        float mAmbient[ 4 ];
        float mDiffuse[ 4 ];
        float mSpecular[ 4 ];
public :
        Material();
        Material( float aAR, float aAG, float aAB, float aAA,
                  float aDR, float aDG, float aDB, float aDA,
		  float aSR, float aSG, float aSB, float aSA
		);

        friend QTextStream& operator << ( QTextStream& aStream, const Material& aMaterial );
        friend QTextStream& operator >> ( QTextStream& aStream, Material& aMaterial );

        const float* GetMaterialForAmbient();
	const float* GetMaterialForDiffuse();
	const float* GetMaterialForSpecular();
};

#endif
