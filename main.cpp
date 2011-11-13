#include <QApplication>
#include <QMainWindow>
#include <QtGui>
#include "MainWindow.h"

int main( int argc, char **argv )
{


    QApplication app( argc, argv );
//    QMainWindow main_win;


//    QMenuBar* mnuBar = new QMenuBar();
//    QMenu* pmnu = new QMenu( "&Menu" );

//    pmnu -> addAction( "&About Qt",
//                        &app,
//                        SLOT( aboutQt() ),
//                        Qt :: CTRL + Qt :: Key_Q
//                     );
//    pmnu -> addSeparator();

//    QAction* pCheckableAction = pmnu -> addAction( "&CheckableItem" );
//    pCheckableAction -> setCheckable( true );
//    pCheckableAction -> setChecked( false );

//    QMenu* pmnuSubMenu = new QMenu( "&SubMenu", pmnu );
//    pmnu -> addMenu( pmnuSubMenu );
//    pmnuSubMenu -> addAction( "&Test" );

//    QAction* pDisabledAction = pmnu -> addAction( "&Disabled" );
//    pDisabledAction -> setEnabled( false );

//    pmnu -> addSeparator();

//    pmnu -> addAction( "&Exit", &app, SLOT( quit() ) );

////    //mnuBar->
//    mnuBar -> addMenu( pmnu );
//    main_win.s
////    //mnuBar.show();

////    QHBoxLayout* menu_layout = new QHBoxLayout;
////    menu_layout -> addWidget( mnuBar );

//    float ambient_light[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
//    float diffuse_light[ 4 ] = { 0.5f, 0.5f, 0.5f, 1.0f };
//    float speculaer_light[ 4 ] = { 0.2f, 0.2f, 0.2f, 1.0f };

//    Scene scene;
//    scene.resize( WindowWidth, WindowHeight );
//    scene.SetLigthOption( ambient_light, diffuse_light, speculaer_light );
//    scene.startTimer( 10 );
//    scene.show();

    MainWindow* main_window = new MainWindow();

    main_window -> show();



    app.exec();
}
