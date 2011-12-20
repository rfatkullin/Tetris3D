#include "Material.h"

#ifndef PHISENTITY_H
#define PHISENTITY_H

class PhisEntity
{
protected:
    Material                mMaterial;
public:
    const static int        MATERIALS_CNT = 8;
    const static Material   msMaterials[ MATERIALS_CNT ];

                            PhisEntity( Material aNewMaterial );
    Material                GetMaterial()  const;
    void                    SetMaterial( Material aNewMaterial );

    virtual void            Draw() = 0;
};

#endif // PHISENTITY_H
