#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <QApplication>
#include <QGLWidget>
#include <QDebug>
#include <stdio.h>
#include "GameObjects.h"

const double Block :: SAFETY_DISTANCE       = Block :: BLOCK_SIZE * Block :: BLOCK_SIZE * 3.0f;
const double Block :: NOT_SAFETY_DISTANCE   = Block :: BLOCK_SIZE * Block :: BLOCK_SIZE;

//Block

const Point3Di Block :: msVerticesI[ Block :: BLOCKS_VERTEX_CNT ] = {
        Point3Di( -Block :: BLOCK_SIZE / 2,  Block :: BLOCK_SIZE / 2, -Block :: BLOCK_SIZE / 2 ),
        Point3Di( -Block :: BLOCK_SIZE / 2,  Block :: BLOCK_SIZE / 2,  Block :: BLOCK_SIZE / 2 ),
        Point3Di(  Block :: BLOCK_SIZE / 2,  Block :: BLOCK_SIZE / 2,  Block :: BLOCK_SIZE / 2 ),
        Point3Di(  Block :: BLOCK_SIZE / 2,  Block :: BLOCK_SIZE / 2, -Block :: BLOCK_SIZE / 2 ),
        Point3Di(  Block :: BLOCK_SIZE / 2, -Block :: BLOCK_SIZE / 2, -Block :: BLOCK_SIZE / 2 ),
        Point3Di(  Block :: BLOCK_SIZE / 2, -Block :: BLOCK_SIZE / 2,  Block :: BLOCK_SIZE / 2 ),
        Point3Di( -Block :: BLOCK_SIZE / 2, -Block :: BLOCK_SIZE / 2,  Block :: BLOCK_SIZE / 2 ),
        Point3Di( -Block :: BLOCK_SIZE / 2, -Block :: BLOCK_SIZE / 2, -Block :: BLOCK_SIZE / 2 )
};

Block :: Block() : GeomEntity( 0, 0, 0 ), PhisEntity( PhisEntity :: msMaterials[ 3 ] )
{
     for ( unsigned int i = 0; i < BLOCKS_VERTEX_CNT; i++ )
        mVerticesF[ i ] = msVerticesI[ i ];
}

Block :: Block( int aNewX, int aNewY, int aNewZ, Material aNewMaterial ) : GeomEntity( aNewX, aNewY, aNewZ ), PhisEntity( aNewMaterial )
{
    for ( unsigned int i = 0; i < BLOCKS_VERTEX_CNT; i++ )
        mVerticesF[ i ] = msVerticesI[ i ];
}

Block :: Block( Point3Di aNewPos, Material aNewMaterial ) : GeomEntity( aNewPos ), PhisEntity( aNewMaterial )
{
    for ( unsigned int i = 0; i < BLOCKS_VERTEX_CNT; i++ )
        mVerticesF[ i ] = msVerticesI[ i ];
}

Block :: Block( const Block& aBlock) : GeomEntity( aBlock.mPosI ), PhisEntity( aBlock.mMaterial )
{
     for ( unsigned int i = 0; i < BLOCKS_VERTEX_CNT; i++ )
        mVerticesF[ i ] = aBlock.mVerticesF[ i ];
}

Block :: Block( float aNewX, float aNewY, float aNewZ, Material aNewMaterial )  : GeomEntity( aNewX, aNewY, aNewZ ), PhisEntity( aNewMaterial )
{
     for ( unsigned int i = 0; i < BLOCKS_VERTEX_CNT; i++ )
        mVerticesF[ i ] = msVerticesI[ i ];
}

Block :: Block( Point3Df aNewPos, Material aNewMaterial ) : GeomEntity( aNewPos ), PhisEntity( aNewMaterial )
{
     for ( unsigned int i = 0; i < BLOCKS_VERTEX_CNT; i++ )
        mVerticesF[ i ] = msVerticesI[ i ];
}

void Block :: Rotate( float &aA, float &aB, float aAngle )
{
    float cos_angle = cos( aAngle );
    float sin_angle = sin( aAngle );
    float old_a = aA;

    aA =  aA    * cos_angle + aB * sin_angle;
    aB = -old_a * sin_angle + aB * cos_angle;
}

void Block :: RotateOnZY( float aAngle, bool aChangeConst )
{
    int prev_y = 0;

    if ( !aChangeConst )
    {
	mPosF = mPosI;
        Rotate( mPosF.mY, mPosF.mZ, aAngle );

	for ( unsigned int i = 0; i < BLOCKS_VERTEX_CNT; ++i )
	{
	    mVerticesF[ i ] = msVerticesI[ i ];
            Rotate( mVerticesF[ i ].mY,  mVerticesF[ i ].mZ, aAngle  );
	}
    }
    else
    {	
        prev_y = mPosI.mY;

        if ( aAngle > Geometry :: eps )
	{
            mPosI.mY =  mPosI.mZ;
            mPosI.mZ = -prev_y;
	}
	else
	{
            mPosI.mY = -mPosI.mZ;
            mPosI.mZ =  prev_y;
	}
	mPosF = mPosI;

	for ( unsigned int i = 0; i < BLOCKS_VERTEX_CNT; ++i )
	    mVerticesF[ i ] = msVerticesI[ i ];
    }

}

void Block :: RotateOnZX( float aAngle, bool aChangeConst )
{
    int	 prev_z = 0;

    if ( !aChangeConst )
    {
	mPosF = mPosI;
	Rotate( mPosF.mZ, mPosF.mX, aAngle );

	for ( unsigned int i = 0; i < BLOCKS_VERTEX_CNT; ++i )
	{
	    mVerticesF[ i ] = msVerticesI[ i ];
	    Rotate( mVerticesF[ i ].mZ, mVerticesF[ i ].mX, aAngle  );
	}
    }
    else
    {
	prev_z = mPosI.mZ;

        if ( aAngle > Geometry :: eps )
	{
	    mPosI.mZ =  mPosI.mX;
	    mPosI.mX =  -prev_z;
	}
	else
	{
	    mPosI.mZ = -mPosI.mX;
	    mPosI.mX =  prev_z;
	}
	mPosF   = mPosI;

	for ( unsigned int i = 0; i < BLOCKS_VERTEX_CNT; ++i )
	    mVerticesF[ i ] = msVerticesI[ i ];
    }
}

void Block :: RotateOnXY( float aAngle, bool aChangeConst )
{
    int prev_x = 0;

    if ( !aChangeConst )
    {
	mPosF = mPosI;
	Rotate( mPosF.mX, mPosF.mY, aAngle );

	for ( unsigned int i = 0; i < BLOCKS_VERTEX_CNT; ++i )
	{
	    mVerticesF[ i ] = msVerticesI[ i ];
	    Rotate( mVerticesF[ i ].mX, mVerticesF[ i ].mY, aAngle  );
	}
    }
    else
    {	
	prev_x = mPosI.mX;

        if ( aAngle > Geometry :: eps )
	{
	    mPosI.mX =  mPosI.mY;
	    mPosI.mY = -prev_x;
	}
	else
	{
	    mPosI.mX = -mPosI.mY;
	    mPosI.mY =  prev_x;
	}
	mPosF   = mPosI;

	for ( unsigned int i = 0; i < BLOCKS_VERTEX_CNT; ++i )
	    mVerticesF[ i ] = msVerticesI[ i ];
    }
}

void Block :: DrawSide( Point3Df aP1, Point3Df aP2, Point3Df aP3, Point3Df aP4 )
{
    glEnable(GL_NORMALIZE);
    Point3Df NormalVector = GetNormalVector( aP1, aP3, aP2 );

    glBegin( GL_QUADS );
	glNormal3f( NormalVector.mX, NormalVector.mY, NormalVector.mZ );
	glVertex3f( aP1.mX, aP1.mY, aP1.mZ );
	glVertex3f( aP2.mX, aP2.mY, aP2.mZ );
	glVertex3f( aP3.mX, aP3.mY, aP3.mZ );
	glVertex3f( aP4.mX, aP4.mY, aP4.mZ );
    glEnd();
}

void Block :: Draw()
{
    Draw( Point3Df( 0.0f, 0.0f, 0.0f ) );
}

void Block :: Draw( Point3Df aFigPos )
{
    /*
	Вершины куба занумерованы в след. порядке( делим куб на 4 части ):
	1)-,+,- 2)-,+,+ 3)+,+,+ 4)+,+,-
	5)+,-,- 6)+,-,+ 7)-,-,+ 8)-,-,-
    */

    glMaterialfv( GL_FRONT, GL_AMBIENT, mMaterial.GetMaterialForAmbient() );
    glMaterialfv( GL_FRONT, GL_DIFFUSE, mMaterial.GetMaterialForDiffuse() );
    glMaterialfv( GL_FRONT, GL_SPECULAR, mMaterial.GetMaterialForSpecular() );

    //Firts side
    DrawSide(	Point3Df( aFigPos.mX + mPosF.mX + mVerticesF[ 0 ].mX,
                          aFigPos.mY + mPosF.mY + mVerticesF[ 0 ].mY,
                          aFigPos.mZ + mPosF.mZ + mVerticesF[ 0 ].mZ ),

                Point3Df( aFigPos.mX + mPosF.mX + mVerticesF[ 1 ].mX,
                          aFigPos.mY + mPosF.mY + mVerticesF[ 1 ].mY,
                          aFigPos.mZ + mPosF.mZ + mVerticesF[ 1 ].mZ ),

                Point3Df( aFigPos.mX + mPosF.mX + mVerticesF[ 2 ].mX,
                          aFigPos.mY + mPosF.mY + mVerticesF[ 2 ].mY,
                          aFigPos.mZ + mPosF.mZ + mVerticesF[ 2 ].mZ ),

                Point3Df( aFigPos.mX + mPosF.mX + mVerticesF[ 3 ].mX,
                          aFigPos.mY + mPosF.mY + mVerticesF[ 3 ].mY,
                          aFigPos.mZ + mPosF.mZ + mVerticesF[ 3 ].mZ )
            );

    //Second side
    DrawSide(	Point3Df( aFigPos.mX + mPosF.mX + mVerticesF[ 1 ].mX,
                          aFigPos.mY + mPosF.mY + mVerticesF[ 1 ].mY,
                          aFigPos.mZ + mPosF.mZ + mVerticesF[ 1 ].mZ ),

                Point3Df( aFigPos.mX + mPosF.mX + mVerticesF[ 6 ].mX,
                          aFigPos.mY + mPosF.mY + mVerticesF[ 6 ].mY,
                          aFigPos.mZ + mPosF.mZ + mVerticesF[ 6 ].mZ ),

                Point3Df( aFigPos.mX + mPosF.mX + mVerticesF[ 5 ].mX,
                          aFigPos.mY + mPosF.mY + mVerticesF[ 5 ].mY,
                          aFigPos.mZ + mPosF.mZ + mVerticesF[ 5 ].mZ ),

                Point3Df( aFigPos.mX + mPosF.mX + mVerticesF[ 2 ].mX,
                          aFigPos.mY + mPosF.mY + mVerticesF[ 2 ].mY,
                          aFigPos.mZ + mPosF.mZ + mVerticesF[ 2 ].mZ )
            );

    //Third side
    DrawSide(	Point3Df( aFigPos.mX + mPosF.mX + mVerticesF[ 2 ].mX,
                          aFigPos.mY + mPosF.mY + mVerticesF[ 2 ].mY,
                          aFigPos.mZ + mPosF.mZ + mVerticesF[ 2 ].mZ ),

                Point3Df( aFigPos.mX + mPosF.mX + mVerticesF[ 5 ].mX,
                          aFigPos.mY + mPosF.mY + mVerticesF[ 5 ].mY,
                          aFigPos.mZ + mPosF.mZ + mVerticesF[ 5 ].mZ ),

                Point3Df( aFigPos.mX + mPosF.mX + mVerticesF[ 4 ].mX,
                          aFigPos.mY + mPosF.mY + mVerticesF[ 4 ].mY,
                          aFigPos.mZ + mPosF.mZ + mVerticesF[ 4 ].mZ ),

                Point3Df( aFigPos.mX + mPosF.mX + mVerticesF[ 3 ].mX,
                          aFigPos.mY + mPosF.mY + mVerticesF[ 3 ].mY,
                          aFigPos.mZ + mPosF.mZ + mVerticesF[ 3 ].mZ )
            );

    //Fourth side
    DrawSide(	Point3Df( aFigPos.mX + mPosF.mX + mVerticesF[ 6 ].mX,
                          aFigPos.mY + mPosF.mY + mVerticesF[ 6 ].mY,
                          aFigPos.mZ + mPosF.mZ + mVerticesF[ 6 ].mZ ),

                Point3Df( aFigPos.mX + mPosF.mX + mVerticesF[ 7 ].mX,
                          aFigPos.mY + mPosF.mY + mVerticesF[ 7 ].mY,
                          aFigPos.mZ + mPosF.mZ + mVerticesF[ 7 ].mZ ),

                Point3Df( aFigPos.mX + mPosF.mX + mVerticesF[ 4 ].mX,
                          aFigPos.mY + mPosF.mY + mVerticesF[ 4 ].mY,
                          aFigPos.mZ + mPosF.mZ + mVerticesF[ 4 ].mZ ),

                Point3Df( aFigPos.mX + mPosF.mX + mVerticesF[ 5 ].mX,
                          aFigPos.mY + mPosF.mY + mVerticesF[ 5 ].mY,
                          aFigPos.mZ + mPosF.mZ + mVerticesF[ 5 ].mZ )
            );

    //Fifth side
    DrawSide(	Point3Df( aFigPos.mX + mPosF.mX + mVerticesF[ 0 ].mX,
                          aFigPos.mY + mPosF.mY + mVerticesF[ 0 ].mY,
                          aFigPos.mZ + mPosF.mZ + mVerticesF[ 0 ].mZ ),

                Point3Df( aFigPos.mX + mPosF.mX + mVerticesF[ 7 ].mX,
                          aFigPos.mY + mPosF.mY + mVerticesF[ 7 ].mY,
                          aFigPos.mZ + mPosF.mZ + mVerticesF[ 7 ].mZ ),

                Point3Df( aFigPos.mX + mPosF.mX + mVerticesF[ 6 ].mX,
                          aFigPos.mY + mPosF.mY + mVerticesF[ 6 ].mY,
                          aFigPos.mZ + mPosF.mZ + mVerticesF[ 6 ].mZ ),

                Point3Df( aFigPos.mX + mPosF.mX + mVerticesF[ 1 ].mX,
                          aFigPos.mY + mPosF.mY + mVerticesF[ 1 ].mY,
                          aFigPos.mZ + mPosF.mZ + mVerticesF[ 1 ].mZ )
            );

    //Sixth side
    DrawSide(	Point3Df( aFigPos.mX + mPosF.mX + mVerticesF[ 3 ].mX,
                          aFigPos.mY + mPosF.mY + mVerticesF[ 3 ].mY,
                          aFigPos.mZ + mPosF.mZ + mVerticesF[ 3 ].mZ ),

                Point3Df( aFigPos.mX + mPosF.mX + mVerticesF[ 4 ].mX,
                          aFigPos.mY + mPosF.mY + mVerticesF[ 4 ].mY,
                          aFigPos.mZ + mPosF.mZ + mVerticesF[ 4 ].mZ ),

                Point3Df( aFigPos.mX + mPosF.mX + mVerticesF[ 7 ].mX,
                          aFigPos.mY + mPosF.mY + mVerticesF[ 7 ].mY,
                          aFigPos.mZ + mPosF.mZ + mVerticesF[ 7 ].mZ ),

                Point3Df( aFigPos.mX + mPosF.mX + mVerticesF[ 0 ].mX,
                          aFigPos.mY + mPosF.mY + mVerticesF[ 0 ].mY,
                          aFigPos.mZ + mPosF.mZ + mVerticesF[ 0 ].mZ )
            );
}

float Block :: LowerBoundXf()
{
    float min_x = mVerticesF[ 0 ].mX;

    for ( unsigned int i = 1; i < BLOCKS_VERTEX_CNT; i++ )
        min_x = Geometry :: Min( min_x, mVerticesF[ i ].mX );

    return min_x + mPosF.mX;
}

float Block :: UpperBoundXf()
{
    float max_x = mVerticesF[ 0 ].mX;

    for ( unsigned int i = 1; i < BLOCKS_VERTEX_CNT; i++ )
        max_x = Geometry :: Max( max_x, mVerticesF[ i ].mX );

    return max_x + mPosF.mX;
}

float Block :: LowerBoundYf()
{
    float min_y = mVerticesF[ 0 ].mY;

    for ( unsigned int i = 1; i < BLOCKS_VERTEX_CNT; i++ )
        min_y = Geometry :: Min( min_y, mVerticesF[ i ].mY );

    return min_y + mPosF.mY;
}

float Block :: UpperBoundYf()
{
    float max_y = mVerticesF[ 0 ].mY;

    for ( unsigned int i = 1; i < BLOCKS_VERTEX_CNT; i++ )
        max_y = Geometry :: Max( max_y, mVerticesF[ i ].mY );

    return max_y + mPosF.mY;
}

float Block :: LowerBoundZf()
{
    float min_z = mVerticesF[ 0 ].mZ;

    for ( unsigned int i = 1; i < BLOCKS_VERTEX_CNT; i++ )
        min_z = Geometry :: Min( min_z, mVerticesF[ i ].mZ );

    return min_z + mPosF.mZ;
}

float Block :: UpperBoundZf()
{
    float max_z = mVerticesF[ 0 ].mZ;

    for ( unsigned int i = 1; i < BLOCKS_VERTEX_CNT; i++ )
        max_z = Geometry :: Max( max_z, mVerticesF[ i ].mZ );

    return max_z + mPosF.mZ;
}

int Block :: LowerBoundXi()
{
    int min_x = msVerticesI[ 0 ].mX;

    for ( unsigned int i = 1; i < BLOCKS_VERTEX_CNT; i++ )
        min_x = Geometry :: Min( min_x, msVerticesI[ i ].mX );

    return min_x + mPosI.mX;
}

int Block :: UpperBoundXi()
{
    int max_y = msVerticesI[ 0 ].mY;

    for ( unsigned int i = 1; i < BLOCKS_VERTEX_CNT; i++ )
        max_y = Geometry :: Max( max_y, msVerticesI[ i ].mY );

    return max_y + mPosI.mX;
}

int Block :: LowerBoundYi()
{
    int min_y = msVerticesI[ 0 ].mY;

    for ( unsigned int i = 1; i < BLOCKS_VERTEX_CNT; i++ )
        min_y = Geometry :: Min( min_y, msVerticesI[ i ].mY );

    return min_y + mPosI.mY;
}

int Block :: UpperBoundYi()
{
    int max_y = msVerticesI[ 0 ].mY;

    for ( unsigned int i = 1; i < BLOCKS_VERTEX_CNT; i++ )
        max_y = Geometry :: Max( max_y, msVerticesI[ i ].mY );

    return max_y + mPosI.mY;
}

int Block :: LowerBoundZi()
{
    int min_z = msVerticesI[ 0 ].mZ;

    for ( unsigned int i = 1; i < BLOCKS_VERTEX_CNT; i++ )
        min_z = Geometry :: Min( min_z, msVerticesI[ i ].mZ );

    return min_z + mPosI.mZ;
}

int Block :: UpperBoundZi()
{
    int max_z = msVerticesI[ 0 ].mZ;

    for ( unsigned int i = 1; i < BLOCKS_VERTEX_CNT; i++ )
        max_z = Geometry :: Max( max_z, msVerticesI[ i ].mZ );

    return max_z + mPosI.mZ;
}

 bool Block :: PointIn( Point3Df aPoint )
 {

   return   ( Geometry :: MixedMul( aPoint          - mVerticesF[ 0 ],
                                    mVerticesF[ 1 ] - mVerticesF[ 0 ],
                                    mVerticesF[ 3 ] - mVerticesF[ 0 ] ) < -Geometry :: eps ) &&

            ( Geometry :: MixedMul( aPoint          - mVerticesF[ 1 ],
                                    mVerticesF[ 6 ] - mVerticesF[ 1 ],
                                    mVerticesF[ 2 ] - mVerticesF[ 1 ] ) < -Geometry :: eps ) &&

            ( Geometry :: MixedMul( aPoint          - mVerticesF[ 2 ],
                                    mVerticesF[ 5 ] - mVerticesF[ 2 ],
                                    mVerticesF[ 3 ] - mVerticesF[ 2 ] ) < -Geometry :: eps ) &&

            ( Geometry :: MixedMul( aPoint          - mVerticesF[ 3 ],
                                    mVerticesF[ 4 ] - mVerticesF[ 3 ],
                                    mVerticesF[ 0 ] - mVerticesF[ 3 ] ) < -Geometry :: eps ) &&

            ( Geometry :: MixedMul( aPoint          - mVerticesF[ 0 ],
                                    mVerticesF[ 7 ] - mVerticesF[ 0 ],
                                    mVerticesF[ 1 ] - mVerticesF[ 0 ] ) < -Geometry :: eps ) &&

            ( Geometry :: MixedMul( aPoint          - mVerticesF[ 6 ],
                                    mVerticesF[ 7 ] - mVerticesF[ 6 ],
                                    mVerticesF[ 5 ] - mVerticesF[ 6 ] ) < -Geometry :: eps );
}

bool Block :: PointIn( Point3Di aPoint )
{
    return PointIn( Point3Df( aPoint ) );
}

bool Block :: CheckEdgesAveragePoint( Block* apBlock )
{
    return apBlock -> PointIn( 0.5f * ( mVerticesF[ 1 ] - mVerticesF[ 0 ] ) + mVerticesF[ 0 ] ) ||
           apBlock -> PointIn( 0.5f * ( mVerticesF[ 2 ] - mVerticesF[ 1 ] ) + mVerticesF[ 1 ] ) ||
           apBlock -> PointIn( 0.5f * ( mVerticesF[ 3 ] - mVerticesF[ 2 ] ) + mVerticesF[ 2 ] ) ||
           apBlock -> PointIn( 0.5f * ( mVerticesF[ 0 ] - mVerticesF[ 3 ] ) + mVerticesF[ 3 ] ) ||
           apBlock -> PointIn( 0.5f * ( mVerticesF[ 6 ] - mVerticesF[ 7 ] ) + mVerticesF[ 7 ] ) ||
           apBlock -> PointIn( 0.5f * ( mVerticesF[ 5 ] - mVerticesF[ 6 ] ) + mVerticesF[ 6 ] ) ||
           apBlock -> PointIn( 0.5f * ( mVerticesF[ 4 ] - mVerticesF[ 5 ] ) + mVerticesF[ 5 ] ) ||
           apBlock -> PointIn( 0.5f * ( mVerticesF[ 7 ] - mVerticesF[ 4 ] ) + mVerticesF[ 4 ] ) ||
           apBlock -> PointIn( 0.5f * ( mVerticesF[ 7 ] - mVerticesF[ 0 ] ) + mVerticesF[ 0 ] ) ||
           apBlock -> PointIn( 0.5f * ( mVerticesF[ 6 ] - mVerticesF[ 1 ] ) + mVerticesF[ 1 ] ) ||
           apBlock -> PointIn( 0.5f * ( mVerticesF[ 5 ] - mVerticesF[ 2 ] ) + mVerticesF[ 2 ] ) ||
           apBlock -> PointIn( 0.5f * ( mVerticesF[ 4 ] - mVerticesF[ 3 ] ) + mVerticesF[ 3 ] );
}

bool Block :: IsIntersect( Block* apBlock )
{
    float distance = ( apBlock -> GetPosf() - mPosF ).Length();

     if ( distance - Geometry :: eps > Block :: SAFETY_DISTANCE )
            return false;
     if ( distance < Block :: NOT_SAFETY_DISTANCE - Geometry :: eps )
            return true;

    for ( unsigned int i = 0; i < BLOCKS_VERTEX_CNT; i++ )
        if ( apBlock -> PointIn( mVerticesF[ i ] ) || PointIn( apBlock -> mVerticesF[ i ] ) )
            return true;

    return this -> CheckEdgesAveragePoint( apBlock ) || apBlock -> CheckEdgesAveragePoint( this );
}

//Figure

Figure :: Figure( int aX, int aY, int aZ, Figures aType, Material aMaterial ) : GeomEntity( aX, aY, aZ ), PhisEntity( aMaterial )
{
    if ( Block :: BLOCK_SIZE % 2 != 0 )
    {
	qDebug() << "Odd block size!\n";
	QApplication :: exit( 1 );
    }

    switch ( aType )
    {
    case IFigure :
        mpBlocks[ 0 ] = new Block(  -( int )Block :: BLOCK_SIZE - ( int )Block :: BLOCK_SIZE / 2,
                                    0,
                                    0,
                                    aMaterial );

        mpBlocks[ 1 ] = new Block( -( int )Block :: BLOCK_SIZE / 2,
                                    0,
                                    0,
                                    aMaterial );

        mpBlocks[ 2 ] = new Block( ( int )Block :: BLOCK_SIZE / 2,
                                    0,
                                    0,
                                    aMaterial );

        mpBlocks[ 3 ] = new Block(  ( int )( Block :: BLOCK_SIZE + Block :: BLOCK_SIZE / 2 ),
                                    0,
                                    0,
                                    aMaterial );
	break;
    case JFigure :
        mpBlocks[ 0 ] = new Block(  ( int )Block :: BLOCK_SIZE / 2,
                                    ( int )Block :: BLOCK_SIZE,
                                    0,
                                    aMaterial );

        mpBlocks[ 1 ] = new Block(  ( int )Block :: BLOCK_SIZE / 2,
                                    0,
                                    0,
                                    aMaterial );

        mpBlocks[ 2 ] = new Block(  ( int )Block :: BLOCK_SIZE / 2,
                                   -( int )( Block :: BLOCK_SIZE ),
                                    0,
                                    aMaterial );

        mpBlocks[ 3 ] = new Block( -( int )Block :: BLOCK_SIZE / 2,
                                   -( int )Block :: BLOCK_SIZE,
                                    0,
                                    aMaterial );
	break;
    case LFigure :
        mpBlocks[ 0 ] = new Block( -( int )Block :: BLOCK_SIZE / 2,
                                    ( int )Block :: BLOCK_SIZE,
                                    0,
                                    aMaterial );

        mpBlocks[ 1 ] = new Block( -( int )Block :: BLOCK_SIZE / 2,
                                    0,
                                    0,
                                    aMaterial );

        mpBlocks[ 2 ] = new Block( -( int )Block :: BLOCK_SIZE / 2,
                                   -( int )Block :: BLOCK_SIZE,
                                    0,
                                    aMaterial );

        mpBlocks[ 3 ] = new Block(  ( int )Block :: BLOCK_SIZE / 2,
                                   -( int )Block :: BLOCK_SIZE,
                                    0,
                                    aMaterial );
	break;
    case OFigure :
        mpBlocks[ 0 ] = new Block( -( int )Block :: BLOCK_SIZE / 2,
                                    ( int )Block :: BLOCK_SIZE / 2,
                                    0,
                                    aMaterial );

        mpBlocks[ 1 ] = new Block(  ( int )Block :: BLOCK_SIZE / 2,
                                    ( int )Block :: BLOCK_SIZE / 2,
                                    0,
                                    aMaterial );

        mpBlocks[ 2 ] = new Block(  ( int )Block :: BLOCK_SIZE / 2,
                                   -( int )Block :: BLOCK_SIZE / 2,
                                    0,
                                    aMaterial );

        mpBlocks[ 3 ] = new Block( -( int )Block :: BLOCK_SIZE / 2,
                                   -( int )Block :: BLOCK_SIZE / 2,
                                    0,
                                    aMaterial );
	break;
    case SFigure :
        mpBlocks[ 0 ] = new Block(  ( int )Block :: BLOCK_SIZE,
                                    ( int )Block :: BLOCK_SIZE / 2,
                                    0,
                                    aMaterial );

        mpBlocks[ 1 ] = new Block(  0,
                                    ( int )Block :: BLOCK_SIZE / 2,
                                    0,
                                    aMaterial );

        mpBlocks[ 2 ] = new Block(  0,
                                   -( int )Block :: BLOCK_SIZE / 2,
                                    0,
                                    aMaterial );

        mpBlocks[ 3 ] = new Block( -( int )Block :: BLOCK_SIZE,
                                   -( int )Block :: BLOCK_SIZE / 2,
                                    0,
                                    aMaterial );
	break;
    case TFigure :
        mpBlocks[ 0 ] = new Block(  ( int )Block :: BLOCK_SIZE,
                                    ( int )Block :: BLOCK_SIZE / 2,
                                    0,
                                    aMaterial );

        mpBlocks[ 1 ] = new Block(  0,
                                    ( int )Block :: BLOCK_SIZE / 2,
                                    0,
                                    aMaterial );

        mpBlocks[ 2 ] = new Block( -( int )Block :: BLOCK_SIZE,
                                    ( int )Block :: BLOCK_SIZE / 2,
                                    0,
                                    aMaterial );

        mpBlocks[ 3 ] = new Block(  0,
                                   -( int )Block :: BLOCK_SIZE / 2,
                                    0,
                                    aMaterial );
	break;
    default : //ZFigure :
        mpBlocks[ 0 ] = new Block( -( int )Block :: BLOCK_SIZE,
                                    ( int )Block :: BLOCK_SIZE / 2,
                                    0,
                                    aMaterial );

        mpBlocks[ 1 ] = new Block(  0,
                                    ( int )Block :: BLOCK_SIZE / 2,
                                    0,
                                    aMaterial );
        mpBlocks[ 2 ] = new Block(  0,
                                   -( int )Block :: BLOCK_SIZE / 2,
                                    0,
                                    aMaterial );

        mpBlocks[ 3 ] = new Block(  ( int )Block :: BLOCK_SIZE,
                                   -( int )Block :: BLOCK_SIZE / 2,
                                    0,
                                    aMaterial );
    }
}

Figure :: Figure( const Figure& aFig ) : GeomEntity( aFig.mPosI ), PhisEntity( mMaterial )
{
    for ( unsigned int i = 0; i < BlocksCount; i++ )
        mpBlocks[ i ] = new Block( aFig.mpBlocks[ i ] -> GetPosi(), aFig.mpBlocks[ i ] -> GetMaterial() );
}

Figure :: ~Figure()
{
    for ( unsigned int i = 0; i < BlocksCount; ++i )
	    delete mpBlocks[ i ];
}

void Figure :: Draw()
{
    for ( unsigned int i = 0; i < BlocksCount; ++i )
	mpBlocks[ i ] -> Draw( mPosF );
}

void Figure :: RotateOnZY( float aAngle, bool aChangeConst )
{
    for ( unsigned int i = 0; i < BlocksCount; ++i )
	mpBlocks[ i ] -> RotateOnZY( aAngle, aChangeConst );
}

void Figure :: RotateOnZX( float aAngle, bool ChangeConst )
{
    for ( unsigned int i = 0; i < BlocksCount; ++i )
        mpBlocks[ i ] -> RotateOnZX( aAngle, ChangeConst );
}

void Figure :: RotateOnXY( float aAngle, bool ChangeConst )
{
    for ( unsigned int i = 0; i < BlocksCount; ++i )
	mpBlocks[ i ] -> RotateOnXY( aAngle, ChangeConst );
}


float Figure :: LowerBoundXf()
{
    float min_x = mpBlocks[ 0 ] -> LowerBoundXf();

    for ( unsigned int i = 1; i < BlocksCount; i++ )
        min_x = Geometry :: Min( min_x, mpBlocks[ i ] -> LowerBoundXf() );

    return min_x + mPosF.mX;
}

float Figure :: UpperBoundXf()
{
    float max_x = mpBlocks[ 0 ] -> UpperBoundXf();

    for ( unsigned int i = 1; i < BlocksCount; i++ )
        max_x = Geometry :: Max( max_x, mpBlocks[ i ] -> UpperBoundXf() );

    return max_x + mPosF.mX;
}

float Figure :: LowerBoundYf()
{
    float min_y = mpBlocks[ 0 ] -> LowerBoundYf();

    for ( unsigned int i = 1; i < BlocksCount; i++ )
        min_y = Geometry :: Min( min_y, mpBlocks[ i ] -> LowerBoundYf() );

    return min_y + mPosF.mY;
}

float Figure :: UpperBoundYf()
{
    float max_y = mpBlocks[ 0 ] -> UpperBoundYf();

    for ( unsigned int i = 1; i < BlocksCount; i++ )
        max_y = Geometry :: Max( max_y, mpBlocks[ i ] -> UpperBoundYf() );

    return max_y + mPosF.mY;
}

float Figure :: LowerBoundZf()
{
    float min_z = mpBlocks[ 0 ] -> LowerBoundZf();

    for ( unsigned int i = 1; i < BlocksCount; i++ )
        min_z = Geometry :: Min( min_z, mpBlocks[ i ] -> LowerBoundZf() );

    return min_z + mPosF.mZ;
}

float Figure :: UpperBoundZf()
{
    float max_z = mpBlocks[ 0 ] -> UpperBoundZf();

    for ( unsigned int i = 1; i < BlocksCount; i++ )
        max_z = Geometry :: Max( max_z, mpBlocks[ i ] -> UpperBoundZf() );

    return max_z + mPosF.mZ;
}

int Figure :: LowerBoundXi()
{
    float min_x = mpBlocks[ 0 ] -> LowerBoundXi();

    for ( unsigned int i = 1; i < BlocksCount; i++ )
        min_x = Geometry :: Min( min_x, mpBlocks[ i ] -> LowerBoundXi() );

    return min_x + mPosI.mX;
}

int Figure :: UpperBoundXi()
{
    float max_x = mpBlocks[ 0 ] -> UpperBoundXi();

    for ( unsigned int i = 1; i < BlocksCount; i++ )
        max_x = Geometry :: Max( max_x, mpBlocks[ i ] -> UpperBoundXi() );

    return max_x + mPosI.mX;
}

int Figure :: LowerBoundYi()
{
    float min_y = mpBlocks[ 0 ] -> LowerBoundYi();

    for ( unsigned int i = 1; i < BlocksCount; i++ )
        min_y = Geometry :: Min( min_y, mpBlocks[ i ] -> LowerBoundYi() );

    return min_y + mPosI.mY;
}

int Figure :: UpperBoundYi()
{
    float max_y = mpBlocks[ 0 ] -> UpperBoundYf();

    for ( unsigned int i = 1; i < BlocksCount; i++ )
        max_y = Geometry :: Max( max_y, mpBlocks[ i ] -> UpperBoundYi() );

    return max_y + mPosI.mY;
}

int Figure :: LowerBoundZi()
{
    float min_z = mpBlocks[ 0 ] -> LowerBoundZi();

    for ( unsigned int i = 1; i < BlocksCount; i++ )
        min_z = Geometry :: Min( min_z, mpBlocks[ i ] -> LowerBoundZi() );

    return min_z + mPosI.mZ;
}

int Figure :: UpperBoundZi()
{
    int max_z = mpBlocks[ 0 ] -> UpperBoundZi();

    for ( unsigned int i = 1; i < BlocksCount; i++ )
        max_z = Geometry :: Max( max_z, mpBlocks[ i ] -> UpperBoundZi() );

    return max_z + mPosI.mZ;
}

Point3Di Figure :: GetBlockPosByIndexi( int aIndex ) const
{
    return mpBlocks[ aIndex ] -> GetPosi();
}

Point3Df Figure :: GetBlockPosByIndexf( int aIndex ) const
{
    return mpBlocks[ aIndex ] -> GetPosf();
}

Material Figure :: GetBlockMaterialByIndex( int aIndex ) const
{
    return mpBlocks[ aIndex ] -> GetMaterial();
}

bool Figure :: IsIntersectWithBlock( Block* apBlock )
{    
    for ( unsigned int i = 0; i < BlocksCount; i++ )
        if ( mpBlocks[ i ] -> IsIntersect( apBlock ) )
            return true;

    return false;
}

void Figure :: SetBlocksAbsCoor()
{
    for ( unsigned int i = 0; i < BlocksCount; i++ )
        mpBlocks[ i ] -> SetPosf( mpBlocks[ i ] -> GetPosf() + mPosF );
}

void Figure :: SetBlocksRelCoor()
{
    for ( unsigned int i = 0; i < BlocksCount; i++ )
        mpBlocks[ i ] -> SetPosf( mpBlocks[ i ] -> GetPosf() - mPosF );
}

void Block :: SetVerAbsCoor()
{
    for ( unsigned int i = 0; i < BLOCKS_VERTEX_CNT; i++ )
        mVerticesF[ i ] = mVerticesF[ i ] + mPosF;
}

void Block :: SetVerRelCoor()
{
     for ( unsigned int i = 0; i < BLOCKS_VERTEX_CNT; i++ )
        mVerticesF[ i ] = mVerticesF[ i ] - mPosF;
}

void Figure :: SetVerAbsCoor()
{
    for ( unsigned int i = 0; i < BlocksCount; i++ )
    {
        mpBlocks[ i ] -> SetPosf( mpBlocks[ i ] -> GetPosf() + mPosF );
        mpBlocks[ i ] -> SetVerAbsCoor();
    }
}

void Figure :: SetVerRelCoor()
{
     for ( unsigned int i = 0; i < BlocksCount; i++ )
    {
        mpBlocks[ i ] -> SetVerRelCoor();
        mpBlocks[ i ] -> SetPosf( mpBlocks[ i ] -> GetPosf() - mPosF );
    }
}

bool Figure :: CheckToCollisonWithBlocks( std :: vector < Block* >& aCollisionBlocks )
{
    bool collision          = false;
    int  count_of_blocks    = aCollisionBlocks.size();

    SetVerAbsCoor();

    for ( int i = 0; i < count_of_blocks; ++i )
    {
        aCollisionBlocks[ i ] -> SetVerAbsCoor();
        if ( IsIntersectWithBlock( aCollisionBlocks[ i ] ) )
        {
            collision = true;
            aCollisionBlocks[ i ] -> SetVerRelCoor();
            break;
        }
        aCollisionBlocks[ i ] -> SetVerRelCoor();
    }

    SetVerRelCoor();

    return collision;
}

Point3Di Figure :: GetLeftMostBlockPosi()
{
    int current_block_x_pos = 0;
    int index_of_block      = 0;
    int min_x_pos           = mpBlocks[ 0 ] -> GetPosByXi();

    for ( unsigned int i = 1; i < BlocksCount; i++ )
    {
        current_block_x_pos = mpBlocks[ i ] -> GetPosByXi();
        if ( min_x_pos > current_block_x_pos )
        {
            index_of_block = i;
            min_x_pos = current_block_x_pos;
        }
    }

    return mpBlocks[ index_of_block ] -> GetPosi() + mPosI;
}

Point3Di Figure :: GetRightMostBlockPosi()
{
    int current_block_x_pos = 0;
    int index_of_block      = 0;
    int max_x_pos           = mpBlocks[ 0 ] -> GetPosByXi();

    for ( unsigned int i = 1; i < BlocksCount; i++ )
    {
        current_block_x_pos = mpBlocks[ i ] -> GetPosByXi();
        if ( max_x_pos < current_block_x_pos )
        {
            index_of_block = i;
            max_x_pos = current_block_x_pos;
        }
    }

    return mpBlocks[ index_of_block ] -> GetPosi() + mPosI;
}


Point3Di Figure :: GetBackMostBlockPosi()
{
    int current_block_z_pos = 0;
    int index_of_block      = 0;
    int min_z_pos           = mpBlocks[ 0 ] -> GetPosByZi();

    for ( unsigned int i = 1; i < BlocksCount; i++ )
    {
        current_block_z_pos = mpBlocks[ i ] -> GetPosByZi();
        if ( min_z_pos > current_block_z_pos )
        {
            index_of_block = i;
            min_z_pos = current_block_z_pos;
        }
    }

    return mpBlocks[ index_of_block ] -> GetPosi() + mPosI;
}

Point3Di Figure :: GetAheadMostBlockPosi()
{
    int current_block_z_pos = 0;
    int index_of_block      = 0;
    int max_z_pos           = mpBlocks[ 0 ] -> GetPosByZi();

    for ( unsigned int i = 1; i < BlocksCount; i++ )
    {
        current_block_z_pos = mpBlocks[ i ] -> GetPosByZi();
        if ( max_z_pos < current_block_z_pos )
        {
            index_of_block = i;
            max_z_pos = current_block_z_pos;
        }
    }

    return mpBlocks[ index_of_block ] -> GetPosi() + mPosI;
}

QTextStream& operator << ( QTextStream& aStream, const Block& aBlock )
{
    aStream << aBlock.mPosI.mX << '\t' << aBlock.mPosI.mY << '\t' << aBlock.mPosI.mZ << '\n';
    aStream << aBlock.mPosF.mX << '\t' << aBlock.mPosF.mY << '\t' << aBlock.mPosF.mZ << '\n';
    aStream << aBlock.mMaterial << '\n';

    for ( int i = 0; i < Block :: BLOCKS_VERTEX_CNT; i++ )
        aStream << aBlock.mVerticesF[ i ].mX << '\t'
                << aBlock.mVerticesF[ i ].mY << '\t'
                << aBlock.mVerticesF[ i ].mZ << '\t';

    return aStream;
}

QTextStream& operator >> ( QTextStream& aStream, Block& aBlock )
{
    aStream >> aBlock.mPosI.mX >> aBlock.mPosI.mY >> aBlock.mPosI.mZ;
    aStream >> aBlock.mPosF.mX >> aBlock.mPosF.mY >> aBlock.mPosF.mZ;
    aStream >> aBlock.mMaterial;

    for ( int i = 0; i < Block :: BLOCKS_VERTEX_CNT; i++ )
        aStream >> aBlock.mVerticesF[ i ].mX
               >> aBlock.mVerticesF[ i ].mY
               >> aBlock.mVerticesF[ i ].mZ;

    return aStream;
}

QTextStream& operator << ( QTextStream& aStream, const Figure& aFigure )
{
    aStream << aFigure.mPosI.mX << '\t' << aFigure.mPosI.mY << '\t' << aFigure.mPosI.mZ << '\n';
    aStream << aFigure.mPosF.mX << '\t' << aFigure.mPosF.mY << '\t' << aFigure.mPosF.mZ << '\n';
    aStream << aFigure.mMaterial << '\n';

    for ( int i = 0; i < Figure :: BlocksCount; i++ )
        aStream << *aFigure.mpBlocks[ i ] << '\n';

    return aStream;
}

QTextStream& operator >> ( QTextStream& aStream, Figure& aFigure )
{
    aStream >> aFigure.mPosI.mX >> aFigure.mPosI.mY >> aFigure.mPosI.mZ;
    aStream >> aFigure.mPosF.mX >> aFigure.mPosF.mY >> aFigure.mPosF.mZ;
    aStream >> aFigure.mMaterial;

    for ( int i = 0; i < Figure :: BlocksCount; i++ )
        aStream >> *aFigure.mpBlocks[ i ];

    return aStream;
}

QTextStream& operator << ( QTextStream& aStream, const Figures& aFigure )
{
    aStream << static_cast< unsigned int >( aFigure );

    return aStream;
}

QTextStream& operator >> ( QTextStream& aStream, Figures& aFigure )
{
    int tmp_figure;
    aStream >> tmp_figure;
    aFigure = static_cast< Figures >( tmp_figure );

    return aStream;
}
