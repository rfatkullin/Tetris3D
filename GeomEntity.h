#include "Geometry.h"

#ifndef GEOMENTITY_H
#define GEOMENTITY_H

class GeomEntity
{
protected:
    Point3Di	    mPosI;
    Point3Df	    mPosF;
public:
                    GeomEntity( int aNewX, int aNewY, int aNewZ );
                    GeomEntity( float aNewX, float aNewY, float aNewZ );
                    GeomEntity( Point3Di aNewPos );
                    GeomEntity( Point3Df aNewPos );
//Position
    Point3Df	    GetPosf() const;
    Point3Di	    GetPosi() const;

    float	    GetPosByXf() const;
    float	    GetPosByYf() const;
    float	    GetPosByZf() const;
    int		    GetPosByXi() const;
    int		    GetPosByYi() const;
    int		    GetPosByZi() const;

    void	    SetPosByXi( int aNewX );
    void	    SetPosByYi( int aNewY );
    void	    SetPosByZi( int aNewZ );
    void	    SetPosByXf( float aNewX );
    void	    SetPosByYf( float aNewY );
    void	    SetPosByZf( float aNewZ );

    void	    SetPosi( int aNewX, int aNewY, int aNewZ );
    void	    SetPosf( float aNewX, float aNewY, float aNewZ );
    void	    SetPosi( Point3Di aNewPos );
    void	    SetPosf( Point3Df aNewPos );

//Bounds
    virtual float   LowerBoundXf() = 0;
    virtual float   UpperBoundXf() = 0;
    virtual float   LowerBoundYf() = 0;
    virtual float   UpperBoundYf() = 0;
    virtual float   LowerBoundZf() = 0;
    virtual float   UpperBoundZf() = 0;

    virtual int	    LowerBoundXi() = 0;
    virtual int	    UpperBoundXi() = 0;
    virtual int	    LowerBoundYi() = 0;
    virtual int	    UpperBoundYi() = 0;
    virtual int	    LowerBoundZi() = 0;
    virtual int	    UpperBoundZi() = 0;

    virtual void    RotateOnZY( float aAngle, bool ChangeConst ) = 0;
    virtual void    RotateOnZX( float aAngle, bool ChangeConst ) = 0;
    virtual void    RotateOnXY( float aAngle, bool ChangeConst ) = 0;

    virtual void    SetVerAbsCoor() = 0;
    virtual void    SetVerRelCoor() = 0;

};

#endif // GEOMENTITY_H
