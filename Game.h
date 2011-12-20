#include <phonon/AbstractAudioOutput>
#include <phonon/MediaObject>
#include <phonon/AudioOutput>
#include <QObject>
#include <QSound>
#include <QFile>
#include "GameObjects.h"

#ifndef GAME_H
#define GAME_H

typedef std :: vector < std :: pair < Block*, std :: pair < int, int > > > BlocksVec;
typedef std :: vector < BlocksVec > ComponentsVec;

class Game : public QObject
{
    Q_OBJECT
public slots :
    void                                Start();
    void                                AmbientMusicStateChange( bool aState );
    void                                SoundsStateChange( bool aState );
    void                                Save();
    bool                                Load();
private slots :
    void                                PrepairFallSound();
    void                                PlayAmbientMusic();
public:
    static const int                    MESSAGES_CNT = 9;
    enum                                Messages { EMPTY = 0,
                                                   NEW_LEVEL,
                                                   COLLAPSE,
                                                   COULDNT_ROTATE_COLLISION,
                                                   COULDNT_SHIFT_COLLISION,
                                                   NEW_GAME,
                                                   GAME_OVER,
                                                   SAVE_GAME,
                                                   LOAD_GAME
                                          };

    enum                                RotatePlane { PLANE_XY = 0,
                                                      PLANE_ZY,
                                                      PLANE_ZX };

    enum                                RotateSide { ROTATE_BY_CLOCK_WISE     = 1,
                                                     ROTATE_BY_ANTI_CLOCKWISE = -1 };

    enum                                Axises { X_AXIS,
                                                 Y_AXIS,
                                                 Z_AXIS };

    enum                                ShiftDirection { SHIFT_DIRECTLY = 1,
                                                         SHIFT_BACK     = -1 };

    enum                                MaterialType { FIGURES_MATERIALS        = 6,
                                                       SELECT_FIGURES_MATERIALS = 6,
                                                       BOTTOM_FIGURES_MATERIALS = 7 };

    static const char* const            MESSAGES[ MESSAGES_CNT ];

    static const char* const            SAVE_FILE;

    static const int                    LEVELS_CNT    = 7;
    static const unsigned int           LEVELS_SCORE[ LEVELS_CNT ];
    static const unsigned int           LEVELS_SPEED[ LEVELS_CNT ];
    static const unsigned int           MAX_GAME_SPEED;

    const unsigned int static           WIDTH                = 8;
    const unsigned int static           LENGTH               = 8;
    const unsigned int static           HEIGHT               = 20;
    const unsigned int static           FIGURES_MAX_CNT      = 7;
    const int static                    FIELD_BEGIN_X        = 1;
    const int static                    FIELD_BEGIN_Y        = 1;
    const int static                    FIELD_BEGIN_Z        = 1;
    const int static                    FIELD_END_X          = static_cast< int >( LENGTH - 1 );
    const int static                    FIELD_END_Y          = static_cast< int >( HEIGHT - 1 );
    const int static                    FIELD_END_Z          = static_cast< int >( WIDTH - 1 );
    const int static                    FIELD_LOWER_BOUND_X  = FIELD_BEGIN_X * Block :: BLOCK_SIZE;
    const int static                    FIELD_UPPER_BOUND_X  = FIELD_END_X   * Block :: BLOCK_SIZE;
    const int static                    FIELD_LOWER_BOUND_Y  = FIELD_BEGIN_Y * Block :: BLOCK_SIZE;
    const int static                    FIELD_UPPER_BOUND_Y  = FIELD_END_Y   * Block :: BLOCK_SIZE;
    const int static                    FIELD_LOWER_BOUND_Z  = FIELD_BEGIN_Z * Block :: BLOCK_SIZE;
    const int static                    FIELD_UPPER_BOUND_Z  = FIELD_END_Z   * Block :: BLOCK_SIZE;
    const int static                    ONE_LEVEL_BLOCKS_CNT = ( FIELD_END_X - FIELD_BEGIN_X ) *
                                                                   ( FIELD_END_Z - FIELD_BEGIN_Z );
    const int static                    ROTATE_STEPS_COUNT   = 20;
    const int static                    BSIZE                = ( int )Block :: BLOCK_SIZE;
    const int static                    HALF_BSIZE           = ( int )Block :: BLOCK_SIZE / 2;
    const int static                    FIGURE_START_Y_POS   = FIELD_END_Y - 2;

                                        Game( QObject* apParent = 0 );
                                        ~Game();

    unsigned int                        GetLevel() const;
    float*                              GetLightPosition() const;

    void                                NextStep();
    void                                DropDownFigure();

    void                                GetSelectFigures( bool* apSelectFigures );
    void                                SetSelectFigures( bool* apSelectFigures );

    //Game state
    void                                GameOver();
    void                                End();
    bool                                IsGameOver();

    //Draw
    void                                DrawWorld() const;
    void                                DrawField() const;
    void                                DrawBlocksOnTheField() const;

    //Figure control
    void                                SetShift( Axises aAxis, ShiftDirection aDirection );
    void                                Rotate( RotatePlane aPlane, RotateSide aSide );

    bool                                AmbientMusicState();

    int                                 GetScore() const;

    const std :: vector < Messages >&   GetMessages() const;
    void                                ClearMessagesList();

    void                                TurnOnSelecting();
    void                                TurnOffSelecting();
private:
    enum                                LightPosition  { LightPosByX = 400 , LightPosByY = 800, LightPosByZ = 300 };

    static const int                    SAFETY_DISTANCE       = 16.0 * BSIZE * BSIZE;
    static const int                    MAX_SELECT_BLOCKS_CNT = 4;
    static float                        msLightPosition[ 4 ];
    static Figures                      msGameFigures[ FIGURES_MAX_CNT ];


    QFile*                              mpSaveFile;
    QTextStream                         mSaveStream;

    std :: vector < Figures >           mPresentFigures;
    std :: vector < Block* >            mBoardBlocks;
    Block*                              mpField[ LENGTH ][ HEIGHT ][ WIDTH ];
    Figure*                             mpCurrentFigure;
    int                                 mFieldBlocksCnt;

    unsigned int                        mScore;
    unsigned int                        mGameLevel;
    unsigned int                        mGameSpeed;
    bool                                mIsGameOver;

    int                                 mFigureDownSteps;

    RotatePlane                         mRotatingPlane;
    Point3Di                            mFigurePosCorrectVec;
    float                               mRotatingAngle;
    bool                                mIsRotate;
    int                                 mFigurePosCorrectStep;
    int                                 mRotatingStep;

    int                                 mpCollapseComponent[ LENGTH ][ HEIGHT ][ WIDTH ];
    ComponentsVec                       mCollapseComponents;
    bool                                mIsCollapse;
    int                                 mFallingComponentsCnt;


    Point3Di                            mSelectBlocksPos[ MAX_SELECT_BLOCKS_CNT ];
    Material                            mSelectBlocksMaterials[ MAX_SELECT_BLOCKS_CNT ];


    int                                 mShiftChecksCnt;
    Axises                              mShiftAxis;
    ShiftDirection                      mShiftDirection;

    Phonon :: MediaObject*              mpAmbientMusicObject;
    Phonon :: MediaObject*              mpBlockFallSoundObject;
    Phonon :: AudioOutput*              mpAudioAmbient;
    Phonon :: AudioOutput*              mpAudioSound;
    bool                                mIsSound;
    bool                                mIsAmbientMusic;

    std :: vector < Messages >          mMessagesList;

    bool                                ShiftFigureByXAxis( ShiftDirection aShift );
    bool                                ShiftFigureByZAxis( ShiftDirection aShift );

    static void                         InitializeStaticData();
    void                                CreateBorderBlocks();

    Point2Df                            GetFigurePositionOnXZ( int aWidthX, int aWidthZ );
    Figure*                             GetNewFigure();

    void                                ChangeSelectBlocks();
    void                                GetPrevColors();

    bool                                CheckToRotate();

    bool                                CheckToCollapse();
    void                                PickUpComponent( int aI, int aK, int aJ );
    void                                PrepairToCollapse();
    void                                CollapseStep();
};

QTextStream&    operator << ( QTextStream& aStream, const Game :: RotatePlane& aPlane );
QTextStream&    operator >> ( QTextStream& aStream, Game :: RotatePlane& aPlane );

QTextStream&    operator << ( QTextStream& aStream, const bool& aBoolValue);
QTextStream&    operator >> ( QTextStream& aStream, bool& aBoolValue );

QTextStream&    operator << ( QTextStream& aStream, const Game :: Axises& aAxis);
QTextStream&    operator >> ( QTextStream& aStream, Game :: Axises& aAxis );

QTextStream&    operator << ( QTextStream& aStream, const Game :: ShiftDirection& aShiftDirection);
QTextStream&    operator >> ( QTextStream& aStream, Game :: ShiftDirection& aShiftDirection );

#endif
