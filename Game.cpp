#include <QGLWidget>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include "Game.h"

const int       Game :: SafetyDistance = 16.0 * BSize * BSize;
const float	Game :: CameraPosChangeKoeff =	0.01f;
const float	Game :: CameraRadius = 1000.0f;
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

    for ( int i = 1; i < Height; ++i )
    {
	for ( int j = 0; j < Length; ++j )
	{
	    field[ j ][ i ][ 0 ]         = new Block( BSize / 2 + j * BSize, BSize / 2 + i * BSize,  BSize / 2 , materials[ 3 ] );
	    field[ j ][ i ][ Width - 1 ] = new Block( BSize / 2 + j * BSize, BSize / 2 + i * BSize,  BSize / 2 + ( Width - 1 ) * BSize, materials[ 3 ] );
	}

	for ( int j = 0; j < Width; ++j )
	{
	    field[ 0 ][ i ][ j ]          = new Block( BSize / 2,                          BSize / 2 + i * BSize, BSize / 2 + j * BSize, materials[ 3 ] );
	    field[ Length - 1 ][ i ][ j ] = new Block( BSize / 2 + ( Length - 1 ) * BSize, BSize / 2 + i * BSize, BSize / 2 + j * BSize, materials[ 3 ] );
	}
    }

     for ( int i = 0; i < Length; ++i )
	for ( int j = 0; j < Width; ++j )
	    field[ i ][ 0 ][ j ]	  =  new Block( BSize / 2 + i * BSize, BSize / 2,                           BSize / 2 + j * BSize, materials[ 3 ] );

     for ( int i = 0; i < Length; ++i )
	for ( int j = 0; j < Width; ++j )
	    field[ i ][ Height - 1 ][ j ] =  new Block( BSize / 2 + i * BSize, BSize / 2 + ( Height - 1 ) * BSize,  BSize / 2 + j * BSize, materials[ 3 ] );
}

void Game :: Start()
{	
    figure_pos_correct_step     =   0;
    last_mouse_position         =   Point3Df( 0.0f, 0.0f, 0.0f );
    camera_position             =   SphericalCoor( pi / 4, pi / 4 );
    current_figure              =   GetNewFigure();
    figure_start_pos_y          =   current_figure -> GetPosByYi();
    figure_down_steps           =   0;
    count_of_blocks_on_field	=   0;
    next_figure                 =   GetNewFigure();
    is_pos_change               =   false;
    rotating_step               =   0;
    game_speed                  =   FirstSpeed;
    rotating                    =   false;
    score                       =   0;
    is_game                     =   true;
    collapse                    =   false;


    srand( time( 0 ) );

    for ( int i = 0; i < Length; ++i )
        for ( int j = 0; j < Width; ++j )
            for ( int k = 0; k < Height; ++k )
                field[ i ][ k ][ j ] = NULL;
 }

Point2Df Game :: GetFigurePositionOnXZ( int width_x, int width_z )
{
    if ( ( width_x / 2 ) % BSize == 0 )
    {
        if ( ( width_z / 2 ) % BSize == 0 )
	    return Point2Df( 0.0f, 0.0f );
	else
            return Point2Df( 0.0f, BSize / 2 );
    }
    else
    {
        if ( ( width_z / 2 ) % BSize == 0 )
            return Point2Df( BSize / 2, 0.0f );
	else
            return Point2Df( BSize / 2, BSize / 2 );
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
	y_position  = ( Height - 3 )  * BSize;

    next_figure = new Figure( 0.0f, 0.0f, 0.0f, IFigure, materials[ rand() % 1/*MaterialsCount*/ ] );
    lower_bound_x = next_figure -> LowerBoundXi();
    upper_bound_x = next_figure -> UpperBoundXi();
    lower_bound_y = next_figure -> LowerBoundYi();
    upper_bound_y = next_figure -> UpperBoundYi();
    lower_bound_z = next_figure -> LowerBoundZi();
    upper_bound_z = next_figure -> UpperBoundZi();

    figure_pos = GetFigurePositionOnXZ( upper_bound_x - lower_bound_x, upper_bound_z - lower_bound_z );

    if ( ( upper_bound_y - lower_bound_y ) % 2 != 0 )
        y_position += BSize / 2;

    next_figure -> SetPosi( Point3Di( BSize * Game :: Length / 2 + figure_pos.x, y_position, BSize * Game :: Width / 2 + figure_pos.y ) );

    return next_figure;
}

bool Game :: CheckToRotate( RotatePlane rotating_plane, float rotating_angle )
{
    std :: vector < Block > collision_blocks;
    Figure*     collis_figure           = new Figure( *current_figure );
    Point3Di    block_pos;
    Point3Di    figure_pos              = current_figure -> GetPosi();
    bool        collision               = false;
    float       final_angle             = 0.0f;
    float       distance                = 0.0f;
    int         collapse_sim_count      = collapse_steps_count;
    int         collision_blocks_count  = 0;

    //Collection blocks in SafetyDistance radius
    for ( int k = 0; k < Height; ++k )
        for ( int i = 0; i < Length; ++i )
            for ( int j = 0; j < Width; ++j )
                if ( field[ i ][ k ][ j ] != NULL )
                {
                    block_pos = field[ i ][ k ][ j ] -> GetPosi();
                    distance = ( figure_pos.x - block_pos.x ) * ( figure_pos.x - block_pos.x )  + ( figure_pos.z - block_pos.z )  * ( figure_pos.z - block_pos.z );
                    if ( distance < SafetyDistance )
                        collision_blocks.push_back( Block( *field[ i ][ k ][ j ] ) );
                }
    collision_blocks_count = collision_blocks.size();

    for ( unsigned int i = 0; ( i < RotateStepsCount ) && ( !collision ); i++ )
    {
            //Manipulation with Figure
            if ( i <= BSize / 2 )           //Correct the figure pos while rotate
                collis_figure -> SetPosi( collis_figure -> GetPosi() + figure_pos_correct_vec );
            final_angle = i * rotating_angle;
            switch ( rotating_plane )
            {
            case PlaneXY :
                collis_figure -> RotateOnXY( final_angle, false );
                break;
            case PlaneZY :
                collis_figure -> RotateOnZY( final_angle, false );
                break;
            default :
                collis_figure -> RotateOnZX( final_angle, false );
            }

            //Manipulating witn field blocks at the collpase
            if ( collapse_sim_count > 0 )
            {
                collapse_sim_count--;

                if ( collapse_sim_count > 0 )
                    for ( int i = 0; i < collision_blocks_count; i++ )
                        collision_blocks[ i ].SetPosi( collision_blocks[ i ].GetPosi() - Point3Di( 0, SeventhSpeed, 0 ) );
                else
                    for ( int i = 0; i < collision_blocks_count; i++ )
                        collision_blocks[ i ].SetPosi( collision_blocks[ i ].GetPosi() -
                            Point3Di( 0, ( collision_blocks[ i ].GetPosByYi() - SeventhSpeed + BSize / 2 ) / ( BSize / 2 ) * ( BSize / 2 ), 0 ) );
            }

            collision = collis_figure -> CheckToCollisonWithBlocks( collision_blocks );
    }

    delete collis_figure;

    return collision;
}
void Game :: CheckToCollapse()
{
    bool    full	= false;
    int	    full_levels = 0;

    for ( int k = FieldBeginY; k < FieldEndY; ++k )
    {
	full = true;
        for ( int i = FieldBeginX; ( i < FieldEndX ) && ( full ); ++i )
            for ( int j = FieldBeginZ; j < FieldEndZ; ++j )
                if ( field[ i ][ k ][ j ] == NULL )
		{
		    full = false;
		    break;
		}
	if ( full )
	    full_levels++;
    }

    if ( full_levels > 0 )
    {
        for ( int k = FieldBeginY; k < FieldEndY; k++ )
	{
            if ( k < full_levels )
                for ( int i = FieldBeginX; i < FieldEndX; ++i )
                    for ( int j = FieldBeginZ; j < FieldEndZ; ++j )
		    {
                        delete field[ i ][ k ][ j ];
                        field[ i ][ k ][ j ] = NULL;
		    }
	    else
                for ( int i = FieldBeginX; i < FieldEndX; ++i )
                    for ( int j = FieldBeginZ; j < FieldEndZ; ++j )
		    {
                        field[ i ][ k - full_levels ][ j ] = field[ i ][ k ][ j ];
                        field[ i ][ k ][ j ] = NULL;
		    }
	}
        collapse_steps_count = ( full_levels * BSize ) / FirstSpeed + 1;
	collapse = true;
        count_of_blocks_on_field -= FieldEndX * FieldEndZ * full_levels;
    }
}

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
	{
            for ( int k = FieldBeginY; k < FieldEndY; k++ )
                for ( int i = FieldBeginX; i < FieldEndX; ++i )
                    for ( int j = FieldBeginZ; j < FieldEndZ; ++j )
                        if ( field[ i ][ k ][ j ] != NULL )
                            field[ i ][ k ][ j ] -> SetPosi( field[ i ][ k ][ j ] -> GetPosi() - Point3Di( 0, FirstSpeed, 0 ) );
	}
	else
	{
	    collapse = false;

            for ( int k = FieldBeginY; k < FieldEndY; k++ )
                for ( int i = FieldBeginX; i < FieldEndX; ++i )
                    for ( int j = FieldBeginZ; j < FieldEndZ; ++j )
                        if ( field[ i ][ k ][ j ] != NULL )
			{
                            block_pos =  field[ i ][ k ][ j ] -> GetPosi();
                            field[ i ][ k ][ j ] -> SetPosi( block_pos.x,
                                                                  ( block_pos.y - FirstSpeed + BSize / 2 ) / ( BSize / 2 ) * ( BSize / 2 ),
								  block_pos.z );
			}
	}
    }

    if ( is_game )
    {
        figure_down_steps++;
        if ( figure_down_steps == 2 )
        {
            current_figure -> SetPosi( current_figure -> GetPosi() - Point3Di( 0, game_speed, 0 ) );
            figure_down_steps = 0;
        }
        figure_pos = current_figure -> GetPosi();

        //Detecting when figure must stop
        for ( unsigned int  i = 0; ( i < Figure :: BlocksCount ) && ( is_game ); i++ )
	{
            block_pos = figure_pos + current_figure -> GetBlockPosByIndexi( i );
            field_index_by_length = block_pos.x / BSize;
            field_index_by_height = block_pos.y / BSize;
            field_index_by_width  = block_pos.z / BSize;

            for ( int j = field_index_by_height - 1; j >= FieldBeginY - 1; --j )
                if ( field[ field_index_by_length ][ j ][ field_index_by_width ] != NULL )
                {
                    if ( block_pos.y - BSize / 2 <= Game :: FieldLowerBoundY + j * BSize )
                    {
                        is_game = false;
                        break;
                    }
                }
	}

	if ( !is_game )
	{
            current_figure -> SetPosByYi( ( current_figure -> GetPosByYi() + game_speed ) / ( BSize / 2 ) * ( BSize / 2  ) );
            figure_pos = current_figure -> GetPosi();
            for ( unsigned int  i = 0; i < Figure :: BlocksCount; i++ )
	    {
                block_pos        = figure_pos + current_figure -> GetBlockPosByIndexi( i );
                field_index_by_length = block_pos.x / BSize;
                field_index_by_width  = block_pos.z / BSize;
                field_index_by_height = block_pos.y / BSize;
                field[ field_index_by_length ][ field_index_by_height ][ field_index_by_width ]
		    = new Block( block_pos.x,
				 block_pos.y,
				 block_pos.z,
				 current_figure -> GetBlockMaterialByIndex( i )
				);
	    }
	    CheckToCollapse();
	    delete current_figure;
	    current_figure    = next_figure;
	    next_figure	      = GetNewFigure();
	    figure_down_steps = 0;
	    game_speed	      = FirstSpeed;
	    is_game	      = true;
            count_of_blocks_on_field += Figure :: BlocksCount;
	    if ( ( !collapse ) && ( field_index_by_height == FieldEndY - 2 ) )
		exit( 0 );
	}    

        //Rotate the figure
	if ( rotating )
	{
            rotating_step++;
            if ( rotating_step <= BSize / 2 )
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
        for ( int i = FieldBeginX - 1; i < FieldEndX; i++ )
        {
                glVertex3f( FieldLowerBoundX + i * BSize, FieldLowerBoundY, FieldLowerBoundZ );
                glVertex3f( FieldLowerBoundX + i * BSize, FieldUpperBoundY, FieldLowerBoundZ );
        }

        for ( int i = FieldBeginY - 1; i < FieldEndY; i++ )
        {
                glVertex3f( FieldLowerBoundX, FieldLowerBoundY + i * BSize, FieldLowerBoundZ );
                glVertex3f( FieldUpperBoundX, FieldLowerBoundY + i * BSize, FieldLowerBoundZ );
        }
    //LowerGrid
	glColor3f( 1.0f, 1.0f, 1.0f );
        for ( int i = FieldBeginZ - 1; i < FieldEndZ; i++ )
	{
                glVertex3f( FieldLowerBoundX, FieldLowerBoundY, FieldLowerBoundZ + i * BSize );
                glVertex3f( FieldUpperBoundX, FieldLowerBoundY, FieldLowerBoundZ + i * BSize );
	}

        for ( int i = FieldBeginX - 1; i < FieldEndX; i++ )
	{
                glVertex3f(  FieldLowerBoundX + i * BSize, FieldLowerBoundY, FieldLowerBoundZ );
                glVertex3f(  FieldLowerBoundX + i * BSize, FieldLowerBoundY, FieldUpperBoundZ );
	}
    //LeftGrid
        glColor3f( 0.5f, 0.0f, 0.0f );
        for ( int i = FieldBeginZ - 1; i < FieldEndZ; i++ )
        {
                glVertex3f(  FieldLowerBoundX, FieldLowerBoundY, FieldLowerBoundZ + i * BSize );
                glVertex3f(  FieldLowerBoundX, FieldUpperBoundY, FieldLowerBoundZ + i * BSize );
        }
        for ( int i = FieldBeginY - 1; i < FieldEndY; i++ )
        {
                glVertex3f( FieldLowerBoundX, FieldLowerBoundY + i * BSize, FieldLowerBoundZ );
                glVertex3f( FieldLowerBoundX, FieldLowerBoundY + i * BSize, FieldUpperBoundZ );
        }
    glEnd();

    glEnable( GL_LIGHTING );
}

void Game :: DrawBlocksOnTheField()
{
    Point3Df rel_position( 0.0f, 0.0f, 0.0f );

    for ( int i = FieldBeginX; i < FieldEndX; ++i )
	for ( int j = FieldBeginZ; j < FieldEndZ; ++j )
	    for ( int k = FieldBeginY; k < FieldEndY; ++k )
                if ( field[ i ][ k ][ j ] != NULL )
                    field[ i ][ k ][ j ] -> Draw( rel_position );
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


void Game :: ShiftFigureByXAxis( ShiftDirection shift )
{
    std :: vector < Block >     collision_block;
    int                         x_coor_collision_blocks;

    if ( shift < 0 )
        x_coor_collision_blocks = current_figure -> GetLeftMostBlockPosi().x / BSize;
    else
        x_coor_collision_blocks = current_figure -> GetRightMostBlockPosi().x / BSize;

    for ( int i = FieldBeginY; i < FieldEndY; ++i )
        for ( int j = FieldBeginZ; j < FieldEndZ; ++j )
            if ( ( x_coor_collision_blocks + shift >= 0 ) && ( x_coor_collision_blocks + shift < Length ) && ( field[ x_coor_collision_blocks + shift ][ i ][ j ] != NULL ) )
                collision_block.push_back( Block( *field[ x_coor_collision_blocks + shift ][ i ][ j ] ) );

    current_figure -> SetPosi( current_figure -> GetPosi() + Point3Di( shift * BSize, 0, 0 ) );
    if ( current_figure -> CheckToCollisonWithBlocks( collision_block ) )
        current_figure -> SetPosi( current_figure -> GetPosi() - Point3Di( shift * BSize, 0, 0 ) );
}

void Game :: ShiftFigureByZAxis( ShiftDirection shift )
{
    std :: vector < Block >     collision_block;
    int                         z_coor_collision_blocks;

    if ( shift < 0 )
        z_coor_collision_blocks = current_figure -> GetBackMostBlockPosi().z/ BSize;
    else
        z_coor_collision_blocks = current_figure -> GetAheadMostBlockPosi().z/ BSize;

    for ( int i = FieldBeginY; i < FieldEndY; ++i )
        for ( int j = FieldBeginX; j < FieldEndX; ++j )
            if ( ( z_coor_collision_blocks + shift >= 0  ) && ( z_coor_collision_blocks + shift < Width ) && ( field[ j ][ i ][ z_coor_collision_blocks + shift ] != NULL ) )
              collision_block.push_back( Block( *field[ j ][ i ][ z_coor_collision_blocks + shift ] ) );

    current_figure -> SetPosi( current_figure -> GetPosi() + Point3Di( 0, 0, shift * BSize ) );
    if ( current_figure -> CheckToCollisonWithBlocks( collision_block ) )
        current_figure -> SetPosi( current_figure -> GetPosi() - Point3Di( 0, 0, shift * BSize ) );
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
                if ( ( ( upper_bound_x - lower_bound_x ) / 2 % BSize != 0 ) ^
                 ( ( upper_bound_y - lower_bound_y ) / 2 % BSize != 0 ) )
		figure_pos_correct_vec = Point3Di( 1, 1, 0 );
	    break;
	case PlaneZY :
                if ( ( ( upper_bound_y - lower_bound_y ) / 2 % BSize != 0 ) ^
                 ( ( upper_bound_z - lower_bound_z ) / 2 % BSize != 0 ) )
		figure_pos_correct_vec = Point3Di( 0, 1, 1 );
	     break;
	default :
                if ( ( ( upper_bound_x - lower_bound_x ) / 2 % BSize != 0 ) ^
                 ( ( upper_bound_z - lower_bound_z ) / 2 % BSize != 0 ) )
		figure_pos_correct_vec = Point3Di( 1, 0, 1 );
	}
	figure_pos_correct_vec = side * figure_pos_correct_vec;
	figure_pos_correct_step = 0;

        if ( CheckToRotate( plane, rotating_angle ) )
            rotating = false;
    }
}

void Game :: DropDownFigure()
{
    game_speed = SeventhSpeed;
}

float Game :: Sign( float a )
{
    if ( a < eps )
	return -1;
    return 1;
}

//float Game :: SetAngleToRange( float angle )
//{
//    int sign = Sign( angle );

//    if ( !InRange( sign * angle,   ) )
//        return angle - sign;
//    return angle;
//}

void Game :: ChangeCameraPosition( float x, float y )
{
    float next_teta       = camera_position.teta  + CameraPosChangeKoeff * y;
    float next_alpha      = camera_position.alpha + CameraPosChangeKoeff * x;
    float next_alpha_sign = Sign( next_alpha );

    if ( InRange( next_teta, -pi / 2, pi / 2 ) )
        camera_position.teta  = next_teta;

    if ( !InRange( next_alpha_sign * next_alpha, 0.0, 2 * pi ) )
        next_alpha -= next_alpha_sign * 2 * pi;
    camera_position.alpha = next_alpha;

    printf( "%f %f\n", camera_position.teta, camera_position.alpha );
}

Point3Df Game :: GetCameraPosition()
{
    float cos_teta = cos( camera_position.teta );
    return Point3Df(  cos_teta * sin( camera_position.alpha ) * CameraRadius, sin( camera_position.teta ) * CameraRadius, cos_teta * cos( camera_position.alpha ) * CameraRadius );
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

 void Game :: SetGameSpeed( GameSpeed new_game_speed )
 {
    game_speed = new_game_speed;
 }

