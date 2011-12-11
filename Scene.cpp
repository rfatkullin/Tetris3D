#include <QMouseEvent>
#include <QKeyEvent>
#include <QDebug>
#include <math.h>
#include <stdlib.h>
#include <iostream>
#include "Scene.h"
#include "Game.h"

const Point3Di Scene :: mCameraShift            = Point3Di ( ( Game :: LENGTH / 2 ) * Block :: BLOCK_SIZE,
                                                             ( Game :: HEIGHT / 2 ) * Block :: BLOCK_SIZE,
                                                             ( Game :: WIDTH / 2  ) * Block :: BLOCK_SIZE );
const float  Scene :: CAMERA_POS_CHANGE_KOEFF    = 0.01f;
const float  Scene :: CAMERA_RADIUS              = 450.0f;
Point2Df     Scene :: mSectorVecs[ Scene :: SectorVecCnt ];

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

    mWidth  = STANDART_SCENE_WIDTH;
    mHeight = STANDART_SCENE_HEIGHT;

    mCameraPosition = SphericalCoor( Geometry :: pi / 4, Geometry :: pi / 8 );
    GetCameraPosition();
    SetViewVectors();

    mRatio              = HEIGHT_RATIO / ( float )WIDTH_RATIO;
    mFrustumAperture    =  45.0f / 180.0f * Geometry :: pi;
    mFrustumNearPlane	=  60;
    mFrustumFarPlane	=  1000;
    mFrustumHalfWidth   =  mFrustumNearPlane * tan( mFrustumAperture );
    mFrustumFocalLength =  (mFrustumFarPlane + mFrustumNearPlane) * 0.8f;
    mFrustumEyeSep      =  mFrustumFocalLength / 30.0f;
    mShowHelp           =  true;
    mIsStereo           = false;

    QGLFormat fmt;
    fmt.setStereo( true );
    setFormat( fmt );

    for ( int i = 0; i < RENDER_MESSAGES_CNT; i++ )
        mRenderMessages[ i ] = Game :: EMPTY;

}

Scene :: ~Scene()
{
    delete mpGame;
}

void Scene :: initializeGL()
{
    glEnable( GL_STEREO );
    glEnable( GL_DOUBLE );
    glEnable( GL_DEPTH_TEST );
    glShadeModel( GL_FLAT );
    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );

    glLightf( GL_LIGHT0, GL_CONSTANT_ATTENUATION,  1.0f );
    glLightf( GL_LIGHT0, GL_LINEAR_ATTENUATION,    0.0f );
    glLightf( GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0f );

}

void Scene :: resizeGL( int aNewWidth, int aNewHeight )
{
}

void Scene :: Resize( int aNewWidth, int aNewHeight )
{
    int factor = 1;

    while ( ( mWidth <= aNewWidth ) && ( mHeight <= aNewHeight ) )
    {
        mWidth  = factor * WIDTH_RATIO * RESOL_FIND_STEP;
        mHeight = factor * HEIGHT_RATIO * RESOL_FIND_STEP;
        ++factor;
    }

    mWidth  -= WIDTH_RATIO * RESOL_FIND_STEP;
    mHeight -= HEIGHT_RATIO * RESOL_FIND_STEP;
}

void Scene :: paintGL()
{
    Point3Df camera_positon;
    Point3Df camera_directon;
    Point3Df right_vec_to_dir;

    camera_positon     = GetCameraPosition();
    camera_directon    = Geometry :: Normalize( -1.0f * camera_positon );
    camera_positon     = camera_positon + mCameraShift;
    right_vec_to_dir   = Geometry :: VectorMul( camera_directon, Point3Df( 0.0f, 1.0f, 0.0f ) );

    glViewport( 0, 0, mWidth, mHeight );

    if ( mIsStereo )
    {
	right_vec_to_dir    = ( mFrustumEyeSep / 2.0f ) * right_vec_to_dir;


        glMatrixMode( GL_PROJECTION );
        glLoadIdentity();

        mFrustumLeft  = -mRatio * mFrustumHalfWidth - 0.5 * mFrustumEyeSep * mFrustumNearPlane / mFrustumFocalLength;
        mFrustumRight =  mRatio * mFrustumHalfWidth - 0.5 * mFrustumEyeSep * mFrustumNearPlane / mFrustumFocalLength;

        glFrustum(  mFrustumLeft, mFrustumRight,
                   -mFrustumHalfWidth * mRatio, mFrustumHalfWidth * mRatio,
                    mFrustumNearPlane, mFrustumFarPlane );

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


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	mFrustumLeft  = -mRatio * mFrustumHalfWidth + 0.5 * mFrustumEyeSep * mFrustumNearPlane / mFrustumFocalLength;
	mFrustumRight =  mRatio * mFrustumHalfWidth + 0.5 * mFrustumEyeSep * mFrustumNearPlane / mFrustumFocalLength;

	glFrustum(  mFrustumLeft, mFrustumRight,
                   -mFrustumHalfWidth * mRatio, mFrustumHalfWidth * mRatio,
                    mFrustumNearPlane, mFrustumFarPlane );

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
        glOrtho( -mWidth / 2, mWidth / 2, -mHeight / 2, mHeight / 2, 0, 1000 );
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
    const std :: vector < Game :: Messages >    messages            = mpGame -> GetMessages();
    int                                         messages_cnt        = messages.size();
    int                                         start_message_index = 0;
    float                                       alpha_can           = 1.0f / RENDER_MESSAGES_CNT;
    QString                                     level_text_buff;
    QString                                     score_text_buff;

   level_text_buff.setNum( mpGame -> GetLevel() );
   score_text_buff.setNum( mpGame -> GetScore() );

    if ( messages_cnt >= RENDER_MESSAGES_CNT )
    {
        start_message_index = messages_cnt - RENDER_MESSAGES_CNT;
        messages_cnt = RENDER_MESSAGES_CNT;
    }
    else
    {
        for ( int i = RENDER_MESSAGES_CNT - messages_cnt - 1; i >= 0; --i )
            mRenderMessages[ i + messages_cnt ] = mRenderMessages[ i  ];
    }

    for ( int i = 0; i < messages_cnt; i++ )
        mRenderMessages[ i ] = messages[ start_message_index + i ];

    glColor3f( 1.0f, 1.0f, 1.0f );
    renderText( mWidth - INFO_X_OFFSET, INFO_Y_OFFSET, ( "Level: " + level_text_buff.toStdString() ).c_str() );
    renderText( mWidth - INFO_X_OFFSET, INFO_Y_OFFSET + 20, (  "Score: " + score_text_buff.toStdString() ).c_str() );

    for ( int i = 0; i < RENDER_MESSAGES_CNT; i++ )
    {
        switch ( mRenderMessages[ i ] )
        {
            case Game :: EMPTY :
                continue;
                break;
            case Game :: COULDNT_ROTATE_COLLISION :
                glColor4f( 0.0f, 1.0f, 1.0f, 1.0f - i * alpha_can );
                break;
            case Game :: COULDNT_SHIFT_COLLISION :
                glColor4f( 0.0f, 1.0f, 1.0f, 1.0f - i * alpha_can );
                break;
            case Game :: COLLAPSE :
                glColor4f( 0.0f, 1.0f, 0.0f, 1.0f - i * alpha_can );
                break;
            case Game :: NEW_GAME :
                glColor4f( 1.0f, 1.0f, 0.0f, 1.0f - i * alpha_can );
                break;
            case Game :: SAVE_GAME :
                glColor4f( 0.0f, 1.0f, 0.0f, 1.0f - i * alpha_can );
                break;
            case Game :: LOAD_GAME :
                glColor4f( 0.0f, 1.0f, 0.0f, 1.0f - i * alpha_can );
                break;
            case Game :: GAME_OVER :
                glColor4f( 1.0f, 0.0f, 0.0f, 1.0f - i * alpha_can );
                break;
             default :
                glColor4f( 1.0f, 1.0f, 1.0f, 1.0f - i * alpha_can );
        }

        if ( mRenderMessages[ i ] != Game :: EMPTY )
            renderText( mWidth - INFO_X_OFFSET, INFO_Y_OFFSET + ( i + 2 ) * 20, Game :: MESSAGES[ mRenderMessages[ i ] ] );
    }

    if ( mShowHelp )
    {
        glColor3f( 1.0f, 1.0f, 1.0f );
        renderText( mWidth - HELP_X_OFFSET, mHeight - HELP_Y_OFFSET,       "Pause                P"  );
        renderText( mWidth - HELP_X_OFFSET, mHeight - HELP_Y_OFFSET + 20,  "This help            F1" );
        renderText( mWidth - HELP_X_OFFSET, mHeight - HELP_Y_OFFSET + 40,  "New game         F2" );
        renderText( mWidth - HELP_X_OFFSET, mHeight - HELP_Y_OFFSET + 60,  "Full screen         F3" );
        renderText( mWidth - HELP_X_OFFSET, mHeight - HELP_Y_OFFSET + 80,  "Ambient music  F4" );
        renderText( mWidth - HELP_X_OFFSET, mHeight - HELP_Y_OFFSET + 100, "Save game        F5" );
        renderText( mWidth - HELP_X_OFFSET, mHeight - HELP_Y_OFFSET + 120, "Load game        F6" );
    }


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

void Scene :: ChangeShowHelp()
{
    mShowHelp = !mShowHelp;
}


