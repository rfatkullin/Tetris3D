#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <QGLWidget>
#include "Figures.h"

Block :: Block( float new_x, float new_y, float new_z, Material new_material )
{
	/*
		Вершины куба занумерованы в след. порядке( делим куб на 4 части ):
		1)-,+,- 2)-,+,+ 3)+,+,+ 4)+,+,-
		5)+,-,- 6)+,-,+ 7)-,-,+ 8)-,-,-
	*/
	current_rel_coordinates = const_rel_coordinates = Point3D( new_x, new_y, new_z );
	material = new_material;
	
	current_vertices[ 0 ] = const_vertices[ 0 ] = Point3D( -BlockSize / 2,  BlockSize / 2, -BlockSize / 2 );
	current_vertices[ 1 ] = const_vertices[ 1 ] = Point3D( -BlockSize / 2,  BlockSize / 2,  BlockSize / 2 );
	current_vertices[ 2 ] = const_vertices[ 2 ] = Point3D(  BlockSize / 2,  BlockSize / 2,  BlockSize / 2 );
	current_vertices[ 3 ] = const_vertices[ 3 ] = Point3D(  BlockSize / 2,  BlockSize / 2, -BlockSize / 2 );
	current_vertices[ 4 ] = const_vertices[ 4 ] = Point3D(  BlockSize / 2, -BlockSize / 2, -BlockSize / 2 );
	current_vertices[ 5 ] = const_vertices[ 5 ] = Point3D(  BlockSize / 2, -BlockSize / 2,  BlockSize / 2 );
	current_vertices[ 6 ] = const_vertices[ 6 ] = Point3D( -BlockSize / 2, -BlockSize / 2,  BlockSize / 2 );
	current_vertices[ 7 ] = const_vertices[ 7 ] = Point3D( -BlockSize / 2, -BlockSize / 2, -BlockSize / 2 );
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
	/*float cos_angle = cos( angle );
	float sin_angle = sin( angle );

	z = -x * sin_angle + z * cos_angle;	
	x = x * cos_angle + z * sin_angle;	*/
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

void Block :: DrawSide( Point3D p1, Point3D p2, Point3D p3, Point3D p4 )
{
	float ambient_light[ 4 ] = { 0.0f, 0.5f, 0.0f, 1.0f };
	float diffues_light[ 4 ] = { 0.0f, 1.0f, 0.0f, 1.0f };
	float specular_light[ 4 ] = { 0.2f, 0.2f, 0.2f, 1.0f };

	Point3D NormalVector = GetNormalVector( p1, p3, p2 );
	glMaterialfv( GL_FRONT, GL_AMBIENT, ambient_light );
	glMaterialfv( GL_FRONT, GL_DIFFUSE, diffues_light );
	glMaterialfv( GL_FRONT, GL_SPECULAR, specular_light );
	
	glBegin( GL_QUADS );
		glNormal3f( NormalVector.x, NormalVector.y, NormalVector.z );
		glVertex3f( p1.x, p1.y, p1.z );
		glVertex3f( p2.x, p2.y, p2.z );
		glVertex3f( p3.x, p3.y, p3.z );
		glVertex3f( p4.x, p4.y, p4.z );
	glEnd();

	/*glColor3f( 1.0f, 1.0f, 1.0f );	
	glBegin( GL_POINTS );
		glVertex3f( 100.0f, 100.0f, 100.0f );
	glEnd();	*/
}

void Block :: Draw( Point3D figure_location )
{
	/*
		Вершины куба занумерованы в след. порядке( делим куб на 4 части ):
		1)-,+,- 2)-,+,+ 3)+,+,+ 4)+,+,-
		5)+,-,- 6)+,-,+ 7)-,-,+ 8)-,-,-
	*/
	
	//Firts side
	DrawSide(	Point3D( figure_location.x + current_rel_coordinates.x + current_vertices[ 0 ].x, figure_location.y + current_rel_coordinates.y + current_vertices[ 0 ].y, figure_location.z + current_rel_coordinates.z + current_vertices[ 0 ].z ),
				Point3D( figure_location.x + current_rel_coordinates.x + current_vertices[ 1 ].x, figure_location.y + current_rel_coordinates.y + current_vertices[ 1 ].y, figure_location.z + current_rel_coordinates.z + current_vertices[ 1 ].z ),
				Point3D( figure_location.x + current_rel_coordinates.x + current_vertices[ 2 ].x, figure_location.y + current_rel_coordinates.y + current_vertices[ 2 ].y, figure_location.z + current_rel_coordinates.z + current_vertices[ 2 ].z ),
				Point3D( figure_location.x + current_rel_coordinates.x + current_vertices[ 3 ].x, figure_location.y + current_rel_coordinates.y + current_vertices[ 3 ].y, figure_location.z + current_rel_coordinates.z + current_vertices[ 3 ].z )
			);
	
	//Second side
	DrawSide(	Point3D( figure_location.x + current_rel_coordinates.x + current_vertices[ 1 ].x, figure_location.y + current_rel_coordinates.y + current_vertices[ 1 ].y, figure_location.z + current_rel_coordinates.z + current_vertices[ 1 ].z ),
				Point3D( figure_location.x + current_rel_coordinates.x + current_vertices[ 6 ].x, figure_location.y + current_rel_coordinates.y + current_vertices[ 6 ].y, figure_location.z + current_rel_coordinates.z + current_vertices[ 6 ].z ),
				Point3D( figure_location.x + current_rel_coordinates.x + current_vertices[ 5 ].x, figure_location.y + current_rel_coordinates.y + current_vertices[ 5 ].y, figure_location.z + current_rel_coordinates.z + current_vertices[ 5 ].z ),
				Point3D( figure_location.x + current_rel_coordinates.x + current_vertices[ 2 ].x, figure_location.y + current_rel_coordinates.y + current_vertices[ 2 ].y, figure_location.z + current_rel_coordinates.z + current_vertices[ 2 ].z )
			);
	
	//Third side
	DrawSide(	Point3D( figure_location.x + current_rel_coordinates.x + current_vertices[ 2 ].x, figure_location.y + current_rel_coordinates.y + current_vertices[ 2 ].y, figure_location.z + current_rel_coordinates.z + current_vertices[ 2 ].z ),
				Point3D( figure_location.x + current_rel_coordinates.x + current_vertices[ 5 ].x, figure_location.y + current_rel_coordinates.y + current_vertices[ 5 ].y, figure_location.z + current_rel_coordinates.z + current_vertices[ 5 ].z ),
				Point3D( figure_location.x + current_rel_coordinates.x + current_vertices[ 4 ].x, figure_location.y + current_rel_coordinates.y + current_vertices[ 4 ].y, figure_location.z + current_rel_coordinates.z + current_vertices[ 4 ].z ),
				Point3D( figure_location.x + current_rel_coordinates.x + current_vertices[ 3 ].x, figure_location.y + current_rel_coordinates.y + current_vertices[ 3 ].y, figure_location.z + current_rel_coordinates.z + current_vertices[ 3 ].z )
			);

	//Fourth side
	DrawSide(	Point3D( figure_location.x + current_rel_coordinates.x + current_vertices[ 6 ].x, figure_location.y + current_rel_coordinates.y + current_vertices[ 6 ].y, figure_location.z + current_rel_coordinates.z + current_vertices[ 6 ].z ),
				Point3D( figure_location.x + current_rel_coordinates.x + current_vertices[ 7 ].x, figure_location.y + current_rel_coordinates.y + current_vertices[ 7 ].y, figure_location.z + current_rel_coordinates.z + current_vertices[ 7 ].z ),
				Point3D( figure_location.x + current_rel_coordinates.x + current_vertices[ 4 ].x, figure_location.y + current_rel_coordinates.y + current_vertices[ 4 ].y, figure_location.z + current_rel_coordinates.z + current_vertices[ 4 ].z ),
				Point3D( figure_location.x + current_rel_coordinates.x + current_vertices[ 5 ].x, figure_location.y + current_rel_coordinates.y + current_vertices[ 5 ].y, figure_location.z + current_rel_coordinates.z + current_vertices[ 5 ].z )
			);

	//Fifth side
	DrawSide(	Point3D( figure_location.x + current_rel_coordinates.x + current_vertices[ 0 ].x, figure_location.y + current_rel_coordinates.y + current_vertices[ 0 ].y, figure_location.z + current_rel_coordinates.z + current_vertices[ 0 ].z ),
				Point3D( figure_location.x + current_rel_coordinates.x + current_vertices[ 7 ].x, figure_location.y + current_rel_coordinates.y + current_vertices[ 7 ].y, figure_location.z + current_rel_coordinates.z + current_vertices[ 7 ].z ),
				Point3D( figure_location.x + current_rel_coordinates.x + current_vertices[ 6 ].x, figure_location.y + current_rel_coordinates.y + current_vertices[ 6 ].y, figure_location.z + current_rel_coordinates.z + current_vertices[ 6 ].z ),
				Point3D( figure_location.x + current_rel_coordinates.x + current_vertices[ 1 ].x, figure_location.y + current_rel_coordinates.y + current_vertices[ 1 ].y, figure_location.z + current_rel_coordinates.z + current_vertices[ 1 ].z )
			);

	//Sixth side
	DrawSide(	Point3D( figure_location.x + current_rel_coordinates.x + current_vertices[ 3 ].x, figure_location.y + current_rel_coordinates.y + current_vertices[ 3 ].y, figure_location.z + current_rel_coordinates.z + current_vertices[ 3 ].z ),
				Point3D( figure_location.x + current_rel_coordinates.x + current_vertices[ 4 ].x, figure_location.y + current_rel_coordinates.y + current_vertices[ 4 ].y, figure_location.z + current_rel_coordinates.z + current_vertices[ 4 ].z ),
				Point3D( figure_location.x + current_rel_coordinates.x + current_vertices[ 7 ].x, figure_location.y + current_rel_coordinates.y + current_vertices[ 7 ].y, figure_location.z + current_rel_coordinates.z + current_vertices[ 7 ].z ),
				Point3D( figure_location.x + current_rel_coordinates.x + current_vertices[ 0 ].x, figure_location.y + current_rel_coordinates.y + current_vertices[ 0 ].y, figure_location.z + current_rel_coordinates.z + current_vertices[ 0 ].z )
			);
}

Figure :: Figure( float x, float y, float z, Figures type )
{
	position = Point3D( x, y, z );
	
	srand( time( 0 ) );
	material = materials[ rand() % MaterialsCount ];

	switch ( type )
	{
	case IFigure :
		blocks[ 0 ] = new Block( 0.0f, BlockSize + BlockSize / 2, 0.0f, material );
		blocks[ 1 ] = new Block( 0.0f, BlockSize / 2, 0.0f, material  );
		blocks[ 2 ] = new Block( 0.0f, -BlockSize / 2, 0.0f, material );
		blocks[ 3 ] = new Block( 0.0f, -( BlockSize + BlockSize / 2 ), 0.0f, material );
		break;
	case JFigure :
		blocks[ 0 ] = new Block( 0.0f, BlockSize + BlockSize / 2, 0.0f, material );
		blocks[ 1 ] = new Block( 0.0f, BlockSize / 2, 0.0f, material );
		blocks[ 2 ] = new Block( 0.0f, -BlockSize / 2, 0.0f, material );
		blocks[ 3 ] = new Block( BlockSize, -( BlockSize + BlockSize / 2 ), 0.0f, material );
		break;
	case LFigure :
		blocks[ 0 ] = new Block( 0.0f, -( BlockSize + BlockSize / 2 ), 0.0f, material );
		blocks[ 1 ] = new Block( 0.0f, BlockSize / 2, 0.0f, material );
		blocks[ 2 ] = new Block( 0.0f, -BlockSize / 2, 0.0f, material );
		blocks[ 3 ] = new Block( -BlockSize, -( BlockSize + BlockSize / 2 ), 0.0f, material );
		break;
	case OFigure :
		blocks[ 0 ] = new Block( -BlockSize / 2, BlockSize / 2, 0.0f, material );
		blocks[ 1 ] = new Block( BlockSize / 2, BlockSize / 2, 0.0f, material );
		blocks[ 2 ] = new Block( BlockSize / 2, -BlockSize / 2, 0.0f, material );
		blocks[ 3 ] = new Block( -BlockSize / 2, -BlockSize / 2, 0.0f, material );
		break;
	case SFigure :
		blocks[ 0 ] = new Block( BlockSize + BlockSize / 2, BlockSize / 2, 0.0f, material );
		blocks[ 1 ] = new Block( 0.0f, BlockSize / 2, 0.0f, material );
		blocks[ 2 ] = new Block( 0.0f, -BlockSize / 2, 0.0f, material );
		blocks[ 3 ] = new Block( -( BlockSize + BlockSize / 2 ), -BlockSize + BlockSize / 2, 0.0f, material );
		break;
	case TFigure :
		blocks[ 0 ] = new Block( BlockSize, BlockSize / 2, 0.0f, material );
		blocks[ 1 ] = new Block( 0.0f, BlockSize / 2, 0.0f, material );
		blocks[ 2 ] = new Block( -BlockSize, BlockSize / 2, 0.0f, material );
		blocks[ 3 ] = new Block( 0.0f, -BlockSize / 2, 0.0f, material );
		break;
	default : //ZFigure :
		blocks[ 0 ] = new Block( -BlockSize, BlockSize / 2, 0.0f, material );
		blocks[ 1 ] = new Block( 0.0f, BlockSize / 2, 0.0f, material );
		blocks[ 2 ] = new Block( 0.0f, -BlockSize / 2, 0.0f, material );
		blocks[ 3 ] = new Block( BlockSize, -BlockSize / 2, 0.0f, material );
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

Point3D Figure :: GetPosition()
{
	return position;
}

void Figure :: SetPostion( Point3D new_position )
{
	position = new_position;
}
