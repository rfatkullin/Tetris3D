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

class GeomEntity
{
protected:
    Material	    material;
    Point3Di	    pos_i;
    Point3Df	    pos_f;
public:
		    GeomEntity( int new_x, int new_y, int new_z, Material new_material );
		    GeomEntity( float new_x, float new_y, float new_z, Material new_material );
		    GeomEntity( Point3Di new_pos, Material new_material );
		    GeomEntity( Point3Df new_pos, Material new_material );
//Position
    Point3Df	    GetPosf() const;
    Point3Di	    GetPosi() const;

    float	    GetPosByXf() const;
    float	    GetPosByYf() const;
    float	    GetPosByZf() const;
    int		    GetPosByXi() const;
    int		    GetPosByYi() const;
    int		    GetPosByZi() const;

    void	    SetPosByXi( int new_x );
    void	    SetPosByYi( int new_y );
    void	    SetPosByZi( int new_z );
    void	    SetPosByXf( float new_x );
    void	    SetPosByYf( float new_y );
    void	    SetPosByZf( float new_z );

    void	    SetPosi( int new_x, int new_y, int new_z );
    void	    SetPosf( float new_x, float new_y, float new_z );
    void	    SetPosi( Point3Di new_pos );
    void	    SetPosf( Point3Df new_pos );

//Material
    Material	    GetMaterial()  const;

//Bounds
    virtual float   LowerBoundXf() = 0;
    virtual float   UpperBoundXf() = 0;
    virtual float   LowerBoundYf() = 0;
    virtual float   UpperBoundYf() = 0;
    virtual float   LowerBoundZf() = 0;
    virtual float   UpperBoundZf() = 0;

    virtual int	    LowerBoundXi() = 0;
    virtual int	    UpperBoundXi() = 0;
    virtual int	    LowerBoundYi() = 0;
    virtual int	    UpperBoundYi() = 0;
    virtual int	    LowerBoundZi() = 0;
    virtual int	    UpperBoundZi() = 0;

    virtual void    RotateOnZY( float angle, bool change_const ) = 0;
    virtual void    RotateOnZX( float angle, bool change_const ) = 0;
    virtual void    RotateOnXY( float angle, bool change_const ) = 0;
    virtual void    Draw() = 0;
};

class Block : public GeomEntity
{
public :
    static const unsigned int	BlocksVertexCount = 8;
    static const int		BlockSize = 32;		//Must be even
private :
    static Point3Di		vertices_i[ BlocksVertexCount ];
    Point3Df			vertices_f[ BlocksVertexCount ];

    void			Rotate  ( float &a, float &b, float angle );
    void			DrawSide( Point3Df x1, Point3Df x2, Point3Df x3, Point3Df x4 );

public:
				Block( int new_x, int new_y, int new_z, Material new_material );
				Block( Point3Di new_Pos, Material new_material );

    virtual float		LowerBoundXf();
    virtual float		UpperBoundXf();
    virtual float		LowerBoundYf();
    virtual float		UpperBoundYf();
    virtual float		LowerBoundZf();
    virtual float		UpperBoundZf();

    virtual int			LowerBoundXi();
    virtual int			UpperBoundXi();
    virtual int			LowerBoundYi();
    virtual int			UpperBoundYi();
    virtual int			LowerBoundZi();
    virtual int			UpperBoundZi();

    virtual void		RotateOnZY( float angle, bool change_const );
    virtual void		RotateOnZX( float angle, bool change_const );
    virtual void		RotateOnXY( float angle, bool change_const );
    virtual void		Draw( Point3Df figure_location  );
    virtual void		Draw();
};


class Figure : public GeomEntity
{
public :
    static const unsigned int  BlocksCount = 4;
private :    
    Block	*blocks[ BlocksCount ];
	
public :
				Figure( int x, int y, int z, Figures type, Material new_material );
				 ~Figure();

    Point3Di			GetBlockPosByIndex( int index ) const;
    Material			GetBlockMaterialByIndex( int index ) const;
    virtual float		LowerBoundXf();
    virtual float		UpperBoundXf();
    virtual float		LowerBoundYf();
    virtual float		UpperBoundYf();
    virtual float		LowerBoundZf();
    virtual float		UpperBoundZf();

    virtual int			LowerBoundXi();
    virtual int			UpperBoundXi();
    virtual int			LowerBoundYi();
    virtual int			UpperBoundYi();
    virtual int			LowerBoundZi();
    virtual int			UpperBoundZi();

    virtual void		RotateOnZY( float angle, bool change_const );
    virtual void		RotateOnZX( float angle, bool change_const );
    virtual void		RotateOnXY( float angle, bool change_const );
    virtual void		Draw();
};

#endif
