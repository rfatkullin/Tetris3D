#ifndef SCENE_H
#define SCENE_H

#include <QGLWidget>
#include "Game.h"

const int   WindowWidth = 800;
const int   WindowHeight = 600;

class Scene : public QGLWidget
{
   Q_OBJECT
public:
    Scene( Game* new_game, QWidget* pwgt = 0 );
    ~Scene();
    void SetLigthOption( float ambient[ 4 ], float diffuse[ 4 ], float specular[ 4 ] );
    void resize( int new_width, int new_height );
    void paintGL();//Какого хера!????
protected:
    void initializeGL();
    void resizeGL( int new_width, int new_height );
public slots :
    void set3D( bool );
private:
    int     scene_width;
    int     scene_height;
    const   Game*  game;
    float   ambient_light[ 4 ];
    float   diffuse_light[ 4 ];
    float   speculaer_light[ 4 ];
};

#endif // SCENE_H
