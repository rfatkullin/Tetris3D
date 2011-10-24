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
    static const int		BlockSize = 32;		//Must be even
private :
    static Point3Di	vertices_i[ BlocksVertexCount ];

    Material		material;
    Point3Df		rel_position_f;
    Point3Di		rel_position_i;
    Point3Df		vertices_f[ BlocksVertexCount ];

    void    Rotate  ( float &a, float &b, float angle );
    void    DrawSide( Point3Df x1, Point3Df x2, Point3Df x3, Point3Df x4 );

public:
	    Block( int new_x, int new_y, int new_z, Material new_material );
	    Block( Point3Di new_position, Material new_material );

    Point3Df	GetPositionF() const;
    Point3Di	GetPositionI() const;
    Material	GetMaterial()  const;
    float	LowerBoundXf();
    float	UpperBoundXf();
    float	LowerBoundYf();
    float	UpperBoundYf();
    float	LowerBoundZf();
    float	UpperBoundZf();

    int		LowerBoundXi();
    int		UpperBoundXi();
    int		LowerBoundYi();
    int		UpperBoundYi();
    int		LowerBoundZi();
    int		UpperBoundZi();

    void    RotateOnZY( float angle, bool change_const );
    void    RotateOnZX( float angle, bool change_const );
    void    RotateOnXY( float angle, bool change_const );
    void    Draw( Point3Df figure_location  );
};

class Figure
{
public :
    static const unsigned int  BlocksCount = 4;
private :
    //Material	material;
    Point3Di	position_i;
    Point3Df	position_f;
    Block	*blocks[ BlocksCount ];
	
public :
		Figure( int x, int y, int z, Figures type, Material new_material );
		~Figure();
    Point3Df	GetPositionF() const;
    Point3Di	GetPositionI() const;
    float	LowerBoundXf();
    float	UpperBoundXf();
    float	LowerBoundYf();
    float	UpperBoundYf();
    float	LowerBoundZf();
    float	UpperBoundZf();

    int	    	LowerBoundXi();
    int		UpperBoundXi();
    int		LowerBoundYi();
    int		UpperBoundYi();
    int		LowerBoundZi();
    int		UpperBoundZi();

    Point3Di	GetBlockPositionByIndex( int index ) const;
    Material	GetBlockMaterialByIndex( int index ) const;
    void	SetPositionI( Point3Di new_position );
    void	SetPositionF( Point3Df new_position );
    void	RotateOnZY( float angle, bool change_const );
    void	RotateOnZX( float angle, bool change_const );
    void	RotateOnXY( float angle, bool change_const );
    void	Draw( );
};

#endif
