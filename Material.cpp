#include "Material.h"

Material :: Material( float aAR, float aAG, float aAB, float aAA,
		      float aDR, float aDG, float aDB, float aDA,
		      float aSR, float aSG, float aSB, float aSA
		    )
{
    mAmbient[ 0 ] = aAR;
    mAmbient[ 1 ] = aAG;
    mAmbient[ 2 ] = aAB;
    mAmbient[ 3 ] = aAA;

    mDiffuse[ 0 ] = aDR;
    mDiffuse[ 1 ] = aDG;
    mDiffuse[ 2 ] = aDB;
    mDiffuse[ 3 ] = aDA;

    mSpecular[ 0 ] = aSR;
    mSpecular[ 1 ] = aSG;
    mSpecular[ 2 ] = aSB;
    mSpecular[ 3 ] = aSA;
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

QTextStream& operator << ( QTextStream& aStream, const Material& aMaterial )
{
    aStream << aMaterial.mAmbient[ 0 ] << '\t' << aMaterial.mAmbient[ 1 ] << '\t'
           << aMaterial.mAmbient[ 2 ] << '\t' << aMaterial.mAmbient[ 3 ] << '\t'
           << aMaterial.mDiffuse[ 0 ] << '\t' << aMaterial.mDiffuse[ 1 ] << '\t'
           << aMaterial.mDiffuse[ 2 ] << '\t' << aMaterial.mDiffuse[ 3 ] << '\t'
           << aMaterial.mSpecular[ 0 ] << '\t' << aMaterial.mSpecular[ 1 ] << '\t'
           << aMaterial.mSpecular[ 2 ] << '\t' << aMaterial.mSpecular[ 3 ];

    return aStream;
}

 QTextStream& operator >> ( QTextStream& aStream, Material& aMaterial )
{
    aStream >> aMaterial.mAmbient[ 0 ]  >> aMaterial.mAmbient[ 1 ]
           >> aMaterial.mAmbient[ 2 ]  >> aMaterial.mAmbient[ 3 ]
           >> aMaterial.mDiffuse[ 0 ]  >> aMaterial.mDiffuse[ 1 ]
           >> aMaterial.mDiffuse[ 2 ]  >> aMaterial.mDiffuse[ 3 ]
           >> aMaterial.mSpecular[ 0 ] >> aMaterial.mSpecular[ 1 ]
           >> aMaterial.mSpecular[ 2 ] >> aMaterial.mSpecular[ 3 ];

    return aStream;
}
