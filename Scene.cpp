#include <QMouseEvent>
#include <QKeyEvent>
#include <QDebug>
#include "Scene.h"
#include "Game.h"

Scene :: Scene( QWidget* pwgt ) : QGLWidget( pwgt )
{
    game = new Game();
    setMouseTracking( false  );
    for ( int i = 0; i < 4; i++	    )
    {
	ambient_light[ i ] = 0.0f;
	diffuse_light[ i ] = 0.0f;
	speculaer_light[ i ] = 0.0f;
    }
}

Scene :: ~Scene()
{
    delete game;
}

void Scene :: initializeGL()
{
    glEnable( GL_DEPTH_TEST );
    glShadeModel( GL_FLAT );
    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );

    glLightf( GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f );
    glLightf( GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0f );
    glLightf( GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0f );

    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho( -WindowWidth / 2, WindowWidth / 2, -WindowHeight / 2, WindowHeight / 2, 0, 3000);
    glViewport( 0, 0, WindowWidth / 2, WindowHeight / 2 );
}

void Scene :: resizeGL(int new_width, int new_height)
{
    //glMatrixMode( GL_MODELVIEW );
    //glViewport( 0, 0, new_width, new_height );
    //paintGL();

}

void Scene :: paintGL()
{
     Point3Df camera_positon;
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glMatrixMode( GL_MODELVIEW );


    camera_positon = game -> GetCameraPosition();
/*
    glViewport( 0, WindowHeight / 2, WindowWidth / 2, WindowHeight / 2 );
    glLoadIdentity();
    gluLookAt( 300.0f, 0.0f, 0.0f, 0, 0, 0, 0, 1, 0 );
    glColor3f( 1.0f, 1.0f, 1.0f );
    renderText( 100, WindowHeight / 2 - 200, "ZY Plane" );   
    game -> DrawWorld();


    glViewport( WindowWidth / 2, WindowHeight / 2, WindowWidth / 2, WindowHeight / 2 );
    glLoadIdentity();
    gluLookAt( 0.0f, 300.0f, 0.0f, 0, 0, 0, 1, 0, 0 );
    glColor3f( 1.0f, 1.0f, 1.0f );
    renderText( WindowWidth / 2 + 100, WindowHeight / 2 - 200, "ZX Plane" );
    game -> DrawWorld();

    glViewport( 0, 0, WindowWidth / 2, WindowHeight / 2 );
    glLoadIdentity();
    gluLookAt( 0.0f, 0.0f, 300.0f, 0, 0, 0, 0, 1, 0 );
    glColor3f( 1.0f, 1.0f, 1.0f );
    renderText( 100, WindowHeight - 200, "XY Plane" );
    game -> DrawWorld();
*/
    glViewport( 0, 0, WindowWidth , WindowHeight );
    glLoadIdentity();
    glScaled( 0.5f, 0.5f, 0.5f );
    gluLookAt( camera_positon.x, camera_positon.y, camera_positon.z, ( Game :: Length / 2  ) * Block :: BlockSize, 0, ( Game :: Width / 2  ) * Block :: BlockSize, 0, 1, 0 );

    glColor3f( 1.0f, 1.0f, 1.0f );
    renderText( WindowWidth / 2 + 100, WindowHeight - 200, "3D Model" );
    game -> DrawWorld();

    glLightfv( GL_LIGHT0, GL_POSITION, game -> GetLightPosition() );

    glFlush();
    swapBuffers();
}

void Scene :: keyPressEvent( QKeyEvent* keyboard )
{
    switch ( keyboard -> key() )
    {
	case Qt :: Key_Escape :
	    exit( 0 );
	    break;
	case Qt :: Key_Space :
	    game -> DropDownFigure();
	    break;
       case Qt :: Key_Right :
	    game -> ShiftFigureByXAxis( Game :: ShiftRight );
	    break;
	case Qt :: Key_Left :
	    game -> ShiftFigureByXAxis( Game :: ShiftLeft );
	    break;
	case Qt :: Key_Up :
	    game -> ShiftFigureByZAxis( Game :: ShiftBack );
	    break;
	case Qt :: Key_Down :
	    game -> ShiftFigureByZAxis( Game :: ShiftAhead );
	    break;
	case Qt :: Key_D :
	    game -> Rotate( Game :: PlaneXY, Game :: RotateByClockWise );
	    break;
	case Qt :: Key_A :
	    game -> Rotate( Game :: PlaneXY, Game :: RotateByAntiClockWise );
	    break;
	case Qt :: Key_W :
	    game -> Rotate( Game :: PlaneZY, Game :: RotateByClockWise );
	    break;
	case Qt :: Key_S :
	    game -> Rotate( Game :: PlaneZY, Game :: RotateByAntiClockWise );
	    break;
	case Qt :: Key_Q :
	    game -> Rotate( Game :: PlaneZX, Game :: RotateByClockWise );
	    break;
	case Qt :: Key_E :
	    game -> Rotate( Game :: PlaneZX, Game :: RotateByAntiClockWise );
	    break;
        case Qt :: Key_P :
            game -> SetGameSpeed( Game :: ZeroSpeed );
            break;
	default:
	    break;
    }
}

void Scene :: mousePressEvent( QMouseEvent* mouse )
{
    if ( mouse -> button() == Qt :: LeftButton )
	game -> SetLastMousePosition( mouse -> globalX(), mouse -> globalY() );
}

void Scene :: mouseMoveEvent( QMouseEvent* mouse )
{
    Point3Df last_mouse_position = game -> GetLastMousePosition();
    game -> ChangeCameraPosition( mouse -> globalX() - last_mouse_position.x, mouse -> globalY() - last_mouse_position.y );
    game -> SetLastMousePosition( mouse -> globalX(), mouse -> globalY() );
}

void Scene :: timerEvent( QTimerEvent * )
{
    game -> NextStep();
    paintGL();
}

void Scene :: SetLigthOption( float ambient[ 4 ], float diffuse[ 4 ], float specular[ 4 ] )
{
    for ( int i = 0; i < 4; i++	    )
    {
	ambient_light[ i ] = ambient[ i ];
	diffuse_light[ i ] = diffuse[ i ];
	speculaer_light[ i ] = specular[ i ];
    }
    glLightfv( GL_LIGHT0, GL_AMBIENT, ambient_light );
    glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse_light );
    glLightfv( GL_LIGHT0, GL_SPECULAR, speculaer_light );
}


