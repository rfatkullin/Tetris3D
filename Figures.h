#include "Geom.h"
#include "Material.h"
 
#ifndef FIGURES_H
#define FIGURES_H

const unsigned int	BLOCKS_NUMBER = 4;
const unsigned int	BLOCKS_VERTEX_COUNT = 8;
const int			BLOCK_SIZE = 30;
const int			MATERIALS_COUNT = 1;

enum Figures { I_FIGURE = 0, J_FIGURE, L_FIGURE, O_FIGURE, S_FIGURE, T_FIGURE, Z_FIGURE }; 

//const Color colors[ COLORS_COUNT ] = { Color( 1.0f, 0.0f, 1.0f ), Color( 0.0f, 1.0f, 0.0f ), Color( 1.0f, 0.0f, 0.0f ), Color( 0.0f, 0.0f, 1.0f ), Color( 0.7f, 0.7f, 0.7f ) };
//const Color colors[ COLORS_COUNT ] = { Color( 0.0f, 1.0f, 0.0f ) };
const Material materials[ MATERIALS_COUNT ] = { Material( 0.0f, 1.0f, 0.0f ) };

class Block
{			  
private: 
	Material	material;
	Point3D		current_rel_coordinates;
	Point3D		const_rel_coordinates;
	Point3D		current_vertices[ BLOCKS_VERTEX_COUNT ];
	Point3D		const_vertices[ BLOCKS_VERTEX_COUNT ];
	
	void	CurrentCoordinatesToConst();
	void	Rotate( float &a, float &b, float angle );
	void	DrawSide( Point3D x1, Point3D x2, Point3D x3, Point3D x4 );
public:
			Block( float new_x, float new_y, float new_z, Material new_material );
	void	RotateOnZY( float angle, bool change_const );
	void	RotateOnZX( float angle, bool change_const );
	void	RotateOnXY( float angle, bool change_const );
	void	Draw( Point3D figure_location );
};

class Figure
{
private:
	Material	material;
	Block		*blocks[ BLOCKS_NUMBER ];
	Point3D		position;	
	
public:
			//Figure(){};
			Figure( float x, float y, float z, Figures type );
			~Figure();
	Point3D	GetPosition();
	void	SetPostion( Point3D new_position );
	void	RotateOnZY( float angle, bool change_const );
	void	RotateOnZX( float angle, bool change_const );
	void	RotateOnXY( float angle, bool change_const );
	void	Draw( );
};

#endif