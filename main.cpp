//#include <stdlib.h>
//#include <QGLWidget>
//#include <math.h>
//#include <string.h>
//#include "Figures.h"
//#include "Game.h"



//float light_position[ 4 ] = { 0.0f, 300.0f, 300.0f, 1 };
//float ambient_light[ 4 ] = { 0.3f, 0.3f, 0.3f, 1.0f };
//float diffuse_light[ 4 ] = { 0.7f, 0.7f, 0.7f, 1.0f };
//float speculaer_light[ 4 ] = { 0.5f, 0.5f, 0.5f, 1.0f };

//unsigned int count_of_draws = 0;

//Point3D last_mouse_position;
//Game game;

//void CameraMotion( int x, int y )
//{
//	game.ChangeCameraPosition( x - last_mouse_position.x, y - last_mouse_position.y );
//	last_mouse_position.x = x;
//	last_mouse_position.y = y;
	
//}

//void MouseClick( int button, int state, int x, int y )
//{
//	if ( state == GLUT_DOWN  )
//		last_mouse_position = Point3D( x, y, 0.0f );
	
//}

//void SpecKeyBoard( int ch, int x, int y )
//{
//	if ( ch == GLUT_KEY_RIGHT )
//		game.ShiftFigureByXAxis( SHIFT_RIGHT );
//	if ( ch == GLUT_KEY_LEFT )
//		game.ShiftFigureByXAxis( SHIFT_LEFT );
//	if ( ch == GLUT_KEY_UP )
//		game.ShiftFigureByZAxis( SHIFT_BACK );
//	if ( ch == GLUT_KEY_DOWN )
//		game.ShiftFigureByZAxis( SHIFT_AHEAD );
//}

//void KeyBoard( unsigned char ch, int x, int y )
//{
//	switch ( ch )
//	{
//		case 27 :
//			exit( 0 );
//			break;
//		case 32 :
//			game.DropDownFigure();
//			break;
//		case 'd' :
//			game.Rotate( PLANE_XY, ROTATE_BY_CLOCK_WISE );
//			break;
//		case 'a' :
//			game.Rotate( PLANE_XY, ROTATE_BY_ANTI_CLOCK_WISE );
//			break;
//		case 'w' :
//			game.Rotate( PLANE_ZY, ROTATE_BY_CLOCK_WISE );
//			break;
//		case 's' :
//			game.Rotate( PLANE_ZY, ROTATE_BY_ANTI_CLOCK_WISE );
//			break;
//		case 'q' :
//			game.Rotate( PLANE_ZX, ROTATE_BY_CLOCK_WISE );
//			break;
//		case 'e' :
//			game.Rotate( PLANE_ZX, ROTATE_BY_ANTI_CLOCK_WISE );
//			break;
//		default:
//			break;
//	}
//}

//void CoordianatesDraw()
//{
//	//Рисуем Ось X
//	glColor3f( 0.0f, 0.0f, 1.0f );
//	glBegin( GL_LINES );
//		glVertex3f( 0.0f, 0.0f, 0.0f );
//		glVertex3f( 200.0f, 0.0f, 0.0f );
//	glEnd();

//	//Рисуем Ось Y
//	glColor3f( 1.0f, 0.0f, 0.0f );
//	glBegin( GL_LINES );
//		glVertex3f( 0.0f, 0.0f, 0.0f );
//		glVertex3f( 0.0f, 200.0f, 0.0f );
//	glEnd();

//	//Рисуем Ось Z
//	glColor3f( 0.0f, 1.0f, 0.0f );
//	glBegin( GL_LINES );
//		glVertex3f( 0.0f, 0.0f, 0.0f );
//		glVertex3f( 0.0f, 0.0f, 200.0f );
//	glEnd();
//}



//void RenderScene()
//{
//	Point3D camera_positon;
//	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
//	glMatrixMode( GL_MODELVIEW );
//	glLoadIdentity();
//	camera_positon = game.GetCameraPosition();
//	gluLookAt( camera_positon.x, camera_positon.y, camera_positon.z, 0, 0, 0, 0, 1, 0 );
//	glLightfv( GL_LIGHT0, GL_POSITION, light_position );
//	glColor3f( 1.0f, 1.0f, 1.0f );
//	glPointSize( 4.0f );
//	glBegin( GL_POINTS );
//		glVertex3fv( light_position );
//	glEnd();
//	game.DrawWorld();
//	//CoordianatesDraw();

//	glutSwapBuffers();
//}

//void MyInitOpenGL()
//{
//	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
//	glOrtho( -WINDOW_WIDTH / 2, WINDOW_WIDTH / 2, -WINDOW_HEIGHT / 2, WINDOW_HEIGHT / 2, 0, 3000);
//	glViewport( 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT );
//}

//void LightInit()
//{
//	glEnable( GL_LIGHTING );
//	glEnable( GL_LIGHT0 );
//	glLightfv( GL_LIGHT0, GL_AMBIENT, ambient_light );
//	glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse_light );
//	glLightfv( GL_LIGHT0, GL_SPECULAR, speculaer_light );
//	glLightf( GL_LIGHT0, GL_CONSTANT_ATTENUATION, 1.0f );
//	glLightf( GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.0f );
//	glLightf( GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 0.0f );
//}

//char number[ 100 ];
//void NextStep( int TimerId )
//{
//	count_of_draws++;
//	itoa( count_of_draws, number, 10 );
//	glutSetWindowTitle( number );
//	game.NextStep();
//	glutTimerFunc( 10, NextStep, 0 );
//	glutPostRedisplay();
//}

//int main(int argc, char **argv)
//{
//	glutInit( &argc, argv );
//	glutInitDisplayMode( GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
//	glutInitWindowSize( WINDOW_WIDTH, WINDOW_HEIGHT );
//	glutInitWindowPosition(200, 100);
//	glutCreateWindow("Simple");
//	glutDisplayFunc(RenderScene);
	
////	glutMouseFunc(Mouse);
//	glutSpecialFunc( SpecKeyBoard );
//	glutKeyboardFunc( KeyBoard );
//	glutTimerFunc( 10, NextStep, 0 );
//	glutMotionFunc( CameraMotion );
//	glutMouseFunc( MouseClick );
//	MyInitOpenGL();
	
//	//Параметры glEnable/glDisable

//	glEnable( GL_DEPTH_TEST );
	
//	//glLightModelfv( GL_LIGHT_MODEL_AMBIENT, ambient_light );
//	LightInit();
//	glEnable( GL_LINE_SMOOTH );
//	glutMainLoop();
//	return 0;
//}
#include <QApplication>
#include "Scene.h"

int main( int argc, char **argv )
{
    float ambient_light[ 4 ] = { 0.3f, 0.3f, 0.3f, 1.0f };
    float diffuse_light[ 4 ] = { 0.7f, 0.7f, 0.7f, 1.0f };
    float speculaer_light[ 4 ] = { 0.5f, 0.5f, 0.5f, 1.0f };

    QApplication app( argc, argv );

    Scene scene;
    scene.SetLigthOption( ambient_light, diffuse_light, speculaer_light );
    scene.startTimer( 10 );
    scene.show();

    app.exec();
}
