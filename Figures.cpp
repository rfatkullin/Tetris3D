#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <QGLWidget>
#include <stdio.h>
#include "Figures.h"

Block :: Block( float new_x, float new_y, float new_z, Material new_material )
{
    /*
	    Вершины куба занумерованы в след. порядке( делим куб на 4 части ):
	    1)-,+,- 2)-,+,+ 3)+,+,+ 4)+,+,-
	    5)+,-,- 6)+,-,+ 7)-,-,+ 8)-,-,-
    */
    current_rel_coordinates = const_rel_coordinates = Point3Df( new_x, new_y, new_z );
    material = new_material;

    current_vertices[ 0 ] = const_vertices[ 0 ] = Point3Df( -Block :: BlockSize / 2,  Block :: BlockSize / 2, -Block :: BlockSize / 2 );
    current_vertices[ 1 ] = const_vertices[ 1 ] = Point3Df( -Block :: BlockSize / 2,  Block :: BlockSize / 2,  Block :: BlockSize / 2 );
    current_vertices[ 2 ] = const_vertices[ 2 ] = Point3Df(  Block :: BlockSize / 2,  Block :: BlockSize / 2,  Block :: BlockSize / 2 );
    current_vertices[ 3 ] = const_vertices[ 3 ] = Point3Df(  Block :: BlockSize / 2,  Block :: BlockSize / 2, -Block :: BlockSize / 2 );
    current_vertices[ 4 ] = const_vertices[ 4 ] = Point3Df(  Block :: BlockSize / 2, -Block :: BlockSize / 2, -Block :: BlockSize / 2 );
    current_vertices[ 5 ] = const_vertices[ 5 ] = Point3Df(  Block :: BlockSize / 2, -Block :: BlockSize / 2,  Block :: BlockSize / 2 );
    current_vertices[ 6 ] = const_vertices[ 6 ] = Point3Df( -Block :: BlockSize / 2, -Block :: BlockSize / 2,  Block :: BlockSize / 2 );
    current_vertices[ 7 ] = const_vertices[ 7 ] = Point3Df( -Block :: BlockSize / 2, -Block :: BlockSize / 2, -Block :: BlockSize / 2 );
}

void Block :: Rotate( float &a, float &b, float angle )
{
    float cos_angle = cos( angle );
    float sin_angle = sin( angle );
    float old_a = a;

    a = a * cos_angle + b * sin_angle;
    b = -old_a * sin_angle + b * cos_angle;
}
void Block :: CurrentCoordinatesToConst()
{
    const_rel_coordinates = current_rel_coordinates;
    for ( int i = 0; i < BlocksVertexCount; ++i )
	const_vertices[ i ] = current_vertices[ i ];
}

void Block :: RotateOnZY( float angle, bool change_const )
{
    current_rel_coordinates = const_rel_coordinates;
    Rotate( current_rel_coordinates.z, current_rel_coordinates.y, angle );

    for ( int i = 0; i < BlocksVertexCount; ++i )
    {
	current_vertices[ i ] = const_vertices[ i ];
	Rotate( current_vertices[ i ].z, current_vertices[ i ].y, angle  );
    }

    if ( change_const )
	CurrentCoordinatesToConst();
}

void Block :: RotateOnZX( float angle, bool change_const )
{
    current_rel_coordinates = const_rel_coordinates;
    Rotate( current_rel_coordinates.z, current_rel_coordinates.x, angle );

    for ( int i = 0; i < BlocksVertexCount; ++i )
    {
	current_vertices[ i ] = const_vertices[ i ];
	Rotate( current_vertices[ i ].z, current_vertices[ i ].x, angle  );
    }

    if ( change_const )
	CurrentCoordinatesToConst();
}

void Block :: RotateOnXY( float angle, bool change_const )
{
    current_rel_coordinates = const_rel_coordinates;
    Rotate( current_rel_coordinates.x, current_rel_coordinates.y, angle );

    for ( int i = 0; i < BlocksVertexCount; ++i )
    {
	current_vertices[ i ] = const_vertices[ i ];
	Rotate( current_vertices[ i ].x, current_vertices[ i ].y, angle  );
    }

    if ( change_const )
	CurrentCoordinatesToConst();
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

    //Firts side
    DrawSide(	Point3Df( figure_location.x + current_rel_coordinates.x + current_vertices[ 0 ].x, figure_location.y + current_rel_coordinates.y + current_vertices[ 0 ].y, figure_location.z + current_rel_coordinates.z + current_vertices[ 0 ].z ),
		Point3Df( figure_location.x + current_rel_coordinates.x + current_vertices[ 1 ].x, figure_location.y + current_rel_coordinates.y + current_vertices[ 1 ].y, figure_location.z + current_rel_coordinates.z + current_vertices[ 1 ].z ),
		Point3Df( figure_location.x + current_rel_coordinates.x + current_vertices[ 2 ].x, figure_location.y + current_rel_coordinates.y + current_vertices[ 2 ].y, figure_location.z + current_rel_coordinates.z + current_vertices[ 2 ].z ),
		Point3Df( figure_location.x + current_rel_coordinates.x + current_vertices[ 3 ].x, figure_location.y + current_rel_coordinates.y + current_vertices[ 3 ].y, figure_location.z + current_rel_coordinates.z + current_vertices[ 3 ].z )
	    );

    //Second side
    DrawSide(	Point3Df( figure_location.x + current_rel_coordinates.x + current_vertices[ 1 ].x, figure_location.y + current_rel_coordinates.y + current_vertices[ 1 ].y, figure_location.z + current_rel_coordinates.z + current_vertices[ 1 ].z ),
		Point3Df( figure_location.x + current_rel_coordinates.x + current_vertices[ 6 ].x, figure_location.y + current_rel_coordinates.y + current_vertices[ 6 ].y, figure_location.z + current_rel_coordinates.z + current_vertices[ 6 ].z ),
		Point3Df( figure_location.x + current_rel_coordinates.x + current_vertices[ 5 ].x, figure_location.y + current_rel_coordinates.y + current_vertices[ 5 ].y, figure_location.z + current_rel_coordinates.z + current_vertices[ 5 ].z ),
		Point3Df( figure_location.x + current_rel_coordinates.x + current_vertices[ 2 ].x, figure_location.y + current_rel_coordinates.y + current_vertices[ 2 ].y, figure_location.z + current_rel_coordinates.z + current_vertices[ 2 ].z )
	    );

    //Third side
    DrawSide(	Point3Df( figure_location.x + current_rel_coordinates.x + current_vertices[ 2 ].x, figure_location.y + current_rel_coordinates.y + current_vertices[ 2 ].y, figure_location.z + current_rel_coordinates.z + current_vertices[ 2 ].z ),
		Point3Df( figure_location.x + current_rel_coordinates.x + current_vertices[ 5 ].x, figure_location.y + current_rel_coordinates.y + current_vertices[ 5 ].y, figure_location.z + current_rel_coordinates.z + current_vertices[ 5 ].z ),
		Point3Df( figure_location.x + current_rel_coordinates.x + current_vertices[ 4 ].x, figure_location.y + current_rel_coordinates.y + current_vertices[ 4 ].y, figure_location.z + current_rel_coordinates.z + current_vertices[ 4 ].z ),
		Point3Df( figure_location.x + current_rel_coordinates.x + current_vertices[ 3 ].x, figure_location.y + current_rel_coordinates.y + current_vertices[ 3 ].y, figure_location.z + current_rel_coordinates.z + current_vertices[ 3 ].z )
	    );

    //Fourth side
    DrawSide(	Point3Df( figure_location.x + current_rel_coordinates.x + current_vertices[ 6 ].x, figure_location.y + current_rel_coordinates.y + current_vertices[ 6 ].y, figure_location.z + current_rel_coordinates.z + current_vertices[ 6 ].z ),
		Point3Df( figure_location.x + current_rel_coordinates.x + current_vertices[ 7 ].x, figure_location.y + current_rel_coordinates.y + current_vertices[ 7 ].y, figure_location.z + current_rel_coordinates.z + current_vertices[ 7 ].z ),
		Point3Df( figure_location.x + current_rel_coordinates.x + current_vertices[ 4 ].x, figure_location.y + current_rel_coordinates.y + current_vertices[ 4 ].y, figure_location.z + current_rel_coordinates.z + current_vertices[ 4 ].z ),
		Point3Df( figure_location.x + current_rel_coordinates.x + current_vertices[ 5 ].x, figure_location.y + current_rel_coordinates.y + current_vertices[ 5 ].y, figure_location.z + current_rel_coordinates.z + current_vertices[ 5 ].z )
	    );

    //Fifth side
    DrawSide(	Point3Df( figure_location.x + current_rel_coordinates.x + current_vertices[ 0 ].x, figure_location.y + current_rel_coordinates.y + current_vertices[ 0 ].y, figure_location.z + current_rel_coordinates.z + current_vertices[ 0 ].z ),
		Point3Df( figure_location.x + current_rel_coordinates.x + current_vertices[ 7 ].x, figure_location.y + current_rel_coordinates.y + current_vertices[ 7 ].y, figure_location.z + current_rel_coordinates.z + current_vertices[ 7 ].z ),
		Point3Df( figure_location.x + current_rel_coordinates.x + current_vertices[ 6 ].x, figure_location.y + current_rel_coordinates.y + current_vertices[ 6 ].y, figure_location.z + current_rel_coordinates.z + current_vertices[ 6 ].z ),
		Point3Df( figure_location.x + current_rel_coordinates.x + current_vertices[ 1 ].x, figure_location.y + current_rel_coordinates.y + current_vertices[ 1 ].y, figure_location.z + current_rel_coordinates.z + current_vertices[ 1 ].z )
	    );

    //Sixth side
    DrawSide(	Point3Df( figure_location.x + current_rel_coordinates.x + current_vertices[ 3 ].x, figure_location.y + current_rel_coordinates.y + current_vertices[ 3 ].y, figure_location.z + current_rel_coordinates.z + current_vertices[ 3 ].z ),
		Point3Df( figure_location.x + current_rel_coordinates.x + current_vertices[ 4 ].x, figure_location.y + current_rel_coordinates.y + current_vertices[ 4 ].y, figure_location.z + current_rel_coordinates.z + current_vertices[ 4 ].z ),
		Point3Df( figure_location.x + current_rel_coordinates.x + current_vertices[ 7 ].x, figure_location.y + current_rel_coordinates.y + current_vertices[ 7 ].y, figure_location.z + current_rel_coordinates.z + current_vertices[ 7 ].z ),
		Point3Df( figure_location.x + current_rel_coordinates.x + current_vertices[ 0 ].x, figure_location.y + current_rel_coordinates.y + current_vertices[ 0 ].y, figure_location.z + current_rel_coordinates.z + current_vertices[ 0 ].z )
	    );
}

float Block :: LowerBoundZ()
{
    float min_z = current_vertices[ 0 ].z;

    for ( int i = 1; i < BlocksVertexCount; i++ )
	min_z = Min( min_z, current_vertices[ i ].z );

    return min_z + current_rel_coordinates.z;
}

float Block :: UpperBoundZ()
{
    float max_z = current_vertices[ 0 ].z;

    for ( int i = 1; i < BlocksVertexCount; i++ )
	max_z = Max( max_z, current_vertices[ i ].z );

    return max_z + current_rel_coordinates.z;
}

float Block :: LowerBoundX()
{
    float min_x = current_vertices[ 0 ].x;

    for ( int i = 1; i < BlocksVertexCount; i++ )
	min_x = Min( min_x, current_vertices[ i ].x );

    return min_x + current_rel_coordinates.x;
}

float Block :: UpperBoundX()
{
    float max_x = current_vertices[ 0 ].x;

    for ( int i = 1; i < BlocksVertexCount; i++ )
	max_x = Max( max_x, current_vertices[ i ].x );

    return max_x + current_rel_coordinates.x;
}

float Block :: LowerBoundY()
{
    float min_y = current_vertices[ 0 ].y;

    for ( int i = 1; i < BlocksVertexCount; i++ )
	min_y = Min( min_y, current_vertices[ i ].y );

    return min_y + current_rel_coordinates.y;
}

float Block :: UpperBoundY()
{
    float max_y = current_vertices[ 0 ].y;

    for ( int i = 1; i < BlocksVertexCount; i++ )
	max_y = Max( max_y, current_vertices[ i ].y );

    return max_y + current_rel_coordinates.y;
}

Figure :: Figure( float x, float y, float z, Figures type, Material new_material )
{
    position = Point3Df( x, y, z );
    material = new_material;

    switch ( type )
    {
    case IFigure :
	blocks[ 0 ] = new Block( 0.0f, Block :: BlockSize + Block :: BlockSize / 2, 0.0f, material );
	blocks[ 1 ] = new Block( 0.0f, Block :: BlockSize / 2, 0.0f, material  );
	blocks[ 2 ] = new Block( 0.0f, -Block :: BlockSize / 2, 0.0f, material );
	blocks[ 3 ] = new Block( 0.0f, -( Block :: BlockSize + Block :: BlockSize / 2 ), 0.0f, material );
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
    glMaterialfv( GL_FRONT, GL_AMBIENT, material.GetMaterialForAmbient() );
    glMaterialfv( GL_FRONT, GL_DIFFUSE, material.GetMaterialForDiffuse() );
    glMaterialfv( GL_FRONT, GL_SPECULAR, material.GetMaterialForSpecular() );

    for ( int i = 0; i < BlocksCount; ++i )
	blocks[ i ] -> Draw( position );
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

Point3Df Figure :: GetPosition()
{
    return position;
}

void Figure :: SetPosition( Point3Df new_position )
{
    position = new_position;
}

float Figure :: LowerBoundX()
{
    float min_x = blocks[ 0 ] -> LowerBoundX();

    for ( int i = 1; i < BlocksCount; i++ )
	min_x = Min( min_x, blocks[ i ] -> LowerBoundX() );

    return min_x + position.x;
}

float Figure :: UpperBoundX()
{
    float max_x = blocks[ 0 ] -> UpperBoundX();

    for ( int i = 1; i < BlocksCount; i++ )
	max_x = Max( max_x, blocks[ i ] -> UpperBoundX() );

    return max_x + position.x;
}

float Figure :: LowerBoundY()
{
    float min_y = blocks[ 0 ] -> LowerBoundY();

    for ( int i = 1; i < BlocksCount; i++ )
	min_y = Min( min_y, blocks[ i ] -> LowerBoundY() );

    return min_y + position.y;
}

float Figure :: UpperBoundY()
{
    float max_y = blocks[ 0 ] -> UpperBoundY();

    for ( int i = 1; i < BlocksCount; i++ )
	max_y = Max( max_y, blocks[ i ] -> UpperBoundY() );

    return max_y + position.y;
}

float Figure :: LowerBoundZ()
{
    float min_z = blocks[ 0 ] -> LowerBoundZ();

    for ( int i = 1; i < BlocksCount; i++ )
	min_z = Min( min_z, blocks[ i ] -> LowerBoundZ() );

    return min_z + position.z;
}

float Figure :: UpperBoundZ()
{
    float max_z = blocks[ 0 ] -> UpperBoundZ();

    for ( int i = 1; i < BlocksCount; i++ )
	max_z = Max( max_z, blocks[ i ] -> UpperBoundZ() );

    return max_z + position.z;
}




