#include <QGLWidget>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include "Game.h"

const float     Game :: SafetyDistance = 16.0 * Block :: BlockSize * Block :: BlockSize;
const float	Game :: CameraPosChangeKoeff =	0.01f;
const float	Game :: CameraRadius = 800.0f;
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

    int block_ind = 0;

    for ( int i = 0; i < FieldHeight; ++i )
    {
        for ( int j = 0; j < FieldLength; ++j )
        {
            field_block_border[ block_ind++ ] = new Block( Block :: BlockSize / 2 + j * Block :: BlockSize, Block :: BlockSize / 2 + i * Block :: BlockSize, -Block :: BlockSize / 2 , materials[ 3 ] );
            field_block_border[ block_ind++ ] = new Block( Block :: BlockSize / 2 + j * Block :: BlockSize, Block :: BlockSize / 2 + i * Block :: BlockSize,  Block :: BlockSize / 2 + FieldWidth * Block :: BlockSize, materials[ 3 ] );
        }

        for ( int j = 0; j < FieldWidth; ++j )
        {
            field_block_border[ block_ind++ ] = new Block( -Block :: BlockSize / 2, Block :: BlockSize / 2 + i * Block :: BlockSize, Block :: BlockSize / 2 + j* Block :: BlockSize, materials[ 3 ] );
            field_block_border[ block_ind++ ] = new Block(  Block :: BlockSize / 2 + FieldLength * Block :: BlockSize, Block :: BlockSize / 2 + i * Block :: BlockSize, Block :: BlockSize / 2 + j * Block :: BlockSize, materials[ 3 ] );
        }
    }

    for ( int i = 0; i < FieldLength; ++i )
        for ( int j = 0; j < FieldWidth; ++j )
            field_block_border[ block_ind++ ] =  new Block( FieldLowerBoundX + Block :: BlockSize / 2 + i * Block :: BlockSize, -Block :: BlockSize / 2, FieldLowerBoundZ + Block :: BlockSize / 2 + j * Block :: BlockSize, materials[ 3 ] );

}

void Game :: Start()
{	
    figure_pos_correct_step =	0;
    last_mouse_position	    =   Point3Df( 0.0f, 0.0f, 0.0f );
    camera_position	    =   SphericalCoor( pi / 4, pi / 4 );
    current_figure	    =   GetNewFigure();
    figure_start_pos_y	    =   current_figure -> GetPosByYi();
    figure_down_steps	    =   0;
    count_of_blocks	    =	0;
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

bool Game :: CheckToRotate( RotatePlane rotating_plane, float rotating_angle )
{
    Point3Df    current_figure_pos  = current_figure -> GetPosf();
    Block*      collision_field     = new Block [ count_of_blocks ];
    Figure*     collis_figure       = new Figure( *current_figure );
    bool        collision           = false;
    float       final_angle         = 0.0f;
    float       distance            = 0.0f;
    int         collapse_sim_count  = collapse_steps_count;

    int block_ind = 0;
      for ( int k = 0; k < FieldHeight; ++k )
        for ( int i = 0; i < FieldLength; ++i )
            for ( int j = 0; j < FieldWidth; ++j )
                if ( field[ i ][ j ][ k ] != NULL )
                    collision_field[ block_ind++ ] = Block( *field[ i ][ j ][ k ] );

    for ( unsigned int i = 0; i < RotateStepsCount; i++ )
    {
            //Manipulation with Figure
            if ( i <= Block :: BlockSize / 2 )
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
                    for ( int i = 0; i < count_of_blocks; i++ )
                        collision_field[ i ].SetPosi( collision_field[ i ].GetPosi() - Point3Di( 0, SeventhSpeed, 0 ) );
                else
                    for ( int i = 0; i < count_of_blocks; i++ )
                        collision_field[ i ].SetPosi( collision_field[ i ].GetPosi() -
                            Point3Di( 0, ( collision_field[ i ].GetPosByYi() - SeventhSpeed + Block :: BlockSize / 2 ) / ( Block :: BlockSize / 2 ) * ( Block :: BlockSize / 2 ), 0 ) );
            }


            collis_figure -> SetVerAbsCoor();

            //Collision with blocks on field
            for ( int i = 0; i < count_of_blocks ; i++ )
            {
                distance = ( current_figure_pos - collision_field[ i ].GetPosf() ).Length();
                if ( distance < SafetyDistance - eps )
                {
                    collision_field[ i ].SetVerAbsCoor();
                    if ( collis_figure -> IsIntersectWithBlock( collision_field[ i ] ) )
                    {
                        collision = true;
                        break;
                    }
                    collision_field[ i ].SetVerRelCoor();
                }
             }

            //Collision with walls
            for ( int i = 0; ( i < BorderBlocksCount ) && ( !collision ) ; i++ )
            {
                distance = ( current_figure_pos - field_block_border[ i ] -> GetPosf() ).Length();
                if ( distance < SafetyDistance - eps )
                {
                    field_block_border[ i ] -> SetVerAbsCoor();
                    if ( collis_figure -> IsIntersectWithBlock( *field_block_border[ i ] ) )
                    {
                        collision = true;
                        break;
                    }
                    field_block_border[ i ] -> SetVerRelCoor();
                }
            }

            collis_figure -> SetVerRelCoor();

    }

    delete [] collision_field;
    delete collis_figure;

    return collision;
}
void Game :: CheckToCollapse()
{
    bool    full	= false;
    int	    full_levels = 0;

    for ( int k = 0; k < FieldHeight; ++k )
    {
	full = true;
	for ( int i = 0; i < FieldLength; ++i )
	    for ( int j = 0; j < FieldWidth; ++j )
		if ( field[ i ][ j ][ k ] == NULL )
		{
		    full = false;
		    break;
		}
	if ( full )
	    full_levels++;
    }

    if ( full_levels > 0 )
    {
        for ( int k = 0; k < FieldHeight; k++ )
	{
            if ( k < full_levels )
		for ( int i = 0; i < FieldLength; ++i )
		    for ( int j = 0; j < FieldWidth; ++j )
		    {
			 delete field[ i ][ j ][ k ];
			field[ i ][ j ][ k ] = NULL;
		    }
	    else
		for ( int i = 0; i < FieldLength; ++i )
		    for ( int j = 0; j < FieldWidth; ++j )
		    {
                        field[ i ][ j ][ k - full_levels + 1 ] = field[ i ][ j ][ k ];
			field[ i ][ j ][ k ] = NULL;
		    }
	}
	collapse_steps_count = ( full_levels * Block :: BlockSize ) / SeventhSpeed + 1;
	collapse = true;
        count_of_blocks	-= FieldLength * FieldWidth * full_levels;
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
            for ( int k = 0; k < FieldHeight; k++ )
		for ( int i = 0; i < FieldLength; ++i )
		    for ( int j = 0; j < FieldWidth; ++j )
			if ( field[ i ][ j ][ k ] != NULL )
			    field[ i ][ j ][ k ] -> SetPosi( field[ i ][ j ][ k ] -> GetPosi() - Point3Di( 0, SeventhSpeed, 0 ) );
	}
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
								  ( block_pos.y - SeventhSpeed + Block :: BlockSize / 2 ) / ( Block :: BlockSize / 2 ) * ( Block :: BlockSize / 2 ),
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
            block_pos = figure_pos + current_figure -> GetBlockPosByIndexi( i );
	    field_index_by_length = block_pos.x / Block :: BlockSize;
	    field_index_by_height = block_pos.y / Block :: BlockSize;
	    field_index_by_width  = block_pos.z / Block :: BlockSize;

            if ( block_pos.y - Block :: BlockSize / 2 <= Game :: FieldLowerBoundY )
            {
                is_game = false;
                break;
            }

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
                block_pos        = figure_pos + current_figure -> GetBlockPosByIndexi( i );
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
	    CheckToCollapse();
	    delete current_figure;
	    current_figure    = next_figure;
	    next_figure	      = GetNewFigure();
	    figure_down_steps = 0;
	    game_speed	      = FirstSpeed;
	    is_game	      = true;
            count_of_blocks   += Figure :: BlocksCount;
	}    

        //Rotate the figure
	if ( rotating )
	{
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
    //for ( int i = 0; i < BorderBlocksCount; i++ )
      //  field_block_border[ i ] -> Draw();
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

        if ( CheckToRotate( plane, rotating_angle ) )
            rotating = false;
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

 void Game :: SetGameSpeed( GameSpeed new_game_speed )
 {
    game_speed = new_game_speed;
 }

