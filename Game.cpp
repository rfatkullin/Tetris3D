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
    figure_pos_correct_step =	0;
    last_mouse_position	    =   Point3Df( 0.0f, 0.0f, 0.0f );
    camera_position	    =   SphericalCoor( pi / 4, pi / 4 );
    current_figure	    =   GetNewFigure();
    figure_start_pos_y	    =   current_figure -> GetPosByYi();
    figure_down_steps	    =   0;
    next_figure		    =   GetNewFigure();
    is_pos_change	    =   false;
    rotating_step	    =   0;
    game_speed		    =   FirstSpeed;
    rotating		    =   false;
    score		    =   0;
    is_game		    =   true;
    collapse		    =	false;

    srand( time( 0 ) );

    for ( int i = 0; i < FieldLength; ++i )
	for ( int j = 0; j < FieldWidth; ++j )
	    for ( int k = 0; k < FieldHeight; ++k )
	    field[ i ][ j ][ k ] = NULL;

    for ( int i = 0; i < FieldLength; ++i )
	for ( int j = 0; j < FieldWidth; ++j )
	 field[ i ][ j ][ 0 ] =  new Block( FieldLowerBoundX + Block :: BlockSize / 2 + i * Block :: BlockSize, FieldLowerBoundY + Block :: BlockSize / 2, FieldLowerBoundZ + Block :: BlockSize / 2 + j * Block :: BlockSize, materials[ 3/*rand() % MaterialsCount*/ ] );
}

Point2Df Game :: GetFigurePositionOnXZ( int width_x, int width_z )
{
    if ( ( width_x / 2 ) % Block :: BlockSize == 0 )
    {
	if ( ( width_z / 2 ) % Block :: BlockSize == 0 )
	    return Point2Df( 0.0f, 0.0f );
	else
	    return Point2Df( 0.0f, Block :: BlockSize / 2 );
    }
    else
    {
	if ( ( width_z / 2 ) % Block :: BlockSize == 0 )
	    return Point2Df( Block :: BlockSize / 2, 0.0f );
	else
	    return Point2Df( Block :: BlockSize / 2, Block :: BlockSize / 2 );
    }
}

Figure* Game :: GetNewFigure()
{
    Point2Df figure_pos;
    int lower_bound_x,
	upper_bound_x,
	lower_bound_y,
	upper_bound_y,
	lower_bound_z,
	upper_bound_z,
	y_position  = FieldHeight * Block :: BlockSize;

    next_figure = new Figure( 0.0f, 0.0f, 0.0f, IFigure, materials[ rand() % 1/*MaterialsCount*/ ] );
    lower_bound_x = next_figure -> LowerBoundXi();
    upper_bound_x = next_figure -> UpperBoundXi();
    lower_bound_y = next_figure -> LowerBoundYi();
    upper_bound_y = next_figure -> UpperBoundYi();
    lower_bound_z = next_figure -> LowerBoundZi();
    upper_bound_z = next_figure -> UpperBoundZi();

    figure_pos = GetFigurePositionOnXZ( upper_bound_x - lower_bound_x, upper_bound_z - lower_bound_z );

    if ( ( upper_bound_y - lower_bound_y ) % 2 != 0 )
	y_position += Block :: BlockSize / 2;

    next_figure -> SetPosi( Point3Di( Block :: BlockSize * Game :: FieldLength / 2 + figure_pos.x, y_position, Block :: BlockSize * Game :: FieldWidth / 2 + figure_pos.y ) );

    return next_figure;
}

void Game :: MoveDownFigure()
{
//    Point3Df figure_pos = current_figure -> GetPosition();
//    current_figure -> SetPosition( Point3Df( figure_pos.x, figure_pos.y - game_speed * 0.5f,  figure_pos.z ));

//    for (  )
}

void Game :: CheckToCollapse()
{
    bool    full	= false;
    int	    full_levels = 0;

    for ( int k = 1; k < FieldHeight; ++k )
    {
	full = true;
	for ( int i = 0; i < FieldLength; ++i )
	    for ( int j = 0; j < FieldWidth; ++j )
		if ( field[ i ][ j ][ k ] == NULL )
		{
		    full = false;
		    break;
		}
	full_levels++;
    }

    for ( int k = FieldHeight; k > full_levels; k-- )
	for ( int i = 0; i < FieldLength; ++i )
	    for ( int j = 0; j < FieldWidth; ++j )
	    {
		 delete field[ i ][ j ][ k - full_levels ];
		 field[ i ][ j ][ k - full_levels ] = field[ i ][ j ][ k ];
		 field[ i ][ j ][ k ] = NULL;
	    }
    if ( full_levels > 0 )
    {
	collapse_steps_count = ( full_levels * Block :: BlockSize ) / SeventhSpeed + 1;
	collapse = true;
    }
}

void DoingCollapse( )
{

}
/*
void Game :: InCollapseShift()
{

}

void Game :: FinalCollapseShift()
{

}
*/

void Game :: NextStep()
{
    Point3Di	figure_pos;
    Point3Di	block_pos;
    float	final_angle		= 0.0f;
    bool	state			= false;
    int		field_index_by_length	= 0;
    int		field_index_by_height	= 0;
    int		field_index_by_width	= 0;

    if ( !is_game )
	return;

    if ( collapse )
    {
	collapse_steps_count--;

	if ( collapse_steps_count > 0 )
	    for ( int k = 0; k < FieldHeight; k++ )
		for ( int i = 0; i < FieldLength; ++i )
		    for ( int j = 0; j < FieldWidth; ++j )
			if ( field[ i ][ j ][ k ] != NULL )
			    field[ i ][ j ][ k ] -> SetPosi( field[ i ][ j ][ k ] -> GetPosi() - Point3Di( 0, game_speed, 0 ) );
	else
	{
	    collapse = false;

	    for ( int k = 0; k < FieldHeight; k++ )
		for ( int i = 0; i < FieldLength; ++i )
		    for ( int j = 0; j < FieldWidth; ++j )
			if ( field[ i ][ j ][ k ] != NULL )
			{
			    block_pos =  field[ i ][ j ][ k ] -> GetPosi();
			    field[ i ][ j ][ k ] -> SetPosi( block_pos.x,
								  ( block_pos.y - game_speed + Block :: BlockSize / 2 ) / ( Block :: BlockSize / 2 ) * ( Block :: BlockSize / 2 ),
								  block_pos.z );
			}
	}
    }

    if ( is_game && !collapse )
    {
	figure_down_steps += game_speed;
	figure_pos = current_figure -> GetPosi();
	current_figure -> SetPosi( Point3Di( figure_pos.x, figure_start_pos_y - ( int )( 0.5 * figure_down_steps ), figure_pos.z ) );

	for ( unsigned int  i = 0; ( i < Figure :: BlocksCount ) && ( is_game ); i++ )
	{
	    block_pos = figure_pos + current_figure -> GetBlockPosByIndex( i );
	    field_index_by_length = block_pos.x / Block :: BlockSize;
	    field_index_by_height = block_pos.y / Block :: BlockSize;
	    field_index_by_width  = block_pos.z / Block :: BlockSize;

	    for ( int j = FieldHeight - 1; j >= 0; j-- )
		if ( field[ field_index_by_length ][ field_index_by_width ][ j ] != NULL )
		{
		    if ( block_pos.y - Block :: BlockSize / 2 <= Game :: FieldLowerBoundY + ( j + 1 ) * Block :: BlockSize )
		    {
			is_game = false;
			break;
		    }
		}
	}

	if ( !is_game )
	{
	    figure_pos.y = figure_start_pos_y - ( int )( 0.5 * figure_down_steps ) / ( Block :: BlockSize / 2 ) * ( Block :: BlockSize / 2 );
	    for ( unsigned int  i = 0; i < Figure :: BlocksCount; i++ )
	    {
		block_pos        = figure_pos + current_figure -> GetBlockPosByIndex( i );
		field_index_by_length = block_pos.x / Block :: BlockSize;
		field_index_by_width  = block_pos.z / Block :: BlockSize;
		field_index_by_height = block_pos.y / Block :: BlockSize;
		field[ field_index_by_length ][ field_index_by_width ][ field_index_by_height ]
		    = new Block( block_pos.x,
				 block_pos.y,
				 block_pos.z,
				 current_figure -> GetBlockMaterialByIndex( i )
				);
	    }
	    //CheckToCollapse();
	    delete current_figure;
	    current_figure    = next_figure;
	    next_figure	      = GetNewFigure();
	    figure_down_steps = 0;
	    game_speed	      = FirstSpeed;
	    is_game	      = true;
	}    

	//Rotate the figure
	if ( rotating )
	{
	    //current_figure -> SetPositionF( current_figure -> GetPositionF() + pos_change_vec );
	    //Point3Df pos = current_figure -> GetPositionF();
	    //printf( "%f %f %f\n", pos.x, pos.y, pos.z );
	    //printf( "%f %f %f\n\n", pos_change_vec.x, pos_change_vec.y, pos_change_vec.z );
	    rotating_step++;
	    if ( rotating_step <= Block :: BlockSize / 2 )
		current_figure -> SetPosi( current_figure -> GetPosi() + figure_pos_correct_vec );
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

void Game :: DrawBlocksOnTheField()
{
    Point3Df rel_position( 0.0f, 0.0f, 0.0f );

    for ( int i = 0; i < FieldLength; ++i )
	for ( int j = 0; j < FieldWidth; ++j )
	    for ( int k = 0; k < FieldHeight; ++k )
		if ( field[ i ][ j ][ k ] != NULL )
		    field[ i ][ j ][ k ] -> Draw( rel_position );
}

void Game :: DrawInterface()
{

}

void Game :: DrawWorld()
{
    DrawField();
    DrawBlocksOnTheField();
   if ( current_figure != NULL )
	current_figure -> Draw();
    //DrawInterface();
}

float* Game :: GetLightPosition()
{
    return light_position;    
}

void Game :: ShiftFigureByXAxis( ShiftDirection shift_koeff )
{
	int		upper_bound_x = current_figure -> UpperBoundXi();
	int		lower_bound_x = current_figure -> LowerBoundXi();
	int	    shift         = shift_koeff * Block :: BlockSize;

	 if ( ( Game :: FieldLowerBoundX  <= lower_bound_x + shift      ) &&
	  ( lower_bound_x + shift         <= Game :: FieldUpperBoundX   ) &&
	  ( Game :: FieldLowerBoundX      <= upper_bound_x + shift      ) &&
	  ( upper_bound_x + shift         <= Game :: FieldUpperBoundX   ) )
		current_figure -> SetPosi( current_figure -> GetPosi() + Point3Di( shift, 0, 0 ) );
}

void Game :: ShiftFigureByZAxis( ShiftDirection shift_koeff )
{
	int		upper_bound_z = current_figure -> UpperBoundZi();
	int		lower_bound_z = current_figure -> LowerBoundZi();
	int	    shift         = shift_koeff * Block :: BlockSize;

	 if ( ( Game :: FieldLowerBoundZ <= lower_bound_z + shift      ) &&
	  ( lower_bound_z + shift		 <= Game :: FieldUpperBoundZ   ) &&
	  ( Game :: FieldLowerBoundZ     <= upper_bound_z + shift      ) &&
	  ( upper_bound_z + shift        <= Game :: FieldUpperBoundZ   ) )
		current_figure -> SetPosi( current_figure -> GetPosi() + Point3Di( 0, 0, shift ) );
}

void Game :: Rotate( RotatePlane plane, RotateSide side )
{
    if ( !rotating )
    {
	rotating_angle = pi / 2 / RotateStepsCount * side;
	rotating_plane = plane;
	rotating = true;

	//Point3Df figure_pos = current_figure -> GetPosition();
	int lower_bound_x = current_figure -> LowerBoundXi();
	int upper_bound_x = current_figure -> UpperBoundXi();
	int lower_bound_y = current_figure -> LowerBoundYi();
	int upper_bound_y = current_figure -> UpperBoundYi();
	int lower_bound_z = current_figure -> LowerBoundZi();
	int upper_bound_z = current_figure -> UpperBoundZi();

	figure_pos_correct_vec = Point3Di( 0, 0, 0 );
	switch ( rotating_plane )
	{
	case PlaneXY :
		if ( ( ( upper_bound_x - lower_bound_x ) / 2 % Block :: BlockSize != 0 ) ^
		 ( ( upper_bound_y - lower_bound_y ) / 2 % Block :: BlockSize != 0 ) )
		figure_pos_correct_vec = Point3Di( 1, 1, 0 );
	    break;
	case PlaneZY :
		if ( ( ( upper_bound_y - lower_bound_y ) / 2 % Block :: BlockSize != 0 ) ^
		 ( ( upper_bound_z - lower_bound_z ) / 2 % Block :: BlockSize != 0 ) )
		figure_pos_correct_vec = Point3Di( 0, 1, 1 );
	     break;
	default :
		if ( ( ( upper_bound_x - lower_bound_x ) / 2 % Block :: BlockSize != 0 ) ^
		 ( ( upper_bound_z - lower_bound_z ) / 2 % Block :: BlockSize != 0 ) )
		figure_pos_correct_vec = Point3Di( 1, 0, 1 );
	}
	figure_pos_correct_step = 0;
	//pos_change_vec = Block :: BlockSize / 2.0f * side / RotateStepsCount * pos_change_vec;
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

