#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <QApplication>
#include <QGLWidget>
#include <QDebug>
#include <stdio.h>
#include "Figures.h"

Point3Di Block :: vertices_i[ BlocksVertexCount ] = {
	Point3Di( -Block :: BlockSize / 2,  Block :: BlockSize / 2, -Block :: BlockSize / 2 ),
	Point3Di( -Block :: BlockSize / 2,  Block :: BlockSize / 2,  Block :: BlockSize / 2 ),
	Point3Di(  Block :: BlockSize / 2,  Block :: BlockSize / 2,  Block :: BlockSize / 2 ),
	Point3Di(  Block :: BlockSize / 2,  Block :: BlockSize / 2, -Block :: BlockSize / 2 ),
	Point3Di(  Block :: BlockSize / 2, -Block :: BlockSize / 2, -Block :: BlockSize / 2 ),
	Point3Di(  Block :: BlockSize / 2, -Block :: BlockSize / 2,  Block :: BlockSize / 2 ),
	Point3Di( -Block :: BlockSize / 2, -Block :: BlockSize / 2,  Block :: BlockSize / 2 ),
	Point3Di( -Block :: BlockSize / 2, -Block :: BlockSize / 2, -Block :: BlockSize / 2 )
};

Block :: Block( int new_x, int new_y, int new_z, Material new_material )
{
	rel_position_f = rel_position_i = Point3Di( new_x, new_y, new_z );
	material = new_material;

	for ( int i = 0; i < BlocksVertexCount; i++ )
	vertices_f[ i ] = vertices_i[ i ];
}

Block :: Block( Point3Di new_position, Material new_material )
{
	rel_position_f = rel_position_i = new_position;
	material = new_material;

	for ( int i = 0; i < BlocksVertexCount; i++ )
	vertices_f[ i ] = vertices_i[ i ];
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

    int		prev_z = 0;

    if ( !change_const )
    {
	rel_position_f = rel_position_i;
	Rotate( rel_position_f.z, rel_position_f.y, angle );

	for ( int i = 0; i < BlocksVertexCount; ++i )
	{
	    vertices_f[ i ] = vertices_i[ i ];
	    Rotate( vertices_f[ i ].z, vertices_f[ i ].y, angle  );
	}
    }
    else
    {	
	prev_z = prev_z = rel_position_i.z;

	if ( angle > eps )
	{
	    rel_position_i.z =  rel_position_i.y;
	    rel_position_i.y = -prev_z;
	}
	else
	{
	    rel_position_i.z = -rel_position_i.y;
	    rel_position_i.y =  prev_z;
	}
	rel_position_f = rel_position_i;

	for ( int i = 0; i < BlocksVertexCount; ++i )
	    vertices_f[ i ] = vertices_i[ i ];
    }

}

void Block :: RotateOnZX( float angle, bool change_const )
{
    int	 prev_z = 0;

    if ( !change_const )
    {
	rel_position_f = rel_position_i;
	Rotate( rel_position_f.z, rel_position_f.x, angle );

	for ( int i = 0; i < BlocksVertexCount; ++i )
	{
	    vertices_f[ i ] = vertices_i[ i ];
	    Rotate( vertices_f[ i ].z, vertices_f[ i ].x, angle  );
	}
    }
    else
    {
	prev_z = rel_position_i.z;

	if ( angle > eps )
	{
	    rel_position_i.z =  rel_position_i.x;
	    rel_position_i.x =  -prev_z;
	}
	else
	{
	    rel_position_i.z = -rel_position_i.x;
	    rel_position_i.x =  prev_z;
	}
	rel_position_f   = rel_position_i;

	for ( int i = 0; i < BlocksVertexCount; ++i )
	    vertices_f[ i ] = vertices_i[ i ];
    }
}

void Block :: RotateOnXY( float angle, bool change_const )
{
    int prev_x = 0;

    if ( !change_const )
    {
	rel_position_f = rel_position_i;
	Rotate( rel_position_f.x, rel_position_f.y, angle );

	for ( int i = 0; i < BlocksVertexCount; ++i )
	{
	    vertices_f[ i ] = vertices_i[ i ];
	    Rotate( vertices_f[ i ].x, vertices_f[ i ].y, angle  );
	}
    }
    else
    {	
	prev_x = rel_position_i.x;

	if ( angle > eps )
	{
	    rel_position_i.x =  rel_position_i.y;
	    rel_position_i.y = -prev_x;
	}
	else
	{
	    rel_position_i.x = -rel_position_i.y;
	    rel_position_i.y =  prev_x;
	}
	rel_position_f   = rel_position_i;

	for ( int i = 0; i < BlocksVertexCount; ++i )
	    vertices_f[ i ] = vertices_i[ i ];
    }
}

void Block :: DrawSide( Point3Df p1, Point3Df p2, Point3Df p3, Point3Df p4 )
{
    glEnable(GL_NORMALIZE);
    Point3Df NormalVector = GetNormalVector( p1, p3, p2 );

    glBegin( GL_QUADS );
	glNormal3f( NormalVector.x, NormalVector.y, NormalVector.z );
	glVertex3f( p1.x, p1.y, p1.z );	
	glVertex3f( p2.x, p2.y, p2.z );	
	glVertex3f( p3.x, p3.y, p3.z );	
	glVertex3f( p4.x, p4.y, p4.z );
    glEnd();
}

void Block :: Draw( Point3Df figure_location )
{
    /*
	Вершины куба занумерованы в след. порядке( делим куб на 4 части ):
	1)-,+,- 2)-,+,+ 3)+,+,+ 4)+,+,-
	5)+,-,- 6)+,-,+ 7)-,-,+ 8)-,-,-
    */

    glMaterialfv( GL_FRONT, GL_AMBIENT, material.GetMaterialForAmbient() );
    glMaterialfv( GL_FRONT, GL_DIFFUSE, material.GetMaterialForDiffuse() );
    glMaterialfv( GL_FRONT, GL_SPECULAR, material.GetMaterialForSpecular() );

    //Firts side
    DrawSide(	Point3Df( figure_location.x + rel_position_f.x + vertices_f[ 0 ].x, figure_location.y + rel_position_f.y + vertices_f[ 0 ].y, figure_location.z + rel_position_f.z + vertices_f[ 0 ].z ),
		Point3Df( figure_location.x + rel_position_f.x + vertices_f[ 1 ].x, figure_location.y + rel_position_f.y + vertices_f[ 1 ].y, figure_location.z + rel_position_f.z + vertices_f[ 1 ].z ),
		Point3Df( figure_location.x + rel_position_f.x + vertices_f[ 2 ].x, figure_location.y + rel_position_f.y + vertices_f[ 2 ].y, figure_location.z + rel_position_f.z + vertices_f[ 2 ].z ),
		Point3Df( figure_location.x + rel_position_f.x + vertices_f[ 3 ].x, figure_location.y + rel_position_f.y + vertices_f[ 3 ].y, figure_location.z + rel_position_f.z + vertices_f[ 3 ].z )
	    );

    //Second side
    DrawSide(	Point3Df( figure_location.x + rel_position_f.x + vertices_f[ 1 ].x, figure_location.y + rel_position_f.y + vertices_f[ 1 ].y, figure_location.z + rel_position_f.z + vertices_f[ 1 ].z ),
		Point3Df( figure_location.x + rel_position_f.x + vertices_f[ 6 ].x, figure_location.y + rel_position_f.y + vertices_f[ 6 ].y, figure_location.z + rel_position_f.z + vertices_f[ 6 ].z ),
		Point3Df( figure_location.x + rel_position_f.x + vertices_f[ 5 ].x, figure_location.y + rel_position_f.y + vertices_f[ 5 ].y, figure_location.z + rel_position_f.z + vertices_f[ 5 ].z ),
		Point3Df( figure_location.x + rel_position_f.x + vertices_f[ 2 ].x, figure_location.y + rel_position_f.y + vertices_f[ 2 ].y, figure_location.z + rel_position_f.z + vertices_f[ 2 ].z )
	    );

    //Third side
    DrawSide(	Point3Df( figure_location.x + rel_position_f.x + vertices_f[ 2 ].x, figure_location.y + rel_position_f.y + vertices_f[ 2 ].y, figure_location.z + rel_position_f.z + vertices_f[ 2 ].z ),
		Point3Df( figure_location.x + rel_position_f.x + vertices_f[ 5 ].x, figure_location.y + rel_position_f.y + vertices_f[ 5 ].y, figure_location.z + rel_position_f.z + vertices_f[ 5 ].z ),
		Point3Df( figure_location.x + rel_position_f.x + vertices_f[ 4 ].x, figure_location.y + rel_position_f.y + vertices_f[ 4 ].y, figure_location.z + rel_position_f.z + vertices_f[ 4 ].z ),
		Point3Df( figure_location.x + rel_position_f.x + vertices_f[ 3 ].x, figure_location.y + rel_position_f.y + vertices_f[ 3 ].y, figure_location.z + rel_position_f.z + vertices_f[ 3 ].z )
	    );

    //Fourth side
    DrawSide(	Point3Df( figure_location.x + rel_position_f.x + vertices_f[ 6 ].x, figure_location.y + rel_position_f.y + vertices_f[ 6 ].y, figure_location.z + rel_position_f.z + vertices_f[ 6 ].z ),
		Point3Df( figure_location.x + rel_position_f.x + vertices_f[ 7 ].x, figure_location.y + rel_position_f.y + vertices_f[ 7 ].y, figure_location.z + rel_position_f.z + vertices_f[ 7 ].z ),
		Point3Df( figure_location.x + rel_position_f.x + vertices_f[ 4 ].x, figure_location.y + rel_position_f.y + vertices_f[ 4 ].y, figure_location.z + rel_position_f.z + vertices_f[ 4 ].z ),
		Point3Df( figure_location.x + rel_position_f.x + vertices_f[ 5 ].x, figure_location.y + rel_position_f.y + vertices_f[ 5 ].y, figure_location.z + rel_position_f.z + vertices_f[ 5 ].z )
	    );

    //Fifth side
    DrawSide(	Point3Df( figure_location.x + rel_position_f.x + vertices_f[ 0 ].x, figure_location.y + rel_position_f.y + vertices_f[ 0 ].y, figure_location.z + rel_position_f.z + vertices_f[ 0 ].z ),
		Point3Df( figure_location.x + rel_position_f.x + vertices_f[ 7 ].x, figure_location.y + rel_position_f.y + vertices_f[ 7 ].y, figure_location.z + rel_position_f.z + vertices_f[ 7 ].z ),
		Point3Df( figure_location.x + rel_position_f.x + vertices_f[ 6 ].x, figure_location.y + rel_position_f.y + vertices_f[ 6 ].y, figure_location.z + rel_position_f.z + vertices_f[ 6 ].z ),
		Point3Df( figure_location.x + rel_position_f.x + vertices_f[ 1 ].x, figure_location.y + rel_position_f.y + vertices_f[ 1 ].y, figure_location.z + rel_position_f.z + vertices_f[ 1 ].z )
	    );

    //Sixth side
    DrawSide(	Point3Df( figure_location.x + rel_position_f.x + vertices_f[ 3 ].x, figure_location.y + rel_position_f.y + vertices_f[ 3 ].y, figure_location.z + rel_position_f.z + vertices_f[ 3 ].z ),
		Point3Df( figure_location.x + rel_position_f.x + vertices_f[ 4 ].x, figure_location.y + rel_position_f.y + vertices_f[ 4 ].y, figure_location.z + rel_position_f.z + vertices_f[ 4 ].z ),
		Point3Df( figure_location.x + rel_position_f.x + vertices_f[ 7 ].x, figure_location.y + rel_position_f.y + vertices_f[ 7 ].y, figure_location.z + rel_position_f.z + vertices_f[ 7 ].z ),
		Point3Df( figure_location.x + rel_position_f.x + vertices_f[ 0 ].x, figure_location.y + rel_position_f.y + vertices_f[ 0 ].y, figure_location.z + rel_position_f.z + vertices_f[ 0 ].z )
	    );
}

float Block :: LowerBoundXf()
{
    float min_x = vertices_f[ 0 ].x;

    for ( int i = 1; i < BlocksVertexCount; i++ )
	min_x = Min( min_x, vertices_f[ i ].x );

    return min_x + rel_position_f.x;
}

float Block :: UpperBoundXf()
{
    float max_x = vertices_f[ 0 ].x;

    for ( int i = 1; i < BlocksVertexCount; i++ )
	max_x = Max( max_x, vertices_f[ i ].x );

    return max_x + rel_position_f.x;
}

float Block :: LowerBoundYf()
{
    float min_y = vertices_f[ 0 ].y;

    for ( int i = 1; i < BlocksVertexCount; i++ )
	min_y = Min( min_y, vertices_f[ i ].y );

    return min_y + rel_position_f.y;
}

float Block :: UpperBoundYf()
{
    float max_y = vertices_f[ 0 ].y;

    for ( int i = 1; i < BlocksVertexCount; i++ )
	max_y = Max( max_y, vertices_f[ i ].y );

    return max_y + rel_position_f.y;
}

float Block :: LowerBoundZf()
{
    float min_z = vertices_f[ 0 ].z;

    for ( int i = 1; i < BlocksVertexCount; i++ )
	min_z = Min( min_z, vertices_f[ i ].z );

    return min_z + rel_position_f.z;
}

float Block :: UpperBoundZf()
{
    float max_z = vertices_f[ 0 ].z;

    for ( int i = 1; i < BlocksVertexCount; i++ )
	max_z = Max( max_z, vertices_f[ i ].z );

    return max_z + rel_position_f.z;
}

int Block :: LowerBoundXi()
{
    int min_x = vertices_i[ 0 ].x;

    for ( int i = 1; i < BlocksVertexCount; i++ )
	min_x = Min( min_x, vertices_i[ i ].x );

    return min_x + rel_position_i.x;
}

int Block :: UpperBoundXi()
{
    int max_y = vertices_i[ 0 ].y;

    for ( int i = 1; i < BlocksVertexCount; i++ )
	max_y = Max( max_y, vertices_i[ i ].y );

    return max_y + rel_position_i.x;
}

int Block :: LowerBoundYi()
{
    int min_y = vertices_i[ 0 ].y;

    for ( int i = 1; i < BlocksVertexCount; i++ )
	min_y = Min( min_y, vertices_i[ i ].y );

    return min_y + rel_position_i.y;
}

int Block :: UpperBoundYi()
{
    int max_y = vertices_i[ 0 ].y;

    for ( int i = 1; i < BlocksVertexCount; i++ )
	max_y = Max( max_y, vertices_i[ i ].y );

    return max_y + rel_position_i.y;
}

int Block :: LowerBoundZi()
{
    int min_z = vertices_i[ 0 ].z;

    for ( int i = 1; i < BlocksVertexCount; i++ )
	min_z = Min( min_z, vertices_i[ i ].z );

    return min_z + rel_position_i.z;
}

int Block :: UpperBoundZi()
{
    int max_z = vertices_i[ 0 ].z;

    for ( int i = 1; i < BlocksVertexCount; i++ )
	max_z = Max( max_z, vertices_i[ i ].z );

    return max_z + rel_position_i.z;
}

Figure :: Figure( int x, int y, int z, Figures type, Material material )
{
    if ( Block :: BlockSize % 2 != 0 )
    {
	qDebug() << "Odd block size!\n";
	QApplication :: exit( 1 );
    }

    position_i = Point3Di( x, y, z );
    position_f = position_i;
    //material = new_material;

    switch ( type )
    {
    case IFigure :
	blocks[ 0 ] = new Block( -( Block :: BlockSize + Block :: BlockSize / 2 ), 0.0f, 0.0f, material );
	blocks[ 1 ] = new Block( -( Block :: BlockSize / 2 ),			   0.0f, 0.0f, material  );
	blocks[ 2 ] = new Block(    Block :: BlockSize / 2,			   0.0f, 0.0f, material );
	blocks[ 3 ] = new Block(    Block :: BlockSize + Block :: BlockSize / 2,   0.0f, 0.0f, material );
	break;
    case JFigure :
	blocks[ 0 ] = new Block( 0.0f, Block :: BlockSize + Block :: BlockSize / 2, 0.0f, material );
	blocks[ 1 ] = new Block( 0.0f, Block :: BlockSize / 2, 0.0f, material );
	blocks[ 2 ] = new Block( 0.0f, -Block :: BlockSize / 2, 0.0f, material );
	blocks[ 3 ] = new Block( Block :: BlockSize, -( Block :: BlockSize + Block :: BlockSize / 2 ), 0.0f, material );
	break;
    case LFigure :
	blocks[ 0 ] = new Block(  Block :: BlockSize / 2,  Block :: BlockSize, 0.0f, material );
	blocks[ 1 ] = new Block(  Block :: BlockSize / 2,  0.0f,	       0.0f, material );
	blocks[ 2 ] = new Block(  Block :: BlockSize / 2, -Block :: BlockSize, 0.0f, material );
	blocks[ 3 ] = new Block( -Block :: BlockSize / 2, -Block :: BlockSize, 0.0f, material );
	break;
    case OFigure :
	blocks[ 0 ] = new Block( -Block :: BlockSize / 2, Block :: BlockSize / 2, 0.0f, material );
	blocks[ 1 ] = new Block( Block :: BlockSize / 2, Block :: BlockSize / 2, 0.0f, material );
	blocks[ 2 ] = new Block( Block :: BlockSize / 2, -Block :: BlockSize / 2, 0.0f, material );
	blocks[ 3 ] = new Block( -Block :: BlockSize / 2, -Block :: BlockSize / 2, 0.0f, material );
	break;
    case SFigure :
	blocks[ 0 ] = new Block( Block :: BlockSize + Block :: BlockSize / 2, Block :: BlockSize / 2, 0.0f, material );
	blocks[ 1 ] = new Block( 0.0f, Block :: BlockSize / 2, 0.0f, material );
	blocks[ 2 ] = new Block( 0.0f, -Block :: BlockSize / 2, 0.0f, material );
	blocks[ 3 ] = new Block( -( Block :: BlockSize + Block :: BlockSize / 2 ), -Block :: BlockSize + Block :: BlockSize / 2, 0.0f, material );
	break;
    case TFigure :
	blocks[ 0 ] = new Block( Block :: BlockSize, Block :: BlockSize / 2, 0.0f, material );
	blocks[ 1 ] = new Block( 0.0f, Block :: BlockSize / 2, 0.0f, material );
	blocks[ 2 ] = new Block( -Block :: BlockSize, Block :: BlockSize / 2, 0.0f, material );
	blocks[ 3 ] = new Block( 0.0f, -Block :: BlockSize / 2, 0.0f, material );
	break;
    default : //ZFigure :
	blocks[ 0 ] = new Block( -Block :: BlockSize, Block :: BlockSize / 2, 0.0f, material );
	blocks[ 1 ] = new Block( 0.0f, Block :: BlockSize / 2, 0.0f, material );
	blocks[ 2 ] = new Block( 0.0f, -Block :: BlockSize / 2, 0.0f, material );
	blocks[ 3 ] = new Block( Block :: BlockSize, -Block :: BlockSize / 2, 0.0f, material );
    }
}

Figure :: ~Figure()
{
    for ( int i = 0; i < BlocksCount; ++i )
	    delete blocks[ i ];
}

void Figure :: Draw()
{
    for ( int i = 0; i < BlocksCount; ++i )
	blocks[ i ] -> Draw( position_f );
}

void Figure :: RotateOnZY( float angle, bool change_const )
{
    for ( int i = 0; i < BlocksCount; ++i )
	blocks[ i ] -> RotateOnZY( angle, change_const );
}

void Figure :: RotateOnZX( float angle, bool change_const )
{
    for ( int i = 0; i < BlocksCount; ++i )
	    blocks[ i ] -> RotateOnZX( angle, change_const );
}

void Figure :: RotateOnXY( float angle, bool change_const )
{
    for ( int i = 0; i < BlocksCount; ++i )
	blocks[ i ] -> RotateOnXY( angle, change_const );
}

Point3Df Figure :: GetPositionF() const
{
    return position_f;
}

Point3Di Figure :: GetPositionI() const
{
    return position_i;
}

void Figure :: SetPositionI( Point3Di new_position )
{
    position_f = position_i = new_position;
}

float Figure :: LowerBoundXf()
{
    float min_x = blocks[ 0 ] -> LowerBoundXf();

    for ( int i = 1; i < BlocksCount; i++ )
	min_x = Min( min_x, blocks[ i ] -> LowerBoundXf() );

    return min_x + position_f.x;
}

float Figure :: UpperBoundXf()
{
    float max_x = blocks[ 0 ] -> UpperBoundXf();

    for ( int i = 1; i < BlocksCount; i++ )
	max_x = Max( max_x, blocks[ i ] -> UpperBoundXf() );

    return max_x + position_f.x;
}

float Figure :: LowerBoundYf()
{
    float min_y = blocks[ 0 ] -> LowerBoundYf();

    for ( int i = 1; i < BlocksCount; i++ )
	min_y = Min( min_y, blocks[ i ] -> LowerBoundYf() );

    return min_y + position_f.y;
}

float Figure :: UpperBoundYf()
{
    float max_y = blocks[ 0 ] -> UpperBoundYf();

    for ( int i = 1; i < BlocksCount; i++ )
	max_y = Max( max_y, blocks[ i ] -> UpperBoundYf() );

    return max_y + position_f.y;
}

float Figure :: LowerBoundZf()
{
    float min_z = blocks[ 0 ] -> LowerBoundZf();

    for ( int i = 1; i < BlocksCount; i++ )
	min_z = Min( min_z, blocks[ i ] -> LowerBoundZf() );

    return min_z + position_f.z;
}

float Figure :: UpperBoundZf()
{
    float max_z = blocks[ 0 ] -> UpperBoundZf();

    for ( int i = 1; i < BlocksCount; i++ )
	max_z = Max( max_z, blocks[ i ] -> UpperBoundZf() );

    return max_z + position_f.z;
}

int Figure :: LowerBoundXi()
{
    float min_x = blocks[ 0 ] -> LowerBoundXi();

    for ( int i = 1; i < BlocksCount; i++ )
	min_x = Min( min_x, blocks[ i ] -> LowerBoundXi() );

    return min_x + position_i.x;
}

int Figure :: UpperBoundXi()
{
    float max_x = blocks[ 0 ] -> UpperBoundXi();

    for ( int i = 1; i < BlocksCount; i++ )
	max_x = Max( max_x, blocks[ i ] -> UpperBoundXi() );

    return max_x + position_i.x;
}

int Figure :: LowerBoundYi()
{
    float min_y = blocks[ 0 ] -> LowerBoundYi();

    for ( int i = 1; i < BlocksCount; i++ )
	min_y = Min( min_y, blocks[ i ] -> LowerBoundYi() );

    return min_y + position_i.y;
}

int Figure :: UpperBoundYi()
{
    float max_y = blocks[ 0 ] -> UpperBoundYf();

    for ( int i = 1; i < BlocksCount; i++ )
	max_y = Max( max_y, blocks[ i ] -> UpperBoundYi() );

    return max_y + position_i.y;
}

int Figure :: LowerBoundZi()
{
    float min_z = blocks[ 0 ] -> LowerBoundZi();

    for ( int i = 1; i < BlocksCount; i++ )
	min_z = Min( min_z, blocks[ i ] -> LowerBoundZi() );

    return min_z + position_i.z;
}

int Figure :: UpperBoundZi()
{
    int max_z = blocks[ 0 ] -> UpperBoundZi();

    for ( int i = 1; i < BlocksCount; i++ )
	max_z = Max( max_z, blocks[ i ] -> UpperBoundZi() );

    return max_z + position_i.z;
}

Point3Di Figure :: GetBlockPositionByIndex( int index ) const
{
    return blocks[ index ] -> GetPositionI();
}
Material Figure :: GetBlockMaterialByIndex( int index ) const
{
	return blocks[ index ] -> GetMaterial();
}

Material Block :: GetMaterial() const
{
	return material;
}

Point3Df Block :: GetPositionF() const
{
    return rel_position_f;
}

Point3Di Block :: GetPositionI() const
{
    return rel_position_i;
}

void Figure :: SetPositionF( Point3Df new_position )
{
    position_f = new_position;
}
