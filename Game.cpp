#include <QGLWidget>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include "Game.h"

const int       Game :: SafetyDistance = 16.0 * BSize * BSize;
const float	Game :: CameraPosChangeKoeff =	0.01f;
const float	Game :: CameraRadius = 1000.0f;
float		Game :: light_position[ 4 ];
Figures         Game :: game_figures[ Game :: FiguresMaxCnt ];

void Game :: InitializeStaticData()
{
    light_position[ 0 ] = LightPosByX;
    light_position[ 1 ] = LightPosByY;
    light_position[ 2 ] = LightPosByZ;
    light_position[ 3 ] = 1.0f;

    game_figures[ 0 ] = IFigure;
    game_figures[ 1 ] = JFigure;
    game_figures[ 2 ] = LFigure;
    game_figures[ 3 ] = OFigure;
    game_figures[ 4 ] = SFigure;
    game_figures[ 5 ] = TFigure;
    game_figures[ 6 ] = ZFigure;
}

Game :: Game() : QObject()
{
    InitializeStaticData();

    present_figures.clear();
    for ( int i = 0; i < FiguresMaxCnt; i++ )
        present_figures.push_back( game_figures[ i ] );
    Start();
}

void Game :: Start()
{
    Point3Di fig_pos;
    Point3Di block_pos;
    int      block_length_pos;
    int      block_width_pos;
    figure_pos_correct_step     =   0;
    last_mouse_position         =   Point3Df( 0.0f, 0.0f, 0.0f );
    camera_position             =   SphericalCoor( pi / 4, pi / 4 );
    mpCurrentFigure             =   GetNewFigure();
    figure_start_pos_y          =   mpCurrentFigure -> GetPosByYi();
    mFigureDownSteps            =   0;
    count_of_blocks_on_field	=   0;
    next_figure                 =   GetNewFigure();
    is_pos_change               =   false;
    rotating_step               =   0;
    mGameSpeed                  =   FirstSpeed;
    rotating                    =   false;
    score                       =   0;
    mIsGame                     =   true;
    mIsCollapse                 =   false;
    count_of_shift_checks       =   0;

    srand( time( 0 ) );

    for ( int i = 0; i < Length; ++i )
        for ( int j = 0; j < Width; ++j )
            for ( int k = 0; k < Height; ++k )
                mpField[ i ][ k ][ j ] = NULL;

     for ( int i = 1; i < Height - 1; ++i )
    {
        for ( int j = 0; j < Length; ++j )
        {
            mpField[ j ][ i ][ 0 ]         = new Block( BSize / 2 + j * BSize,
                                                        BSize / 2 + i * BSize,
                                                        BSize / 2 ,
                                                        materials[ BottomFiguresMaterial ] );

            mpField[ j ][ i ][ Width - 1 ] = new Block( BSize / 2 + j * BSize,
                                                        BSize / 2 + i * BSize,
                                                        BSize / 2 + ( Width - 1 ) * BSize,
                                                        materials[ BottomFiguresMaterial ] );

            mBoardBlocks.push_back( mpField[ j ][ i ][ 0 ] );
            mBoardBlocks.push_back( mpField[ j ][ i ][ Width - 1 ] );
        }

        for ( int j = 0; j < Width; ++j )
        {
            mpField[ 0 ][ i ][ j ]          = new Block( BSize / 2,
                                                         BSize / 2 + i * BSize,
                                                         BSize / 2 + j * BSize,
                                                         materials[ BottomFiguresMaterial ] );

            mpField[ Length - 1 ][ i ][ j ] = new Block( BSize / 2 + ( Length - 1 ) * BSize,
                                                         BSize / 2 + i * BSize,
                                                         BSize / 2 + j * BSize,
                                                         materials[ BottomFiguresMaterial ] );

            mBoardBlocks.push_back( mpField[ 0 ][ i ][ j ] );
            mBoardBlocks.push_back( mpField[ Length - 1 ][ i ][ j ] );
        }
    }

     for ( int i = 0; i < Length; ++i )
        for ( int j = 0; j < Width; ++j )
        {
            mpField[ i ][ 0 ][ j ] = new Block( BSize / 2 + i * BSize,
                                                BSize / 2,
                                                BSize / 2 + j * BSize,
                                                materials[ BottomFiguresMaterial ] );

            mBoardBlocks.push_back( mpField[ i ][ 0 ][ j ] );
        }

     for ( int i = 0; i < Length; ++i )
        for ( int j = 0; j < Width; ++j )
        {
            mpField[ i ][ Height - 1 ][ j ] = new Block( BSize / 2 + i * BSize,
                                                         BSize / 2 + ( Height - 1 ) * BSize,
                                                         BSize / 2 + j * BSize,
                                                         materials[ BottomFiguresMaterial ] );

            mBoardBlocks.push_back( mpField[ i ][ Height - 1 ][ j ] );
        }

    //Select blocks
    fig_pos = mpCurrentFigure -> GetPosi();
    for ( unsigned int i = 0; i < Figure :: BlocksCount; i++ )
    {
        block_pos = mpCurrentFigure -> GetBlockPosByIndexi( i ) + fig_pos;
        block_length_pos = block_pos.x / Game :: BSize;
        block_width_pos  = block_pos.z / Game :: BSize;

        select_blocks_pos[ i ] = Point3Di( block_length_pos, 0, block_width_pos );
        select_blocks_materials[ i ] = mpField[ block_length_pos ][ 0 ][ block_width_pos ] -> GetMaterial();
    }

    for ( unsigned int i = 0; i < Figure :: BlocksCount; i++ )
        mpField[ select_blocks_pos[ i ].x ][ select_blocks_pos[ i ].y ][ select_blocks_pos[ i ].z ] -> SetMaterial( materials[ SelectFigureMaterial ] );
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

    next_figure = new Figure( 0.0f, 0.0f, 0.0f, present_figures[ 3/*rand() % present_figures.size()*/ ], materials[ rand() % FiguresMaterials ] );
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

bool Game :: CheckToRotate( RotatePlane aRotatingPlane, float aRotatingAngle )
{
    std :: vector < Point3Di >  sim_field_blocks_pos;
    std :: vector < Block* >    sim_field_blocks;
    int                         sim_field_blocks_cnt        = 0;
    Figure*                     sim_collis_figure           = new Figure( *mpCurrentFigure );
    Point3Di                    sim_block_pos;
    Point3Di                    sim_figure_pos              = sim_collis_figure -> GetPosi();
    bool                        sim_is_collision            = false;
    float                       sim_final_angle             = 0.0f;
    float                       sim_distance                = 0.0f;
    int                         sim_collapse_steps_cnt      = mCollapseStepsCnt;
    int                         sim_figure_down_steps       = mFigureDownSteps;

    //Collection blocks in SafetyDistance radius
    for ( int k = FieldBeginY; k < FieldEndY; ++k )
        for ( int i = FieldBeginX; i < FieldEndX; ++i )
            for ( int j = FieldBeginZ; j < FieldEndZ; ++j )
                if ( mpField[ i ][ k ][ j ] != NULL )
                {
                    sim_block_pos = mpField[ i ][ k ][ j ] -> GetPosi();
                    sim_distance = ( sim_figure_pos.x - sim_block_pos.x ) * ( sim_figure_pos.x - sim_block_pos.x ) +
                        ( sim_figure_pos.z - sim_block_pos.z ) * ( sim_figure_pos.z - sim_block_pos.z );
                    if ( sim_distance < SafetyDistance )
                    {
                        sim_field_blocks.push_back( mpField[ i ][ k ][ j ] );
                        sim_field_blocks_pos.push_back( sim_block_pos );
                    }
                }

    sim_field_blocks_cnt = sim_field_blocks.size();

    for ( unsigned int i = 0; ( i < RotateStepsCount ) && ( !sim_is_collision ); ++i )
    {
        sim_figure_down_steps++;
        if ( sim_figure_down_steps == 2 )
        {
            sim_collis_figure -> SetPosByYi( sim_collis_figure -> GetPosByYi() - mGameSpeed );
            sim_figure_down_steps = 0;
        }

        if ( i <= BSize / 2 )
            sim_collis_figure -> SetPosi( sim_collis_figure -> GetPosi() + figure_pos_correct_vec );

        sim_final_angle = i * aRotatingAngle;

        switch ( rotating_plane )
        {
        case PlaneXY :
            sim_collis_figure -> RotateOnXY( sim_final_angle, false );
            break;
        case PlaneZY :
            sim_collis_figure -> RotateOnZY( sim_final_angle, false );
            break;
        default :
            sim_collis_figure -> RotateOnZX( sim_final_angle, false );
        }

        //Manipulating witn field blocks at the collpase
        if ( sim_collapse_steps_cnt > 0 )
        {
            sim_collapse_steps_cnt--;

            if ( sim_collapse_steps_cnt > 0 )
                for ( int i = 0; i < sim_field_blocks_cnt; i++ )
                    sim_field_blocks[ i ] -> SetPosi( sim_field_blocks[ i ] -> GetPosi() - Point3Di( 0, SeventhSpeed, 0 ) );
            else
                for ( int i = 0; i < sim_field_blocks_cnt; i++ )
                    sim_field_blocks[ i ] -> SetPosi( sim_field_blocks[ i ] -> GetPosi() -
                        Point3Di( 0, ( sim_field_blocks[ i ] -> GetPosByYi() - SeventhSpeed + BSize / 2 ) / ( BSize / 2 ) * ( BSize / 2 ), 0 ) );
        }

        sim_is_collision = sim_collis_figure -> CheckToCollisonWithBlocks( sim_field_blocks ) ||
                           sim_collis_figure -> CheckToCollisonWithBlocks( mBoardBlocks );
    }

    for ( int i = 0; i < sim_field_blocks_cnt; i++ )
        sim_field_blocks[ i ] -> SetPosi( sim_field_blocks_pos[ i ] );

    delete sim_collis_figure;

    return sim_is_collision;
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
                if ( mpField[ i ][ k ][ j ] == NULL )
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
            if ( k < full_levels + FieldBeginY )
                for ( int i = FieldBeginX; i < FieldEndX; ++i )
                    for ( int j = FieldBeginZ; j < FieldEndZ; ++j )
		    {
                        delete mpField[ i ][ k ][ j ];
                        mpField[ i ][ k ][ j ] = NULL;
		    }
	    else
                for ( int i = FieldBeginX; i < FieldEndX; ++i )
                    for ( int j = FieldBeginZ; j < FieldEndZ; ++j )
		    {
                        mpField[ i ][ k - full_levels ][ j ] = mpField[ i ][ k ][ j ];
                        mpField[ i ][ k ][ j ] = NULL;
		    }
	}
        mCollapseStepsCnt = ( full_levels * BSize ) / FirstSpeed + 1;
        mIsCollapse = true;
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

    if ( !mIsGame )
	return;

    if ( mIsCollapse )
    {
        mCollapseStepsCnt--;
        if ( mCollapseStepsCnt < 0 )
            mIsCollapse = false;

        for ( int k = FieldBeginY; k < FieldEndY; k++ )
            for ( int i = FieldBeginX; i < FieldEndX; ++i )
                for ( int j = FieldBeginZ; j < FieldEndZ; ++j )
                    if ( mpField[ i ][ k ][ j ] != NULL )
                    {
                        if ( mIsCollapse )
                            mpField[ i ][ k ][ j ] -> SetPosi( mpField[ i ][ k ][ j ] -> GetPosi() - Point3Di( 0, FirstSpeed, 0 ) );
                        else
                        {
                            block_pos =  mpField[ i ][ k ][ j ] -> GetPosi();
                            mpField[ i ][ k ][ j ] -> SetPosi( block_pos.x,
                                                             ( block_pos.y - FirstSpeed + BSize / 2 ) / ( BSize / 2 ) * ( BSize / 2 ),
                                                             block_pos.z );
                        }

                    }
    }

    if ( mIsGame )
    {
        mFigureDownSteps++;
        if ( mFigureDownSteps == 2 )
        {
            mpCurrentFigure -> SetPosi( mpCurrentFigure -> GetPosi() - Point3Di( 0, mGameSpeed, 0 ) );
            mFigureDownSteps = 0;
        }

        if ( ( !rotating ) && ( count_of_shift_checks > 0 ) )
        {
            count_of_shift_checks--;
            switch ( shift_axis )
                {
                    case XAxis :
                        ShiftFigureByXAxis( shift_direction );
                        break;
                    case ZAxis :
                        ShiftFigureByZAxis( shift_direction );
                        break;
                    default :
                        break;
                }
        }

        //Detecting when figure must stop
        figure_pos = mpCurrentFigure -> GetPosi();
        for ( unsigned int  i = 0; ( i < Figure :: BlocksCount ) && ( mIsGame ); i++ )
	{
            block_pos = figure_pos + mpCurrentFigure -> GetBlockPosByIndexi( i );
            field_index_by_length = block_pos.x / BSize;
            field_index_by_height = block_pos.y / BSize;
            field_index_by_width  = block_pos.z / BSize;

            for ( int j = field_index_by_height - 1; j >= FieldBeginY - 1; --j )
                if ( mpField[ field_index_by_length ][ j ][ field_index_by_width ] != NULL )
                {
                    if ( block_pos.y - BSize / 2 <= Game :: FieldLowerBoundY + j * BSize )
                    {
                        mIsGame = false;
                        break;
                    }
                }
	}

        if ( !mIsGame )
	{
            mpCurrentFigure -> SetPosByYi( ( mpCurrentFigure -> GetPosByYi() + mGameSpeed ) / ( BSize / 2 ) * ( BSize / 2  ) );
            figure_pos = mpCurrentFigure -> GetPosi();
            for ( unsigned int  i = 0; i < Figure :: BlocksCount; i++ )
	    {
                block_pos        = figure_pos + mpCurrentFigure -> GetBlockPosByIndexi( i );
                field_index_by_length = block_pos.x / BSize;
                field_index_by_width  = block_pos.z / BSize;
                field_index_by_height = block_pos.y / BSize;
                mpField[ field_index_by_length ][ field_index_by_height ][ field_index_by_width ]
		    = new Block( block_pos.x,
				 block_pos.y,
				 block_pos.z,
                                 mpCurrentFigure -> GetBlockMaterialByIndex( i )
				);
	    }
	    CheckToCollapse();
            delete mpCurrentFigure;
            mpCurrentFigure    = next_figure;
            ChangeSelectBlocks();
	    next_figure	      = GetNewFigure();
            mFigureDownSteps  = 0;
            mGameSpeed	      = FirstSpeed;
            mIsGame	      = true;
            count_of_blocks_on_field += Figure :: BlocksCount;
            if ( ( !mIsCollapse ) && ( field_index_by_height == FieldEndY - 2 ) )
		exit( 0 );
	}    

        //Rotate the figure
	if ( rotating )
	{
            rotating_step++;
            if ( rotating_step <= BSize / 2 )
                mpCurrentFigure -> SetPosi( mpCurrentFigure -> GetPosi() + figure_pos_correct_vec );
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
                mpCurrentFigure -> RotateOnXY( final_angle, state );
		break;
	    case PlaneZY :
                mpCurrentFigure -> RotateOnZY( final_angle, state );
		break;
	    default :
                mpCurrentFigure -> RotateOnZX( final_angle, state );
	    }

            if ( !rotating )
                ChangeSelectBlocks();
	}
    }
}

void Game :: DrawField() const
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

void Game :: DrawBlocksOnTheField() const
{
    Point3Df rel_position( 0.0f, 0.0f, 0.0f );

    for ( int i = FieldBeginX; i < FieldEndX; ++i )
	for ( int j = FieldBeginZ; j < FieldEndZ; ++j )
            for ( int k = FieldBeginY - 1; k < FieldEndY; ++k )
                if ( mpField[ i ][ k ][ j ] != NULL )
                    mpField[ i ][ k ][ j ] -> Draw( rel_position );
}

void Game :: DrawInterface() const
{

}

void Game :: DrawWorld() const
{
    DrawField();
    DrawBlocksOnTheField();
    if ( mpCurrentFigure != NULL )
        mpCurrentFigure -> Draw();
      //DrawInterface();
}

float* Game :: GetLightPosition() const
{
    return light_position;    
}


void Game :: ShiftFigureByXAxis( ShiftDirection shift )
{
    std :: vector < Block* >     collision_block;
    int                         x_coor_collision_blocks;

    if ( shift < 0 )
        x_coor_collision_blocks = mpCurrentFigure -> GetLeftMostBlockPosi().x / BSize;
    else
        x_coor_collision_blocks = mpCurrentFigure -> GetRightMostBlockPosi().x / BSize;

    for ( int i = FieldBeginY; i < FieldEndY; ++i )
        for ( int j = FieldBeginZ; j < FieldEndZ; ++j )
            if ( ( x_coor_collision_blocks + shift >= 0 ) && ( x_coor_collision_blocks + shift < Length ) && ( mpField[ x_coor_collision_blocks + shift ][ i ][ j ] != NULL ) )
                collision_block.push_back( mpField[ x_coor_collision_blocks + shift ][ i ][ j ] );

    mpCurrentFigure -> SetPosi( mpCurrentFigure -> GetPosi() + Point3Di( shift * BSize, 0, 0 ) );
    if ( mpCurrentFigure -> CheckToCollisonWithBlocks( collision_block ) ) //Cancel shift
        mpCurrentFigure -> SetPosi( mpCurrentFigure -> GetPosi() - Point3Di( shift * BSize, 0, 0 ) );
    else
    {
        count_of_shift_checks = 0;
        ChangeSelectBlocks();
    }
}

void Game :: ShiftFigureByZAxis( ShiftDirection shift )
{
    std :: vector < Block* >     collision_block;
    int                         z_coor_collision_blocks;

    if ( shift < 0 )
        z_coor_collision_blocks = mpCurrentFigure -> GetBackMostBlockPosi().z/ BSize;
    else
        z_coor_collision_blocks = mpCurrentFigure -> GetAheadMostBlockPosi().z/ BSize;

    for ( int i = FieldBeginY; i < FieldEndY; ++i )
        for ( int j = FieldBeginX; j < FieldEndX; ++j )
            if ( ( z_coor_collision_blocks + shift >= 0  ) && ( z_coor_collision_blocks + shift < Width ) && ( mpField[ j ][ i ][ z_coor_collision_blocks + shift ] != NULL ) )
              collision_block.push_back( mpField[ j ][ i ][ z_coor_collision_blocks + shift ] );

    mpCurrentFigure -> SetPosi( mpCurrentFigure -> GetPosi() + Point3Di( 0, 0, shift * BSize ) );
    if ( mpCurrentFigure -> CheckToCollisonWithBlocks( collision_block ) ) //Cancel shift
        mpCurrentFigure -> SetPosi( mpCurrentFigure -> GetPosi() - Point3Di( 0, 0, shift * BSize ) );
    else
    {
        count_of_shift_checks = 0;
        ChangeSelectBlocks();
    }
}

void Game :: Rotate( RotatePlane plane, RotateSide side )
{
    if ( !rotating )
    {
	rotating_angle = pi / 2 / RotateStepsCount * side;
	rotating_plane = plane;
	rotating = true;

        //Point3Df figure_pos = mpCurrentFigure -> GetPosition();
        int lower_bound_x = mpCurrentFigure -> LowerBoundXi();
        int upper_bound_x = mpCurrentFigure -> UpperBoundXi();
        int lower_bound_y = mpCurrentFigure -> LowerBoundYi();
        int upper_bound_y = mpCurrentFigure -> UpperBoundYi();
        int lower_bound_z = mpCurrentFigure -> LowerBoundZi();
        int upper_bound_z = mpCurrentFigure -> UpperBoundZi();

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
    mGameSpeed = SeventhSpeed;
}


void Game :: ChangeCameraPosition( float x, float y )
{
    float next_teta       = camera_position.teta  + CameraPosChangeKoeff * y;
    float next_alpha      = camera_position.alpha - CameraPosChangeKoeff * x;
    int   next_alpha_sign = Geometry :: Sign( next_alpha );

    if ( Geometry :: InRange( next_teta, -pi / 2, pi / 2 ) )
        camera_position.teta  = next_teta;

    if ( !Geometry :: InRange( next_alpha_sign * next_alpha, 0.0, 2 * pi ) )
        next_alpha -= next_alpha_sign * 2 * pi;
    camera_position.alpha = next_alpha;   
}

Point3Df Game :: GetCameraPosition() const
{
    float cos_teta = cos( camera_position.teta );
    return Point3Df(   ( Game :: Length / 2  ) * BSize + cos_teta * sin( camera_position.alpha ) * CameraRadius, sin( camera_position.teta ) * CameraRadius,  ( Game :: Width / 2  ) * BSize + cos_teta * cos( camera_position.alpha ) * CameraRadius );
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
    mGameSpeed = new_game_speed;
 }

void Game :: ChangeSelectBlocks()
{    
    Point3Di fig_pos;
    Point3Di block_pos;
    int      block_length_pos;
    int      block_width_pos;

    for ( unsigned int i = 0; i < Figure :: BlocksCount; i++ )
         if ( mpField[ select_blocks_pos[ i ].x ][ select_blocks_pos[ i ].y ] [ select_blocks_pos[ i ].z ] != NULL )
                    mpField[ select_blocks_pos[ i ].x ][ select_blocks_pos[ i ].y ] [ select_blocks_pos[ i ].z ] -> SetMaterial( select_blocks_materials[ i ] );

    fig_pos = mpCurrentFigure -> GetPosi();
    for ( unsigned int i = 0; i < Figure :: BlocksCount; i++ )
    {
        block_pos = mpCurrentFigure -> GetBlockPosByIndexi( i ) + fig_pos;
        block_length_pos = block_pos.x / Game :: BSize;
        block_width_pos  = block_pos.z / Game :: BSize;

        for ( int j = FieldEndY - 1; j >= 0; j-- )
            if ( mpField[ block_length_pos ][ j ][ block_width_pos ] != NULL )
            {
                select_blocks_pos[ i ] = Point3Di( block_length_pos, j, block_width_pos );
                select_blocks_materials[ i ] = mpField[ block_length_pos ][ j ][ block_width_pos ] -> GetMaterial();
                break;
            }
    }

     for ( unsigned int i = 0; i < Figure :: BlocksCount; i++ )
        mpField[ select_blocks_pos[ i ].x ][ select_blocks_pos[ i ].y ] [ select_blocks_pos[ i ].z ] -> SetMaterial( materials[ SelectFigureMaterial ] );
}

void Game :: SetShift( Axises axis, ShiftDirection direction )
{
   shift_axis = axis;
   shift_direction = direction;
   count_of_shift_checks = 5;
}

void Game :: ChangePause()
{
    mIsGame = !mIsGame;
}

bool Game :: IsPause()
{
    return !mIsGame;
}

void Game :: GetSelectFigures( bool* select_figures )
{
    for ( int i = 0; i < FiguresMaxCnt; i++ )
        select_figures[ i ] = false;
    for ( std :: vector < Figures > :: iterator it = present_figures.begin(); it != present_figures.end(); ++it )
        select_figures[ ( *it ) ] = true;
}

void Game :: SetSelectFigures( bool* select_figures )
{
     present_figures.clear();
     for ( int i = 0; i < FiguresMaxCnt; i++ )
        if ( select_figures[ i ] )
            present_figures.push_back( game_figures[ i ] );
}
