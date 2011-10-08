#include "Geom.h"
#include "Figures.h"

#ifndef GAME_H
#define GAME_H

class Game
{
private:
	enum		    { FieldWidth = 4, FieldLength = 4, FieldHeight = 4,RotateStepsCount = 20 };
	enum ShiftDirection { ShiftRight = 1, ShiftLeft = -1, ShitBack = -1, ShiftAhead = 1 };
	enum LightPosition  { LightPosByX = 0 , LightPosByY = 300, LightPosByZ = 300 };
	enum RotatePlane    { PlaneXY, PlaneZY, PlaneZX };
	enum RotateSide	    { RotateByClockWise, RotateByAntiClockWise };
	enum GameSpeed	    { FirstSpeed = 1, SecondSpeed = 2, ThirdSpeed = 4, FourthSpeed = 8, FifthSpeed = 10, SixthSpeed = 12, SeventhSpeed = 14 };

	static const float	CameraRadius;
	static const float	CameraPosChangeKoeff;
	static const int	FieldPositionByY;
	unsigned char		field[ FieldWidth ][ FieldLength ][ FieldHeight ];
	SphericalCoor		camera_position;
	unsigned int		score;	
	static float		light_position[ 4 ];
	RotatePlane		rotating_plane;
	GameSpeed		game_speed;
	Figure*			current_figure;
	float			rotating_angle;
	bool			rotating;
	int			rotating_step;
	
	void			DrawField();
	void			DrawInterface();
public:
				Game();
	Point3D			GetCameraPosition();
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
};

#endif
