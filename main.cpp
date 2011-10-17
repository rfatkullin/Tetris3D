#include <QApplication>
#include "Scene.h"

int main( int argc, char **argv )
{
    float ambient_light[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float diffuse_light[ 4 ] = { 0.5f, 0.5f, 0.5f, 1.0f };
    float speculaer_light[ 4 ] = { 0.2f, 0.2f, 0.2f, 1.0f };

    QApplication app( argc, argv );

    Scene scene;
    scene.resize( WindowWidth, WindowHeight );
    scene.SetLigthOption( ambient_light, diffuse_light, speculaer_light );
    scene.startTimer( 10 );
    scene.show();

    app.exec();
}
