
#ifndef POINT_H
#define POINT_H

struct Point3Df;
struct Point3Di;
struct Point2Df;

class Geometry
{
public :
    static const float pi;
    static const float eps;
    static Point3Df    Normalize( const Point3Df& vec );
    static Point3Di    VectorMul( const Point3Di& a, const Point3Di& b );
    static Point3Df    VectorMul( const Point3Df& a, const Point3Df& b );
    static float       Max( float a, float b );
    static float       Min( float a, float b );
    static float       MixedMul( const Point3Df& a, const Point3Df& b, const Point3Df& c );
    static float       MixedMul( const Point3Di& a, const Point3Di& b, const Point3Di& c );
    static float       ScalarMul( const Point3Df& a, const  Point3Df& b );
    static float       ScalarMul( const Point3Di& a, const Point3Di& b );
    static float       Square( float aValue );
    static float       ParalSquare( Point2Df& aA, Point2Df& aB );
    static bool        InRange( float x, float a, float b );
    static int         Sign( float a );
};

struct Point3Di
{
    int mX;
    int mY;
    int mZ;

    Point3Di( int new_x, int new_y, int new_z );// : x( new_x ), y( new_y ), z( new_z ) { }
    Point3Di() : mX( 0 ), mY( 0 ), mZ( 0 ) {}
    Point3Di& operator = ( const Point3Di& right_obj );
    Point3Di& operator = ( const Point3Df& right_obj );
    Point3Di operator - ( const Point3Di& right_obj );
    Point3Di operator + ( const Point3Di& right_obj );

    int   Length() const;
};

struct Point3Df
{
    float mX;
    float mY;
    float mZ;

    Point3Df( float new_x, float new_y, float new_z ) : mX( new_x ), mY( new_y ), mZ( new_z ) { }
    Point3Df() : mX( 0.0f ), mY( 0.0f ), mZ( 0.0f ) {}
    Point3Df( const Point3Df& obj ) : mX( obj.mX ), mY( obj.mY ), mZ( obj.mZ ) { }
    Point3Df( const Point3Di& obj ) : mX( ( float )obj.mX ), mY( ( float )obj.mY ), mZ( ( float )obj.mZ ) { }
    Point3Df& operator = ( const Point3Df& right_obj );
    Point3Df& operator = ( const Point3Di& right_obj );
    Point3Df  operator - ( const Point3Df& right_obj );
    Point3Df  operator + ( const Point3Df& right_obj );

    float   Length() const;
};

struct Point2Df
{
    float mX;
    float mY;

    Point2Df( float new_x, float new_y ) : mX( new_x ), mY( new_y ) {}
    Point2Df() : mX( 0.0f ), mY( 0.0f ) {}
    Point2Df& operator = ( const Point2Df& right_obj );
    Point2Df  operator - ( const Point2Df& right_obj );
    Point2Df  operator + ( const Point2Df& right_obj );

};

Point3Df operator * ( float a, const Point3Df& obj );
Point3Df GetNormalVector( Point3Df p1, Point3Df p2, Point3Df p3 );

struct SphericalCoor
{
    float mTeta;
    float mAlpha;

    SphericalCoor( float new_teta, float new_alpha ) : mTeta( new_teta ), mAlpha( new_alpha ) { }
    SphericalCoor() : mTeta( 0.0f ), mAlpha( 0.0f ) { }
};

#endif
