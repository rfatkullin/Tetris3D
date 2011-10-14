#include <QGLWidget>
#include <math.h>
#include <stdio.h>
#include "Game.h"
#include "Figures.h"

const float	Game :: CameraPosChangeKoeff =	0.01f;
const float	Game :: CameraRadius = 300.0f;
const int	Game :: FieldPositionByY = -200;
float		Game :: light_position[ 4 ];

void Game :: InitializeStaticData()
{
    light_position[ 0 ] = LightPosByX;
    light_position[ 1 ] = LightPosByY;
    light_position[ 2 ] = LightPosByZ;
    light_position[ 3 ] = 1.0f;
}

Game :: Game()
{
    InitializeStaticData();
    Start();
}

void Game :: Start()
{	
    srand( time( 0 ) );
    last_mouse_position =   Point3Df( 0.0f, 0.0f, 0.0f );
    camera_position =	    SphericalCoor( pi / 4, pi / 4 );
    current_figure =	    new Figure( 0.0f, 300.0f, -( Block :: BlockSize / 2 ), LFigure, materials[ rand() % 1/*MaterialsCount*/ ] );
    rotating_step =	    0;
    game_speed =	    FirstSpeed;
    rotating =		    false;
    score =		    0;

    for ( int i = 0; i < FieldWidth; ++i )
	for ( int j = 0; j < FieldLength; ++j )
	    for ( int k = 0; k < FieldHeight; ++k )
		field[ i ][ j ][ k ] = NULL;

    for ( int i = 0; i < FieldWidth; ++i )
	for ( int j = 0; j < FieldLength; ++j )
	    field[ i ][ j ][ 0 ] =  new Block( i * Block :: BlockSize / 2, FieldPositionByY, j * Block :: BlockSize / 2, materials[ rand() % MaterialsCount ] );
}

void Game :: MoveDownFigure()
{
    Point3Df figure_position = current_figure -> GetPosition();
    current_figure -> SetPostion( Point3Df( figure_position.x, figure_position.y - game_speed * 0.5f,  figure_position.z ));

    for (  )
}

void Game :: NextStep()
{
    float	final_angle = 0.0f;
    bool	state = false;

    MoveDiwnFigure();
    //Move down the figure
    /*
    Point3Df figure_position = current_figure -> GetPosition();
    if ( figure_position.y - game_speed * 0.5f - Block :: BlockSize / 2 - Block :: BlockSize >= FieldPositionByY )
	    current_figure -> SetPostion( Point3Df( figure_position.x, figure_position.y - game_speed * 0.5f,  figure_position.z ));
    */
    //Rotate the figure
    if ( rotating )
    {
	rotating_step++;
	final_angle = rotating_step * rotating_angle;
	if ( rotating_step == RotateStepsCount )
	{
		rotating_step = 0;
		rotating = false;
		state = true;
	}
	switch ( rotating_plane )
	{
	case PlaneXY :
	    check_figure -> RotateOnXY( final_angle, state );
	    break;
	case PlaneZY :
	    check_figure -> RotateOnZY( final_angle, state );
	    break;
	default :
	    check_figure -> RotateOnZX( final_angle, state );
	}
    }
}

void Game :: DrawField()
{
    glDisable( GL_LIGHTING );
    glColor3f( 1.0f, 1.0f, 1.0f );

    glBegin( GL_LINES );
	for ( int i = 0; i <= FieldLength; ++i )
	{
	    glVertex3f( ( -FieldWidth / 2 + i ) * Block :: BlockSize, FieldPositionByY, ( -FieldLength / 2 ) * Block :: BlockSize );
	    glVertex3f( ( -FieldWidth / 2 + i ) * Block :: BlockSize, FieldPositionByY, (  FieldLength / 2 ) * Block :: BlockSize );
	}
	for ( int i = 0; i <= FieldWidth; ++i )
	{
	    glVertex3f( (  FieldWidth / 2 ) * Block :: BlockSize, FieldPositionByY, ( -FieldLength / 2 + i ) * Block :: BlockSize );
	    glVertex3f( ( -FieldWidth / 2 ) * Block :: BlockSize, FieldPositionByY, ( -FieldLength / 2 + i ) * Block :: BlockSize );
	}
     glEnd();
     glEnable( GL_LIGHTING );
}

void Game :: DrawInterface()
{

}

void Game :: DrawWorld()
{
    DrawField();
    current_figure -> Draw();
}

float* Game :: GetLightPosition()
{
    return light_position;    
}

void Game :: ShiftFigureByXAxis( ShiftDirection shift_koeff )
{
    Point3Df	position = current_figure -> GetPosition();
    float	max_x_pos = current_figure -> MaxByX();
    float	min_x_pos = current_figure -> MinByX();
    float	shift = shift_koeff * Block :: BlockSize;

    if ( ( Game :: FieldMinBound <= min_x_pos + shift  ) && ( min_x_pos + shift <= Game :: FieldMaxBound ) &&
	 ( Game :: FieldMinBound <= max_x_pos + shift  ) && ( max_x_pos + shift <= Game :: FieldMaxBound )
       )
	position.x += shift;

    current_figure -> SetPostion( position );
}

void Game :: ShiftFigureByZAxis( ShiftDirection shift_koeff )
{
    Point3Df	position = current_figure -> GetPosition();
    float	max_z_pos = current_figure -> MaxByZ();
    float	min_z_pos = current_figure -> MinByZ();
    float	shift = shift_koeff * Block :: BlockSize;

    if ( ( Game :: FieldMinBound <= min_z_pos + shift  ) && ( min_z_pos + shift <= Game :: FieldMaxBound ) &&
	 ( Game :: FieldMinBound <= max_z_pos + shift  ) && ( max_z_pos + shift <= Game :: FieldMaxBound )
       )
	position.z += shift;

    current_figure -> SetPostion( position );
}

void Game :: Rotate( RotatePlane plane, RotateSide side )
{
    if ( !rotating )
    {
	rotating_angle = pi / 2 / RotateStepsCount;
	if ( side == RotateByClockWise )
	    rotating_angle *= -1;
	rotating_plane = plane;
	if ( CheckUpRotate() )
	    rotating = true;
    }
}

void Game :: DropDownFigure()
{
    game_speed = SeventhSpeed;
}


void Game :: ChangeCameraPosition( float x, float y )
{
    if ( InRange( camera_position.teta + CameraPosChangeKoeff * y, -pi / 2, pi / 2 )	)
	camera_position.teta += CameraPosChangeKoeff * y;
    camera_position.alpha -= CameraPosChangeKoeff * x;
    camera_position.alpha = camera_position.alpha - ( ( int )( camera_position.alpha / 2 / pi ) * 2 * pi );
}

Point3Df Game :: GetCameraPosition()
{
    float cos_teta = cos( camera_position.teta );
    return Point3Df( cos_teta * sin( camera_position.alpha ) * CameraRadius, sin( camera_position.teta ) * CameraRadius, cos_teta * cos( camera_position.alpha ) * CameraRadius );
}

void Game :: SetLastMousePosition( float x, float y )
{
    last_mouse_position.x = x;
    last_mouse_position.y = y;
}

Point3Df Game :: GetLastMousePosition( )
{
    return last_mouse_position;
}

bool Game :: CheckCollisions()
{

}

bool Game :: CeckUpRotate()
{
    Figure check_figure = ( *current_figure );

    //Checking every step while rotates
    for ( int i = 0; i < RotateStepsCount; i++ )
    {
	final_angle = i * rotating_angle;

	switch ( rotating_plane )
	{
	case PlaneXY :
	    check_figure -> RotateOnXY( final_angle, state );
	    break;
	case PlaneZY :
	    check_figure -> RotateOnZY( final_angle, state );
	    break;
	default :
	    check_figure -> RotateOnZX( final_angle, state );
	}

	if ( !CheckCollisions() )
	    return false;
    }
}

