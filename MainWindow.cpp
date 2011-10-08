#include "MainWindow.h"

MainWindow :: MainWindow( QWidget* pwgt ) : QGLWidget( pwgt )
{

}

void MainWindow :: initializeGL()
{
    qglClearColor( Qt :: black );
}

void MainWindow :: resizeGL(int new_width, int new_height)
{
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();
    glViewport( 0, 0, (GLint)new_width, (GLint)new_height );
    glOrtho( 0, 100, 100, 0, -1, 1 );
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


