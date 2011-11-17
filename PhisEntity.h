#include "Material.h"

#ifndef PHISENTITY_H
#define PHISENTITY_H

class PhisEntity
{
protected:
    Material	    mMaterial;
public:
                    PhisEntity( Material new_material );
    Material        GetMaterial()  const;
    void            SetMaterial( Material new_material );

    virtual void    Draw() = 0;
};

#endif // PHISENTITY_H
