#include "Geom.h"
#include "Material.h"
 
#ifndef FIGURES_H
#define FIGURES_H

const unsigned int  BlocksCount = 4;
const unsigned int  BlocksVertexCount = 8;
const int	    BlockSize = 30;
const int	    MaterialsCount = 1;

enum Figures { IFigure = 0, JFigure, LFigure, OFigure, SFigure, TFigure, ZFigure };

//const Color colors[ COLORS_COUNT ] = { Color( 1.0f, 0.0f, 1.0f ), Color( 0.0f, 1.0f, 0.0f ), Color( 1.0f, 0.0f, 0.0f ), Color( 0.0f, 0.0f, 1.0f ), Color( 0.7f, 0.7f, 0.7f ) };
//const Color colors[ COLORS_COUNT ] = { Color( 0.0f, 1.0f, 0.0f ) };
const Material materials[ MaterialsCount ] = { Material( 0.0f, 1.0f, 0.0f ) };

class Block
{			  
private: 
    Material    material;
    Point3D	current_rel_coordinates;
    Point3D	const_rel_coordinates;
    Point3D	current_vertices[ BlocksVertexCount ];
    Point3D	const_vertices[ BlocksVertexCount ];

    void    CurrentCoordinatesToConst();
    void    Rotate  ( float &a, float &b, float angle );
    void    DrawSide( Point3D x1, Point3D x2, Point3D x3, Point3D x4 );
public:
	    Block( float new_x, float new_y, float new_z, Material new_material );
    void    RotateOnZY( float angle, bool change_const );
    void    RotateOnZX( float angle, bool change_const );
    void    RotateOnXY( float angle, bool change_const );
    void    Draw( Point3D figure_location );
};

class Figure
{
private:
    Material	material;
    Point3D	position;
    Block	*blocks[ BlocksCount ];

	
public:
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
