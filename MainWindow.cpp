#include <QMouseEvent>
#include <QKeyEvent>
#include "MainWindow.h"
#include "Geom.h"
#include "Game.h"

MainWindow :: MainWindow( QWidget* pwgt ) : QGLWidget( pwgt )
{
    game = new Game();
    setMouseTracking( false  );
}

MainWindow :: ~MainWindow()
{
    delete game;
}

void MainWindow :: initializeGL()
{
    glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho( -WindowWidth / 2, WindowWidth / 2, -WindowHeight / 2, WindowHeight / 2, 0, 3000);
    glViewport( 0, 0, WindowWidth, WindowHeight );
}

void MainWindow :: resizeGL(int new_width, int new_height)
{
     glViewport( 0, 0, new_width, new_height );
}

void MainWindow :: paintGL()
{
    Point3D camera_positon;
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    camera_positon = game -> GetCameraPosition();
    gluLookAt( camera_positon.x, camera_positon.y, camera_positon.z, 0, 0, 0, 0, 1, 0 );
    glLightfv( GL_LIGHT0, GL_POSITION, game -> GetLightPosition() );
    glColor3f( 1.0f, 1.0f, 1.0f );
    glPointSize( 4.0f );
    glBegin( GL_POINTS );
    glVertex3fv( game -> GetLightPosition() );
    glEnd();
    game -> DrawWorld();
    //CoordianatesDraw();

    swapBuffers();
}

void MainWindow :: keyPressEvent( QKeyEvent* keyboard )
{
    switch ( keyboard -> key() )
    {
	    case Qt :: Key_Escape :
		    exit( 0 );
		    break;
	    case Qt :: Key_Space :
		    game -> DropDownFigure();
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

void MainWindow :: mousePressEvent( QMouseEvent* mouse )
{
    if ( mouse -> button() ==Qt :: LeftButton )
	game -> SetLastMousePosition( mouse -> globalX(), mouse -> globalY() );
}

void MainWindow :: mouseMoveEvent( QMouseEvent* mouse )
{
    Point3D last_mouse_position = game -> GetLastMousePosition();
    game -> ChangeCameraPosition( mouse -> globalX() - last_mouse_position.x, mouse -> globalY() - last_mouse_position.y );
    game -> SetLastMousePosition( mouse -> globalX(), mouse -> globalY() );
}

void Game :: timerEvent( QTimerEvent * )
{

}

