#include <QGLWidget>
#include <math.h>
#include <stdio.h>
#include "Game.h"
#include "Figures.h"

const int	Game :: FIELD_POSITION_BY_Y_AXIS = -200;
const float	Game :: CAMERA_POS_CHANGE_KOEFF =	0.01f;
const float	Game :: CAMERA_RADIUS =				300.0f;

Game :: Game()
{
	Start();
}

void Game :: Start()
{	
	camera_position =	SphericalCoor( pi / 4, pi / 4 );
	current_figure =	new Figure( 0.0f, 300.0f, 0.0f, L_FIGURE );	
	rotating_step =		0;	
	game_speed =		FIRST_SPEED;	
	rotating =			false;
	score =				0;	
	
	for ( int i = 0; i < FIELD_WIDTH; ++i )
		for ( int j = 0; j < FIELD_LENGTH; ++j )
			for ( int k = 0; k < FIELD_HEIGHT; ++k )
				field[ i ][ j ][ k ] = 0;	
}

void Game :: NextStep()
{
	float	final_angle = 0.0f;
	bool	state = false;
	
	//Move down the figure
	Point3D figure_position = current_figure -> GetPosition();
	if ( figure_position.y - game_speed * 0.5f >= FIELD_POSITION_BY_Y_AXIS )
		current_figure -> SetPostion( Point3D( figure_position.x, figure_position.y - game_speed * 0.5f,  figure_position.z ));

	//Rotate the figure
	if ( rotating )
	{
		rotating_step++;
		final_angle = rotating_step * rotating_angle;
		if ( rotating_step == ROTATE_STEP_COUNT )
		{
			rotating_step = 0;
			rotating = false;
			state = true;			
		}
		if ( rotating_plane == PLANE_XY )
			current_figure -> RotateOnXY( final_angle, state );
		else if ( rotating_plane == PLANE_ZY )
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
		for ( int i = 0; i <= FIELD_LENGTH; ++i )
		{
			glVertex3f( ( -FIELD_WIDTH / 2 + i ) * BLOCK_SIZE, FIELD_POSITION_BY_Y_AXIS, ( -FIELD_LENGTH / 2 ) * BLOCK_SIZE );
			glVertex3f( ( -FIELD_WIDTH / 2 + i ) * BLOCK_SIZE, FIELD_POSITION_BY_Y_AXIS, (  FIELD_LENGTH / 2 ) * BLOCK_SIZE );
		}
		for ( int i = 0; i <= FIELD_WIDTH; ++i )
		{
			glVertex3f( (  FIELD_WIDTH / 2 ) * BLOCK_SIZE, FIELD_POSITION_BY_Y_AXIS, ( -FIELD_LENGTH / 2 + i ) * BLOCK_SIZE );
			glVertex3f( ( -FIELD_WIDTH / 2 ) * BLOCK_SIZE, FIELD_POSITION_BY_Y_AXIS, ( -FIELD_LENGTH / 2 + i ) * BLOCK_SIZE );
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

void Game :: ShiftFigureByXAxis( ShiftDirection shift )
{
	Point3D position = current_figure -> GetPosition();
	
	if ( ( position.x + shift * BLOCK_SIZE >= -FIELD_WIDTH / 2 * BLOCK_SIZE ) && ( position.x + shift * BLOCK_SIZE <= FIELD_WIDTH / 2 * BLOCK_SIZE ) )
		position.x += shift * BLOCK_SIZE;
	current_figure  -> SetPostion( position );
}

void Game :: ShiftFigureByZAxis( ShiftDirection shift )
{
	Point3D position = current_figure -> GetPosition();
	if ( ( position.z + shift * BLOCK_SIZE >= -FIELD_LENGTH / 2 * BLOCK_SIZE ) && ( position.z + shift * BLOCK_SIZE <= FIELD_LENGTH / 2 * BLOCK_SIZE ) )
		position.z += shift * BLOCK_SIZE;
	current_figure  -> SetPostion( position );
}

void Game :: Rotate( RotatePlane plane, RotateSide side )
{
	if ( !rotating )
	{
		rotating_angle = pi / 2 / ROTATE_STEP_COUNT;
		if ( side == ROTATE_BY_CLOCK_WISE )
			rotating_angle *= -1;
		rotating_plane = plane;
		rotating = true;
	}
}

void Game :: DropDownFigure()
{
	game_speed = SEVENTH_SPEED;
}


void Game :: ChangeCameraPosition( float x, float y )
{
	if ( InRange( camera_position.teta + CAMERA_POS_CHANGE_KOEFF * y, -pi / 2, pi / 2 )	)
		camera_position.teta += CAMERA_POS_CHANGE_KOEFF * y;
	camera_position.alpha -= CAMERA_POS_CHANGE_KOEFF * x;
	camera_position.alpha = camera_position.alpha - ( ( int )( camera_position.alpha / 2 / pi ) * 2 * pi );
}

Point3D Game :: GetCameraPosition()
{
	float cos_teta = cos( camera_position.teta );
	return Point3D( cos_teta * sin( camera_position.alpha ) * CAMERA_RADIUS, sin( camera_position.teta ) * CAMERA_RADIUS, cos_teta * cos( camera_position.alpha ) * CAMERA_RADIUS );
}


