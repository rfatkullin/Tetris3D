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
    static const int            BLOCK_SIZE = 32;		//Must be even
    static const double         SAFETY_DISTANCE;
    static const double         NOT_SAFETY_DISTANCE;
private :
    static const Point3Di       msVerticesI[ BLOCKS_VERTEX_CNT ];
    Point3Df			mVerticesF[ BLOCKS_VERTEX_CNT ];


    void			Rotate ( float &aA, float &aB, float aAngle );
    void			DrawSide( Point3Df aX1, Point3Df aX2, Point3Df aX3, Point3Df aX4 );

public:
                                Block();
                                Block( int aNewX, int aNewY, int NewZ, Material aNewMaterial );
                                Block( float aNewX, float aNewY, float NewZ, Material aNewMaterial );
                                Block( Point3Di aNewPos, Material aNewMaterial );
                                Block( Point3Df aNewPos, Material aNewMaterial );
                                Block( const Block& aBlock);


    bool                        PointIn( Point3Df aPoint );
    bool                        PointIn( Point3Di aPoint );
    bool                        CheckEdgesAveragePoint( Block* apBlock );
    bool                        IsIntersect( Block* apBlock );
    void                        Draw( Point3Df aFigureLocation  );

    friend QTextStream& operator << ( QTextStream& aStream, const Block& aMaterial );
    friend QTextStream& operator >> ( QTextStream& aStream, Block& aMaterial );
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

    virtual void		RotateOnZY( float aAngle, bool aChangeConst );
    virtual void		RotateOnZX( float aAngle, bool aChangeConst );
    virtual void		RotateOnXY( float aAngle, bool aChangeConst );

    virtual void                SetVerAbsCoor();
    virtual void                SetVerRelCoor();

    virtual void		Draw();
};


class Figure : public GeomEntity, public PhisEntity
{
public :
    static const unsigned int   BlocksCount = 4;
private :    
    Block                       *mpBlocks[ BlocksCount ];
	
public :
				Figure( int aX, int aY, int aZ, Figures aType, Material aNewMaterial );
                                Figure( const Figure& aFig );
				 ~Figure();

    Point3Di			GetBlockPosByIndexi( int aIndex ) const;
    Point3Df			GetBlockPosByIndexf( int aIndex ) const;
    Point3Di                    GetLeftMostBlockPosi();
    Point3Di                    GetRightMostBlockPosi();
    Point3Di                    GetBackMostBlockPosi();
    Point3Di                    GetAheadMostBlockPosi();
    Material			GetBlockMaterialByIndex( int aIndex ) const;
    bool                        IsIntersectWithBlock( Block* apBlock );
    bool                        CheckToCollisonWithBlocks( std :: vector < Block* >& aBlocks );
    void                        SetBlocksAbsCoor();
    void                        SetBlocksRelCoor();

    friend QTextStream& operator << ( QTextStream& aStream, const Figure& aMaterial );
    friend QTextStream& operator >> ( QTextStream& aStream, Figure& aMaterial )	;

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

    virtual void		RotateOnZY( float aAngle, bool aChangeConst );
    virtual void		RotateOnZX( float aAngle, bool aChangeConst );
    virtual void		RotateOnXY( float aAngle, bool aChangeConst );

    virtual void                SetVerAbsCoor();
    virtual void                SetVerRelCoor();

    virtual void		Draw();
};

QTextStream& operator << ( QTextStream& aStream, const Figures& aFigure );
QTextStream& operator >> ( QTextStream& aStream, Figures& aFigure );

#endif
