
#ifndef POINT_H
#define POINT_H

const float pi = 3.141592653f;
const float eps = 1e-5;

struct Point3D
{
    float x;
    float y;
    float z;

    Point3D( float new_x, float new_y, float new_z ) : x( new_x ), y( new_y ), z( new_z ) { }
    Point3D() : x( 0.0f ), y( 0.0f ), z( 0.0f ) {}
    Point3D& operator = ( const Point3D& rigth_obj );
    Point3D operator - ( const Point3D& rigth_obj );
    Point3D operator + ( const Point3D& right_obj );
};

Point3D operator * ( float a, const Point3D& obj );
Point3D GetNormalVector( Point3D p1, Point3D p2, Point3D p3 );

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
