#include <math.h>
#include "Geom.h"

Point3Df& Point3Df :: operator = ( const Point3Df& right_obj )
{
	x = right_obj.x;
	y = right_obj.y;
	z = right_obj.z;

	return *this;
}

Point3Df Point3Df :: operator - ( const Point3Df& right_obj )
{
    Point3Df tmp;
    tmp.x = x - right_obj.x;
    tmp.y = y - right_obj.y;
    tmp.z = z - right_obj.z;

    return tmp;
}

Point3Df Point3Df :: operator + ( const Point3Df& right_obj )
{
    Point3Df tmp;
    tmp.x = x + right_obj.x;
    tmp.y = y + right_obj.y;
    tmp.z = z + right_obj.z;

    return tmp;
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


