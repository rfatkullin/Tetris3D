#include "Geom.h"
#include "Material.h"
 
#ifndef FIGURES_H
#define FIGURES_H

enum Figures { IFigure = 0, JFigure, LFigure, OFigure, SFigure, TFigure, ZFigure };
const int	MaterialsCount = 6;
const Material	materials[ MaterialsCount ] = {
    Material(  0.0f, 1.0f, 0.0f, 1.0f,
	       0.0f, 1.0f, 0.0f, 1.0f,
	       0.2f, 0.2f, 0.2f, 1.0f
	    ),
    Material(  1.0f, 0.0f, 0.0f, 1.0f,
	       1.0f, 0.0f, 0.0f, 1.0f,
	       0.2f, 0.2f, 0.2f, 1.0f
	    ),
    Material(  0.0f, 0.0f, 1.0f, 1.0f,
    	       0.0f, 0.0f, 1.0f, 1.0f,
    	       0.2f, 0.2f, 0.2f, 1.0f
	    ),
    Material(  1.0f, 0.0f, 1.0f, 1.0f,
	       1.0f, 0.0f, 1.0f, 1.0f,
	       0.2f, 0.2f, 0.2f, 1.0f
	    ),
    Material(  1.0f, 1.0f, 0.0f, 1.0f,
	       1.0f, 1.0f, 0.0f, 1.0f,
	       0.2f, 0.2f, 0.2f, 1.0f
	    ),
    Material(  0.0f, 1.0f, 1.0f, 1.0f,
	       0.0f, 1.0f, 1.0f, 1.0f,
	       0.2f, 0.2f, 0.2f, 1.0f
	    )
    };

class Block
{
public :
    static const unsigned int	BlocksVertexCount = 8;
    static const int		BlockSize = 30;
private :
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
    float   MinByX();
    float   MaxByX();
    float   MinByZ();
    float   MaxByZ();
    void    RotateOnZY( float angle, bool change_const );
    void    RotateOnZX( float angle, bool change_const );
    void    RotateOnXY( float angle, bool change_const );
    void    Draw( Point3D figure_location );
};

class Figure
{
public :
    static const unsigned int  BlocksCount = 4;
private :
    Material	material;
    Point3D	position;
    Block	*blocks[ BlocksCount ];

	
public :
		Figure( float x, float y, float z, Figures type, Material new_material );
		~Figure();
    Point3D	GetPosition();
    float	MinByX();
    float	MaxByX();
    float	MinByZ();
    float	MaxByZ();
    void	SetPostion( Point3D new_position );
    void	RotateOnZY( float angle, bool change_const );
    void	RotateOnZX( float angle, bool change_const );
    void	RotateOnXY( float angle, bool change_const );
    void	Draw( );
};

#endif
