#include "Geom.h"
#include "Figures.h"

#ifndef GAME_H
#define GAME_H

class Game
{
public:
	Game();
	enum	RotatePlane	{ PlaneXY, PlaneZY, PlaneZX };
	enum	RotateSide	{ RotateByClockWise, RotateByAntiClockWise };
	enum	ShiftDirection	{ ShiftRight = 1, ShiftLeft = -1, ShiftBack = -1, ShiftAhead = 1 };

	static	void		InitializeStaticData();

	Point3Df			GetCameraPosition();
	Point3Df			GetLastMousePosition();
	void			SetLastMousePosition( float x, float y );
	float*			GetLightPosition();
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
	enum		    { FieldWidth = 4, FieldLength = 4, FieldHeight = 4, RotateStepsCount = 20 };
	enum LightPosition  { LightPosByX = 0 , LightPosByY = 300, LightPosByZ = 300 };
	enum GameSpeed	    { FirstSpeed = 1, SecondSpeed = 2, ThirdSpeed = 4, FourthSpeed = 8, FifthSpeed = 10, SixthSpeed = 12, SeventhSpeed = 14 };
	enum		    { FieldMaxBound = FieldWidth / 2 * Block :: BlockSize, FieldMinBound = -FieldWidth / 2 * Block :: BlockSize  };
	static const float	CameraRadius;
	static const float	CameraPosChangeKoeff;
	static const int	FieldPositionByY;
	static float		light_position[ 4 ];

	Block*			field[ FieldWidth ][ FieldLength ][ FieldHeight ];
	unsigned int		score;
	SphericalCoor		camera_position;
	Point3Df			last_mouse_position;

	RotatePlane		rotating_plane;
	GameSpeed		game_speed;
	Figure*			current_figure;
	float			rotating_angle;
	bool			rotating;
	int			rotating_step;
	
	void			DrawField();
	void			DrawInterface();

	void			MoveDownFigure();
	bool			CheckUpRotate();

};

#endif
