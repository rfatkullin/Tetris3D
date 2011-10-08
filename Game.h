#include "Geom.h"
#include "Figures.h"

#ifndef GAME_H
#define GAME_H

enum ShiftDirection{ SHIFT_RIGHT = 1, SHIFT_LEFT = -1, SHIFT_BACK = -1, SHIFT_AHEAD = 1 };
enum RotateSide{ ROTATE_BY_CLOCK_WISE, ROTATE_BY_ANTI_CLOCK_WISE };
enum RotatePlane{ PLANE_XY, PLANE_ZY, PLANE_ZX };
enum GameSpeed{ FIRST_SPEED = 1, SECOND_SPEED = 2, THIRD_SPEED = 4, FOURTH_SPEED = 8, FIFTH_SPEED = 10, SIXTH_SPEED = 12, SEVENTH_SPEED = 14 };


class Game
{
private:
	enum { FIELD_WIDTH = 4, FIELD_LENGTH = 4, FIELD_HEIGHT = 4, ROTATE_STEP_COUNT = 20 };
	static const float	CAMERA_RADIUS;
	static const float	CAMERA_POS_CHANGE_KOEFF;
	static const int	FIELD_POSITION_BY_Y_AXIS;
	unsigned char		field[ FIELD_WIDTH ][ FIELD_LENGTH ][ FIELD_HEIGHT ];
	SphericalCoor		camera_position;
	unsigned int		score;	
	RotatePlane			rotating_plane;
	GameSpeed			game_speed;
	Figure*				current_figure;
	float				rotating_angle;
	bool				rotating;
	int					rotating_step;	
	
	void				DrawField();
	void				DrawInterface();	
public:
						Game();			
	Point3D				GetCameraPosition();
	void				ChangeCameraPosition( float x, float y );
	void				DrawWorld();
	void				SetGameSpeed( unsigned int new_game_speed );
	void				Start();
	void				NextStep();
	void				ShiftFigureByXAxis( ShiftDirection shift );
	void				ShiftFigureByZAxis( ShiftDirection shift );
	void				Rotate( RotatePlane plane, RotateSide side );	
	void				DropDownFigure();
};

#endif