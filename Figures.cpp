#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <QApplication>
#include <QGLWidget>
#include <QDebug>
#include <stdio.h>
#include "Figures.h"

//GeomEntity

GeomEntity :: GeomEntity( int new_x, int new_y, int new_z, Material new_material )
{
    material = new_material;
    pos_i    = Point3Di( new_x, new_y, new_z );
    pos_f    = pos_i;

}

GeomEntity :: GeomEntity( float new_x, float new_y, float new_z, Material new_material )
{
    material = new_material;
    pos_i = Point3Di( ( int )new_x, ( int )new_y, ( int )new_z );
    pos_i = pos_f;
}

GeomEntity :: GeomEntity( Point3Di new_pos, Material new_material )
{
    material = new_material;
    pos_i = new_pos;
    pos_f = new_pos;
}

GeomEntity :: GeomEntity( Point3Df new_pos, Material new_material )
{
    material = new_material;
    pos_i = new_pos;
    pos_f = new_pos;
}


Point3Df GeomEntity :: GetPosf() const
{
    return pos_f;
}

Point3Di GeomEntity :: GetPosi() const
{
    return pos_i;
}

float GeomEntity :: GetPosByXf() const
{
    return pos_f.x;
}

float GeomEntity :: GetPosByYf() const
{
    return pos_f.y;
}

float GeomEntity :: GetPosByZf() const
{
    return pos_f.z;
}

int GeomEntity :: GetPosByXi() const
{
    return pos_i.x;
}

int GeomEntity :: GetPosByYi() const
{
    return pos_i.y;
}

int GeomEntity :: GetPosByZi() const
{
    return pos_i.z;
}

void GeomEntity :: SetPosByXi( int new_x )
{
    pos_i.x = new_x;
}

void GeomEntity :: SetPosByYi( int new_y )
{
    pos_i.y = new_y;
}

void GeomEntity :: SetPosByZi( int new_z )
{
    pos_i.z = new_z;
}

 void GeomEntity :: SetPosByXf( float new_x )
 {
    pos_f.x = new_x;
 }

void GeomEntity :: SetPosByYf( float new_y )
{
    pos_f.y = new_y;
}

void GeomEntity :: SetPosByZf( float new_z )
{
    pos_f.z = new_z;
}

void GeomEntity :: SetPosi( int new_x, int new_y, int new_z )
{
    pos_i = Point3Di( new_x, new_y, new_z );
    pos_f = pos_i;
}

void GeomEntity :: SetPosf( float new_x, float new_y, float new_z )
{
    pos_f.x = new_x;
    pos_f.y = new_y;
    pos_f.z = new_z;
}

void GeomEntity :: SetPosi( Point3Di new_pos )
{
    pos_i = new_pos;
    pos_f = new_pos;
}

void GeomEntity :: SetPosf( Point3Df new_pos )
{
    pos_f = new_pos;
}

Material GeomEntity :: GetMaterial()  const
{
    return material;
}

//Block

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

Block :: Block( int new_x, int new_y, int new_z, Material new_material ) : GeomEntity( new_x, new_y, new_z, new_material )
{
	for ( unsigned int i = 0; i < BlocksVertexCount; i++ )
	    vertices_f[ i ] = vertices_i[ i ];
}

Block :: Block( Point3Di new_pos, Material new_material ) : GeomEntity( new_pos, new_material )
{
	for ( unsigned int i = 0; i < BlocksVertexCount; i++ )
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

    int	    prev_z = 0;

    if ( !change_const )
    {
	pos_f = pos_i;
	Rotate( pos_f.z, pos_f.y, angle );

	for ( unsigned int i = 0; i < BlocksVertexCount; ++i )
	{
	    vertices_f[ i ] = vertices_i[ i ];
	    Rotate( vertices_f[ i ].z, vertices_f[ i ].y, angle  );
	}
    }
    else
    {	
	prev_z = pos_i.z;

	if ( angle > eps )
	{
	    pos_i.z =  pos_i.y;
	    pos_i.y = -prev_z;
	}
	else
	{
	    pos_i.z = -pos_i.y;
	    pos_i.y =  prev_z;
	}
	pos_f = pos_i;

	for ( unsigned int i = 0; i < BlocksVertexCount; ++i )
	    vertices_f[ i ] = vertices_i[ i ];
    }

}

void Block :: RotateOnZX( float angle, bool change_const )
{
    int	 prev_z = 0;

    if ( !change_const )
    {
	pos_f = pos_i;
	Rotate( pos_f.z, pos_f.x, angle );

	for ( unsigned int i = 0; i < BlocksVertexCount; ++i )
	{
	    vertices_f[ i ] = vertices_i[ i ];
	    Rotate( vertices_f[ i ].z, vertices_f[ i ].x, angle  );
	}
    }
    else
    {
	prev_z = pos_i.z;

	if ( angle > eps )
	{
	    pos_i.z =  pos_i.x;
	    pos_i.x =  -prev_z;
	}
	else
	{
	    pos_i.z = -pos_i.x;
	    pos_i.x =  prev_z;
	}
	pos_f   = pos_i;

	for ( unsigned int i = 0; i < BlocksVertexCount; ++i )
	    vertices_f[ i ] = vertices_i[ i ];
    }
}

void Block :: RotateOnXY( float angle, bool change_const )
{
    int prev_x = 0;

    if ( !change_const )
    {
	pos_f = pos_i;
	Rotate( pos_f.x, pos_f.y, angle );

	for ( unsigned int i = 0; i < BlocksVertexCount; ++i )
	{
	    vertices_f[ i ] = vertices_i[ i ];
	    Rotate( vertices_f[ i ].x, vertices_f[ i ].y, angle  );
	}
    }
    else
    {	
	prev_x = pos_i.x;

	if ( angle > eps )
	{
	    pos_i.x =  pos_i.y;
	    pos_i.y = -prev_x;
	}
	else
	{
	    pos_i.x = -pos_i.y;
	    pos_i.y =  prev_x;
	}
	pos_f   = pos_i;

	for ( unsigned int i = 0; i < BlocksVertexCount; ++i )
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

    glMaterialfv( GL_FRONT, GL_AMBIENT, material.GetMaterialForAmbient() );
    glMaterialfv( GL_FRONT, GL_DIFFUSE, material.GetMaterialForDiffuse() );
    glMaterialfv( GL_FRONT, GL_SPECULAR, material.GetMaterialForSpecular() );

    //Firts side
    DrawSide(	Point3Df( fig_pos.x + pos_f.x + vertices_f[ 0 ].x, fig_pos.y + pos_f.y + vertices_f[ 0 ].y, fig_pos.z + pos_f.z + vertices_f[ 0 ].z ),
		Point3Df( fig_pos.x + pos_f.x + vertices_f[ 1 ].x, fig_pos.y + pos_f.y + vertices_f[ 1 ].y, fig_pos.z + pos_f.z + vertices_f[ 1 ].z ),
		Point3Df( fig_pos.x + pos_f.x + vertices_f[ 2 ].x, fig_pos.y + pos_f.y + vertices_f[ 2 ].y, fig_pos.z + pos_f.z + vertices_f[ 2 ].z ),
		Point3Df( fig_pos.x + pos_f.x + vertices_f[ 3 ].x, fig_pos.y + pos_f.y + vertices_f[ 3 ].y, fig_pos.z + pos_f.z + vertices_f[ 3 ].z )
	    );

    //Second side
    DrawSide(	Point3Df( fig_pos.x + pos_f.x + vertices_f[ 1 ].x, fig_pos.y + pos_f.y + vertices_f[ 1 ].y, fig_pos.z + pos_f.z + vertices_f[ 1 ].z ),
		Point3Df( fig_pos.x + pos_f.x + vertices_f[ 6 ].x, fig_pos.y + pos_f.y + vertices_f[ 6 ].y, fig_pos.z + pos_f.z + vertices_f[ 6 ].z ),
		Point3Df( fig_pos.x + pos_f.x + vertices_f[ 5 ].x, fig_pos.y + pos_f.y + vertices_f[ 5 ].y, fig_pos.z + pos_f.z + vertices_f[ 5 ].z ),
		Point3Df( fig_pos.x + pos_f.x + vertices_f[ 2 ].x, fig_pos.y + pos_f.y + vertices_f[ 2 ].y, fig_pos.z + pos_f.z + vertices_f[ 2 ].z )
	    );

    //Third side
    DrawSide(	Point3Df( fig_pos.x + pos_f.x + vertices_f[ 2 ].x, fig_pos.y + pos_f.y + vertices_f[ 2 ].y, fig_pos.z + pos_f.z + vertices_f[ 2 ].z ),
		Point3Df( fig_pos.x + pos_f.x + vertices_f[ 5 ].x, fig_pos.y + pos_f.y + vertices_f[ 5 ].y, fig_pos.z + pos_f.z + vertices_f[ 5 ].z ),
		Point3Df( fig_pos.x + pos_f.x + vertices_f[ 4 ].x, fig_pos.y + pos_f.y + vertices_f[ 4 ].y, fig_pos.z + pos_f.z + vertices_f[ 4 ].z ),
		Point3Df( fig_pos.x + pos_f.x + vertices_f[ 3 ].x, fig_pos.y + pos_f.y + vertices_f[ 3 ].y, fig_pos.z + pos_f.z + vertices_f[ 3 ].z )
	    );

    //Fourth side
    DrawSide(	Point3Df( fig_pos.x + pos_f.x + vertices_f[ 6 ].x, fig_pos.y + pos_f.y + vertices_f[ 6 ].y, fig_pos.z + pos_f.z + vertices_f[ 6 ].z ),
		Point3Df( fig_pos.x + pos_f.x + vertices_f[ 7 ].x, fig_pos.y + pos_f.y + vertices_f[ 7 ].y, fig_pos.z + pos_f.z + vertices_f[ 7 ].z ),
		Point3Df( fig_pos.x + pos_f.x + vertices_f[ 4 ].x, fig_pos.y + pos_f.y + vertices_f[ 4 ].y, fig_pos.z + pos_f.z + vertices_f[ 4 ].z ),
		Point3Df( fig_pos.x + pos_f.x + vertices_f[ 5 ].x, fig_pos.y + pos_f.y + vertices_f[ 5 ].y, fig_pos.z + pos_f.z + vertices_f[ 5 ].z )
	    );

    //Fifth side
    DrawSide(	Point3Df( fig_pos.x + pos_f.x + vertices_f[ 0 ].x, fig_pos.y + pos_f.y + vertices_f[ 0 ].y, fig_pos.z + pos_f.z + vertices_f[ 0 ].z ),
		Point3Df( fig_pos.x + pos_f.x + vertices_f[ 7 ].x, fig_pos.y + pos_f.y + vertices_f[ 7 ].y, fig_pos.z + pos_f.z + vertices_f[ 7 ].z ),
		Point3Df( fig_pos.x + pos_f.x + vertices_f[ 6 ].x, fig_pos.y + pos_f.y + vertices_f[ 6 ].y, fig_pos.z + pos_f.z + vertices_f[ 6 ].z ),
		Point3Df( fig_pos.x + pos_f.x + vertices_f[ 1 ].x, fig_pos.y + pos_f.y + vertices_f[ 1 ].y, fig_pos.z + pos_f.z + vertices_f[ 1 ].z )
	    );

    //Sixth side
    DrawSide(	Point3Df( fig_pos.x + pos_f.x + vertices_f[ 3 ].x, fig_pos.y + pos_f.y + vertices_f[ 3 ].y, fig_pos.z + pos_f.z + vertices_f[ 3 ].z ),
		Point3Df( fig_pos.x + pos_f.x + vertices_f[ 4 ].x, fig_pos.y + pos_f.y + vertices_f[ 4 ].y, fig_pos.z + pos_f.z + vertices_f[ 4 ].z ),
		Point3Df( fig_pos.x + pos_f.x + vertices_f[ 7 ].x, fig_pos.y + pos_f.y + vertices_f[ 7 ].y, fig_pos.z + pos_f.z + vertices_f[ 7 ].z ),
		Point3Df( fig_pos.x + pos_f.x + vertices_f[ 0 ].x, fig_pos.y + pos_f.y + vertices_f[ 0 ].y, fig_pos.z + pos_f.z + vertices_f[ 0 ].z )
	    );
}

float Block :: LowerBoundXf()
{
    float min_x = vertices_f[ 0 ].x;

    for ( unsigned int i = 1; i < BlocksVertexCount; i++ )
	min_x = Min( min_x, vertices_f[ i ].x );

    return min_x + pos_f.x;
}

float Block :: UpperBoundXf()
{
    float max_x = vertices_f[ 0 ].x;

    for ( unsigned int i = 1; i < BlocksVertexCount; i++ )
	max_x = Max( max_x, vertices_f[ i ].x );

    return max_x + pos_f.x;
}

float Block :: LowerBoundYf()
{
    float min_y = vertices_f[ 0 ].y;

    for ( unsigned int i = 1; i < BlocksVertexCount; i++ )
	min_y = Min( min_y, vertices_f[ i ].y );

    return min_y + pos_f.y;
}

float Block :: UpperBoundYf()
{
    float max_y = vertices_f[ 0 ].y;

    for ( unsigned int i = 1; i < BlocksVertexCount; i++ )
	max_y = Max( max_y, vertices_f[ i ].y );

    return max_y + pos_f.y;
}

float Block :: LowerBoundZf()
{
    float min_z = vertices_f[ 0 ].z;

    for ( unsigned int i = 1; i < BlocksVertexCount; i++ )
	min_z = Min( min_z, vertices_f[ i ].z );

    return min_z + pos_f.z;
}

float Block :: UpperBoundZf()
{
    float max_z = vertices_f[ 0 ].z;

    for ( unsigned int i = 1; i < BlocksVertexCount; i++ )
	max_z = Max( max_z, vertices_f[ i ].z );

    return max_z + pos_f.z;
}

int Block :: LowerBoundXi()
{
    int min_x = vertices_i[ 0 ].x;

    for ( unsigned int i = 1; i < BlocksVertexCount; i++ )
	min_x = Min( min_x, vertices_i[ i ].x );

    return min_x + pos_i.x;
}

int Block :: UpperBoundXi()
{
    int max_y = vertices_i[ 0 ].y;

    for ( unsigned int i = 1; i < BlocksVertexCount; i++ )
	max_y = Max( max_y, vertices_i[ i ].y );

    return max_y + pos_i.x;
}

int Block :: LowerBoundYi()
{
    int min_y = vertices_i[ 0 ].y;

    for ( unsigned int i = 1; i < BlocksVertexCount; i++ )
	min_y = Min( min_y, vertices_i[ i ].y );

    return min_y + pos_i.y;
}

int Block :: UpperBoundYi()
{
    int max_y = vertices_i[ 0 ].y;

    for ( unsigned int i = 1; i < BlocksVertexCount; i++ )
	max_y = Max( max_y, vertices_i[ i ].y );

    return max_y + pos_i.y;
}

int Block :: LowerBoundZi()
{
    int min_z = vertices_i[ 0 ].z;

    for ( unsigned int i = 1; i < BlocksVertexCount; i++ )
	min_z = Min( min_z, vertices_i[ i ].z );

    return min_z + pos_i.z;
}

int Block :: UpperBoundZi()
{
    int max_z = vertices_i[ 0 ].z;

    for ( unsigned int i = 1; i < BlocksVertexCount; i++ )
	max_z = Max( max_z, vertices_i[ i ].z );

    return max_z + pos_i.z;
}


//Figure

Figure :: Figure( int x, int y, int z, Figures type, Material material ) : GeomEntity( x, y, z, material )
{
    if ( Block :: BlockSize % 2 != 0 )
    {
	qDebug() << "Odd block size!\n";
	QApplication :: exit( 1 );
    }

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
    for ( unsigned int i = 0; i < BlocksCount; ++i )
	    delete blocks[ i ];
}

void Figure :: Draw()
{
    for ( unsigned int i = 0; i < BlocksCount; ++i )
	blocks[ i ] -> Draw( pos_f );
}

void Figure :: RotateOnZY( float angle, bool change_const )
{
    for ( unsigned int i = 0; i < BlocksCount; ++i )
	blocks[ i ] -> RotateOnZY( angle, change_const );
}

void Figure :: RotateOnZX( float angle, bool change_const )
{
    for ( unsigned int i = 0; i < BlocksCount; ++i )
	    blocks[ i ] -> RotateOnZX( angle, change_const );
}

void Figure :: RotateOnXY( float angle, bool change_const )
{
    for ( unsigned int i = 0; i < BlocksCount; ++i )
	blocks[ i ] -> RotateOnXY( angle, change_const );
}


float Figure :: LowerBoundXf()
{
    float min_x = blocks[ 0 ] -> LowerBoundXf();

    for ( unsigned int i = 1; i < BlocksCount; i++ )
	min_x = Min( min_x, blocks[ i ] -> LowerBoundXf() );

    return min_x + pos_f.x;
}

float Figure :: UpperBoundXf()
{
    float max_x = blocks[ 0 ] -> UpperBoundXf();

    for ( unsigned int i = 1; i < BlocksCount; i++ )
	max_x = Max( max_x, blocks[ i ] -> UpperBoundXf() );

    return max_x + pos_f.x;
}

float Figure :: LowerBoundYf()
{
    float min_y = blocks[ 0 ] -> LowerBoundYf();

    for ( unsigned int i = 1; i < BlocksCount; i++ )
	min_y = Min( min_y, blocks[ i ] -> LowerBoundYf() );

    return min_y + pos_f.y;
}

float Figure :: UpperBoundYf()
{
    float max_y = blocks[ 0 ] -> UpperBoundYf();

    for ( unsigned int i = 1; i < BlocksCount; i++ )
	max_y = Max( max_y, blocks[ i ] -> UpperBoundYf() );

    return max_y + pos_f.y;
}

float Figure :: LowerBoundZf()
{
    float min_z = blocks[ 0 ] -> LowerBoundZf();

    for ( unsigned int i = 1; i < BlocksCount; i++ )
	min_z = Min( min_z, blocks[ i ] -> LowerBoundZf() );

    return min_z + pos_f.z;
}

float Figure :: UpperBoundZf()
{
    float max_z = blocks[ 0 ] -> UpperBoundZf();

    for ( unsigned int i = 1; i < BlocksCount; i++ )
	max_z = Max( max_z, blocks[ i ] -> UpperBoundZf() );

    return max_z + pos_f.z;
}

int Figure :: LowerBoundXi()
{
    float min_x = blocks[ 0 ] -> LowerBoundXi();

    for ( unsigned int i = 1; i < BlocksCount; i++ )
	min_x = Min( min_x, blocks[ i ] -> LowerBoundXi() );

    return min_x + pos_i.x;
}

int Figure :: UpperBoundXi()
{
    float max_x = blocks[ 0 ] -> UpperBoundXi();

    for ( unsigned int i = 1; i < BlocksCount; i++ )
	max_x = Max( max_x, blocks[ i ] -> UpperBoundXi() );

    return max_x + pos_i.x;
}

int Figure :: LowerBoundYi()
{
    float min_y = blocks[ 0 ] -> LowerBoundYi();

    for ( unsigned int i = 1; i < BlocksCount; i++ )
	min_y = Min( min_y, blocks[ i ] -> LowerBoundYi() );

    return min_y + pos_i.y;
}

int Figure :: UpperBoundYi()
{
    float max_y = blocks[ 0 ] -> UpperBoundYf();

    for ( unsigned int i = 1; i < BlocksCount; i++ )
	max_y = Max( max_y, blocks[ i ] -> UpperBoundYi() );

    return max_y + pos_i.y;
}

int Figure :: LowerBoundZi()
{
    float min_z = blocks[ 0 ] -> LowerBoundZi();

    for ( unsigned int i = 1; i < BlocksCount; i++ )
	min_z = Min( min_z, blocks[ i ] -> LowerBoundZi() );

    return min_z + pos_i.z;
}

int Figure :: UpperBoundZi()
{
    int max_z = blocks[ 0 ] -> UpperBoundZi();

    for ( unsigned int i = 1; i < BlocksCount; i++ )
	max_z = Max( max_z, blocks[ i ] -> UpperBoundZi() );

    return max_z + pos_i.z;
}

Point3Di Figure :: GetBlockPosByIndex( int index ) const
{
    return blocks[ index ] -> GetPosi();
}

Material Figure :: GetBlockMaterialByIndex( int index ) const
{
	return blocks[ index ] -> GetMaterial();
}
