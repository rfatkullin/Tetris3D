#include <QGLWidget>
#include <math.h>
#include <stdio.h>
#include <time.h>
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
    is_pos_change =	    false;
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
//    Point3Df figure_position = current_figure -> GetPosition();
//    current_figure -> SetPosition( Point3Df( figure_position.x, figure_position.y - game_speed * 0.5f,  figure_position.z ));

//    for (  )
}

void Game :: NextStep()
{
    float	final_angle = 0.0f;
    bool	state = false;

    //MoveDiwnFigure();
    //Move down the figure

    Point3Df figure_position = current_figure -> GetPosition();
    if ( figure_position.y - game_speed * 0.5f - Block :: BlockSize / 2 - Block :: BlockSize >= FieldLowerBoundY )
	    current_figure -> SetPosition( Point3Df( figure_position.x, figure_position.y - game_speed * 0.5f,  figure_position.z ));

    //Rotate the figure
    if ( rotating )
    {
	current_figure -> SetPosition( current_figure -> GetPosition() + pos_change_vec );
	Point3Df pos = current_figure -> GetPosition();
	//printf( "%f %f %f\n", pos.x, pos.y, pos.z );
	//printf( "%f %f %f\n\n", pos_change_vec.x, pos_change_vec.y, pos_change_vec.z );
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
	    current_figure -> RotateOnXY( final_angle, state );
	    break;
	case PlaneZY :
	    current_figure -> RotateOnZY( final_angle, state );
	    break;
	default :
	    current_figure -> RotateOnZX( final_angle, state );
	}
    }
}

void Game :: DrawField()
{
    //Camera( +, +, + ) and looking ( 0, 0, 0 )
    glDisable( GL_LIGHTING );


    glBegin( GL_LINES );
	glColor3f( 0.5f, 0.5f, 0.5f );
    //FarGridZ
	for ( int i = 0; i <= FieldLength; i++ )
	{
	    glVertex3f( FieldLowerBoundX + i * Block :: BlockSize, FieldLowerBoundY, FieldLowerBoundZ );
	    glVertex3f( FieldLowerBoundX + i * Block :: BlockSize, FieldUpperBoundY, FieldLowerBoundZ );
	}

	for ( int i = 0; i <= FieldHeight; i++ )
	{
	    glVertex3f( FieldLowerBoundX, FieldLowerBoundY + i * Block :: BlockSize, FieldLowerBoundZ );
	    glVertex3f( FieldUpperBoundX, FieldLowerBoundY + i * Block :: BlockSize, FieldLowerBoundZ );
	}
    //LowerGrid
	glColor3f( 1.0f, 1.0f, 1.0f );
	for ( int i = 0; i <= FieldWidth; i++ )
	{
	    glVertex3f( FieldLowerBoundX, FieldLowerBoundY, FieldLowerBoundZ + i * Block :: BlockSize );
	    glVertex3f( FieldUpperBoundX, FieldLowerBoundY, FieldLowerBoundZ + i * Block :: BlockSize );
	}

	for ( int i = 0; i <= FieldLength; i++ )
	{
	    glVertex3f(  FieldLowerBoundX + i * Block :: BlockSize, FieldLowerBoundY, FieldLowerBoundZ );
	    glVertex3f(  FieldLowerBoundX + i * Block :: BlockSize, FieldLowerBoundY, FieldUpperBoundZ );
	}
    //LeftGrid
	glColor3f( 0.5f, 0.0f, 0.0f );
	for ( int i = 0; i <= FieldWidth; i++ )
	{
	    glVertex3f(  FieldLowerBoundX, FieldLowerBoundY, FieldLowerBoundZ + i * Block :: BlockSize );
	    glVertex3f(  FieldLowerBoundX, FieldUpperBoundY, FieldLowerBoundZ + i * Block :: BlockSize );
	}
	for ( int i = 0; i <= FieldHeight; i++ )
	{
	    glVertex3f( FieldLowerBoundX, FieldLowerBoundY + i * Block :: BlockSize, FieldLowerBoundZ );
	    glVertex3f( FieldLowerBoundX, FieldLowerBoundY + i * Block :: BlockSize, FieldUpperBoundZ );
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
    float	max_x_pos = current_figure -> UpperBoundX();
    float	min_x_pos = current_figure -> LowerBoundX();
    float	shift = shift_koeff * Block :: BlockSize;

    if ( ( Game :: FieldLowerBoundX <= min_x_pos + shift  ) && ( min_x_pos + shift <= Game :: FieldUpperBoundX ) &&
	 ( Game :: FieldLowerBoundX <= max_x_pos + shift  ) && ( max_x_pos + shift <= Game :: FieldUpperBoundX )
       )
	position.x += shift;

    current_figure -> SetPosition( position );
}

void Game :: ShiftFigureByZAxis( ShiftDirection shift_koeff )
{
    Point3Df	position = current_figure -> GetPosition();
    float	max_z_pos = current_figure -> UpperBoundZ();
    float	min_z_pos = current_figure -> LowerBoundZ();
    float	shift = shift_koeff * Block :: BlockSize;

    if ( ( Game :: FieldLowerBoundZ <= min_z_pos + shift  ) && ( min_z_pos + shift <= Game :: FieldUpperBoundZ ) &&
	 ( Game :: FieldLowerBoundZ <= max_z_pos + shift  ) && ( max_z_pos + shift <= Game :: FieldUpperBoundZ )
       )
	position.z += shift;

    current_figure -> SetPosition( position );
}

void Game :: Rotate( RotatePlane plane, RotateSide side )
{
    if ( !rotating )
    {
	rotating_angle = pi / 2 / RotateStepsCount * side;
	rotating_plane = plane;
	rotating = true;

	//Point3Df figure_pos = current_figure -> GetPosition();
	float lower_bound_x = current_figure -> LowerBoundX();
	float upper_bound_x = current_figure -> UpperBoundX();
	float lower_bound_y = current_figure -> LowerBoundY();
	float upper_bound_y = current_figure -> UpperBoundY();
	float lower_bound_z = current_figure -> LowerBoundZ();
	float upper_bound_z = current_figure -> UpperBoundZ();

	pos_change_vec = Point3Df( 0.0f, 0.0f, 0.0f );
	switch ( rotating_plane )
	{
	case PlaneXY :
	    if ( ( ( int )( upper_bound_x - lower_bound_x ) / 2 % Block :: BlockSize != 0 ) ^
		 ( ( int )( upper_bound_y - lower_bound_y ) / 2 % Block :: BlockSize != 0 ) )
		pos_change_vec = Point3Df( 1.0f, 1.0f, 0.0f );
	    break;
	case PlaneZY :
	    if ( ( ( int )( upper_bound_y - lower_bound_y ) / 2 % Block :: BlockSize != 0 ) ^
		 ( ( int )( upper_bound_z - lower_bound_z ) / 2 % Block :: BlockSize != 0 ) )
		pos_change_vec = Point3Df( 0.0f, 1.0f, 1.0f );
	     break;
	default :
	    if ( ( ( int )( ( upper_bound_x - lower_bound_x ) / 2.0f + 0.5f ) % Block :: BlockSize != 0 ) ^
		 ( ( int )( ( upper_bound_z - lower_bound_z ) / 2.0f + 0.5f ) % Block :: BlockSize != 0 ) )
		pos_change_vec = Point3Df( 1.0f, 0.0f, 1.0f );
	}
	pos_change_vec = Block :: BlockSize / 2.0f * side / RotateStepsCount * pos_change_vec;
	printf( "%d %d\n", ( int )( upper_bound_x - lower_bound_x ), ( int )( upper_bound_z - lower_bound_z ) );
	printf( "%f %f %f\n", pos_change_vec.x, pos_change_vec.y, pos_change_vec.z );
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

//bool Game :: CeckUp
//()
//{
//    Figure check_figure = ( *current_figure );

//    //Checking every step while rotates
//    for ( int i = 0; i < RotateStepsCount; i++ )
//    {
//	final_angle = i * rotating_angle;

//	switch ( rotating_plane )
//	{
//	case PlaneXY :
//	    check_figure -> RotateOnXY( final_angle, state );
//	    break;
//	case PlaneZY :
//	    check_figure -> RotateOnZY( final_angle, state );
//	    break;
//	default :
//	    check_figure -> RotateOnZX( final_angle, state );
//	}

//	//Check Collisions

//    }
//}

