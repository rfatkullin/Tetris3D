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
    camera_position     =   SphericalCoor( pi / 4, pi / 4 );
    current_figure      =   GetNewFigure();
    figure_down_steps	=   0;
    next_figure         =   GetNewFigure();
    is_pos_change       =   false;
    rotating_step	=   0;
    game_speed		=   FirstSpeed;
    rotating		=   false;
    score		=   0;
    is_game		=   true;

    for ( int i = 0; i < FieldWidth; ++i )
	for ( int j = 0; j < FieldLength; ++j )
	    for ( int k = 0; k < FieldHeight; ++k )
		field[ i ][ j ][ k ] = NULL;

    for ( int i = 0; i < FieldWidth; ++i )
	for ( int j = 0; j < FieldLength; ++j )
	    field[ i ][ j ][ 0 ] =  new Block( i * Block :: BlockSize / 2, FieldPositionByY, j * Block :: BlockSize / 2, materials[ rand() % MaterialsCount ] );
}

Point2Df Game :: GetFigurePositionOnXZ( int width_x, int width_z )
{
    if ( width_x % 2 == 0 )
    {
	if ( width_z % 2 == 0 )
	    return Point2Df( 0.0f, 0.0f );
	else
	    return Point2Df( 0.0f, Block :: BlockSize / 2 );
    }
    else
    {
	if ( width_z % 2 == 0 )
	    return Point2Df( Block :: BlockSize / 2, 0.0f );
	else
	    return Point2Df( Block :: BlockSize / 2, Block :: BlockSize / 2 );
    }
}

Figure* Game :: GetNewFigure()
{
    int lower_bound_x = current_figure -> LowerBoundXi();
    int upper_bound_x = current_figure -> UpperBoundXi();
    int lower_bound_y = current_figure -> LowerBoundYi();
    int upper_bound_y = current_figure -> UpperBoundYi();
    int lower_bound_z = current_figure -> LowerBoundZi();
    int upper_bound_z = current_figure -> UpperBoundZi();
    int y_position    = FieldHeight * Block :: BlockSize;

    next_figure = new Figure( 0.0f, 0.0f, 0.0f, IFigure, materials[ rand() % 1/*MaterialsCount*/ ] );

    Point2Df figure_position = GetFigurePositionOnXZ( upper_bound_x - lower_bound_x, upper_bound_z - lower_bound_z );

    if ( ( upper_bound_y - lower_bound_y ) % 2 != 0 )
	y_position += Block :: BlockSize / 2;

    next_figure -> SetPosition( Point3Df( figure_position.x, y_position, figure_position.y ) );

    return next_figure;
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

    if ( is_game )
    {
	//figure_down_steps++;
	Point3Df figure_position = current_figure -> GetPosition();
	Point3Di blocks_position[ Figure :: BlocksCount ];
	for ( int  i = 0; i < Figure :: BlocksCount; i++ )
	    blocks_position[ i ] = figure_position + current_figure -> GetBlockPositionByIndex( i );

	float y_lower_bound = current_figure -> LowerBoundYf();
	if ( y_lower_bound - game_speed * 1 >= FieldLowerBoundYf )
	    current_figure -> SetPosition( Point3Df( figure_position.x, figure_position.y - game_speed,  figure_position.z ));
	else
	{
	    is_game = false;
	    current_figure -> SetPosition( Point3Df( figure_position.x, figure_position.y - ( y_lower_bound - FieldLowerBoundYf ),  figure_position.z ));
	}
    }


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
	    glVertex3f( FieldLowerBoundX, FieldLowerBoundYf, FieldLowerBoundZ + i * Block :: BlockSize );
	    glVertex3f( FieldUpperBoundX, FieldLowerBoundYf, FieldLowerBoundZ + i * Block :: BlockSize );
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
//    for ( int i = 0; i < FieldWidth; ++i )
//	for ( int j = 0; j < FieldLength; ++j )
//	    field[ i ][ j ][ 0 ] -> Draw( Point3Df( 0.0f, 0.0f, 0.0f ));
}

float* Game :: GetLightPosition()
{
    return light_position;    
}

void Game :: ShiftFigureByXAxis( ShiftDirection shift_koeff )
{
    Point3Df	position = current_figure -> GetPosition();
    float	max_x_pos = current_figure -> UpperBoundX();
    float	min_x_pos = current_figure -> LowerBoundXf();
    float	shift = shift_koeff * Block :: BlockSize;

    if ( ( Game :: FieldLowerBoundXf <= min_x_pos + shift  ) && ( min_x_pos + shift <= Game :: FieldUpperBoundX ) &&
	 ( Game :: FieldLowerBoundXf <= max_x_pos + shift  ) && ( max_x_pos + shift <= Game :: FieldUpperBoundX )
       )
	position.x += shift;

    current_figure -> SetPosition( position );
}

void Game :: ShiftFigureByZAxis( ShiftDirection shift_koeff )
{
    Point3Df	position = current_figure -> GetPosition();
    float	max_z_pos = current_figure -> UpperBoundZ();
    float	min_z_pos = current_figure -> LowerBoundZf();
    float	shift = shift_koeff * Block :: BlockSize;

    if ( ( Game :: FieldLowerBoundZf <= min_z_pos + shift  ) && ( min_z_pos + shift <= Game :: FieldUpperBoundZ ) &&
	 ( Game :: FieldLowerBoundZf <= max_z_pos + shift  ) && ( max_z_pos + shift <= Game :: FieldUpperBoundZ )
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
	int lower_bound_x = current_figure -> LowerBoundXf();
	int upper_bound_x = current_figure -> UpperBoundX();
	int lower_bound_y = current_figure -> LowerBoundYf();
	int upper_bound_y = current_figure -> UpperBoundY();
	int lower_bound_z = current_figure -> LowerBoundZf();
	int upper_bound_z = current_figure -> UpperBoundZ();

	pos_change_vec = Point3Df( 0.0f, 0.0f, 0.0f );
	switch ( rotating_plane )
	{
	case PlaneXY :
	    if ( ( ( upper_bound_x - lower_bound_x ) / 2 % Block :: BlockSize != 0 ) ^
		 ( ( upper_bound_y - lower_bound_y ) / 2 % Block :: BlockSize != 0 ) )
		pos_change_vec = Point3Df( 1.0f, 1.0f, 0.0f );
	    break;
	case PlaneZY :
	    if ( ( ( upper_bound_y - lower_bound_y ) / 2 % Block :: BlockSize != 0 ) ^
		 ( ( upper_bound_z - lower_bound_z ) / 2 % Block :: BlockSize != 0 ) )
		pos_change_vec = Point3Df( 0.0f, 1.0f, 1.0f );
	     break;
	default :
	    if ( ( ( upper_bound_x - lower_bound_x ) / 2 % Block :: BlockSize != 0 ) ^
		 ( ( upper_bound_z - lower_bound_z ) / 2 % Block :: BlockSize != 0 ) )
		pos_change_vec = Point3Df( 1.0f, 0.0f, 1.0f );
	}
	pos_change_vec = Block :: BlockSize / 2.0f * side / RotateStepsCount * pos_change_vec;
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

