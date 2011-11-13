#include "GameObjects.h"

#ifndef GAME_H
#define GAME_H

class Game
{
public:
	Game();
	enum	RotatePlane	{ PlaneXY, PlaneZY, PlaneZX };
	enum	RotateSide	{ RotateByClockWise = -1, RotateByAntiClockWise = 1 };
        enum    Axises          { XAxis, YAxis, ZAxis };
	enum	ShiftDirection	{ ShiftRight = 1, ShiftLeft = -1, ShiftBack = -1, ShiftAhead = 1 };
        enum    GameSpeed       { ZeroSpeed = 0, FirstSpeed = 1, SecondSpeed = 2, ThirdSpeed = 4, FourthSpeed = 8, FifthSpeed = 10, SixthSpeed = 12, SeventhSpeed = 14 };

	static	void		InitializeStaticData();

	Point3Df		GetCameraPosition();
	Point3Df		GetLastMousePosition();
	Point2Df		GetFigurePositionOnXZ( int width_x, int width_z );
	Figure*			GetNewFigure();
	float*			GetLightPosition();
	void			SetLastMousePosition( float x, float y );
	void			ChangeCameraPosition( float x, float y );
	void			DrawWorld();
        void			SetGameSpeed( GameSpeed new_game_speed );
	void			Start();
	void			NextStep();
	void			ShiftFigureByXAxis( ShiftDirection shift );
	void			ShiftFigureByZAxis( ShiftDirection shift );
	void			Rotate( RotatePlane plane, RotateSide side );
	void			DropDownFigure();
        void                    ChangePause();
        bool                    IsPause();
        void                    SetShift( Axises axis, ShiftDirection direction );

        enum		    { Width             = 12,
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
			      BSize             = Block :: BlockSize
                            };
private:
        enum LightPosition  { LightPosByX = 0 , LightPosByY = 300, LightPosByZ = 300 };
        static const int        SafetyDistance;
        static const float	CameraRadius;
	static const float	CameraPosChangeKoeff;
        //static const int	FieldPositionByY;
	static float		light_position[ 4 ];

        //Block*                  field_block_border[ BorderBlocksCount ];
        Block*			field[ Length ][ Height ][ Width ];
        int			count_of_blocks_on_field;
//Camera manipulating
	SphericalCoor		camera_position;
	Point3Df		last_mouse_position;
        //float			SetAngleToRange( float angle );
	float			Sign( float a );
//Figures
	Figure*			current_figure;
	Figure*			next_figure;
//Game state
	unsigned int		score;
	bool			is_game;
//Moving
	void			MoveDownFigure();
	GameSpeed		game_speed;
	float			rotating_angle;
	bool			rotating;
	bool			is_pos_change;
	int			figure_start_pos_y;
	int			figure_down_steps;
	int			rotating_step;
//Rotating
	RotatePlane		rotating_plane;
	Point3Di		figure_pos_correct_vec;
	int			figure_pos_correct_step;
        bool                    CheckToRotate( RotatePlane rotating_plane, float rotating_angle );
        bool			CheckToCollisionByXY();
//Drawing
	void			DrawField();
	void			DrawInterface();
	void			DrawBlocksOnTheField();
//Collapse
	bool			collapse;
	void			CheckToCollapse();
	int			collapse_steps_count;
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
