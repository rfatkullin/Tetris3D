#include "Geometry.h"

#ifndef GEOMENTITY_H
#define GEOMENTITY_H

class GeomEntity
{
protected:
    Point3Di	    mPosI;
    Point3Df	    mPosF;
public:
                    GeomEntity( int new_x, int new_y, int new_z );
                    GeomEntity( float new_x, float new_y, float new_z );
                    GeomEntity( Point3Di new_pos );
                    GeomEntity( Point3Df new_pos );
//Position
    Point3Df	    GetPosf() const;
    Point3Di	    GetPosi() const;

    float	    GetPosByXf() const;
    float	    GetPosByYf() const;
    float	    GetPosByZf() const;
    int		    GetPosByXi() const;
    int		    GetPosByYi() const;
    int		    GetPosByZi() const;

    void	    SetPosByXi( int new_x );
    void	    SetPosByYi( int new_y );
    void	    SetPosByZi( int new_z );
    void	    SetPosByXf( float new_x );
    void	    SetPosByYf( float new_y );
    void	    SetPosByZf( float new_z );

    void	    SetPosi( int new_x, int new_y, int new_z );
    void	    SetPosf( float new_x, float new_y, float new_z );
    void	    SetPosi( Point3Di new_pos );
    void	    SetPosf( Point3Df new_pos );

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

    virtual void    RotateOnZY( float angle, bool change_const ) = 0;
    virtual void    RotateOnZX( float angle, bool change_const ) = 0;
    virtual void    RotateOnXY( float angle, bool change_const ) = 0;

    virtual void    SetVerAbsCoor() = 0;
    virtual void    SetVerRelCoor() = 0;

};

#endif // GEOMENTITY_H
