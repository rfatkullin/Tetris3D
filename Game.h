#include "Geom.h"
#include "Figures.h"

#ifndef GAME_H
#define GAME_H

class Game
{
public:
	Game();
	enum	RotatePlane	{ PlaneXY, PlaneZY, PlaneZX };
	enum	RotateSide	{ RotateByClockWise = -1, RotateByAntiClockWise = 1 };
	enum	ShiftDirection	{ ShiftRight = 1, ShiftLeft = -1, ShiftBack = -1, ShiftAhead = 1 };

	static	void		InitializeStaticData();

	Point3Df		GetCameraPosition();
	Point3Df		GetLastMousePosition();
	Point2Df		GetFigurePositionOnXZ( int width_x, int width_z );
	Figure*			GetNewFigure();
	float*			GetLightPosition();
	void			SetLastMousePosition( float x, float y );
	void			ChangeCameraPosition( float x, float y );
	void			DrawWorld();
	void			SetGameSpeed( unsigned int new_game_speed );
	void			Start();
	void			NextStep();
	void			ShiftFigureByXAxis( ShiftDirection shift );
	void			ShiftFigureByZAxis( ShiftDirection shift );
	void			Rotate( RotatePlane plane, RotateSide side );
	void			DropDownFigure();
private:
	enum		    { FieldWidth = 4, FieldLength = 4, FieldHeight = 12, RotateStepsCount = 20 };
	enum LightPosition  { LightPosByX = 0 , LightPosByY = 300, LightPosByZ = 300 };
	enum GameSpeed	    { FirstSpeed = 1, SecondSpeed = 2, ThirdSpeed = 4, FourthSpeed = 8, FifthSpeed = 10, SixthSpeed = 12, SeventhSpeed = 14 };
	enum		    { FieldLowerBoundX =  0,
			      FieldUpperBoundX =  FieldLength * Block :: BlockSize,
			      FieldLowerBoundY =  0,
			      FieldUpperBoundY =  FieldHeight * Block :: BlockSize,
			      FieldLowerBoundZ =  0,
			      FieldUpperBoundZ =  FieldLength * Block :: BlockSize };
	static const float	CameraRadius;
	static const float	CameraPosChangeKoeff;
	static const int	FieldPositionByY;
	static float		light_position[ 4 ];

	Block*			field[ FieldLength ][ FieldWidth ][ FieldHeight ];

//Camera manipulating
	SphericalCoor		camera_position;
	Point3Df		last_mouse_position;
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
	bool			CheckUpRotate();
//Drawing
	void			DrawField();
	void			DrawInterface();
	void			DrawBlocksOnTheField();
//Collapse
	bool			collapse;
	void			CheckToCollapse();
	int			collapse_steps_count;
};

#endif
