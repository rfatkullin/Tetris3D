
#ifndef POINT_H
#define POINT_H

const float pi = 3.141592653f;
const float eps = 1e-5f;

struct Point3Df
{
    float x;
    float y;
    float z;

    Point3Df( float new_x, float new_y, float new_z ) : x( new_x ), y( new_y ), z( new_z ) { }
    Point3Df() : x( 0.0f ), y( 0.0f ), z( 0.0f ) {}
    Point3Df& operator = ( const Point3Df& rigth_obj );
    Point3Df operator - ( const Point3Df& rigth_obj );
    Point3Df operator + ( const Point3Df& right_obj );
};

struct Point3Di
{
    int x;
    int y;
    int z;

    Point3Di( int new_x, int new_y, int new_z ) : x( new_x ), y( new_y ), z( new_z ) { }
    Point3Di() : x( 0 ), y( 0 ), z( 0 ) {}
    Point3Di& operator = ( const Point3Di& rigth_obj );
    Point3Di operator - ( const Point3Di& rigth_obj );
    Point3Di operator + ( const Point3Di& right_obj );
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

bool	InRange( float x, float a, float b );
float	Max( float a, float b );
float	Min( float a, float b );

#endif
