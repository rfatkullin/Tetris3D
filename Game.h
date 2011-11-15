#include <QObject>
#include "GameObjects.h"

#ifndef GAME_H
#define GAME_H

class Game : public QObject
{
    Q_OBJECT
public slots :
    void        Start();
public:
    enum	RotatePlane	{ PLANE_XY, PLANE_ZY, PLANE_ZX };
    enum	RotateSide	{ ROTATE_BY_CLOCK_WISE = -1,
                                  ROTATE_BY_ANTI_CLOCKWISE = 1 };
    enum        Axises          { XAxis, YAxis, ZAxis };

    enum	ShiftDirection	{ ShiftDirectly = 1,
                                  ShiftBack     = -1 };

    enum        GameSpeed       { ZERO_SPEED    = 0,
                                  FIRST_SPEED   = 1,
                                  SECOND_SPEED  = 2,
                                  THIRD_SPEED   = 4,
                                  FOURTH_SPEED  = 8,
                                  FIFTH_SPEED   = 10,
                                  SIXTH_SPEED   = 12,
                                  SEVENTH_SPEED = 14 };

    enum         MaterialTypes { FiguresMaterials = 6,
                                 SelectFigureMaterial = 6,
                                 BottomFiguresMaterial = 7 };

       enum   GameConstants { Width             = 12,
                              Length            = 12,
                              Height            = 20,
                              FieldBeginX       = 1,
                              FieldBeginY       = 1,
                              FieldBeginZ       = 1,
                              FieldEndX         = Length - 1,
                              FieldEndY         = Height - 1,
                              FieldEndZ         = Width - 1,
                              FieldLowerBoundX  = FieldBeginX * Block :: BlockSize,
                              FieldUpperBoundX  = FieldEndX   * Block :: BlockSize,
                              FieldLowerBoundY  = FieldBeginY * Block :: BlockSize,
                              FieldUpperBoundY  = FieldEndY   * Block :: BlockSize,
                              FieldLowerBoundZ  = FieldBeginZ * Block :: BlockSize,
                              FieldUpperBoundZ  = FieldEndZ   * Block :: BlockSize,
                              RotateStepsCount  = 20,
                              BSize             = Block :: BlockSize,
                              FiguresMaxCnt     = 7,
                              FIGURE_START_Y_POS = FieldEndY - 2
                            };

                             Game();

    static void		InitializeStaticData();
    Point3Df		GetCameraPosition() const;
    Point3Df		GetLastMousePosition();
    Point2Df		GetFigurePositionOnXZ( int width_x, int width_z );
    Figure*			GetNewFigure();
    float*			GetLightPosition() const;
    void			SetLastMousePosition( float x, float y );
    void			ChangeCameraPosition( float x, float y );
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
    void                    ChangePause();
    bool                    IsPause();
    void                    SetShift( Axises axis, ShiftDirection direction );
    void                    GetSelectFigures( bool* select_figures );
    void                    SetSelectFigures( bool* select_figures );


private:
    enum LightPosition  { LightPosByX = 400 , LightPosByY = 800, LightPosByZ = 300 };
    static const int    SafetyDistance;
    static const float	CameraRadius;
    static const float	CameraPosChangeKoeff;
    static float	light_position[ 4 ];
    static Figures      game_figures[ FiguresMaxCnt ];

    std :: vector < Figures >   present_figures;
    std :: vector < Block* >	mBoardBlocks;//board_blocks;//[ Length ][ Height ][ Width ];
    Block*			mpField[ Length ][ Height ][ Width ];
    int                         count_of_blocks_on_field;
//Camera manipulating
    SphericalCoor		camera_position;
    Point3Df		last_mouse_position;
    float			Sign( float a );
//Figures
    Figure*			mpCurrentFigure;
    Figure*			next_figure;
//Game state
    unsigned int		score;
    bool			mIsGame;//is_game;
//Moving
    void			MoveDownFigure();
    GameSpeed                  mGameSpeed;//game_speed;
    float			rotating_angle;
    bool			rotating;
    bool			is_pos_change;
    int			mFigureDownSteps;
    int			rotating_step;
//Rotating
    RotatePlane		rotating_plane;
    Point3Di		figure_pos_correct_vec;
    int			figure_pos_correct_step;
    bool                    CheckToRotate( RotatePlane rotating_plane, float rotating_angle );
    bool			CheckToCollisionByXY();

//Collapse
    bool			mIsCollapse;
    void			CheckToCollapse();
    int                     mCollapseStepsCnt;//collapse_steps_count;
//Selecting block to show where will fall the figure
    static const int        MaxSelectBlockCount = 4;
    Point3Di                select_blocks_pos[ MaxSelectBlockCount ];
    Material                select_blocks_materials[ MaxSelectBlockCount ];
    void                    ChangeSelectBlocks();
//Shift
    int                     count_of_shift_checks;
    Axises                  shift_axis;
    ShiftDirection          shift_direction;
};

#endif
