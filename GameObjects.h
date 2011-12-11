#include <QTextStream>
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
    static const unsigned int	BLOCKS_VERTEX_CNT = 8;
    static const unsigned int	BLOCK_SIZE = 32;		//Must be even
    static const double         SAFETY_DISTANCE;
    static const double         NOT_SAFETY_DISTANCE;
private :
    static const Point3Di       msVerticesI[ BLOCKS_VERTEX_CNT ];
    Point3Df			mVerticesF[ BLOCKS_VERTEX_CNT ];


    void			Rotate ( float &a, float &b, float angle );
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
    bool                        CheckEdgesAveragePoint( Block* block );
    bool                        IsIntersect( Block* block );
    void                        Draw( Point3Df figure_location  );

    friend QTextStream& operator << ( QTextStream& stream, const Block& material );
    friend QTextStream& operator >> ( QTextStream& stream, Block& material );
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
    Block                       *mpBlocks[ BlocksCount ];
	
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
    bool                        IsIntersectWithBlock( Block* block );
    bool                        CheckToCollisonWithBlocks( std :: vector < Block* >& mpBlocks );
    void                        SetBlocksAbsCoor();
    void                        SetBlocksRelCoor();

    friend QTextStream& operator << ( QTextStream& stream, const Figure& material );
    friend QTextStream& operator >> ( QTextStream& stream, Figure& material )	;

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

QTextStream& operator << ( QTextStream& stream, const Figures& figure );
QTextStream& operator >> ( QTextStream& stream, Figures& figure );

#endif
