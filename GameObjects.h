#include <vector>
#include "GeomEntity.h"
#include "PhisEntity.h"
#include "Material.h"
 
#ifndef FIGURES_H
#define FIGURES_H

enum Figures { IFigure = 0, JFigure, LFigure, OFigure, SFigure, TFigure, ZFigure };

class Block : public GeomEntity, public PhisEntity
{
public :
    static const unsigned int	BlocksVertexCount = 8;
    static const int		BlockSize = 32;		//Must be even
    static const double         SafetyDistanceBetweenBlocks;
    static const double         NonSafetyDistanceBetweenBlocks;
private :
    Point3Df			vertices_f[ BlocksVertexCount ];

    void			Rotate  ( float &a, float &b, float angle );
    void			DrawSide( Point3Df x1, Point3Df x2, Point3Df x3, Point3Df x4 );

public:
                                Block();
                                Block( int new_x, int new_y, int new_z, Material new_material );
                                Block( float new_x, float new_y, float new_z, Material new_material );
                                Block( Point3Di new_Pos, Material new_material );
                                Block( Point3Df new_Pos, Material new_material );
                                Block( const Block& block);


    bool                        PointIn( Point3Df point );
    bool                        PointIn( Point3Di point );
    bool                        CheckEdgesAveragePoint( Block& block );
    bool                        IsIntersect( Block& block );
    void                        Draw( Point3Df figure_location  );

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

    virtual void                SetVerAbsCoor();
    virtual void                SetVerRelCoor();

    virtual void		Draw();
};


class Figure : public GeomEntity, public PhisEntity
{
public :
    static const unsigned int  BlocksCount = 4;
private :    
    Block                       *blocks[ BlocksCount ];
	
public :
				Figure( int x, int y, int z, Figures type, Material new_material );
                                Figure( const Figure& fig );
				 ~Figure();

    Point3Di			GetBlockPosByIndexi( int index ) const;
    Point3Df			GetBlockPosByIndexf( int index ) const;
    Point3Di                    GetLeftMostBlockPosi();
    Point3Di                    GetRightMostBlockPosi();
    Point3Di                    GetBackMostBlockPosi();
    Point3Di                    GetAheadMostBlockPosi();
    Material			GetBlockMaterialByIndex( int index ) const;
    bool                        IsIntersectWithBlock( Block& block );
    bool                        CheckToCollisonWithBlocks( std :: vector < Block >& blocks );
    void                        SetBlocksAbsCoor();
    void                        SetBlocksRelCoor();

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

    virtual void                SetVerAbsCoor();
    virtual void                SetVerRelCoor();

    virtual void		Draw();
};

#endif
