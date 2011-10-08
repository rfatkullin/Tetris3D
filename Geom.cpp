#include <math.h>
#include "Geom.h"

Point3D& Point3D :: operator = ( const Point3D& right_obj )
{
	x = right_obj.x;
	y = right_obj.y;
	z = right_obj.z;

	return *this;
}

Point3D Point3D :: operator - ( const Point3D& right_obj )
{
	Point3D tmp;
	tmp.x = x - right_obj.x;
	tmp.y = y - right_obj.y;
	tmp.z = z - right_obj.z;

	return tmp;
}

Point3D Point3D :: operator + ( const Point3D& right_obj )
{
	Point3D tmp;
	tmp.x = x + right_obj.x;
	tmp.y = y + right_obj.y;
	tmp.z = z + right_obj.z;

	return tmp;
}

Point3D operator * ( float a, const Point3D& obj )
{
	return Point3D( a * obj.x, a * obj.y, a * obj.z );
}

Point3D GetNormalVector( Point3D p1, Point3D p2, Point3D p3 )
{
	Point3D a( p3.x - p1.x, p3.y - p1.y, p3.z - p1.z );
	Point3D b( p2.x - p1.x, p2.y - p1.y, p2.z - p1.z );
	Point3D n = Point3D( a.y * b.z - b.y * a.z, a.z * b.x - a.x * b.z , a.x * b.y - b.x * a.y  );
	float vector_length = sqrt( n.x * n.x + n.y * n.y + n.z * n.z );
	return Point3D( n.x / vector_length, n.y / vector_length, n.z / vector_length  );
}

bool InRange( float x, float a, float b )
{
	return ( x >= a + eps ) && ( x <= b - eps );
}

