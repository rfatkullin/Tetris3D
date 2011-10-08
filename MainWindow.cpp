#include "MainWindow.h"
#include "Geom.h"
#include "Game.h"

MainWindow :: MainWindow( QWidget* pwgt ) : QGLWidget( pwgt )
{
    game = new Game();
    setMouseTracking( true );
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
    Point3D camera_positon;
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();
    camera_positon = game -> GetCameraPosition();
    gluLookAt( camera_positon.x, camera_positon.y, camera_positon.z, 0, 0, 0, 0, 1, 0 );
    glLightfv( GL_LIGHT0, GL_POSITION, light_position );
    glColor3f( 1.0f, 1.0f, 1.0f );
    glPointSize( 4.0f );
    glBegin( GL_POINTS );
	    glVertex3fv( light_position );
    glEnd();
    game.DrawWorld();
    //CoordianatesDraw();

    glutSwapBuffers();
}

void MainWindow :: paintGL()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glBegin( GL_QUADS );
	glColor3f( 1, 0, 0 );
	glVertex2f( 0, 100 );

	glColor3f( 0, 1, 0 );
	glVertex2f( 100, 100 );

	glColor3f( 0, 0, 1 );
	glVertex2f( 100, 0 );

	glColor3f( 1, 1, 1 );
	glVertex2f( 0, 0 );
    glEnd();
}

void MainWindow :: keyPressEvent( QKeyEvent* keyboard )
{
    switch ( keyboard -> key )
    {
	    case Qt :: Key_Escape :
		    exit( 0 );
		    break;
	    case Qt :: Key_Space :
		    game.DropDownFigure();
		    break;
	    case Qt :: Key_D :
		    game.Rotate( PLANE_XY, ROTATE_BY_CLOCK_WISE );
		    break;
	    case Qt :: Key_A :
		    game.Rotate( PLANE_XY, ROTATE_BY_ANTI_CLOCK_WISE );
		    break;
	    case Qt :: Key_W :
		    game.Rotate( PLANE_ZY, ROTATE_BY_CLOCK_WISE );
		    break;
	    case Qt :: Key_S :
		    game.Rotate( PLANE_ZY, ROTATE_BY_ANTI_CLOCK_WISE );
		    break;
	    case Qt :: Key_Q :
		    game.Rotate( PLANE_ZX, ROTATE_BY_CLOCK_WISE );
		    break;
	    case Qt :: Key_E :
		    game.Rotate( PLANE_ZX, ROTATE_BY_ANTI_CLOCK_WISE );
		    break;
	    default:
		    break;
    }
}

void MainWindow :: mousePressEvent( QMouseEvent* mouse )
{
    if ( mouse ->   )
	    last_mouse_position = Point3D( x, y, 0.0f );
}

void mouseMoveEvent( QMouseEvent* mouse );

