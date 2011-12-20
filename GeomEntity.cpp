#include "GeomEntity.h"

GeomEntity :: GeomEntity( int aNewX, int aNewY, int aNewZ )
{
    mPosI    = Point3Di( aNewX, aNewY, aNewZ );
    mPosF    = mPosI;

}

GeomEntity :: GeomEntity( float aNewX, float aNewY, float aNewZ )
{
    mPosF = Point3Df( aNewX, aNewY, aNewZ );
    mPosI = mPosF;
}

GeomEntity :: GeomEntity( Point3Di aNewPos )
{
    mPosI = aNewPos;
    mPosF = aNewPos;
}

GeomEntity :: GeomEntity( Point3Df aNewPos )
{
    mPosI = aNewPos;
    mPosF = aNewPos;
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

void GeomEntity :: SetPosByXi( int aNewX )
{
    mPosI.mX = aNewX;
    mPosF.mX = static_cast< float > ( aNewX );
}

void GeomEntity :: SetPosByYi( int aNewY )
{
    mPosI.mY = aNewY;
    mPosF.mY = static_cast< float > ( aNewY );
}

void GeomEntity :: SetPosByZi( int aNewZ )
{
    mPosI.mZ = aNewZ;
    mPosF.mZ = static_cast< float > ( aNewZ );
}

 void GeomEntity :: SetPosByXf( float aNewX )
 {
    mPosF.mX = aNewX;
 }

void GeomEntity :: SetPosByYf( float aNewY )
{
    mPosF.mY = aNewY;
}

void GeomEntity :: SetPosByZf( float aNewZ )
{
    mPosF.mZ = aNewZ;
}

void GeomEntity :: SetPosi( int aNewX, int aNewY, int aNewZ )
{
    mPosI = Point3Di( aNewX, aNewY, aNewZ );
    mPosF = mPosI;
}

void GeomEntity :: SetPosf( float aNewX, float aNewY, float aNewZ )
{
    mPosF.mX = aNewX;
    mPosF.mY = aNewY;
    mPosF.mZ = aNewZ;
}

void GeomEntity :: SetPosi( Point3Di aNewPos )
{
    mPosI = aNewPos;
    mPosF = aNewPos;
}

void GeomEntity :: SetPosf( Point3Df aNewPos )
{
    mPosF = aNewPos;
}

