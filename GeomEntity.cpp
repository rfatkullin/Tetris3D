#include "GeomEntity.h"

GeomEntity :: GeomEntity( int new_x, int new_y, int new_z )
{
    pos_i    = Point3Di( new_x, new_y, new_z );
    pos_f    = pos_i;

}

GeomEntity :: GeomEntity( float new_x, float new_y, float new_z )
{
    pos_f = Point3Df( new_x, new_y, new_z );
    pos_i = pos_f;
}

GeomEntity :: GeomEntity( Point3Di new_pos )
{
    pos_i = new_pos;
    pos_f = new_pos;
}

GeomEntity :: GeomEntity( Point3Df new_pos )
{
    pos_i = new_pos;
    pos_f = new_pos;
}


Point3Df GeomEntity :: GetPosf() const
{
    return pos_f;
}

Point3Di GeomEntity :: GetPosi() const
{
    return pos_i;
}

float GeomEntity :: GetPosByXf() const
{
    return pos_f.x;
}

float GeomEntity :: GetPosByYf() const
{
    return pos_f.y;
}

float GeomEntity :: GetPosByZf() const
{
    return pos_f.z;
}

int GeomEntity :: GetPosByXi() const
{
    return pos_i.x;
}

int GeomEntity :: GetPosByYi() const
{
    return pos_i.y;
}

int GeomEntity :: GetPosByZi() const
{
    return pos_i.z;
}

void GeomEntity :: SetPosByXi( int new_x )
{
    pos_i.x = new_x;
}

void GeomEntity :: SetPosByYi( int new_y )
{
    pos_i.y = new_y;
}

void GeomEntity :: SetPosByZi( int new_z )
{
    pos_i.z = new_z;
}

 void GeomEntity :: SetPosByXf( float new_x )
 {
    pos_f.x = new_x;
 }

void GeomEntity :: SetPosByYf( float new_y )
{
    pos_f.y = new_y;
}

void GeomEntity :: SetPosByZf( float new_z )
{
    pos_f.z = new_z;
}

void GeomEntity :: SetPosi( int new_x, int new_y, int new_z )
{
    pos_i = Point3Di( new_x, new_y, new_z );
    pos_f = pos_i;
}

void GeomEntity :: SetPosf( float new_x, float new_y, float new_z )
{
    pos_f.x = new_x;
    pos_f.y = new_y;
    pos_f.z = new_z;
}

void GeomEntity :: SetPosi( Point3Di new_pos )
{
    pos_i = new_pos;
    pos_f = new_pos;
}

void GeomEntity :: SetPosf( Point3Df new_pos )
{
    pos_f = new_pos;
}

