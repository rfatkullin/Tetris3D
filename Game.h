#include <phonon/AbstractAudioOutput>
#include <phonon/MediaObject>
#include <phonon/AudioOutput>
#include <QObject>
#include <QSound>
#include "GameObjects.h"

#ifndef GAME_H
#define GAME_H

typedef std :: vector < std :: pair < Block*, std :: pair < int, int > > > BlocksVec;
typedef std :: vector < BlocksVec > ComponentsVec;

class Game : public QObject
{
    Q_OBJECT
public slots :
    void                        Start();
    void			AmbientMusicStateChange( bool aState );
    void			SoundsStateChange( bool aState );
private slots :
    void			PrepairFallSound();
    void			PlayAmbientMusic();
public:
    static const int            MESSAGES_CNT = 7;
    enum                        Messages { EMPTY = 0,
                                           NEW_LEVEL,
                                           COLLAPSE,
                                           COULDNT_ROTATE_COLLISION,
                                           COULDNT_SHIFT_COLLISION,
                                           NEW_GAME,
                                           GAME_OVER
                                          };

    enum                        RotatePlane { PLANE_XY,
                                              PLANE_ZY,
                                              PLANE_ZX };

    enum                        RotateSide { ROTATE_BY_CLOCK_WISE     = 1,
                                             ROTATE_BY_ANTI_CLOCKWISE = -1 };

    enum                        Axises { X_AXIS,
                                         Y_AXIS,
                                         Z_AXIS };

    enum                        ShiftDirection { SHIFT_DIRECTLY = 1,
                                                 SHIFT_BACK     = -1 };

    enum                        MaterialType { FIGURES_MATERIALS        = 6,
                                               SELECT_FIGURES_MATERIALS = 6,
                                               BOTTOM_FIGURES_MATERIALS = 7 };

    static const char*          MESSAGES[ MESSAGES_CNT ];

    static const int            LEVELS_CNT    = 7;
    static const unsigned int   LEVELS_SCORE[ LEVELS_CNT ];
    static const unsigned int   LEVELS_SPEED[ LEVELS_CNT ];
    static const unsigned int   MAX_GAME_SPEED;

    const unsigned int static	WIDTH                = 8;
    const unsigned int static	LENGTH               = 8;
    const unsigned int static   HEIGHT               = 20;
    const unsigned int static   FIGURES_MAX_CNT      = 7;
    const int static            FIELD_BEGIN_X        = 1;
    const int static            FIELD_BEGIN_Y        = 1;
    const int static            FIELD_BEGIN_Z        = 1;
    const int static            FIELD_END_X          = static_cast< int >( LENGTH - 1 );
    const int static            FIELD_END_Y          = static_cast< int >( HEIGHT - 1 );
    const int static            FIELD_END_Z          = static_cast< int >( WIDTH - 1 );
    const int static            FIELD_LOWER_BOUND_X  = FIELD_BEGIN_X * Block :: BLOCK_SIZE;
    const int static            FIELD_UPPER_BOUND_X  = FIELD_END_X   * Block :: BLOCK_SIZE;
    const int static            FIELD_LOWER_BOUND_Y  = FIELD_BEGIN_Y * Block :: BLOCK_SIZE;
    const int static            FIELD_UPPER_BOUND_Y  = FIELD_END_Y   * Block :: BLOCK_SIZE;
    const int static            FIELD_LOWER_BOUND_Z  = FIELD_BEGIN_Z * Block :: BLOCK_SIZE;
    const int static            FIELD_UPPER_BOUND_Z  = FIELD_END_Z   * Block :: BLOCK_SIZE;
    const int static            ONE_LEVEL_BLOCKS_CNT = ( FIELD_END_X - FIELD_BEGIN_X ) *
                                                           ( FIELD_END_Z - FIELD_BEGIN_Z );
    const int static            ROTATE_STEPS_COUNT   = 20;
    const int static            BSIZE                = ( int )Block :: BLOCK_SIZE;
    const int static            HALF_BSIZE           = ( int )Block :: BLOCK_SIZE / 2;
    const int static            FIGURE_START_Y_POS   = FIELD_END_Y - 2;

				Game( QObject* parent = 0 );
				~Game();

    unsigned int		GetLevel() const;
    float*			GetLightPosition() const;

    void			NextStep();
    void			DropDownFigure();

    void                        GetSelectFigures( bool* select_figures );
    void                        SetSelectFigures( bool* select_figures );
    void			SearchAndSetSteps( int i, int k, int j, int color );

    //Game state
    void			GameOver();
    void			End();
    bool                        IsGameOver();

    //Draw
    void			DrawWorld() const;
    void			DrawField() const;
    void			DrawBlocksOnTheField() const;

    //Figure control
    void                        SetShift( Axises axis, ShiftDirection direction );
    void			Rotate( RotatePlane plane, RotateSide side );

    bool                        AmbientMusicState();

    int                         GetScore() const;

    const std :: vector < Messages >&   GetMessages() const;
    void                                ClearMessagesList();

private:
    enum                        LightPosition  { LightPosByX = 400 , LightPosByY = 800, LightPosByZ = 300 };
    static const int            SAFETY_DISTANCE;


    static float                msLightPosition[ 4 ];
    static Figures              msGameFigures[ FIGURES_MAX_CNT ];

    std :: vector < Figures >   mPresentFigures;
    std :: vector < Block* >	mBoardBlocks;
    Block*			mpField[ LENGTH ][ HEIGHT ][ WIDTH ];
    int                         mpCollapseComponent[ LENGTH ][ HEIGHT ][ WIDTH ];
    void			CreateBorderBlocks();

    Figure*			mpCurrentFigure;

    unsigned int		mScore;
    unsigned int		mGameLevel;
    unsigned int                mGameSpeed;
    bool                        mIsGameOver;

    float			mRotatingAngle;
    bool			mIsRotate;
    void			MoveDownFigure();

    int                         mFigureDownSteps;
    int                         mRotatingStep;

    RotatePlane                 mRotatingPlane;
    Point3Di                    mFigurePosCorrectVec;
    int                         mFigurePosCorrectStep;
    bool                        CheckToRotate();
    bool			CheckToCollisionByXY();

    ComponentsVec               component_block;
    bool                        mIsCollapse;
    bool                        CheckToCollapse();
    void			PickUpComponent( int a_i, int a_k, int a_j );
    void			PrepairToCollapse();
    void                        CollapseStep();
    int                         mFallingComponentsCnt;

    static const int                    MaxSelectBlockCount = 4;
    Point3Di                            mSelectBlocksPos[ MaxSelectBlockCount ];
    Material                            mSelectBlocksMaterials[ MaxSelectBlockCount ];
    void                                ChangeSelectBlocks();
    void                                GetPrevColors();

    int                                 mShiftChecksCnt;
    Axises                              mShiftAxis;
    ShiftDirection                      mShiftDirection;

    Phonon :: MediaObject*              mpAmbientMusicObject;
    Phonon :: MediaObject*              mpBlockFallSoundObject;
    Phonon :: AudioOutput*              mpAudioAmbient;
    Phonon :: AudioOutput*              mpAudioSound;
    bool                                mIsSound;
    bool                                mIsAmbientMusic;

    bool                                ShiftFigureByXAxis( ShiftDirection shift );
    bool                                ShiftFigureByZAxis( ShiftDirection shift );

    static void                         InitializeStaticData();
    Point2Df                            GetFigurePositionOnXZ( int width_x, int width_z );
    Figure*                             GetNewFigure();

    std :: vector < Messages >          mMessagesList;
};

#endif
