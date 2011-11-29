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

const Point3Di vertices_i[ 8 ] = {
        Point3Di( -( int )Block :: BLOCK_SIZE / 2,  ( int )Block :: BLOCK_SIZE / 2, -( int )Block :: BLOCK_SIZE / 2 ),
        Point3Di( -( int )Block :: BLOCK_SIZE / 2,  ( int )Block :: BLOCK_SIZE / 2,  ( int )Block :: BLOCK_SIZE / 2 ),
        Point3Di(  ( int )Block :: BLOCK_SIZE / 2,  ( int )Block :: BLOCK_SIZE / 2,  ( int )Block :: BLOCK_SIZE / 2 ),
        Point3Di(  ( int )Block :: BLOCK_SIZE / 2,  ( int )Block :: BLOCK_SIZE / 2, -( int )Block :: BLOCK_SIZE / 2 ),
        Point3Di(  ( int )Block :: BLOCK_SIZE / 2, -( int )Block :: BLOCK_SIZE / 2, -( int )Block :: BLOCK_SIZE / 2 ),
        Point3Di(  ( int )Block :: BLOCK_SIZE / 2, -( int )Block :: BLOCK_SIZE / 2,  ( int )Block :: BLOCK_SIZE / 2 ),
        Point3Di( -( int )Block :: BLOCK_SIZE / 2, -( int )Block :: BLOCK_SIZE / 2,  ( int )Block :: BLOCK_SIZE / 2 ),
        Point3Di( -( int )Block :: BLOCK_SIZE / 2, -( int )Block :: BLOCK_SIZE / 2, -( int )Block :: BLOCK_SIZE / 2 )
};

Block :: Block() : GeomEntity( 0, 0, 0 ), PhisEntity( materials[ 3 ] )
{
     for ( unsigned int i = 0; i < BLOCKS_VERTEX_CNT; i++ )
        mVerticesF[ i ] = vertices_i[ i ];
}

Block :: Block( int new_x, int new_y, int new_z, Material new_material ) : GeomEntity( new_x, new_y, new_z ), PhisEntity( new_material )
{
    for ( unsigned int i = 0; i < BLOCKS_VERTEX_CNT; i++ )
        mVerticesF[ i ] = vertices_i[ i ];
}

Block :: Block( Point3Di new_pos, Material new_material ) : GeomEntity( new_pos ), PhisEntity( new_material )
{
    for ( unsigned int i = 0; i < BLOCKS_VERTEX_CNT; i++ )
        mVerticesF[ i ] = vertices_i[ i ];
}

Block :: Block( const Block& block) : GeomEntity( block.mPosI ), PhisEntity( block.mMaterial )
{
     for ( unsigned int i = 0; i < BLOCKS_VERTEX_CNT; i++ )
        mVerticesF[ i ] = block.mVerticesF[ i ];
}

Block :: Block( float new_x, float new_y, float new_z, Material new_material )  : GeomEntity( new_x, new_y, new_z ), PhisEntity( new_material )
{
     for ( unsigned int i = 0; i < BLOCKS_VERTEX_CNT; i++ )
        mVerticesF[ i ] = vertices_i[ i ];
}

Block :: Block( Point3Df new_Pos, Material new_material ) : GeomEntity( new_Pos ), PhisEntity( new_material )
{
     for ( unsigned int i = 0; i < BLOCKS_VERTEX_CNT; i++ )
        mVerticesF[ i ] = vertices_i[ i ];
}

void Block :: Rotate( float &a, float &b, float angle )
{
    float cos_angle = cos( angle );
    float sin_angle = sin( angle );
    float old_a = a;

    a = a * cos_angle + b * sin_angle;
    b = -old_a * sin_angle + b * cos_angle;
}

void Block :: RotateOnZY( float angle, bool change_const )
{
    int	    prev_y = 0;

    if ( !change_const )
    {
	mPosF = mPosI;
        Rotate( mPosF.mY, mPosF.mZ, angle );

	for ( unsigned int i = 0; i < BLOCKS_VERTEX_CNT; ++i )
	{
	    mVerticesF[ i ] = vertices_i[ i ];
            Rotate( mVerticesF[ i ].mY,  mVerticesF[ i ].mZ, angle  );
	}
    }
    else
    {	
        prev_y = mPosI.mY;

        if ( angle > Geometry :: eps )
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
	    mVerticesF[ i ] = vertices_i[ i ];
    }

}

void Block :: RotateOnZX( float angle, bool change_const )
{
    int	 prev_z = 0;

    if ( !change_const )
    {
	mPosF = mPosI;
	Rotate( mPosF.mZ, mPosF.mX, angle );

	for ( unsigned int i = 0; i < BLOCKS_VERTEX_CNT; ++i )
	{
	    mVerticesF[ i ] = vertices_i[ i ];
	    Rotate( mVerticesF[ i ].mZ, mVerticesF[ i ].mX, angle  );
	}
    }
    else
    {
	prev_z = mPosI.mZ;

        if ( angle > Geometry :: eps )
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
	    mVerticesF[ i ] = vertices_i[ i ];
    }
}

void Block :: RotateOnXY( float angle, bool change_const )
{
    int prev_x = 0;

    if ( !change_const )
    {
	mPosF = mPosI;
	Rotate( mPosF.mX, mPosF.mY, angle );

	for ( unsigned int i = 0; i < BLOCKS_VERTEX_CNT; ++i )
	{
	    mVerticesF[ i ] = vertices_i[ i ];
	    Rotate( mVerticesF[ i ].mX, mVerticesF[ i ].mY, angle  );
	}
    }
    else
    {	
	prev_x = mPosI.mX;

        if ( angle > Geometry :: eps )
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
	    mVerticesF[ i ] = vertices_i[ i ];
    }
}

void Block :: DrawSide( Point3Df p1, Point3Df p2, Point3Df p3, Point3Df p4 )
{
    glEnable(GL_NORMALIZE);
    Point3Df NormalVector = GetNormalVector( p1, p3, p2 );

    glBegin( GL_QUADS );
	glNormal3f( NormalVector.mX, NormalVector.mY, NormalVector.mZ );
	glVertex3f( p1.mX, p1.mY, p1.mZ );	
	glVertex3f( p2.mX, p2.mY, p2.mZ );	
	glVertex3f( p3.mX, p3.mY, p3.mZ );	
	glVertex3f( p4.mX, p4.mY, p4.mZ );
    glEnd();
}

void Block :: Draw()
{
    Draw( Point3Df( 0.0f, 0.0f, 0.0f ) );
}

void Block :: Draw( Point3Df fig_pos )
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
    DrawSide(	Point3Df( fig_pos.mX + mPosF.mX + mVerticesF[ 0 ].mX, fig_pos.mY + mPosF.mY + mVerticesF[ 0 ].mY, fig_pos.mZ + mPosF.mZ + mVerticesF[ 0 ].mZ ),
                Point3Df( fig_pos.mX + mPosF.mX + mVerticesF[ 1 ].mX, fig_pos.mY + mPosF.mY + mVerticesF[ 1 ].mY, fig_pos.mZ + mPosF.mZ + mVerticesF[ 1 ].mZ ),
                Point3Df( fig_pos.mX + mPosF.mX + mVerticesF[ 2 ].mX, fig_pos.mY + mPosF.mY + mVerticesF[ 2 ].mY, fig_pos.mZ + mPosF.mZ + mVerticesF[ 2 ].mZ ),
                Point3Df( fig_pos.mX + mPosF.mX + mVerticesF[ 3 ].mX, fig_pos.mY + mPosF.mY + mVerticesF[ 3 ].mY, fig_pos.mZ + mPosF.mZ + mVerticesF[ 3 ].mZ )
            );

    //Second side
    DrawSide(	Point3Df( fig_pos.mX + mPosF.mX + mVerticesF[ 1 ].mX, fig_pos.mY + mPosF.mY + mVerticesF[ 1 ].mY, fig_pos.mZ + mPosF.mZ + mVerticesF[ 1 ].mZ ),
                Point3Df( fig_pos.mX + mPosF.mX + mVerticesF[ 6 ].mX, fig_pos.mY + mPosF.mY + mVerticesF[ 6 ].mY, fig_pos.mZ + mPosF.mZ + mVerticesF[ 6 ].mZ ),
                Point3Df( fig_pos.mX + mPosF.mX + mVerticesF[ 5 ].mX, fig_pos.mY + mPosF.mY + mVerticesF[ 5 ].mY, fig_pos.mZ + mPosF.mZ + mVerticesF[ 5 ].mZ ),
                Point3Df( fig_pos.mX + mPosF.mX + mVerticesF[ 2 ].mX, fig_pos.mY + mPosF.mY + mVerticesF[ 2 ].mY, fig_pos.mZ + mPosF.mZ + mVerticesF[ 2 ].mZ )
            );

    //Third side
    DrawSide(	Point3Df( fig_pos.mX + mPosF.mX + mVerticesF[ 2 ].mX, fig_pos.mY + mPosF.mY + mVerticesF[ 2 ].mY, fig_pos.mZ + mPosF.mZ + mVerticesF[ 2 ].mZ ),
                Point3Df( fig_pos.mX + mPosF.mX + mVerticesF[ 5 ].mX, fig_pos.mY + mPosF.mY + mVerticesF[ 5 ].mY, fig_pos.mZ + mPosF.mZ + mVerticesF[ 5 ].mZ ),
                Point3Df( fig_pos.mX + mPosF.mX + mVerticesF[ 4 ].mX, fig_pos.mY + mPosF.mY + mVerticesF[ 4 ].mY, fig_pos.mZ + mPosF.mZ + mVerticesF[ 4 ].mZ ),
                Point3Df( fig_pos.mX + mPosF.mX + mVerticesF[ 3 ].mX, fig_pos.mY + mPosF.mY + mVerticesF[ 3 ].mY, fig_pos.mZ + mPosF.mZ + mVerticesF[ 3 ].mZ )
            );

    //Fourth side
    DrawSide(	Point3Df( fig_pos.mX + mPosF.mX + mVerticesF[ 6 ].mX, fig_pos.mY + mPosF.mY + mVerticesF[ 6 ].mY, fig_pos.mZ + mPosF.mZ + mVerticesF[ 6 ].mZ ),
                Point3Df( fig_pos.mX + mPosF.mX + mVerticesF[ 7 ].mX, fig_pos.mY + mPosF.mY + mVerticesF[ 7 ].mY, fig_pos.mZ + mPosF.mZ + mVerticesF[ 7 ].mZ ),
                Point3Df( fig_pos.mX + mPosF.mX + mVerticesF[ 4 ].mX, fig_pos.mY + mPosF.mY + mVerticesF[ 4 ].mY, fig_pos.mZ + mPosF.mZ + mVerticesF[ 4 ].mZ ),
                Point3Df( fig_pos.mX + mPosF.mX + mVerticesF[ 5 ].mX, fig_pos.mY + mPosF.mY + mVerticesF[ 5 ].mY, fig_pos.mZ + mPosF.mZ + mVerticesF[ 5 ].mZ )
            );

    //Fifth side
    DrawSide(	Point3Df( fig_pos.mX + mPosF.mX + mVerticesF[ 0 ].mX, fig_pos.mY + mPosF.mY + mVerticesF[ 0 ].mY, fig_pos.mZ + mPosF.mZ + mVerticesF[ 0 ].mZ ),
                Point3Df( fig_pos.mX + mPosF.mX + mVerticesF[ 7 ].mX, fig_pos.mY + mPosF.mY + mVerticesF[ 7 ].mY, fig_pos.mZ + mPosF.mZ + mVerticesF[ 7 ].mZ ),
                Point3Df( fig_pos.mX + mPosF.mX + mVerticesF[ 6 ].mX, fig_pos.mY + mPosF.mY + mVerticesF[ 6 ].mY, fig_pos.mZ + mPosF.mZ + mVerticesF[ 6 ].mZ ),
                Point3Df( fig_pos.mX + mPosF.mX + mVerticesF[ 1 ].mX, fig_pos.mY + mPosF.mY + mVerticesF[ 1 ].mY, fig_pos.mZ + mPosF.mZ + mVerticesF[ 1 ].mZ )
            );

    //Sixth side
    DrawSide(	Point3Df( fig_pos.mX + mPosF.mX + mVerticesF[ 3 ].mX, fig_pos.mY + mPosF.mY + mVerticesF[ 3 ].mY, fig_pos.mZ + mPosF.mZ + mVerticesF[ 3 ].mZ ),
                Point3Df( fig_pos.mX + mPosF.mX + mVerticesF[ 4 ].mX, fig_pos.mY + mPosF.mY + mVerticesF[ 4 ].mY, fig_pos.mZ + mPosF.mZ + mVerticesF[ 4 ].mZ ),
                Point3Df( fig_pos.mX + mPosF.mX + mVerticesF[ 7 ].mX, fig_pos.mY + mPosF.mY + mVerticesF[ 7 ].mY, fig_pos.mZ + mPosF.mZ + mVerticesF[ 7 ].mZ ),
                Point3Df( fig_pos.mX + mPosF.mX + mVerticesF[ 0 ].mX, fig_pos.mY + mPosF.mY + mVerticesF[ 0 ].mY, fig_pos.mZ + mPosF.mZ + mVerticesF[ 0 ].mZ )
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
    int min_x = vertices_i[ 0 ].mX;

    for ( unsigned int i = 1; i < BLOCKS_VERTEX_CNT; i++ )
        min_x = Geometry :: Min( min_x, vertices_i[ i ].mX );

    return min_x + mPosI.mX;
}

int Block :: UpperBoundXi()
{
    int max_y = vertices_i[ 0 ].mY;

    for ( unsigned int i = 1; i < BLOCKS_VERTEX_CNT; i++ )
        max_y = Geometry :: Max( max_y, vertices_i[ i ].mY );

    return max_y + mPosI.mX;
}

int Block :: LowerBoundYi()
{
    int min_y = vertices_i[ 0 ].mY;

    for ( unsigned int i = 1; i < BLOCKS_VERTEX_CNT; i++ )
        min_y = Geometry :: Min( min_y, vertices_i[ i ].mY );

    return min_y + mPosI.mY;
}

int Block :: UpperBoundYi()
{
    int max_y = vertices_i[ 0 ].mY;

    for ( unsigned int i = 1; i < BLOCKS_VERTEX_CNT; i++ )
        max_y = Geometry :: Max( max_y, vertices_i[ i ].mY );

    return max_y + mPosI.mY;
}

int Block :: LowerBoundZi()
{
    int min_z = vertices_i[ 0 ].mZ;

    for ( unsigned int i = 1; i < BLOCKS_VERTEX_CNT; i++ )
        min_z = Geometry :: Min( min_z, vertices_i[ i ].mZ );

    return min_z + mPosI.mZ;
}

int Block :: UpperBoundZi()
{
    int max_z = vertices_i[ 0 ].mZ;

    for ( unsigned int i = 1; i < BLOCKS_VERTEX_CNT; i++ )
        max_z = Geometry :: Max( max_z, vertices_i[ i ].mZ );

    return max_z + mPosI.mZ;
}

 bool Block :: PointIn( Point3Df point )
 {

   return   ( Geometry :: MixedMul( point           - mVerticesF[ 0 ],
                                    mVerticesF[ 1 ] - mVerticesF[ 0 ],
                                    mVerticesF[ 3 ] - mVerticesF[ 0 ] ) < -Geometry :: eps ) &&
            ( Geometry :: MixedMul( point           - mVerticesF[ 1 ],
                                    mVerticesF[ 6 ] - mVerticesF[ 1 ],
                                    mVerticesF[ 2 ] - mVerticesF[ 1 ] ) < -Geometry :: eps ) &&
            ( Geometry :: MixedMul( point           - mVerticesF[ 2 ],
                                    mVerticesF[ 5 ] - mVerticesF[ 2 ],
                                    mVerticesF[ 3 ] - mVerticesF[ 2 ] ) < -Geometry :: eps ) &&
            ( Geometry :: MixedMul( point           - mVerticesF[ 3 ],
                                    mVerticesF[ 4 ] - mVerticesF[ 3 ],
                                    mVerticesF[ 0 ] - mVerticesF[ 3 ] ) < -Geometry :: eps ) &&
            ( Geometry :: MixedMul( point           - mVerticesF[ 0 ],
                                    mVerticesF[ 7 ] - mVerticesF[ 0 ],
                                    mVerticesF[ 1 ] - mVerticesF[ 0 ] ) < -Geometry :: eps ) &&
            ( Geometry :: MixedMul( point           - mVerticesF[ 6 ],
                                    mVerticesF[ 7 ] - mVerticesF[ 6 ],
                                    mVerticesF[ 5 ] - mVerticesF[ 6 ] ) < -Geometry :: eps );
}

bool Block :: PointIn( Point3Di point )
{
    return PointIn( Point3Df( point ) );
}

bool Block :: CheckEdgesAveragePoint( Block* block )
{
    return block -> PointIn( 0.5f * ( mVerticesF[ 1 ] - mVerticesF[ 0 ] ) + mVerticesF[ 0 ] ) ||
           block -> PointIn( 0.5f * ( mVerticesF[ 2 ] - mVerticesF[ 1 ] ) + mVerticesF[ 1 ] ) ||
           block -> PointIn( 0.5f * ( mVerticesF[ 3 ] - mVerticesF[ 2 ] ) + mVerticesF[ 2 ] ) ||
           block -> PointIn( 0.5f * ( mVerticesF[ 0 ] - mVerticesF[ 3 ] ) + mVerticesF[ 3 ] ) ||
           block -> PointIn( 0.5f * ( mVerticesF[ 6 ] - mVerticesF[ 7 ] ) + mVerticesF[ 7 ] ) ||
           block -> PointIn( 0.5f * ( mVerticesF[ 5 ] - mVerticesF[ 6 ] ) + mVerticesF[ 6 ] ) ||
           block -> PointIn( 0.5f * ( mVerticesF[ 4 ] - mVerticesF[ 5 ] ) + mVerticesF[ 5 ] ) ||
           block -> PointIn( 0.5f * ( mVerticesF[ 7 ] - mVerticesF[ 4 ] ) + mVerticesF[ 4 ] ) ||
           block -> PointIn( 0.5f * ( mVerticesF[ 7 ] - mVerticesF[ 0 ] ) + mVerticesF[ 0 ] ) ||
           block -> PointIn( 0.5f * ( mVerticesF[ 6 ] - mVerticesF[ 1 ] ) + mVerticesF[ 1 ] ) ||
           block -> PointIn( 0.5f * ( mVerticesF[ 5 ] - mVerticesF[ 2 ] ) + mVerticesF[ 2 ] ) ||
           block -> PointIn( 0.5f * ( mVerticesF[ 4 ] - mVerticesF[ 3 ] ) + mVerticesF[ 3 ] );
}

bool Block :: IsIntersect( Block* block )
{
    for ( unsigned int i = 0; i < BLOCKS_VERTEX_CNT; i++ )
        if ( block -> PointIn( mVerticesF[ i ] ) || PointIn( block -> mVerticesF[ i ] ) )
            return true;

    return this -> CheckEdgesAveragePoint( block ) || block -> CheckEdgesAveragePoint( this );
}

//Figure

Figure :: Figure( int x, int y, int z, Figures type, Material material ) : GeomEntity( x, y, z ), PhisEntity( material )
{
    if ( Block :: BLOCK_SIZE % 2 != 0 )
    {
	qDebug() << "Odd block size!\n";
	QApplication :: exit( 1 );
    }

    switch ( type )
    {
    case IFigure :
        mpBlocks[ 0 ] = new Block(  -( int )Block :: BLOCK_SIZE - ( int )Block :: BLOCK_SIZE / 2,
                                    0,
                                    0,
                                    material );

        mpBlocks[ 1 ] = new Block( -( int )Block :: BLOCK_SIZE / 2,
                                    0,
                                    0,
                                    material );

        mpBlocks[ 2 ] = new Block( ( int )Block :: BLOCK_SIZE / 2,
                                    0,
                                    0,
                                    material );

        mpBlocks[ 3 ] = new Block(  ( int )( Block :: BLOCK_SIZE + Block :: BLOCK_SIZE / 2 ),
                                    0,
                                    0,
                                    material );
	break;
    case JFigure :
        mpBlocks[ 0 ] = new Block(  ( int )Block :: BLOCK_SIZE / 2,
                                    ( int )Block :: BLOCK_SIZE,
                                    0,
                                    material );

        mpBlocks[ 1 ] = new Block(  ( int )Block :: BLOCK_SIZE / 2,
                                    0,
                                    0,
                                    material );

        mpBlocks[ 2 ] = new Block(  ( int )Block :: BLOCK_SIZE / 2,
                                   -( int )( Block :: BLOCK_SIZE ),
                                    0,
                                    material );

        mpBlocks[ 3 ] = new Block( -( int )Block :: BLOCK_SIZE / 2,
                                   -( int )Block :: BLOCK_SIZE,
                                    0,
                                    material );
	break;
    case LFigure :
        mpBlocks[ 0 ] = new Block( -( int )Block :: BLOCK_SIZE / 2,
                                    ( int )Block :: BLOCK_SIZE,
                                    0,
                                    material );

        mpBlocks[ 1 ] = new Block( -( int )Block :: BLOCK_SIZE / 2,
                                    0,
                                    0,
                                    material );

        mpBlocks[ 2 ] = new Block( -( int )Block :: BLOCK_SIZE / 2,
                                   -( int )Block :: BLOCK_SIZE,
                                    0,
                                    material );

        mpBlocks[ 3 ] = new Block(  ( int )Block :: BLOCK_SIZE / 2,
                                   -( int )Block :: BLOCK_SIZE,
                                    0,
                                    material );
	break;
    case OFigure :
        mpBlocks[ 0 ] = new Block( -( int )Block :: BLOCK_SIZE / 2,
                                    ( int )Block :: BLOCK_SIZE / 2,
                                    0,
                                    material );

        mpBlocks[ 1 ] = new Block(  ( int )Block :: BLOCK_SIZE / 2,
                                    ( int )Block :: BLOCK_SIZE / 2,
                                    0,
                                    material );

        mpBlocks[ 2 ] = new Block(  ( int )Block :: BLOCK_SIZE / 2,
                                   -( int )Block :: BLOCK_SIZE / 2,
                                    0,
                                    material );

        mpBlocks[ 3 ] = new Block( -( int )Block :: BLOCK_SIZE / 2,
                                   -( int )Block :: BLOCK_SIZE / 2,
                                    0,
                                    material );
	break;
    case SFigure :
        mpBlocks[ 0 ] = new Block(  ( int )Block :: BLOCK_SIZE,
                                    ( int )Block :: BLOCK_SIZE / 2,
                                    0,
                                    material );

        mpBlocks[ 1 ] = new Block(  0,
                                    ( int )Block :: BLOCK_SIZE / 2,
                                    0,
                                    material );

        mpBlocks[ 2 ] = new Block(  0,
                                   -( int )Block :: BLOCK_SIZE / 2,
                                    0,
                                    material );

        mpBlocks[ 3 ] = new Block( -( int )Block :: BLOCK_SIZE,
                                   -( int )( Block :: BLOCK_SIZE + Block :: BLOCK_SIZE / 2 ),
                                    0,
                                    material );
	break;
    case TFigure :
        mpBlocks[ 0 ] = new Block(  ( int )Block :: BLOCK_SIZE,
                                    ( int )Block :: BLOCK_SIZE / 2,
                                    0,
                                    material );

        mpBlocks[ 1 ] = new Block(  0,
                                    ( int )Block :: BLOCK_SIZE / 2,
                                    0,
                                    material );

        mpBlocks[ 2 ] = new Block( -( int )Block :: BLOCK_SIZE,
                                    ( int )Block :: BLOCK_SIZE / 2,
                                    0,
                                    material );

        mpBlocks[ 3 ] = new Block(  0,
                                   -( int )Block :: BLOCK_SIZE / 2,
                                    0,
                                    material );
	break;
    default : //ZFigure :
        mpBlocks[ 0 ] = new Block( -( int )Block :: BLOCK_SIZE,
                                    ( int )Block :: BLOCK_SIZE / 2,
                                    0,
                                    material );

        mpBlocks[ 1 ] = new Block(  0,
                                    ( int )Block :: BLOCK_SIZE / 2,
                                    0,
                                    material );
        mpBlocks[ 2 ] = new Block(  0,
                                   -( int )Block :: BLOCK_SIZE / 2,
                                    0,
                                    material );

        mpBlocks[ 3 ] = new Block(  ( int )Block :: BLOCK_SIZE,
                                   -( int )Block :: BLOCK_SIZE / 2,
                                    0,
                                    material );
    }
}

Figure :: Figure( const Figure& fig ) : GeomEntity( fig.mPosI ), PhisEntity( mMaterial )
{
    for ( unsigned int i = 0; i < BlocksCount; i++ )
        mpBlocks[ i ] = new Block( fig.mpBlocks[ i ] -> GetPosi(), fig.mpBlocks[ i ] -> GetMaterial() );
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

void Figure :: RotateOnZY( float angle, bool change_const )
{
    for ( unsigned int i = 0; i < BlocksCount; ++i )
	mpBlocks[ i ] -> RotateOnZY( angle, change_const );
}

void Figure :: RotateOnZX( float angle, bool change_const )
{
    for ( unsigned int i = 0; i < BlocksCount; ++i )
        mpBlocks[ i ] -> RotateOnZX( angle, change_const );
}

void Figure :: RotateOnXY( float angle, bool change_const )
{
    for ( unsigned int i = 0; i < BlocksCount; ++i )
	mpBlocks[ i ] -> RotateOnXY( angle, change_const );
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

Point3Di Figure :: GetBlockPosByIndexi( int index ) const
{
    return mpBlocks[ index ] -> GetPosi();
}

Point3Df Figure :: GetBlockPosByIndexf( int index ) const
{
    return mpBlocks[ index ] -> GetPosf();
}

Material Figure :: GetBlockMaterialByIndex( int index ) const
{
    return mpBlocks[ index ] -> GetMaterial();
}

bool Figure :: IsIntersectWithBlock( Block* block )
{
    float distance = 0.0f;
    for ( unsigned int i = 0; i < BlocksCount; i++ )
    {
        distance = ( block -> GetPosf() - mpBlocks[ i ] -> GetPosf() ).Length();
        if ( distance - Geometry :: eps > Block :: SAFETY_DISTANCE )
            continue;
        if ( distance < Block :: NOT_SAFETY_DISTANCE - Geometry :: eps )
            return true;
        if ( mpBlocks[ i ] -> IsIntersect( block ) )
            return true;
    }
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

bool Figure :: CheckToCollisonWithBlocks( std :: vector < Block* >& collision_blocks )
{
    bool collision          = false;
    int  count_of_blocks    = collision_blocks.size();

    SetVerAbsCoor();

    for ( int i = 0; i < count_of_blocks; ++i )
    {
        collision_blocks[ i ] -> SetVerAbsCoor();
        if ( IsIntersectWithBlock( collision_blocks[ i ] ) )
        {
            collision = true;
            collision_blocks[ i ] -> SetVerRelCoor();
            break;
        }
        collision_blocks[ i ] -> SetVerRelCoor();
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

