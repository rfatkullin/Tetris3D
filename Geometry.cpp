#include <math.h>
#include "Geometry.h"

Point3Df& Point3Df :: operator = ( const Point3Df& right_obj )
{
    x = right_obj.x;
    y = right_obj.y;
    z = right_obj.z;

    return *this;
}

Point3Df& Point3Df :: operator = ( const Point3Di& right_obj )
{
    x = right_obj.x;
    y = right_obj.y;
    z = right_obj.z;

    return *this;
}

Point3Df Point3Df :: operator - ( const Point3Df& right_obj )
{
   return Point3Df( x - right_obj.x, y - right_obj.y, z - right_obj.z );
}

Point3Df Point3Df :: operator + ( const Point3Df& right_obj )
{
    return Point3Df( x + right_obj.x, y + right_obj.y, z + right_obj.z );
}

Point3Di& Point3Di :: operator = ( const Point3Di& right_obj )
{
    x = right_obj.x;
    y = right_obj.y;
    z = right_obj.z;

    return *this;
}

Point3Di& Point3Di :: operator = ( const Point3Df& right_obj )
{
    x = ( int )right_obj.x;
    y = ( int )right_obj.y;
    z = ( int )right_obj.z;

    return *this;
}

Point3Di  Point3Di :: operator - ( const Point3Di& right_obj )
{
   return Point3Di( x - right_obj.x, y - right_obj.y, z - right_obj.z );
}

Point3Di  Point3Di :: operator + ( const Point3Di& right_obj )
{
   return Point3Di( x + right_obj.x, y + right_obj.y, z + right_obj.z );
}

Point3Df operator * ( float a, const Point3Df& obj )
{
    return Point3Df( a * obj.x, a * obj.y, a * obj.z );
}

Point3Df GetNormalVector( Point3Df p1, Point3Df p2, Point3Df p3 )
{
	Point3Df a( p3.x - p1.x, p3.y - p1.y, p3.z - p1.z );
	Point3Df b( p2.x - p1.x, p2.y - p1.y, p2.z - p1.z );
	Point3Df n = Point3Df( a.y * b.z - b.y * a.z, a.z * b.x - a.x * b.z , a.x * b.y - b.x * a.y  );
	float vector_length = sqrt( n.x * n.x + n.y * n.y + n.z * n.z );
	return Point3Df( n.x / vector_length, n.y / vector_length, n.z / vector_length  );
}

Point2Df& Point2Df :: operator = ( const Point2Df& right_obj )
{
    x = right_obj.x;
    y = right_obj.y;

    return *this;

}

Point2Df Point2Df :: operator - ( const Point2Df& right_obj )
{
    return Point2Df( x - right_obj.x, y - right_obj.y );
}

Point2Df Point2Df :: operator + ( const Point2Df& right_obj )
{
    return Point2Df( x + right_obj.x, y + right_obj.y );
}

bool InRange( float x, float a, float b )
{
	return ( x >= a + eps ) && ( x <= b - eps );
}

float Max( float a, float b )
{
    if ( a < b - eps )
	return b;
    return a;
}

float Min( float a, float b )
{
    if ( a < b - eps )
	return a;
    return b;
}

float   ScalarMul( const Point3Df& a, const Point3Df& b )
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float   ScalarMul( const Point3Di& a, const Point3Di& b )
{
    return ScalarMul( Point3Df( a ), Point3Df( b ) );
}

Point3Df VectorMul( const Point3Df& a, const Point3Df& b )
{
    return Point3Df( a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x );
}

Point3Di VectorMul( const Point3Di& a, const Point3Di& b )
{
    return Point3Di( a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x );
}


float   MixedMul( const Point3Df& a, const Point3Df& b, const Point3Df& c )
{
    return ScalarMul( a, VectorMul( b, c ) );
}

float   MixedMul( const Point3Di& a, const Point3Di& b, const Point3Di& c )
{
    return ScalarMul( a, VectorMul( b, c ) );
}

float Point3Df :: Length()
{
    return x * x + y * y + z * z;
}

int Point3Di :: Length()
{
    return x * x + y * y + z * z;
}
