
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
    static float       MixedMul(  const Point3Df& aA, const Point3Df& aB, const Point3Df& aC );
    static float       MixedMul(  const Point3Di& aA, const Point3Di& aB, const Point3Di& aC );
    static Point3Di    VectorMul( const Point3Di& aA, const Point3Di& aB );
    static Point3Df    VectorMul( const Point3Df& aA, const Point3Df& aB );
    static float       ScalarMul( const Point3Df& aA, const Point3Df& aB );
    static float       ScalarMul( const Point3Di& aA, const Point3Di& aB );
    static float       ParalSquare( Point2Df& aA, Point2Df& aB );
    static bool        InRange( float aX, float aA, float aB );
    static Point3Df    Normalize( const Point3Df& aVec );
    static float       Max( float aA, float aB );
    static float       Min( float aA, float aB );
    static float       Square( float aValue );
    static int         Sign( float aA );
};

struct Point3Di
{
    int mX;
    int mY;
    int mZ;

    Point3Di( int aNewX, int aNewY, int aNewZ );
    Point3Di() : mX( 0 ), mY( 0 ), mZ( 0 ) {}
    Point3Di& operator = ( const Point3Di& aRightObj );
    Point3Di& operator = ( const Point3Df& aRightObj );
    Point3Di  operator - ( const Point3Di& aRightObj );
    Point3Di  operator + ( const Point3Di& aRightObj );

    int   Length() const;
};

struct Point3Df
{
    float mX;
    float mY;
    float mZ;

    Point3Df( float aNewX, float aNewY, float aNewZ ) : mX( aNewX ), mY( aNewY ), mZ( aNewZ ) { }
    Point3Df() : mX( 0.0f ),
                 mY( 0.0f ),
                 mZ( 0.0f )
                 {}

    Point3Df( const Point3Df& aObj ) : mX( aObj.mX ),
                                       mY( aObj.mY ),
                                       mZ( aObj.mZ )
                                       {}

    Point3Df( const Point3Di& aObj ) : mX( static_cast< float >( aObj.mX ) ),
                                       mY( static_cast< float >( aObj.mY ) ),
                                       mZ( static_cast< float >( aObj.mZ ) )
                                       {}

    Point3Df& operator = ( const Point3Df& RightObj );
    Point3Df& operator = ( const Point3Di& RightObj );
    Point3Df  operator - ( const Point3Df& RightObj );
    Point3Df  operator + ( const Point3Df& RightObj );

    float   Length() const;
};

struct Point2Df
{
    float mX;
    float mY;

    Point2Df( float aNewX, float aNewY ) : mX( aNewX ),
                                           mY( aNewY )
                                           {}

    Point2Df() : mX( 0.0f ),
                 mY( 0.0f )
                 {}

    Point2Df& operator = ( const Point2Df& RightObj );
    Point2Df  operator - ( const Point2Df& RightObj );
    Point2Df  operator + ( const Point2Df& RightObj );

};

Point3Df operator * ( float aA, const Point3Df& aObj );
Point3Df GetNormalVector( Point3Df aP1, Point3Df aP2, Point3Df aP3 );

struct SphericalCoor
{
    float mTeta;
    float mAlpha;

    SphericalCoor( float aNewTeta, float aNewAlpha ) : mTeta( aNewTeta ),
                                                       mAlpha( aNewAlpha )
                                                       {}

    SphericalCoor() : mTeta( 0.0f ),
                      mAlpha( 0.0f )
                      {}
};

#endif
