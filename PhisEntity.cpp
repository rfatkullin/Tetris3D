#include "PhisEntity.h"

const Material	PhisEntity :: msMaterials[ MATERIALS_CNT ] = {
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


PhisEntity :: PhisEntity( Material aNewMaterial )
{
    mMaterial = aNewMaterial;
}

Material PhisEntity :: GetMaterial() const
{
    return mMaterial;
}

void PhisEntity :: SetMaterial( Material aNewMaterial )
{
    mMaterial = aNewMaterial;
}
