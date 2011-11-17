#include <QMouseEvent>
#include <QKeyEvent>
#include <QDebug>
#include <math.h>
#include "Scene.h"
#include "Game.h"

const Point3Di Scene :: mCameraShift            = Point3Di ( ( Game :: LENGTH / 2 ) * Block :: BLOCK_SIZE,
                                                             ( Game :: HEIGHT / 2 ) * Block :: BLOCK_SIZE,
                                                             ( Game :: WIDTH / 2  ) * Block :: BLOCK_SIZE );
const float Scene :: CAMERA_POS_CHANGE_KOEFF    = 0.01f;
const float Scene :: CAMERA_RADIUS              = 1000.0f;
Point2Df    Scene :: mSectorVecs[ Scene :: SectorVecCnt ];

Scene :: Scene( Game* const new_game, QWidget* pwgt ) : QGLWidget( pwgt )
{
    mpGame = new_game;
    setMouseTracking( false  );
    for ( int i = 0; i < 4; i++	    )
    {
	mAmbientLight[ i ] = 0.0f;
        mDiffuseLight[ i ] = 0.0f;
	mSpecularLight[ i ] = 0.0f;
    }
    mSceneWIdth  = WindowWidth;
    mSceneHeight = WindowHeight;

    mCameraPosition = SphericalCoor( pi / 4, pi / 8 );
    GetCameraPosition();
    SetViewVectors();
}

Scene :: ~Scene()
{
    delete mpGame;
}

void Scene :: initializeGL()
{
    glEnable( GL_DEPTH_TEST );
    glShadeModel( GL_FLAT );
    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );

    glLightf( GL_LIGHT0, GL_CONSTANT_ATTENUATION,  1.0f );
    glLightf( GL_LIGHT0, GL_LINEAR_ATTENUATION,    0.0f );
    glLightf( GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0f );

    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho( -WindowWidth / 2, WindowWidth / 2, -WindowHeight / 2, WindowHeight / 2, 0, 3000);
}

void Scene :: resizeGL( int new_width, int new_height )
{
    Resize( new_width, new_height );
}

void Scene :: Resize( int new_width, int new_height )
{
    mSceneWIdth = new_width;
    mSceneHeight = new_height;
}

void Scene :: paintGL()
{
     Point3Df camera_positon;
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glMatrixMode( GL_MODELVIEW );




//    glViewport( 0, WindowHeight / 2, WindowWidth / 2, WindowHeight / 2 );
//    glLoadIdentity();
//    glScaled( 0.5f, 0.5f, 0.5f );
//    gluLookAt( 300.0f, 0.0f, 0.0f, 0, 0, 0, 0, 1, 0 );
//    glColor3f( 1.0f, 1.0f, 1.0f );
//    renderText( 100, WindowHeight / 2 - 200, "ZY Plane" );
//    game -> DrawWorld();


//    glViewport( WindowWidth / 2, WindowHeight / 2, WindowWidth / 2, WindowHeight / 2 );
//    glLoadIdentity();
//    glScaled( 0.5f, 0.5f, 0.5f );
//    gluLookAt( 0.0f, 300.0f, 0.0f, 0, 0, 0, 1, 0, 0 );
//    glColor3f( 1.0f, 1.0f, 1.0f );
//    renderText( WindowWidth / 2 + 100, WindowHeight / 2 - 200, "ZX Plane" );
//    game -> DrawWorld();

//    glViewport( 0, 0, WindowWidth / 2, WindowHeight / 2 );
//    glLoadIdentity();
//    //glScaled( 0.5f, 0.5f, 0.5f );
//    gluLookAt( 0.0f, 0.0f, 300.0f, 0, 0, 0, 0, 1, 0 );
//    glColor3f( 1.0f, 1.0f, 1.0f );
//    renderText( 100, WindowHeight - 200, "XY Plane" );
//    game -> DrawWorld();

    camera_positon = GetCameraPosition();
    camera_positon = camera_positon + mCameraShift;


    glViewport( 0, 0, mSceneWIdth, mSceneHeight );
    glLoadIdentity();
    glScaled( 0.5f, 0.5f, 0.5f );
    gluLookAt( camera_positon.mX, camera_positon.mY, camera_positon.mZ,
               mCameraShift.mX,   mCameraShift.mY,   mCameraShift.mZ,
               0, 1, 0 );
    glColor3f( 1.0f, 1.0f, 1.0f );
    renderText( 10, 20, "3D Model" );
    renderText( 10, mSceneHeight - 10, "PAUSE - P" );
    mpGame -> DrawWorld();

    glLightfv( GL_LIGHT0, GL_POSITION, mpGame -> GetLightPosition() );

    glFlush();
    swapBuffers();
}

//void Scene :: keyPressEvent( QKeyEvent* keyboard )
//{

//}

//void Scene :: mousePressEvent( QMouseEvent* mouse )
//{

//}

//void Scene :: mouseMoveEvent( QMouseEvent* mouse )
//{

//}

//void Scene :: timerEvent( QTimerEvent * )
//{

//}

void Scene :: SetLigthOption( float ambient[ 4 ], float diffuse[ 4 ], float specular[ 4 ] )
{
    for ( int i = 0; i < 4; i++	    )
    {
	mAmbientLight[ i ] = ambient[ i ];
        mDiffuseLight[ i ] = diffuse[ i ];
	mSpecularLight[ i ] = specular[ i ];
    }
    glLightfv( GL_LIGHT0, GL_AMBIENT, mAmbientLight );
    glLightfv( GL_LIGHT0, GL_DIFFUSE, mDiffuseLight );
    glLightfv( GL_LIGHT0, GL_SPECULAR, mSpecularLight );
}

void Scene :: set3D( bool is_3d)
{
    if ( is_3d )
        exit( 0 );
}

void Scene :: ChangeCameraPosition( float x, float y )
{
    float next_teta       = mCameraPosition.mTeta  + CAMERA_POS_CHANGE_KOEFF * y;
    float next_alpha      = mCameraPosition.mAlpha - CAMERA_POS_CHANGE_KOEFF * x;
    int   next_alpha_sign = Geometry :: Sign( next_alpha );

    if ( Geometry :: InRange( next_teta, -pi / 2, pi / 2 ) )
        mCameraPosition.mTeta  = next_teta;

    if ( !Geometry :: InRange( next_alpha_sign * next_alpha, 0.0, 2 * pi ) )
        next_alpha -= next_alpha_sign * 2 * pi;
    mCameraPosition.mAlpha = next_alpha;
}

Point3Df Scene :: GetCameraPosition() const
{
    float cos_teta = cos( mCameraPosition.mTeta );
    return Point3Df( cos_teta * sin( mCameraPosition.mAlpha ) * CAMERA_RADIUS,
                     sin( mCameraPosition.mTeta ) * CAMERA_RADIUS,
                     cos_teta * cos( mCameraPosition.mAlpha ) * CAMERA_RADIUS );
}

void Scene :: SetViewVectors()
{    
    mSectorVecs[ 3 ] = Point2Df( -1.0f, -1.0f );
    mSectorVecs[ 0 ] = Point2Df( -1.0f,  1.0f );
    mSectorVecs[ 1 ] = Point2Df(  1.0f,  1.0f );
    mSectorVecs[ 2 ] = Point2Df(  1.0f, -1.0f );

}

int Scene :: GetViewSide()
{
    Point2Df        current_pos  =  Point2Df( sin( mCameraPosition.mAlpha ), cos( mCameraPosition.mAlpha ) );
    unsigned int    next_vec_ind = 0;
    int             sign1        = 0;
    int             sign2        = 0;

    for ( unsigned int i = 0; i < SectorVecCnt; i++ )
    {
        next_vec_ind = ( i + 1 ) % SectorVecCnt;

        sign1 = Geometry :: Sign( Geometry :: ParalSquare( current_pos, mSectorVecs[ i ] ) );
        sign2 = Geometry :: Sign( Geometry :: ParalSquare( current_pos, mSectorVecs[ next_vec_ind ] ) );

        if ( ( sign1 != sign2 ) && ( sign1 > 0 ) )
                return i;
    }

    return 0;
}
