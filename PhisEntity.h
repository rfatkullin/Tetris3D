#include "Material.h"

#ifndef PHISENTITY_H
#define PHISENTITY_H

class PhisEntity
{
protected:
    Material	    material;
public:
                    PhisEntity( Material new_material );
    Material        GetMaterial()  const;

    virtual void    Draw() = 0;
};

#endif // PHISENTITY_H
