#include "GeomEntity.h"

GeomEntity :: GeomEntity( int new_x, int new_y, int new_z )
{
    mPosI    = Point3Di( new_x, new_y, new_z );
    mPosF    = mPosI;

}

GeomEntity :: GeomEntity( float new_x, float new_y, float new_z )
{
    mPosF = Point3Df( new_x, new_y, new_z );
    mPosI = mPosF;
}

GeomEntity :: GeomEntity( Point3Di new_pos )
{
    mPosI = new_pos;
    mPosF = new_pos;
}

GeomEntity :: GeomEntity( Point3Df new_pos )
{
    mPosI = new_pos;
    mPosF = new_pos;
}


Point3Df GeomEntity :: GetPosf() const
{
    return mPosF;
}

Point3Di GeomEntity :: GetPosi() const
{
    return mPosI;
}

float GeomEntity :: GetPosByXf() const
{
    return mPosF.mX;
}

float GeomEntity :: GetPosByYf() const
{
    return mPosF.mY;
}

float GeomEntity :: GetPosByZf() const
{
    return mPosF.mZ;
}

int GeomEntity :: GetPosByXi() const
{
    return mPosI.mX;
}

int GeomEntity :: GetPosByYi() const
{
    return mPosI.mY;
}

int GeomEntity :: GetPosByZi() const
{
    return mPosI.mZ;
}

void GeomEntity :: SetPosByXi( int new_x )
{
    mPosI.mX = new_x;
}

void GeomEntity :: SetPosByYi( int new_y )
{
    mPosI.mY = new_y;
}

void GeomEntity :: SetPosByZi( int new_z )
{
    mPosI.mZ = new_z;
}

 void GeomEntity :: SetPosByXf( float new_x )
 {
    mPosF.mX = new_x;
 }

void GeomEntity :: SetPosByYf( float new_y )
{
    mPosF.mY = new_y;
}

void GeomEntity :: SetPosByZf( float new_z )
{
    mPosF.mZ = new_z;
}

void GeomEntity :: SetPosi( int new_x, int new_y, int new_z )
{
    mPosI = Point3Di( new_x, new_y, new_z );
    mPosF = mPosI;
}

void GeomEntity :: SetPosf( float new_x, float new_y, float new_z )
{
    mPosF.mX = new_x;
    mPosF.mY = new_y;
    mPosF.mZ = new_z;
}

void GeomEntity :: SetPosi( Point3Di new_pos )
{
    mPosI = new_pos;
    mPosF = new_pos;
}

void GeomEntity :: SetPosf( Point3Df new_pos )
{
    mPosF = new_pos;
}

