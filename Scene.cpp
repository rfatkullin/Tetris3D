#include <QMouseEvent>
#include <QKeyEvent>
#include <QDebug>
#include <math.h>
#include <stdlib.h>
#include "Scene.h"
#include "Game.h"

const Point3Di Scene :: mCameraShift            = Point3Di ( ( Game :: LENGTH / 2 ) * Block :: BLOCK_SIZE,
                                                             ( Game :: HEIGHT / 2 ) * Block :: BLOCK_SIZE,
                                                             ( Game :: WIDTH / 2  ) * Block :: BLOCK_SIZE );
const float Scene :: CAMERA_POS_CHANGE_KOEFF    = 0.01f;
const float Scene :: CAMERA_RADIUS              = 450.0f;
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
    mSceneWidth  = SCENE_WIDTH;
    mSceneHeight = SCENE_HEIGHT;


    mCameraPosition = SphericalCoor( Geometry :: pi / 4, Geometry :: pi / 8 );
    GetCameraPosition();
    SetViewVectors();
    Resize( SCENE_WIDTH, SCENE_HEIGHT );

    mRatio = mSceneWidth / ( float )mSceneHeight;
    mFrustumAperture    =  45.0f / 180.0f * Geometry :: pi;
    mFrustumNearPlane	=  60;
    mFrustumFarPlane	=  800;
    mFrustumHalfWidth   =  mFrustumNearPlane * tan( mFrustumAperture );
    mFrustumFocalLength =  mFrustumNearPlane / 2;
    mFrustumEyeSep      =  mFrustumFocalLength / 30.0f;
    mIsOneSide		=  false;

//    QGLFormat fmt;
//    fmt.setStereo( true );
//    setFormat( fmt );
    mIsStereo	 = false;

}

Scene :: ~Scene()
{
    delete mpGame;
}

void Scene :: initializeGL()
{
    glEnable( GL_STEREO );
    glEnable( GL_DOUBLE );
    glEnable( GL_SMOOTH );
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
    glOrtho( -SCENE_WIDTH / 2, SCENE_WIDTH / 2, -SCENE_HEIGHT / 2, SCENE_HEIGHT / 2, 0, 3000);

}

void Scene :: resizeGL( int new_width, int new_height )
{
    //Resize( new_width, new_height );
}

void Scene :: Resize( int new_width, int new_height )
{
    //mSceneWIdth = new_width;
    //mSceneHeight = new_height;
}

//void Scene :: paintGL()
//{
//    glViewport( 0, 0, mSceneWIdth, mSceneHeight );

//    glMatrixMode( GL_MODELVIEW );
//    glDrawBuffer( GL_BACK_RIGHT );
//    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
//    glLoadIdentity();
//    gluLookAt( 0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f );
//    glColor3f( 1.0f, 0.0f, 0.0f );
//    glBegin( GL_QUADS );
//	glVertex3f( -100.0f, -100.0f, 0.0f );
//	glVertex3f( 100.0f, -100.0f, 0.0f );
//	glVertex3f( 100.0f, 100.0f, 0.0f );
//	glVertex3f( -100.0f, 100.0f, 0.0f );
//    glEnd();

//    glMatrixMode( GL_MODELVIEW );
//    glDrawBuffer( GL_BACK_LEFT );
//    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
//    glLoadIdentity();
//    gluLookAt( 0.0f, 0.0f, 10.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f );
//    glColor3f( 0.0f, 0.0f, 1.0f );
//    glBegin( GL_QUADS );
//	glVertex3f( -100.0f, -100.0f, 0.0f );
//	glVertex3f( 100.0f, -100.0f, 0.0f );
//	glVertex3f( 100.0f, 100.0f, 0.0f );
//	glVertex3f( -100.0f, 100.0f, 0.0f );
//    glEnd();

//    swapBuffers();
//}

void Scene :: paintGL()
{
    Point3Df camera_positon;
    Point3Df camera_directon;
    Point3Df right_vec_to_dir;

    camera_positon     = GetCameraPosition();
    camera_directon    = Geometry :: Normalize( -1.0f * camera_positon );
    camera_positon     = camera_positon + mCameraShift;
    right_vec_to_dir   = Geometry :: VectorMul( camera_directon, Point3Df( 0.0f, 1.0f, 0.0f ) );

    glViewport( 0, 0, mSceneWidth, mSceneHeight );

    if ( mIsStereo )
    {
	right_vec_to_dir    = ( mFrustumEyeSep / 2.0f ) * right_vec_to_dir;

	if ( !mIsOneSide )
	{
	    glMatrixMode( GL_PROJECTION );
	    glLoadIdentity();

	    mFrustumLeft  = -mRatio * mFrustumHalfWidth - 0.5 * mFrustumEyeSep * mFrustumNearPlane / mFrustumFocalLength;
	    mFrustumRight =  mRatio * mFrustumHalfWidth - 0.5 * mFrustumEyeSep * mFrustumNearPlane / mFrustumFocalLength;

	    glFrustum( mFrustumLeft, mFrustumRight, -mFrustumHalfWidth, mFrustumHalfWidth, mFrustumNearPlane, mFrustumFarPlane );

	    glMatrixMode( GL_MODELVIEW );
	    glDrawBuffer( GL_BACK_RIGHT );
	    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	    glLoadIdentity();
	    gluLookAt( camera_positon.mX + right_vec_to_dir.mX,
		       camera_positon.mY + right_vec_to_dir.mY,
		       camera_positon.mZ + right_vec_to_dir.mZ,
		       camera_positon.mX + right_vec_to_dir.mX + camera_directon.mX,
		       camera_positon.mY + right_vec_to_dir.mY + camera_directon.mY,
		       camera_positon.mZ + right_vec_to_dir.mZ + camera_directon.mZ,
		       0, 1, 0 );
	    mpGame -> DrawWorld();
	    glLightfv( GL_LIGHT0, GL_POSITION, mpGame -> GetLightPosition() );
	}

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	mFrustumLeft  = -mRatio * mFrustumHalfWidth + 0.5 * mFrustumEyeSep * mFrustumNearPlane / mFrustumFocalLength;
	mFrustumRight =  mRatio * mFrustumHalfWidth + 0.5 * mFrustumEyeSep * mFrustumNearPlane / mFrustumFocalLength;

	glFrustum( mFrustumLeft, mFrustumRight, -mFrustumHalfWidth, mFrustumHalfWidth, mFrustumNearPlane, mFrustumFarPlane );

	glMatrixMode( GL_MODELVIEW );
	glDrawBuffer( GL_BACK_LEFT );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();
	gluLookAt( camera_positon.mX - right_vec_to_dir.mX,
		   camera_positon.mY - right_vec_to_dir.mY,
		   camera_positon.mZ - right_vec_to_dir.mZ,
		   camera_positon.mX - right_vec_to_dir.mX + camera_directon.mX,
		   camera_positon.mY - right_vec_to_dir.mY + camera_directon.mY,
		   camera_positon.mZ - right_vec_to_dir.mZ + camera_directon.mZ,
		   0, 1, 0 );
	mpGame -> DrawWorld();
	glLightfv( GL_LIGHT0, GL_POSITION, mpGame -> GetLightPosition() );
    }
    else
    {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho( -SCENE_WIDTH / 2, SCENE_WIDTH / 2, -SCENE_HEIGHT / 2, SCENE_HEIGHT / 2, 0, 1000 );
	glMatrixMode( GL_MODELVIEW );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();
        gluLookAt( camera_positon.mX, camera_positon.mY, camera_positon.mZ,
		   mCameraShift.mX,   mCameraShift.mY,   mCameraShift.mZ,
		   0, 1, 0 );
	mpGame -> DrawWorld();
	glLightfv( GL_LIGHT0, GL_POSITION, mpGame -> GetLightPosition() );
    }

    DrawTextInformation();

    swapBuffers();
}

void Scene :: DrawTextInformation()
{
    unsigned int    text_buffer_len = 0;
    unsigned int    game_level	    = mpGame -> GetLevel();
    char*	    text_buffer	    = new char[ 100 ];

    strcpy( text_buffer, "Level: " );
    text_buffer_len = strlen( text_buffer );
    text_buffer[ text_buffer_len ] = ( char )( game_level + ( unsigned int )'0' );
    text_buffer[ text_buffer_len + 1 ] = 0;

    glColor3f( 1.0f, 1.0f, 1.0f );
    //renderText( 10, 20, "3D Model" );
    //renderText( 10, mSceneHeight - 10, "PAUSE - P" );

    renderText( SCENE_WIDTH - 200, 10, text_buffer );

    delete [] text_buffer;
}

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
	mIsStereo = true;
    else
	mIsStereo = false;
}

void Scene :: ChangeCameraPosition( float x, float y )
{
    float next_teta       = mCameraPosition.mTeta  + CAMERA_POS_CHANGE_KOEFF * y;
    float next_alpha      = mCameraPosition.mAlpha - CAMERA_POS_CHANGE_KOEFF * x;
    int   next_alpha_sign = Geometry :: Sign( next_alpha );

    if ( Geometry :: InRange( next_teta, -Geometry :: pi / 2, Geometry :: pi / 2 ) )
        mCameraPosition.mTeta  = next_teta;

    if ( !Geometry :: InRange( next_alpha_sign * next_alpha, 0.0, 2 * Geometry :: pi ) )
        next_alpha -= next_alpha_sign * 2 * Geometry :: pi;
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
