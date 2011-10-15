#include <QMouseEvent>
#include <QKeyEvent>
#include "Scene.h"
#include "Geom.h"
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
    glViewport( 0, 0, WindowWidth, WindowHeight );
}

void Scene :: resizeGL(int new_width, int new_height)
{
     glViewport( 0, 0, new_width, new_height );
}

void Scene :: paintGL()
{
    Point3Df camera_positon;
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    camera_positon = game -> GetCameraPosition();
//  gluLookAt( camera_positon.x, camera_positon.y, camera_positon.z, 0, 0, 0, 0, 1, 0 );
//  glLightfv( GL_LIGHT0, GL_POSITION, game -> GetLightPosition() );

    gluLookAt( camera_positon.x, camera_positon.y, camera_positon.z, 0, 0, 0, 0, 1, 0 );
    glViewport( 0, 0, WindowWidth, WindowHeight );
    game -> DrawWorld();

    glLightfv( GL_LIGHT0, GL_POSITION, game -> GetLightPosition() );
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
	    default:
		break;
    }
}

void Scene :: mousePressEvent( QMouseEvent* mouse )
{
    if ( mouse -> button() ==Qt :: LeftButton )
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


