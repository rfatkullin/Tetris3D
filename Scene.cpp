#include <QMouseEvent>
#include <QKeyEvent>
#include <QDebug>
#include "Scene.h"
#include "Game.h"

Scene :: Scene( Game* const new_game, QWidget* pwgt ) : QGLWidget( pwgt )
{
    game = new_game;
    setMouseTracking( false  );
    for ( int i = 0; i < 4; i++	    )
    {
	ambient_light[ i ] = 0.0f;
	diffuse_light[ i ] = 0.0f;
	speculaer_light[ i ] = 0.0f;
    }
    scene_width  = WindowWidth;
    scene_height = WindowHeight;
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
    //glViewport( 0, 0, WindowWidth / 2, WindowHeight / 2 );
}

void Scene :: resizeGL( int new_width, int new_height )
{
    resize( new_width, new_height );
}

void Scene :: resize( int new_width, int new_height )
{
    scene_width = new_width;
    scene_height = new_height;
}

void Scene :: paintGL()
{
     Point3Df camera_positon;
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glMatrixMode( GL_MODELVIEW );


    camera_positon = game -> GetCameraPosition();

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

    glViewport( 0, 0, scene_width, scene_height );
    glLoadIdentity();
    glScaled( 0.5f, 0.5f, 0.5f );
    gluLookAt( camera_positon.x, camera_positon.y, camera_positon.z, ( Game :: Length / 2  ) * Block :: BlockSize, ( Game :: Height / 2  ) * Block :: BlockSize, ( Game :: Width / 2  ) * Block :: BlockSize, 0, 1, 0 );

    glColor3f( 1.0f, 1.0f, 1.0f );
    renderText( 10, 20, "3D Model" );
    renderText( 10, scene_height - 10, "PAUSE - P" );
    game -> DrawWorld();

    glLightfv( GL_LIGHT0, GL_POSITION, game -> GetLightPosition() );

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
	ambient_light[ i ] = ambient[ i ];
	diffuse_light[ i ] = diffuse[ i ];
	speculaer_light[ i ] = specular[ i ];
    }
    glLightfv( GL_LIGHT0, GL_AMBIENT, ambient_light );
    glLightfv( GL_LIGHT0, GL_DIFFUSE, diffuse_light );
    glLightfv( GL_LIGHT0, GL_SPECULAR, speculaer_light );
}

void Scene :: set3D( bool is_3d)
{
    if ( is_3d )
        exit( 0 );
}
