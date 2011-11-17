#include <QObject>
#include "GameObjects.h"

#ifndef GAME_H
#define GAME_H

class Game : public QObject
{
    Q_OBJECT
public slots :
    void                        Start();
public:
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

    enum                        GameSpeed { ZERO_SPEED    = 0,
                                            FIRST_SPEED   = 1,
                                            SECOND_SPEED  = 2,
                                            THIRD_SPEED   = 4,
                                            FOURTH_SPEED  = 8,
                                            FIFTH_SPEED   = 10,
                                            SIXTH_SPEED   = 12,
                                            SEVENTH_SPEED = 14 };

    enum                        MaterialType { FIGURES_MATERIALS        = 6,
                                               SELECT_FIGURES_MATERIALS = 6,
                                               BOTTOM_FIGURES_MATERIALS = 7 };

    enum                        GameConstants { WIDTH                = 8,
                                                LENGTH               = 8,
                                                HEIGHT               = 20,
                                                FIELD_BEGIN_X        = 1,
                                                FIELD_BEGIN_Y         = 1,
                                                FIELD_BEGIN_Z        = 1,
                                                FIELD_END_X          = LENGTH - 1,
                                                FIELD_END_Y          = HEIGHT - 1,
                                                FIELD_END_Z          = WIDTH - 1,
                                                FIELD_LOWER_BOUND_X  = FIELD_BEGIN_X * Block :: BLOCK_SIZE,
                                                FIELD_UPPER_BOUND_X  = FIELD_END_X   * Block :: BLOCK_SIZE,
                                                FIELD_LOWER_BOUND_Y  = FIELD_BEGIN_Y  * Block :: BLOCK_SIZE,
                                                FIELD_UPPER_BOUND_Y  = FIELD_END_Y   * Block :: BLOCK_SIZE,
                                                FIELD_LOWER_BOUND_Z  = FIELD_BEGIN_Z * Block :: BLOCK_SIZE,
                                                FIELD_UPPER_BOUND_Z  = FIELD_END_Z   * Block :: BLOCK_SIZE,
                                                ROTATE_STEPS_COUNT   = 20,
                                                BSIZE                = Block :: BLOCK_SIZE,
                                                HALF_BSIZE           = Block :: BLOCK_SIZE / 2,
                                                FIGURES_MAX_CNT      = 7,
                                                FIGURE_START_Y_POS   = FIELD_END_Y - 2
                                };   

                                Game();

    static void                 InitializeStaticData();
    Point2Df                    GetFigurePositionOnXZ( int width_x, int width_z );
    Figure*			GetNewFigure();
    float*			GetLightPosition() const;
    void			DrawWorld() const;
    void			DrawField() const;
    void			DrawInterface() const ;
    void			DrawBlocksOnTheField() const;
    void			SetGameSpeed( GameSpeed new_game_speed );
    void			NextStep();
    void			ShiftFigureByXAxis( ShiftDirection shift );
    void			ShiftFigureByZAxis( ShiftDirection shift );
    void			Rotate( RotatePlane plane, RotateSide side );
    void			DropDownFigure();
    void                        ChangePause();
    bool                        IsPause();
    void                        SetShift( Axises axis, ShiftDirection direction );
    void                        GetSelectFigures( bool* select_figures );
    void                        SetSelectFigures( bool* select_figures );
private:
    enum                        LightPosition  { LightPosByX = 400 , LightPosByY = 800, LightPosByZ = 300 };
    static const int            SAFETY_DISTANCE;


    static float                msLightPosition[ 4 ];
    static Figures              msGameFigures[ FIGURES_MAX_CNT ];

    std :: vector < Figures >   mPresentFigures;
    std :: vector < Block* >	mBoardBlocks;
    Block*			mpField[ LENGTH ][ HEIGHT ][ WIDTH ];
    int                         mFieldBlockCnt;

    Figure*			mpCurrentFigure;
    Figure*			mpNextFigure;

    unsigned int		mScore;
    bool			mIsGame;

    GameSpeed                   mGameSpeed;
    float			mRotatingAngle;
    bool			mIsRotate;
    void			MoveDownFigure();

    int                         mFigureDownSteps;
    int                         mRotatingStep;

    RotatePlane                 mRotatingPlane;
    Point3Di                    mFigurePosCorrectVec;
    int                         mFigurePosCirrectStep;
    bool                        CheckToRotate(/* RotatePlane aRotatingPlane, float aRotatingAngle */);
    bool			CheckToCollisionByXY();

    bool                        mIsCollapse;
    int                         mCollapseStepsCnt;
    void                        CheckToCollapse();

    static const int            MaxSelectBlockCount = 4;
    Point3Di                    mSelectBLocksPos[ MaxSelectBlockCount ];
    Material                    mSelectBlocksMaterials[ MaxSelectBlockCount ];
    void                        ChangeSelectBlocks();

    int                         mShiftChecksCnt;
    Axises                      mShiftAxis;
    ShiftDirection              mShiftDirection;
};

#endif
