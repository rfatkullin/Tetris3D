#include <math.h>
#include "Geometry.h"


const float Geometry :: pi = 3.141592653f;
const float Geometry :: eps = 1e-5f;

Point3Df& Point3Df :: operator = ( const Point3Df& right_obj )
{
    mX = right_obj.mX;
    mY = right_obj.mY;
    mZ = right_obj.mZ;

    return *this;
}

Point3Df& Point3Df :: operator = ( const Point3Di& right_obj )
{
    mX = ( float )right_obj.mX;
    mY = ( float )right_obj.mY;
    mZ = ( float )right_obj.mZ;

    return *this;
}

Point3Df Point3Df :: operator - ( const Point3Df& right_obj )
{
   return Point3Df( mX - right_obj.mX, mY - right_obj.mY, mZ - right_obj.mZ );
}

Point3Df Point3Df :: operator + ( const Point3Df& right_obj )
{
    return Point3Df( mX + right_obj.mX, mY + right_obj.mY, mZ + right_obj.mZ );
}

Point3Di :: Point3Di( int new_x, int new_y, int new_z )
{
    mX = new_x;
    mY = new_y;
    mZ = new_z;
}

Point3Di& Point3Di :: operator = ( const Point3Di& right_obj )
{
    mX = right_obj.mX;
    mY = right_obj.mY;
    mZ = right_obj.mZ;

    return *this;
}

Point3Di& Point3Di :: operator = ( const Point3Df& right_obj )
{
    mX = ( int )right_obj.mX;
    mY = ( int )right_obj.mY;
    mZ = ( int )right_obj.mZ;

    return *this;
}

Point3Di  Point3Di :: operator - ( const Point3Di& right_obj )
{
   return Point3Di( mX - right_obj.mX, mY - right_obj.mY, mZ - right_obj.mZ );
}

Point3Di  Point3Di :: operator + ( const Point3Di& right_obj )
{
   return Point3Di( mX + right_obj.mX, mY + right_obj.mY, mZ + right_obj.mZ );
}

Point3Df operator * ( float a, const Point3Df& obj )
{
    return Point3Df( a * obj.mX, a * obj.mY, a * obj.mZ );
}

Point3Df GetNormalVector( Point3Df p1, Point3Df p2, Point3Df p3 )
{
	Point3Df a( p3.mX - p1.mX, p3.mY - p1.mY, p3.mZ - p1.mZ );
	Point3Df b( p2.mX - p1.mX, p2.mY - p1.mY, p2.mZ - p1.mZ );
	Point3Df n = Point3Df( a.mY * b.mZ - b.mY * a.mZ, a.mZ * b.mX - a.mX * b.mZ , a.mX * b.mY - b.mX * a.mY  );
	float vector_length = sqrt( n.mX * n.mX + n.mY * n.mY + n.mZ * n.mZ );
	return Point3Df( n.mX / vector_length, n.mY / vector_length, n.mZ / vector_length  );
}

Point2Df& Point2Df :: operator = ( const Point2Df& right_obj )
{
    mX = right_obj.mX;
    mY = right_obj.mY;

    return *this;

}

Point2Df Point2Df :: operator - ( const Point2Df& right_obj )
{
    return Point2Df( mX - right_obj.mX, mY - right_obj.mY );
}

Point2Df Point2Df :: operator + ( const Point2Df& right_obj )
{
    return Point2Df( mX + right_obj.mX, mY + right_obj.mY );
}

int Geometry :: Sign( float a )
{
    if ( a < eps )
        return -1;
    return 1;
}

bool Geometry :: InRange( float x, float a, float b )
{
	return ( x >= a + eps ) && ( x <= b - eps );
}

float Geometry :: Max( float a, float b )
{
    if ( a < b - eps )
	return b;
    return a;
}

float Geometry :: Min( float a, float b )
{
    if ( a < b - eps )
	return a;
    return b;
}

float Geometry :: ScalarMul( const Point3Df& a, const Point3Df& b )
{
    return a.mX * b.mX + a.mY * b.mY + a.mZ * b.mZ;
}

float Geometry :: ScalarMul( const Point3Di& a, const Point3Di& b )
{
    return ScalarMul( Point3Df( a ), Point3Df( b ) );
}

Point3Df Geometry :: VectorMul( const Point3Df& a, const Point3Df& b )
{
    return Point3Df( a.mY * b.mZ - a.mZ * b.mY, a.mZ * b.mX - a.mX * b.mZ, a.mX * b.mY - a.mY * b.mX );
}

Point3Di Geometry :: VectorMul( const Point3Di& a, const Point3Di& b )
{
    return Point3Di( a.mY * b.mZ - a.mZ * b.mY, a.mZ * b.mX - a.mX * b.mZ, a.mX * b.mY - a.mY * b.mX );
}


float Geometry :: MixedMul( const Point3Df& a, const Point3Df& b, const Point3Df& c )
{
    return ScalarMul( a, VectorMul( b, c ) );
}

float Geometry :: MixedMul( const Point3Di& a, const Point3Di& b, const Point3Di& c )
{
    return ScalarMul( a, VectorMul( b, c ) );
}

float Point3Df :: Length() const
{
    return mX * mX + mY * mY + mZ * mZ;
}

int Point3Di :: Length() const
{
    return mX * mX + mY * mY + mZ * mZ;
}

float Geometry :: Square( float aValue )
{
    return aValue * aValue;
}

float Geometry :: ParalSquare( Point2Df& aA, Point2Df& aB )
{
    return aA.mX * aB.mY - aA.mY * aB.mX;
}

Point3Df Geometry :: Normalize( const Point3Df& vec )
{
    float length = sqrt( vec.Length() );
    return Point3Df( vec.mX / length, vec.mY / length, vec.mZ / length );
}
