#include "Geom.h"
#include "Material.h"
 
#ifndef FIGURES_H
#define FIGURES_H

enum Figures { IFigure = 0, JFigure, LFigure, OFigure, SFigure, TFigure, ZFigure };
const int	MaterialsCount = 6;
const Material	materials[ MaterialsCount ] = {
    Material(  0.0f, 1.0f, 0.0f, 1.0f,
	       0.0f, 1.0f, 0.0f, 1.0f,
	       0.0f, 0.0f, 0.0f, 1.0f
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
    Point3Df	current_rel_coordinates;
    Point3Df	const_rel_coordinates;
    Point3Df	current_vertices[ BlocksVertexCount ];
    Point3Df	const_vertices[ BlocksVertexCount ];

    void    CurrentCoordinatesToConst();
    void    Rotate  ( float &a, float &b, float angle );
    void    DrawSide( Point3Df x1, Point3Df x2, Point3Df x3, Point3Df x4 );
public:
	    Block( float new_x, float new_y, float new_z, Material new_material );

    float   LowerBoundX();
    float   UpperBoundX();
    float   LowerBoundY();
    float   UpperBoundY();
    float   LowerBoundZ();
    float   UpperBoundZ();

    void    RotateOnZY( float angle, bool change_const );
    void    RotateOnZX( float angle, bool change_const );
    void    RotateOnXY( float angle, bool change_const );
    void    Draw( Point3Df figure_location );
};

class Figure
{
public :
    static const unsigned int  BlocksCount = 4;
private :
    Material	material;
    Point3Df	position;
    Block	*blocks[ BlocksCount ];

	
public :
		Figure( float x, float y, float z, Figures type, Material new_material );
		~Figure();
    Point3Df	GetPosition();
    float	LowerBoundX();
    float	UpperBoundX();
    float	LowerBoundY();
    float	UpperBoundY();
    float	LowerBoundZ();
    float	UpperBoundZ();

    void	SetPosition( Point3Df new_position );
    void	RotateOnZY( float angle, bool change_const );
    void	RotateOnZX( float angle, bool change_const );
    void	RotateOnXY( float angle, bool change_const );
    void	Draw( );
};

#endif
