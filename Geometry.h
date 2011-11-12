
#ifndef POINT_H
#define POINT_H

const float pi = 3.141592653f;
const float eps = 1e-5f;

struct Point3Df;

struct Point3Di
{
    int x;
    int y;
    int z;

    Point3Di( int new_x, int new_y, int new_z );// : x( new_x ), y( new_y ), z( new_z ) { }
    Point3Di() : x( 0 ), y( 0 ), z( 0 ) {}
    Point3Di& operator = ( const Point3Di& right_obj );
    Point3Di& operator = ( const Point3Df& right_obj );
    Point3Di operator - ( const Point3Di& right_obj );
    Point3Di operator + ( const Point3Di& right_obj );

    int   Length();
};

struct Point3Df
{
    float x;
    float y;
    float z;

    Point3Df( float new_x, float new_y, float new_z ) : x( new_x ), y( new_y ), z( new_z ) { }
    Point3Df() : x( 0.0f ), y( 0.0f ), z( 0.0f ) {}
    Point3Df( const Point3Df& obj ) : x( obj.x ), y( obj.y ), z( obj.z ) { }
    Point3Df( const Point3Di& obj ) : x( obj.x ), y( obj.y ), z( obj.z ) { }
    Point3Df& operator = ( const Point3Df& right_obj );
    Point3Df& operator = ( const Point3Di& right_obj );
    Point3Df  operator - ( const Point3Df& right_obj );
    Point3Df  operator + ( const Point3Df& right_obj );

    float   Length();
};

struct Point2Df
{
    float x;
    float y;

    Point2Df( float new_x, float new_y ) : x( new_x ), y( new_y ) {}
    Point2Df() : x( 0.0f ), y( 0.0f ) {}
    Point2Df& operator = ( const Point2Df& right_obj );
    Point2Df  operator - ( const Point2Df& right_obj );
    Point2Df  operator + ( const Point2Df& right_obj );

};

Point3Df operator * ( float a, const Point3Df& obj );
Point3Df GetNormalVector( Point3Df p1, Point3Df p2, Point3Df p3 );

struct SphericalCoor
{
    float teta;
    float alpha;

    SphericalCoor( float new_teta, float new_alpha ) : teta( new_teta ), alpha( new_alpha ) { }
    SphericalCoor() : teta( 0.0f ), alpha( 0.0f ) { }
};

class Geometry
{
public :
    static Point3Di    VectorMul( const Point3Di& a, const Point3Di& b );
    static Point3Df    VectorMul( const Point3Df& a, const Point3Df& b );
    static float       Max( float a, float b );
    static float       Min( float a, float b );
    static float       MixedMul( const Point3Df& a, const Point3Df& b, const Point3Df& c );
    static float       MixedMul( const Point3Di& a, const Point3Di& b, const Point3Di& c );
    static float       ScalarMul( const Point3Df& a, const  Point3Df& b );
    static float       ScalarMul( const Point3Di& a, const Point3Di& b );
    static bool        InRange( float x, float a, float b );
    static int         Sign( float a );
};

#endif
