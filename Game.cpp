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
    last_mouse_position =   Point3D( 0.0f, 0.0f, 0.0f );
    camera_position =	    SphericalCoor( pi / 4, pi / 4 );
    current_figure =	    new Figure( 0.0f, 300.0f, 0.0f, LFigure );
    rotating_step =	    0;
    game_speed =	    FirstSpeed;
    rotating =		    false;
    score =		    0;

    for ( int i = 0; i < FieldWidth; ++i )
	for ( int j = 0; j < FieldLength; ++j )
	    for ( int k = 0; k < FieldHeight; ++k )
		field[ i ][ j ][ k ] = 0;
}

void Game :: NextStep()
{
	float	final_angle = 0.0f;
	bool	state = false;
	
	//Move down the figure
	Point3D figure_position = current_figure -> GetPosition();
	if ( figure_position.y - game_speed * 0.5f >= FieldPositionByY )
		current_figure -> SetPostion( Point3D( figure_position.x, figure_position.y - game_speed * 0.5f,  figure_position.z ));

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
		if ( rotating_plane == PlaneXY )
			current_figure -> RotateOnXY( final_angle, state );
		else if ( rotating_plane == PlaneZY )
			current_figure -> RotateOnZY( final_angle, state );
		else
			current_figure -> RotateOnZX( final_angle, state );
	}
}

void Game :: DrawField()
{
	float ambient_light[ 4 ] = { 0.5f, 0.5f, 0.5f, 1.0f };
	float diffues_light[ 4 ] = { 0.0f, 0.0f, 0.0f, 1.0f };
	float specular_light[ 4 ] = { 0.0f, 0.0f, 0.0f, 1.0f };

	glMaterialfv( GL_FRONT, GL_AMBIENT, ambient_light );
	glMaterialfv( GL_FRONT, GL_DIFFUSE, diffues_light );
	glMaterialfv( GL_FRONT, GL_SPECULAR, specular_light );

	glBegin( GL_LINES );
		for ( int i = 0; i <= FieldLength; ++i )
		{
			glVertex3f( ( -FieldWidth / 2 + i ) * BlockSize, FieldPositionByY, ( -FieldLength / 2 ) * BlockSize );
			glVertex3f( ( -FieldWidth / 2 + i ) * BlockSize, FieldPositionByY, (  FieldLength / 2 ) * BlockSize );
		}
		for ( int i = 0; i <= FieldWidth; ++i )
		{
			glVertex3f( (  FieldWidth / 2 ) * BlockSize, FieldPositionByY, ( -FieldLength / 2 + i ) * BlockSize );
			glVertex3f( ( -FieldWidth / 2 ) * BlockSize, FieldPositionByY, ( -FieldLength / 2 + i ) * BlockSize );
		}
	glEnd();		
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
    //return 0;
}

void Game :: ShiftFigureByXAxis( ShiftDirection shift )
{
	Point3D position = current_figure -> GetPosition();
	
	if ( ( position.x + shift * BlockSize >= -FieldWidth / 2 * BlockSize ) && ( position.x + shift * BlockSize <= FieldWidth / 2 * BlockSize ) )
		position.x += shift * BlockSize;
	current_figure  -> SetPostion( position );
}

void Game :: ShiftFigureByZAxis( ShiftDirection shift )
{
	Point3D position = current_figure -> GetPosition();
	if ( ( position.z + shift * BlockSize >= -FieldLength / 2 * BlockSize ) && ( position.z + shift * BlockSize <= FieldLength / 2 * BlockSize ) )
		position.z += shift * BlockSize;
	current_figure  -> SetPostion( position );
}

void Game :: Rotate( RotatePlane plane, RotateSide side )
{
	if ( !rotating )
	{
		rotating_angle = pi / 2 / RotateStepsCount;
		if ( side == RotateByClockWise )
			rotating_angle *= -1;
		rotating_plane = plane;
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

Point3D Game :: GetCameraPosition()
{
	float cos_teta = cos( camera_position.teta );
	return Point3D( cos_teta * sin( camera_position.alpha ) * CameraRadius, sin( camera_position.teta ) * CameraRadius, cos_teta * cos( camera_position.alpha ) * CameraRadius );
}

void Game :: SetLastMousePosition( float x, float y )
{
    last_mouse_position.x = x;
    last_mouse_position.y = y;
}

Point3D Game :: GetLastMousePosition( )
{
    return last_mouse_position;
}


