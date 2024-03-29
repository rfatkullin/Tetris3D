#include <QGLWidget>
#include <math.h>
#include <stdio.h>
#include <time.h>
#include "Game.h"
#include "PhisEntity.h"

float               Game :: msLightPosition[ 4 ];
Figures             Game :: msGameFigures[ Game :: FIGURES_MAX_CNT ];

const char* const   Game :: MESSAGES[ Game :: MESSAGES_CNT ] = {  "Empty!",
                                                              "New level!",
                                                              "Collapse!",
                                                              "Could not rotate - collision!",
                                                              "Could not shift - collision!",
                                                              "New game!",
                                                              "Game over!",
                                                              "Save game!",
                                                              "Load game!"
                                                           };

const char* const   Game :: SAVE_FILE = "save.dat";

const unsigned int  Game :: LEVELS_SPEED[ Game :: LEVELS_CNT ] = { 1,
                                                                    2,
                                                                    4,
                                                                    8,
                                                                    10,
                                                                    12,
                                                                    14
                                                                  };

const unsigned int  Game :: LEVELS_SCORE[ Game :: LEVELS_CNT ] = {  0,
                                                                     200,
                                                                     400,
                                                                     600,
                                                                     800,
                                                                     1000,
                                                                     1200
                                                                   };

const unsigned int  Game :: MAX_GAME_SPEED = LEVELS_SPEED[ Game :: LEVELS_CNT - 1 ];

Game :: ~Game()
{
    //Delete all blocks
    for ( int i = FIELD_BEGIN_X; i < FIELD_END_X; ++i )
	for ( int k = FIELD_BEGIN_Y; k < FIELD_END_Y; ++k )
	    for ( int j = FIELD_BEGIN_Z; j < FIELD_END_Z; ++j )
		delete mpField[ i ][ k ][ j ];

    //Delete figure
    delete mpCurrentFigure;   

    //Stop media
    mpAmbientMusicObject    -> stop();
    mpBlockFallSoundObject  -> stop();

}

void Game :: InitializeStaticData()
{
    msLightPosition[ 0 ] = LightPosByX;
    msLightPosition[ 1 ] = LightPosByY;
    msLightPosition[ 2 ] = LightPosByZ;
    msLightPosition[ 3 ] = 1.0f;

    msGameFigures[ 0 ] = IFigure;
    msGameFigures[ 1 ] = JFigure;
    msGameFigures[ 2 ] = LFigure;
    msGameFigures[ 3 ] = OFigure;
    msGameFigures[ 4 ] = SFigure;
    msGameFigures[ 5 ] = TFigure;
    msGameFigures[ 6 ] = ZFigure;
}

Game :: Game( QObject* apParent ) : QObject( apParent )
{
    mScore                  = 0;
    mFieldBlocksCnt         = 0;
    mpCurrentFigure         = NULL;
    mGameLevel              = 1;
    mIsSound		    = true;
    mIsGameOver             = false;
    mIsAmbientMusic         = true;
    mpAmbientMusicObject    = new Phonon :: MediaObject( this );
    mpBlockFallSoundObject  = new Phonon :: MediaObject( this );
    mpAudioAmbient	    = new Phonon :: AudioOutput( Phonon :: MusicCategory, this );
    mpAudioSound	    = new Phonon :: AudioOutput( Phonon :: MusicCategory, this );

    Phonon :: createPath( mpAmbientMusicObject, mpAudioAmbient );
    Phonon :: createPath( mpBlockFallSoundObject, mpAudioSound );

    mpAmbientMusicObject    -> setCurrentSource( Phonon :: MediaSource( "Sounds/Ambient.mp3") );
    mpBlockFallSoundObject  -> setCurrentSource( Phonon :: MediaSource( "Sounds/Fall.wav") );

    connect( mpAmbientMusicObject,   SIGNAL( finished() ), SLOT( PlayAmbientMusic() ) );
    connect( mpBlockFallSoundObject, SIGNAL( finished() ), SLOT( PrepairFallSound() ) );

    InitializeStaticData();
    CreateBorderBlocks();

    mPresentFigures.clear();
    for ( int i = 0; i < FIGURES_MAX_CNT; i++ )
	mPresentFigures.push_back( msGameFigures[ i ] );

    mpSaveFile = new QFile( SAVE_FILE, this );
    mSaveStream.setDevice( mpSaveFile );
}

void Game :: CreateBorderBlocks()
{
    for ( unsigned int i = 0; i < LENGTH; ++i )
        for ( unsigned int j = 0; j < WIDTH; ++j )
            for ( unsigned int k = 0; k < HEIGHT; ++k )
		mpField[ i ][ k ][ j ] = NULL;

    for ( unsigned int i = 1; i < HEIGHT - 1; ++i )
    {
        for ( unsigned int j = 0; j < LENGTH; ++j )
	{
            mpField[ j ][ i ][ 0 ]         = new Block( ( int )HALF_BSIZE + j * BSIZE,
                                                        ( int )HALF_BSIZE + i * BSIZE,
                                                        ( int )HALF_BSIZE ,
							PhisEntity :: msMaterials[ BOTTOM_FIGURES_MATERIALS ] );

            mpField[ j ][ i ][ WIDTH - 1 ] = new Block( ( int )( HALF_BSIZE + j * BSIZE ),
                                                        ( int )( HALF_BSIZE + i * BSIZE ),
                                                        ( int )( HALF_BSIZE + ( WIDTH - 1 ) * BSIZE ),
							PhisEntity :: msMaterials[ BOTTOM_FIGURES_MATERIALS ] );

	    mBoardBlocks.push_back( mpField[ j ][ i ][ 0 ] );
	    mBoardBlocks.push_back( mpField[ j ][ i ][ WIDTH - 1 ] );
	}

	for ( int j = 0; j < WIDTH; ++j )
	{
            mpField[ 0 ][ i ][ j ]          = new Block( ( int )( BSIZE / 2 ),
                                                         ( int )( HALF_BSIZE + i * BSIZE ),
                                                         ( int )( HALF_BSIZE + j * BSIZE ),
							 PhisEntity :: msMaterials[ BOTTOM_FIGURES_MATERIALS ] );

            mpField[ LENGTH - 1 ][ i ][ j ] = new Block( ( int )( HALF_BSIZE + ( LENGTH - 1 ) * BSIZE ),
                                                         ( int )( HALF_BSIZE + i * BSIZE ),
                                                         ( int )( HALF_BSIZE + j * BSIZE ),
							 PhisEntity :: msMaterials[ BOTTOM_FIGURES_MATERIALS ] );

	    mBoardBlocks.push_back( mpField[ 0 ][ i ][ j ] );
	    mBoardBlocks.push_back( mpField[ LENGTH - 1 ][ i ][ j ] );
	}
    }

     for ( int i = 0; i < LENGTH; ++i )
	for ( int j = 0; j < WIDTH; ++j )
	{
            mpField[ i ][ 0 ][ j ] = new Block( ( int )HALF_BSIZE + i * BSIZE,
                                                ( int )BSIZE / 2,
                                                ( int )HALF_BSIZE + j * BSIZE,
						PhisEntity :: msMaterials[ BOTTOM_FIGURES_MATERIALS ] );

	    mBoardBlocks.push_back( mpField[ i ][ 0 ][ j ] );
	}

     for ( int i = 0; i < LENGTH; ++i )
	for ( int j = 0; j < WIDTH; ++j )
	{
            mpField[ i ][ HEIGHT - 1 ][ j ] = new Block( ( int )( HALF_BSIZE + i * BSIZE ),
                                                         ( int )( HALF_BSIZE + ( HEIGHT - 1 ) * BSIZE ),
                                                         ( int )( HALF_BSIZE + j * BSIZE ),
							 PhisEntity :: msMaterials[ BOTTOM_FIGURES_MATERIALS ] );

	    mBoardBlocks.push_back( mpField[ i ][ HEIGHT - 1 ][ j ] );
	}

}

void Game :: Start()
{
    Point3Di fig_pos;
    Point3Di block_pos;
    int      block_length_pos;
    int      block_width_pos;

    mFigurePosCorrectStep      = 0;
    mFieldBlocksCnt            = 0;
    mpCurrentFigure            = GetNewFigure();
    mFigureDownSteps           = 0;    
    mRotatingStep              = 0;
    mGameLevel                 = 1;
    mGameSpeed                 = LEVELS_SPEED[ mGameLevel - 1 ];
    mIsRotate                  = false;
    mScore                     = 0;
    mIsCollapse                = false;
    mShiftChecksCnt            = 0;
    mIsGameOver                = false;

    srand( time( 0 ) );

    for ( int i = FIELD_BEGIN_X; i < FIELD_END_X; ++i )
	for ( int k = FIELD_BEGIN_Y; k < FIELD_END_Y; ++k )
	    for ( int j = FIELD_BEGIN_Z; j < FIELD_END_Z; ++j )
		mpField[ i ][ k ][ j ] = NULL;

    //Select blocks
    fig_pos = mpCurrentFigure -> GetPosi();
    for ( unsigned int i = 0; i < Figure :: BlocksCount; i++ )
    {
        block_pos = mpCurrentFigure -> GetBlockPosByIndexi( i ) + fig_pos;
        block_length_pos = block_pos.mX / Game :: BSIZE;
        block_width_pos  = block_pos.mZ / Game :: BSIZE;

        mSelectBlocksPos[ i ] = Point3Di( block_length_pos, 0, block_width_pos );
        mSelectBlocksMaterials[ i ] = mpField[ block_length_pos ][ 0 ][ block_width_pos ] -> GetMaterial();
    }

    for ( unsigned int i = 0; i < Figure :: BlocksCount; i++ )
        mpField[ mSelectBlocksPos[ i ].mX ][ mSelectBlocksPos[ i ].mY ][ mSelectBlocksPos[ i ].mZ ]
            -> SetMaterial( PhisEntity :: msMaterials[ SELECT_FIGURES_MATERIALS ] );

     if ( mIsAmbientMusic )
        mpAmbientMusicObject -> play();

     mMessagesList.clear();
     mMessagesList.push_back( NEW_GAME );
 }

void Game :: End()
{
    for ( int i = FIELD_BEGIN_X; i < FIELD_END_X; ++i )
	for ( int k = FIELD_BEGIN_Y; k < FIELD_END_Y; ++k )
	    for ( int j = FIELD_BEGIN_Z; j < FIELD_END_Z; ++j )
		if ( mpField[ i ][ k ][ j ] != NULL )
		{
		    delete mpField[ i ][ k ][ j ];
		    mpField[ i ][ k ][ j ] = NULL;
		}

    TurnOffSelecting();

    for ( ComponentsVec :: iterator comp_it = mCollapseComponents.begin(); comp_it != mCollapseComponents.end(); ++comp_it )
        ( *comp_it ).clear();
    mCollapseComponents.clear();

    if ( mpCurrentFigure != NULL )
        delete mpCurrentFigure;

    mpCurrentFigure = NULL;

}

Point2Df Game :: GetFigurePositionOnXZ( int aWidthX, int aWidthZ )
{
    if ( ( aWidthX / 2 ) % BSIZE == 0 )
    {
        if ( ( aWidthZ / 2 ) % BSIZE == 0 )
	    return Point2Df( 0.0f, 0.0f );
	else
            return Point2Df( 0.0f, HALF_BSIZE );
    }
    else
    {
        if ( ( aWidthZ / 2 ) % BSIZE == 0 )
            return Point2Df( BSIZE / 2, 0.0f );
	else
            return Point2Df( BSIZE / 2, HALF_BSIZE );
    }
}

Figure* Game :: GetNewFigure()
{
    Point2Df    figure_pos;
    int         lower_bound_x;
    int     	upper_bound_x;
    int         lower_bound_y;
    int         upper_bound_y;
    int         lower_bound_z;
    int         upper_bound_z;
    int         y_position  = FIGURE_START_Y_POS  * BSIZE;

    Figure* new_figure = new Figure( 0.0f, 0.0f, 0.0f,
                               mPresentFigures[ rand() % mPresentFigures.size() ],
                               PhisEntity :: msMaterials[ rand() % FIGURES_MATERIALS ] );

    lower_bound_x = new_figure -> LowerBoundXi();
    upper_bound_x = new_figure -> UpperBoundXi();
    lower_bound_y = new_figure -> LowerBoundYi();
    upper_bound_y = new_figure -> UpperBoundYi();
    lower_bound_z = new_figure -> LowerBoundZi();
    upper_bound_z = new_figure -> UpperBoundZi();

    figure_pos = GetFigurePositionOnXZ( upper_bound_x - lower_bound_x, upper_bound_z - lower_bound_z );

    if ( ( upper_bound_y - lower_bound_y ) % 2 != 0 )
        y_position += BSIZE / 2;

    new_figure -> SetPosi( BSIZE * LENGTH / 2 + figure_pos.mX, y_position, BSIZE * WIDTH / 2 + figure_pos.mY );

    return new_figure;
}

bool Game :: CheckToRotate( )
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
    int                         sim_figure_down_steps       = mFigureDownSteps;

    //Collection blocks in SafetyDistance radius
    for ( int k = FIELD_BEGIN_Y; k < FIELD_END_Y; ++k )
        for ( int i = FIELD_BEGIN_X; i < FIELD_END_X; ++i )
            for ( int j = FIELD_BEGIN_Z; j < FIELD_END_Z; ++j )
                if ( mpField[ i ][ k ][ j ] != NULL )
                {
                    sim_block_pos = mpField[ i ][ k ][ j ] -> GetPosi();
                    sim_distance = Geometry :: Square( sim_figure_pos.mX - sim_block_pos.mX ) +
                                   Geometry :: Square( sim_figure_pos.mZ - sim_block_pos.mZ );
                    if ( sim_distance < SAFETY_DISTANCE )
                    {
                        sim_field_blocks.push_back( mpField[ i ][ k ][ j ] );
                        sim_field_blocks_pos.push_back( sim_block_pos );
                    }
                }

    sim_field_blocks_cnt = sim_field_blocks.size();

    for ( unsigned int i = 0; ( i < ROTATE_STEPS_COUNT ) && ( !sim_is_collision ); ++i )
    {
        sim_figure_down_steps++;
        if ( sim_figure_down_steps == 2 )
        {
            sim_collis_figure -> SetPosByYi( sim_collis_figure -> GetPosByYi() - mGameSpeed );
            sim_figure_down_steps = 0;
        }

        if ( i <= HALF_BSIZE )
            sim_collis_figure -> SetPosi( sim_collis_figure -> GetPosi() + mFigurePosCorrectVec );

        sim_final_angle = i * mRotatingAngle;

        switch ( mRotatingPlane )
        {
        case PLANE_XY :
            sim_collis_figure -> RotateOnXY( sim_final_angle, false );
            break;
        case PLANE_ZY :
            sim_collis_figure -> RotateOnZY( sim_final_angle, false );
            break;
        default :
            sim_collis_figure -> RotateOnZX( sim_final_angle, false );
        }      

        sim_is_collision = sim_collis_figure -> CheckToCollisonWithBlocks( sim_field_blocks ) ||
                           sim_collis_figure -> CheckToCollisonWithBlocks( mBoardBlocks );
    }

    for ( int i = 0; i < sim_field_blocks_cnt; i++ )
        sim_field_blocks[ i ] -> SetPosi( sim_field_blocks_pos[ i ] );

    delete sim_collis_figure;

    return sim_is_collision;
}

int MinI( int a, int b )
{
    if ( a > b )
	return b;
    return a;
}

void Game :: PickUpComponent( int aI, int aK, int aJ )
{
    mpCollapseComponent[ aI ][ aK ][ aJ ] = 1;
    mCollapseComponents.back().push_back(
	std :: pair< Block*, std :: pair < int, int > > ( mpField[ aI ][ aK ][ aJ ],
							  std :: pair< int, int >( aI, aJ ) ) );

    if ( ( aI - 1 > FIELD_BEGIN_X )			&&
	 (  mpField[ aI - 1 ][ aK ][ aJ ] != NULL )	&&
	 ( mpCollapseComponent[ aI - 1 ][ aK ][ aJ ] == 0 ) )
	    PickUpComponent( aI - 1, aK, aJ );

    if ( ( aI + 1 < FIELD_END_X )			&&
	 (  mpField[ aI + 1 ][ aK ][ aJ ] != NULL )	&&
	 ( mpCollapseComponent[ aI + 1 ][ aK ][ aJ ] == 0 ) )
	    PickUpComponent( aI + 1, aK, aJ );

     if ( ( aK - 1 > FIELD_BEGIN_Y )			&&
	  (  mpField[ aI ][ aK - 1 ][ aJ ] != NULL ) &&
	  ( mpCollapseComponent[ aI ][ aK - 1 ][ aJ ] == 0 ) )
	    PickUpComponent( aI, aK - 1, aJ );

    if ( ( aK + 1 < FIELD_END_Y )			&&
	 (  mpField[ aI ][ aK + 1 ][ aJ ] != NULL )  &&
	 ( mpCollapseComponent[ aI ][ aK + 1 ][ aJ ] == 0 ) )
	    PickUpComponent( aI, aK + 1, aJ );

    if ( ( aJ - 1 > FIELD_BEGIN_Z )			&&
	 (  mpField[ aI ][ aK ][ aJ - 1 ] != NULL )	&&
	 ( mpCollapseComponent[ aI ][ aK ][ aJ - 1 ] == 0 ) )
	    PickUpComponent( aI, aK, aJ - 1 );

    if ( ( aJ + 1 < FIELD_END_Z )			&&
	 (  mpField[ aI ][ aK ][ aJ + 1 ] != NULL )	&&
	 ( mpCollapseComponent[ aI ][ aK ][ aJ + 1 ] == 0 ) )
	    PickUpComponent( aI, aK, aJ + 1 );
}

void Game :: PrepairToCollapse()
{
   for ( int k = FIELD_BEGIN_Y; k < FIELD_END_Y; ++k )
	for ( int i = FIELD_BEGIN_X; i < FIELD_END_X; ++i )
	    for ( int j = FIELD_BEGIN_Z; j < FIELD_END_Z; ++j )
		mpCollapseComponent[ i ][ k ][ j ] = 0;

    mFallingComponentsCnt = 0;
    for ( int k = FIELD_BEGIN_Y; k < FIELD_END_Y; ++k )
	for ( int i = FIELD_BEGIN_X; i < FIELD_END_X; ++i )
	    for ( int j = FIELD_BEGIN_Z; j < FIELD_END_Z; ++j )
		if ( ( mpField[ i ][ k ][ j ] != NULL ) && ( mpCollapseComponent[ i ][ k ][ j ] == 0 ) )
                {
                    mFallingComponentsCnt++;
                    mCollapseComponents.push_back( BlocksVec() );
		    PickUpComponent( i, k, j );
                }
    for ( int k = FIELD_BEGIN_Y; k < FIELD_END_Y; ++k )
        for ( int i = FIELD_BEGIN_X; i < FIELD_END_X; ++i )
            for ( int j = FIELD_BEGIN_Z; j < FIELD_END_Z; ++j )
                mpField[ i ][ k ][ j ] = NULL;
}

bool Game :: CheckToCollapse()
{
    bool    full = false;

    for ( int k = FIELD_BEGIN_Y; k < FIELD_END_Y; ++k )
    {
        full = true;
         for ( int i = FIELD_BEGIN_X; ( i < FIELD_END_X ) && ( full ); ++i )
            for ( int j = FIELD_BEGIN_Z; j < FIELD_END_Z; ++j )
                if ( mpField[ i ][ k ][ j ] == NULL )
                {
                    full = false;
                    break;
                }

         if ( full )
         {
            for ( int i = FIELD_BEGIN_X; i < FIELD_END_X; ++i )
                for ( int j = FIELD_BEGIN_Z; j < FIELD_END_Z; ++j )
                {
                    delete mpField[ i ][ k ][ j ];
                    mpField[ i ][ k ][ j ] = NULL;
                }

            mFieldBlocksCnt -= ONE_LEVEL_BLOCKS_CNT;
            mScore          += ONE_LEVEL_BLOCKS_CNT;
            mIsCollapse     = true;
         }
    }

    if ( mIsCollapse )
        PrepairToCollapse();

    return mIsCollapse;
}

void Game :: CollapseStep()
{
    bool stop;
    int field_y_coor;
    int world_y_coor;
    int i;
    int j;

    for ( ComponentsVec :: iterator comp_it = mCollapseComponents.begin(); comp_it != mCollapseComponents.end(); ++comp_it )
    {
        stop = false;

        for ( BlocksVec :: iterator block_it = comp_it -> begin(); ( block_it != comp_it -> end() ) && ( !stop ); ++block_it  )
        {
            world_y_coor = ( block_it -> first ) -> GetPosByYi() - MAX_GAME_SPEED;
            field_y_coor =   world_y_coor / BSIZE;
            i = ( block_it -> second ).first;
            j = ( block_it -> second ).second;

	   for ( int k = field_y_coor - 1; field_y_coor >= FIELD_BEGIN_Y - 1; --field_y_coor )
                if ( mpField[ i ][ k ][ j ] != NULL )
                {
                    if (  world_y_coor - HALF_BSIZE <= Game :: FIELD_LOWER_BOUND_Y + k * BSIZE )
                    {
                        stop = true;
                        break;
                    }
                }
        }

        for ( BlocksVec :: iterator block_it = comp_it -> begin(); block_it != comp_it -> end(); ++block_it  )
        {
            if ( stop )
            {
		world_y_coor = ( block_it -> first ) -> GetPosByYi();
		field_y_coor = world_y_coor / BSIZE;
		world_y_coor = world_y_coor / HALF_BSIZE * HALF_BSIZE;
		i = ( block_it -> second ).first;
                j = ( block_it -> second ).second;
                ( block_it -> first ) -> SetPosByYi( world_y_coor );
                mpField[ i ][ field_y_coor ][ j ] = block_it -> first;
            }
            else
		( block_it -> first ) -> SetPosByYi( ( block_it -> first ) -> GetPosByYi() - MAX_GAME_SPEED );
	}

        if ( stop )
        {
            comp_it -> clear();
            mFallingComponentsCnt--;
        }
    }

    if ( mFallingComponentsCnt == 0 )
    {
        mIsCollapse = false;
        ChangeSelectBlocks();
        CheckToCollapse();
    }
}

void Game :: NextStep()
{
    Point3Di	figure_pos;
    Point3Di	block_pos;
    float	final_angle		= 0.0f;
    bool	is_to_commit		= false;
    bool        figure_stopped          = false;
    bool        shift_res               = false;
    int		field_index_by_length	= 0;
    int		field_index_by_height	= 0;
    int		field_index_by_width	= 0;

    if ( mIsCollapse )
        CollapseStep();

    mFigureDownSteps++;
    if ( mFigureDownSteps == 2 )
    {
        mpCurrentFigure -> SetPosi( mpCurrentFigure -> GetPosi() - Point3Di( 0, mGameSpeed, 0 ) );
        mFigureDownSteps = 0;
    }

    if ( ( !mIsRotate ) && ( mShiftChecksCnt > 0 ) )
    {
        mShiftChecksCnt--;
        switch ( mShiftAxis )
            {
                case X_AXIS :
                    shift_res = ShiftFigureByXAxis( mShiftDirection );
                    break;
                case Z_AXIS :
                    shift_res = ShiftFigureByZAxis( mShiftDirection );
                    break;
                default :
                    break;
            }

        if ( ( mShiftChecksCnt == 0 ) && ( !shift_res ) )
            mMessagesList.push_back( COULDNT_SHIFT_COLLISION );
    }

    //Detecting when figure must stop
    figure_pos = mpCurrentFigure -> GetPosi();
    for ( unsigned int  i = 0; ( i < Figure :: BlocksCount ) && ( !figure_stopped ); i++ )
    {
        block_pos = figure_pos + mpCurrentFigure -> GetBlockPosByIndexi( i );
        field_index_by_length = block_pos.mX / BSIZE;
        field_index_by_height = block_pos.mY / BSIZE;
        field_index_by_width  = block_pos.mZ / BSIZE;

        for ( int j = field_index_by_height - 1; j >= FIELD_BEGIN_Y - 1; --j )
            if ( mpField[ field_index_by_length ][ j ][ field_index_by_width ] != NULL )
            {
                if ( block_pos.mY - HALF_BSIZE <= Game :: FIELD_LOWER_BOUND_Y + j * BSIZE )
                {
                    figure_stopped = true;
                    break;
                }
            }
    }

    if ( figure_stopped )
    {
        mpCurrentFigure -> SetPosByYi( ( mpCurrentFigure -> GetPosByYi() + mGameSpeed ) / ( HALF_BSIZE ) * ( HALF_BSIZE  ) );
        figure_pos = mpCurrentFigure -> GetPosi();

        for ( unsigned int  i = 0; i < Figure :: BlocksCount; i++ )
        {
            block_pos        = figure_pos + mpCurrentFigure -> GetBlockPosByIndexi( i );
            field_index_by_length = block_pos.mX / BSIZE;
            field_index_by_width  = block_pos.mZ / BSIZE;
            field_index_by_height = block_pos.mY / BSIZE;
            mpField[ field_index_by_length ][ field_index_by_height ][ field_index_by_width ]
                = new Block( block_pos.mX,
                             block_pos.mY,
                             block_pos.mZ,
                             mpCurrentFigure -> GetBlockMaterialByIndex( i )
                            );
        }

        if ( mIsSound )
            mpBlockFallSoundObject -> play();

        if ( CheckToCollapse() )
            mMessagesList.push_back( Game :: COLLAPSE );

        delete mpCurrentFigure;

        mpCurrentFigure     = GetNewFigure();

        for ( int i = mGameLevel; i < LEVELS_CNT; i++ )
            if ( mScore >= LEVELS_SCORE[ i ] )
            {
                mMessagesList.push_back( NEW_LEVEL );
                mGameLevel = i + 1;
            }

        mGameSpeed = LEVELS_SPEED[ mGameLevel - 1 ];
        mFigureDownSteps    = 0;
        mScore              += Figure :: BlocksCount;
        mFieldBlocksCnt     += Figure :: BlocksCount;

        ChangeSelectBlocks();

        //GameOver!!!
        if ( ( !mIsCollapse ) && ( field_index_by_height >= FIELD_END_Y - 3 ) )
        {
            delete mpCurrentFigure;
            mpCurrentFigure = NULL;
            mIsGameOver = true;
            mMessagesList.push_back( GAME_OVER );
        }

    }

    //Rotate the figure
    if ( mIsRotate )
    {
        mRotatingStep++;

        //Correcting figure position
        if ( mRotatingStep <= HALF_BSIZE )
            mpCurrentFigure -> SetPosi( mpCurrentFigure -> GetPosi() + mFigurePosCorrectVec );

        final_angle = mRotatingStep * mRotatingAngle;

        if ( mRotatingStep == ROTATE_STEPS_COUNT )
        {
            mRotatingStep   = 0;
            mIsRotate       = false;
            is_to_commit    = true;
        }

        switch ( mRotatingPlane )
        {
        case PLANE_XY :
            mpCurrentFigure -> RotateOnXY( final_angle, is_to_commit );
            break;
        case PLANE_ZY :
            mpCurrentFigure -> RotateOnZY( final_angle, is_to_commit );
            break;
        default :
            mpCurrentFigure -> RotateOnZX( final_angle, is_to_commit );
        }

        if ( !mIsRotate )
            ChangeSelectBlocks();
    }
}

void Game :: DrawField() const
{
    //Camera( +, +, + ) and looking ( 0, 0, 0 )
    glDisable( GL_LIGHTING );

    glBegin( GL_LINES );
	glColor3f( 0.5f, 0.5f, 0.5f );
    //FarGridZ
        for ( int i = FIELD_BEGIN_X - 1; i < FIELD_END_X; i++ )
        {
                glVertex3f( FIELD_LOWER_BOUND_X + i * BSIZE, FIELD_LOWER_BOUND_Y, FIELD_LOWER_BOUND_Z );
                glVertex3f( FIELD_LOWER_BOUND_X + i * BSIZE, FIELD_UPPER_BOUND_Y, FIELD_LOWER_BOUND_Z );
        }

        for ( int i = FIELD_BEGIN_Y - 1; i < FIELD_END_Y; i++ )
        {
                glVertex3f( FIELD_LOWER_BOUND_X, FIELD_LOWER_BOUND_Y + i * BSIZE, FIELD_LOWER_BOUND_Z );
                glVertex3f( FIELD_UPPER_BOUND_X, FIELD_LOWER_BOUND_Y + i * BSIZE, FIELD_LOWER_BOUND_Z );
        }
    //LowerGrid
	glColor3f( 1.0f, 1.0f, 1.0f );
        for ( int i = FIELD_BEGIN_Z - 1; i < FIELD_END_Z; i++ )
	{
                glVertex3f( FIELD_LOWER_BOUND_X, FIELD_LOWER_BOUND_Y, FIELD_LOWER_BOUND_Z + i * BSIZE );
                glVertex3f( FIELD_UPPER_BOUND_X, FIELD_LOWER_BOUND_Y, FIELD_LOWER_BOUND_Z + i * BSIZE );
	}

        for ( int i = FIELD_BEGIN_X - 1; i < FIELD_END_X; i++ )
	{
                glVertex3f(  FIELD_LOWER_BOUND_X + i * BSIZE, FIELD_LOWER_BOUND_Y, FIELD_LOWER_BOUND_Z );
                glVertex3f(  FIELD_LOWER_BOUND_X + i * BSIZE, FIELD_LOWER_BOUND_Y, FIELD_UPPER_BOUND_Z );
	}
    //LeftGrid
        glColor3f( 0.5f, 0.0f, 0.0f );
        for ( int i = FIELD_BEGIN_Z - 1; i < FIELD_END_Z; i++ )
        {
                glVertex3f(  FIELD_LOWER_BOUND_X, FIELD_LOWER_BOUND_Y, FIELD_LOWER_BOUND_Z + i * BSIZE );
                glVertex3f(  FIELD_LOWER_BOUND_X, FIELD_UPPER_BOUND_Y, FIELD_LOWER_BOUND_Z + i * BSIZE );
        }
        for ( int i = FIELD_BEGIN_Y - 1; i < FIELD_END_Y; i++ )
        {
                glVertex3f( FIELD_LOWER_BOUND_X, FIELD_LOWER_BOUND_Y + i * BSIZE, FIELD_LOWER_BOUND_Z );
                glVertex3f( FIELD_LOWER_BOUND_X, FIELD_LOWER_BOUND_Y + i * BSIZE, FIELD_UPPER_BOUND_Z );
        }
    glEnd();

    glEnable( GL_LIGHTING );
}

void Game :: DrawBlocksOnTheField() const
{
    Point3Df rel_position( 0.0f, 0.0f, 0.0f );

    for ( int i = FIELD_BEGIN_X; i < FIELD_END_X; ++i )
        for ( int j = FIELD_BEGIN_Z; j < FIELD_END_Z; ++j )
            for ( int k = FIELD_BEGIN_Y - 1; k < FIELD_END_Y; ++k )
                if ( mpField[ i ][ k ][ j ] != NULL )
                    mpField[ i ][ k ][ j ] -> Draw( rel_position );
}

void Game :: DrawWorld() const
{
    DrawField();
    DrawBlocksOnTheField();
    if ( mpCurrentFigure != NULL )
        mpCurrentFigure -> Draw();

    for ( ComponentsVec :: const_iterator comp_it = mCollapseComponents.begin(); comp_it != mCollapseComponents.end(); ++comp_it )
	for ( BlocksVec :: const_iterator block_it = comp_it -> begin(); block_it != comp_it -> end(); ++block_it  )
            ( block_it -> first ) -> Draw();
}

float* Game :: GetLightPosition() const
{
    return msLightPosition;
}


bool Game :: ShiftFigureByXAxis( ShiftDirection aShift )
{
    std :: vector < Block* >    collision_block;
    bool                        result = false;
    int                         x_coor_collision_blocks;

    if ( aShift < 0 )
        x_coor_collision_blocks = mpCurrentFigure -> GetLeftMostBlockPosi().mX / BSIZE;
    else
        x_coor_collision_blocks = mpCurrentFigure -> GetRightMostBlockPosi().mX / BSIZE;

    for ( int i = FIELD_BEGIN_Y; i < FIELD_END_Y; ++i )
        for ( int j = FIELD_BEGIN_Z; j < FIELD_END_Z; ++j )
            if ( ( x_coor_collision_blocks + aShift >= 0 ) && ( x_coor_collision_blocks + aShift < LENGTH ) && ( mpField[ x_coor_collision_blocks + aShift ][ i ][ j ] != NULL ) )
                collision_block.push_back( mpField[ x_coor_collision_blocks + aShift ][ i ][ j ] );

    mpCurrentFigure -> SetPosi( mpCurrentFigure -> GetPosi() + Point3Di( aShift * BSIZE, 0, 0 ) );
    if ( mpCurrentFigure -> CheckToCollisonWithBlocks( collision_block ) ) //Cancel shift
    {
        mpCurrentFigure -> SetPosi( mpCurrentFigure -> GetPosi() - Point3Di( aShift * BSIZE, 0, 0 ) );
        result = false;
    }
    else
    {
        mShiftChecksCnt = 0;
        ChangeSelectBlocks();
        result = true;
    }

    return result;
}

bool Game :: ShiftFigureByZAxis( ShiftDirection aShift )
{
    std :: vector < Block* >    collision_block;
    bool                        result = false;
    int                         z_coor_collision_blocks;

    if ( aShift < 0 )
        z_coor_collision_blocks = mpCurrentFigure -> GetBackMostBlockPosi().mZ/ BSIZE;
    else
        z_coor_collision_blocks = mpCurrentFigure -> GetAheadMostBlockPosi().mZ/ BSIZE;

    for ( int i = FIELD_BEGIN_Y; i < FIELD_END_Y; ++i )
        for ( int j = FIELD_BEGIN_X; j < FIELD_END_X; ++j )
            if ( ( z_coor_collision_blocks + aShift >= 0  ) && ( z_coor_collision_blocks + aShift < WIDTH ) && ( mpField[ j ][ i ][ z_coor_collision_blocks + aShift ] != NULL ) )
              collision_block.push_back( mpField[ j ][ i ][ z_coor_collision_blocks + aShift ] );

    mpCurrentFigure -> SetPosi( mpCurrentFigure -> GetPosi() + Point3Di( 0, 0, aShift * BSIZE ) );
    if ( mpCurrentFigure -> CheckToCollisonWithBlocks( collision_block ) ) //Cancel shift
    {
        mpCurrentFigure -> SetPosi( mpCurrentFigure -> GetPosi() - Point3Di( 0, 0, aShift * BSIZE ) );
        result = false;
    }
    else
    {
        mShiftChecksCnt = 0;
        ChangeSelectBlocks();
        result = true;
    }

    return result;
}

void Game :: Rotate( RotatePlane aPlane, RotateSide aSide )
{
    if ( !mIsRotate )
    {
        mRotatingAngle = Geometry :: pi / 2 / ROTATE_STEPS_COUNT * aSide;
        mRotatingPlane = aPlane;
        mIsRotate = true;

        int lower_bound_x = mpCurrentFigure -> LowerBoundXi();
        int upper_bound_x = mpCurrentFigure -> UpperBoundXi();
        int lower_bound_y = mpCurrentFigure -> LowerBoundYi();
        int upper_bound_y = mpCurrentFigure -> UpperBoundYi();
        int lower_bound_z = mpCurrentFigure -> LowerBoundZi();
        int upper_bound_z = mpCurrentFigure -> UpperBoundZi();

        mFigurePosCorrectVec = Point3Di( 0, 0, 0 );
        switch ( mRotatingPlane )
	{
        case PLANE_XY :
                if ( ( ( upper_bound_x - lower_bound_x ) / 2 % BSIZE != 0 ) ^
                 ( ( upper_bound_y - lower_bound_y ) / 2 % BSIZE != 0 ) )
                mFigurePosCorrectVec = Point3Di( aSide, -1, 0 );
	    break;
        case PLANE_ZY :
                if ( ( ( upper_bound_y - lower_bound_y ) / 2 % BSIZE != 0 ) ^
                 ( ( upper_bound_z - lower_bound_z ) / 2 % BSIZE != 0 ) )
                mFigurePosCorrectVec = Point3Di( 0, -1, -aSide );
	     break;
	default :
                if ( ( ( upper_bound_x - lower_bound_x ) / 2 % BSIZE != 0 ) ^
                 ( ( upper_bound_z - lower_bound_z ) / 2 % BSIZE != 0 ) )
                mFigurePosCorrectVec = Point3Di( aSide, 0, aSide );
	}

        mFigurePosCorrectStep = 0;

        if ( CheckToRotate() )
        {
            mIsRotate = false;
            mMessagesList.push_back( COULDNT_ROTATE_COLLISION );
        }
    }
}

void Game :: DropDownFigure()
{
    mGameSpeed = MAX_GAME_SPEED;
}


void Game :: TurnOffSelecting()
{
    for ( unsigned int i = 0; i < Figure :: BlocksCount; i++ )
        if ( mpField[ mSelectBlocksPos[ i ].mX ][ mSelectBlocksPos[ i ].mY ] [ mSelectBlocksPos[ i ].mZ ] != NULL )
            mpField[ mSelectBlocksPos[ i ].mX ][ mSelectBlocksPos[ i ].mY ] [ mSelectBlocksPos[ i ].mZ ]
                -> SetMaterial( mSelectBlocksMaterials[ i ] );
}

void Game :: TurnOnSelecting()
{
    for ( unsigned int i = 0; i < Figure :: BlocksCount; i++ )
        if ( mpField[ mSelectBlocksPos[ i ].mX ][ mSelectBlocksPos[ i ].mY ] [ mSelectBlocksPos[ i ].mZ ] != NULL )
            mpField[ mSelectBlocksPos[ i ].mX ][ mSelectBlocksPos[ i ].mY ] [ mSelectBlocksPos[ i ].mZ ]
                -> SetMaterial( PhisEntity :: msMaterials[ SELECT_FIGURES_MATERIALS ] );
}

void Game :: ChangeSelectBlocks()
{    
    Point3Di fig_pos;
    Point3Di block_pos;
    int      block_length_pos;
    int      block_width_pos;
    int      block_height_pos;

    TurnOffSelecting();

    fig_pos = mpCurrentFigure -> GetPosi();
    for ( unsigned int i = 0; i < Figure :: BlocksCount; i++ )
    {
        block_pos = mpCurrentFigure -> GetBlockPosByIndexi( i ) + fig_pos;
        block_length_pos  = block_pos.mX / Game :: BSIZE;
        block_width_pos   = block_pos.mZ / Game :: BSIZE;
        block_height_pos  = block_pos.mY / Game :: BSIZE;

        for ( int j = block_height_pos - 1; j >= FIELD_BEGIN_Y - 1; j-- )
            if ( mpField[ block_length_pos ][ j ][ block_width_pos ] != NULL )
            {
                mSelectBlocksPos[ i ] = Point3Di( block_length_pos, j, block_width_pos );
                mSelectBlocksMaterials[ i ] = mpField[ block_length_pos ][ j ][ block_width_pos ]
                    -> GetMaterial();
                break;
            }
    }

     for ( unsigned int i = 0; i < Figure :: BlocksCount; i++ )
        mpField[ mSelectBlocksPos[ i ].mX ][ mSelectBlocksPos[ i ].mY ] [ mSelectBlocksPos[ i ].mZ ]
            -> SetMaterial( PhisEntity :: msMaterials[ SELECT_FIGURES_MATERIALS ] );
}

void Game :: SetShift( Axises aAxis, ShiftDirection aDirection )
{
   mShiftAxis = aAxis;
   mShiftDirection = aDirection;
   mShiftChecksCnt = 5;
}

void Game :: GetSelectFigures( bool* apSelectFigures )
{
    for ( int i = 0; i < FIGURES_MAX_CNT; i++ )
        apSelectFigures[ i ] = false;
    for ( std :: vector < Figures > :: iterator it = mPresentFigures.begin(); it != mPresentFigures.end(); ++it )
        apSelectFigures[ ( *it ) ] = true;
}

void Game :: SetSelectFigures( bool* apSelectFigures )
{
     mPresentFigures.clear();
     for ( int i = 0; i < FIGURES_MAX_CNT; i++ )
        if ( apSelectFigures[ i ] )
            mPresentFigures.push_back( msGameFigures[ i ] );
}

void Game :: AmbientMusicStateChange( bool aState )
{
    mIsAmbientMusic = aState;

    if ( !mIsAmbientMusic )
	mpAmbientMusicObject -> stop();
    else
	mpAmbientMusicObject -> play();
}

void Game :: SoundsStateChange( bool aState )
{
    mIsSound = aState;
}

void Game :: PrepairFallSound()
{
    mpBlockFallSoundObject -> setCurrentSource( Phonon :: MediaSource( "Sounds/Fall.wav") );
}

void Game :: PlayAmbientMusic()
{
    mpAmbientMusicObject -> setCurrentSource( Phonon :: MediaSource( "Sounds/Ambient.mp3" ) );
    mpAmbientMusicObject -> play();
}

bool Game :: IsGameOver()
{
    if ( mIsGameOver )
    {
        mIsGameOver = false;
        return true;
    }

    return mIsGameOver;
}

unsigned int Game :: GetLevel() const
{
    return mGameLevel;
}

const std :: vector < Game :: Messages >& Game :: GetMessages() const
{
    return mMessagesList;
}

bool Game :: AmbientMusicState()
{
    return mIsAmbientMusic;
}

int Game :: GetScore() const
{
    return mScore;
}

void Game :: ClearMessagesList()
{
    mMessagesList.clear();
}

void Game :: Save()
{
    TurnOffSelecting();
    mpSaveFile -> open( QIODevice :: WriteOnly );

    mSaveStream << mPresentFigures.size() << '\n';
    for ( std :: vector < Figures > :: iterator it = mPresentFigures.begin(); it != mPresentFigures.end(); ++it )
        mSaveStream << ( *it ) << '\t';
    mSaveStream << '\n';

    mSaveStream << mIsCollapse << '\n';

    if ( !mIsCollapse )
    {
        mSaveStream << mFieldBlocksCnt << '\n';
        for ( int i = FIELD_BEGIN_X; i < FIELD_END_X; ++i )
            for ( int k = FIELD_BEGIN_Y; k < FIELD_END_Y; ++k )
                for ( int j = FIELD_BEGIN_Z; j < FIELD_END_Z; ++j )
                    if ( mpField[ i ][ k ][ j ] != NULL )
                    {
                        mSaveStream << i << '\t' << k << '\t' << j << '\n';
                        mSaveStream << *mpField[ i ][ k ][ j ] << '\n';
                    }
    }
    else
    {
        mSaveStream << mCollapseComponents.size() << '\n';
        for ( ComponentsVec :: iterator comp_it = mCollapseComponents.begin(); comp_it != mCollapseComponents.end(); ++comp_it )
        {
            mSaveStream << ( *comp_it ).size() << '\n';
            for ( BlocksVec :: iterator block_it = ( *comp_it ).begin(); block_it != ( *comp_it ).end(); ++block_it )
            {
                mSaveStream <<  *( block_it -> first );
                mSaveStream << ( block_it -> second.first ) << '\t' << ( block_it -> second.second ) << '\n';
            }
        }
    }

    mSaveStream << *mpCurrentFigure;

    mSaveStream << mScore           << '\n';

    mSaveStream << mGameLevel       << '\n';

    mSaveStream << mGameSpeed       << '\n';

    mSaveStream << mRotatingAngle   << '\n';

    mSaveStream << mIsRotate        << '\n';

    mSaveStream << mFigureDownSteps << '\n';

    mSaveStream << mRotatingStep    << '\n';

    mSaveStream << mRotatingPlane   << '\n';

    mSaveStream << mFigurePosCorrectVec.mX << '\t'
                << mFigurePosCorrectVec.mY << '\t'
                << mFigurePosCorrectVec.mZ << '\n';

    mSaveStream << mFigurePosCorrectStep << '\n';

    mSaveStream << mFallingComponentsCnt << '\n';

    for ( int i = 0; i < MAX_SELECT_BLOCKS_CNT; i++ )
        mSaveStream << mSelectBlocksPos[ i ].mX << '\t'
                    << mSelectBlocksPos[ i ].mY << '\t'
                    << mSelectBlocksPos[ i ].mZ << '\n';

    for ( int i = 0; i < MAX_SELECT_BLOCKS_CNT; ++i )
        mSaveStream << mSelectBlocksMaterials[ i ] << '\t';

    mSaveStream << mShiftChecksCnt  << '\n';
    mSaveStream << mShiftAxis       << '\n';
    mSaveStream << mShiftDirection  << '\n';

    mpSaveFile -> close();
    mMessagesList.push_back( SAVE_GAME );

    TurnOnSelecting();
}

bool Game :: Load()
{
    Block*  tmp_block = NULL;
    Figures tmp_present_figure;
    int     tmp_first;
    int     tmp_second;
    int     present_figures_cnt = 0;
    int     component_block_cnt = 0;
    int     block_cnt           = 0;
    int     blocks_i_ind        = 0;
    int     blocks_k_ind        = 0;
    int     blocks_j_ind        = 0;


    if ( !mpSaveFile -> open( QIODevice :: ReadOnly ) )
        return false;

    End();

    mPresentFigures.clear();
    mSaveStream >> present_figures_cnt;
    for ( int i = 0; i < present_figures_cnt; ++i )
    {
        mSaveStream >> tmp_present_figure;
        mPresentFigures.push_back( tmp_present_figure );
    }

    mSaveStream >> mIsCollapse;

    if ( !mIsCollapse )
    {
        mSaveStream >> mFieldBlocksCnt;
        for ( int i = 0; i < mFieldBlocksCnt; ++i )
        {
            mSaveStream >> blocks_i_ind >> blocks_k_ind >> blocks_j_ind;
            mpField[ blocks_i_ind ][ blocks_k_ind ][ blocks_j_ind ] = new Block();
            mSaveStream >> *mpField[ blocks_i_ind ][ blocks_k_ind ][ blocks_j_ind ];
        }
    }
    else
    {
        mSaveStream >> component_block_cnt;
        for ( int i = 0; i < component_block_cnt; i++ )
        {
            mCollapseComponents.push_back( BlocksVec() );
            mSaveStream >> block_cnt;
            for ( int j = 0; j < block_cnt; j++ )
            {
                tmp_block = new Block();
                mSaveStream >> *tmp_block;
                mSaveStream >> tmp_first >> tmp_second;
                mCollapseComponents.back().push_back( std :: pair< Block*, std :: pair< int, int > >(
                                                  tmp_block,
                                                  std :: pair< int, int >( tmp_first, tmp_second ) ) );

            }
        }
    }

    mpCurrentFigure = GetNewFigure();
    mSaveStream >> *mpCurrentFigure;

    mSaveStream >> mScore;

    mSaveStream >> mGameLevel;

    mSaveStream >> mGameSpeed;

    mSaveStream >> mRotatingAngle;

    mSaveStream >> mIsRotate;

    mSaveStream >> mFigureDownSteps;

    mSaveStream >> mRotatingStep;

    mSaveStream >> mRotatingPlane;

    mSaveStream >> mFigurePosCorrectVec.mX
                >> mFigurePosCorrectVec.mY
                >> mFigurePosCorrectVec.mZ;

    mSaveStream >> mFigurePosCorrectStep;


    mSaveStream >> mFallingComponentsCnt;

    for ( int i = 0; i < MAX_SELECT_BLOCKS_CNT; i++ )
        mSaveStream >> mSelectBlocksPos[ i ].mX
                    >> mSelectBlocksPos[ i ].mY
                    >> mSelectBlocksPos[ i ].mZ;

    for ( int i = 0; i < MAX_SELECT_BLOCKS_CNT; ++i )
        mSaveStream >> mSelectBlocksMaterials[ i ];

    mSaveStream >> mShiftChecksCnt;
    mSaveStream >> mShiftAxis;
    mSaveStream >> mShiftDirection;

    mpSaveFile -> close();
    mMessagesList.push_back( LOAD_GAME );

    TurnOnSelecting();

    return true;
}

QTextStream& operator << ( QTextStream& aStream, const Game :: RotatePlane& aPlane )
{
    aStream << static_cast< int >( aPlane );

    return aStream;
}

QTextStream& operator >> ( QTextStream& aStream, Game :: RotatePlane& aPlane )
{
    int tmp_plane;
    aStream >> tmp_plane;
    aPlane = static_cast< Game :: RotatePlane >( tmp_plane );

    return aStream;
}

QTextStream& operator << ( QTextStream& aStream, const bool& aBoolValue)
{
    aStream << static_cast< int >( aBoolValue );

    return aStream;
}

QTextStream& operator >> ( QTextStream& aStream, bool& aBoolValue )
{
    int tmp_bool_value;

    aStream >> tmp_bool_value;
    aBoolValue = static_cast< bool > ( tmp_bool_value );

    return aStream;
}

QTextStream& operator << ( QTextStream& aStream, const Game :: Axises& aAxis)
{
    aStream << static_cast< int >( aAxis );

    return aStream;
}

QTextStream& operator >> ( QTextStream& aStream, Game :: Axises& aAxis )
{
    int tmp_axis;
    aStream >> tmp_axis;
    aAxis = static_cast< Game :: Axises >( aAxis );

    return aStream;
}

QTextStream& operator << ( QTextStream& aStream, const Game :: ShiftDirection& aShiftDirection)
{
    aStream << static_cast< int > (aShiftDirection );

    return aStream;
}

QTextStream& operator >> ( QTextStream& aStream, Game :: ShiftDirection& aShiftDirection )
{
    int tmp_shift_direction;
    aStream >> tmp_shift_direction;
    aShiftDirection = static_cast < Game :: ShiftDirection >( tmp_shift_direction );

    return aStream;
}

