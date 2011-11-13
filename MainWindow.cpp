#include <QApplication>
#include <QKeyEvent>
#include "MainWindow.h"

MainWindow :: MainWindow() : QMainWindow()
{
    setMinimumSize( MIN_WIDTH, MIN_HEIGHT );
    CreateScene();
    CreateActions();
    CreateMenus();
    CreateStatusBar();

    setCentralWidget( scene );
}

void MainWindow :: CreateActions()
{
    new_game_action = new QAction( "&New game", this );
    connect( new_game_action, SIGNAL( triggered() ), this, SLOT( NewGame() ) );

    exit_game_action = new QAction( "&Exit", this );
    connect( exit_game_action, SIGNAL( triggered() ), this, SLOT( close() ) );

    turn_3d_action = new QAction( "3D effect", this );
    turn_3d_action -> setCheckable( true );
    turn_3d_action -> setChecked( false );
    connect( turn_3d_action, SIGNAL( toggled( bool ) ), (Scene*)scene, SLOT( set3D( bool ) ) );

    with_i_action = new QAction( "I figures", this );
    with_i_action -> setCheckable( true );
    with_i_action -> setChecked( true );

    with_j_action = new QAction( "J figures", this );
    with_j_action -> setCheckable( true );
    with_j_action -> setChecked( true );

    with_l_action = new QAction( "L figures", this );
    with_l_action -> setCheckable( true );
    with_l_action -> setChecked( true );

    with_o_action = new QAction( "O figures", this );
    with_o_action -> setCheckable( true );
    with_o_action -> setChecked( true );

    with_s_action = new QAction( "S figures", this );
    with_s_action -> setCheckable( true );
    with_s_action -> setChecked( true );

    with_t_action = new QAction( "T figures", this );
    with_t_action -> setCheckable( true );
    with_t_action -> setChecked( true );

    with_z_action = new QAction( "Z figures", this );
    with_z_action -> setCheckable( true );
    with_z_action -> setChecked( true );

}

void MainWindow :: CreateMenus()
{
    main_menu = menuBar() -> addMenu( "&Game" );
    main_menu -> addAction( new_game_action );
    main_menu -> addAction( exit_game_action );

    settings_menu = menuBar() -> addMenu( "&Settings" );
    settings_menu -> addAction( turn_3d_action );

    figures_menu = settings_menu -> addMenu( "&Figures" );
    figures_menu -> addAction( with_i_action );
    figures_menu -> addAction( with_j_action );
    figures_menu -> addAction( with_l_action );
    figures_menu -> addAction( with_o_action );
    figures_menu -> addAction( with_s_action );
    figures_menu -> addAction( with_t_action );
    figures_menu -> addAction( with_z_action );

}

void MainWindow :: CreateScene()
{
    float ambient_light[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float diffuse_light[ 4 ] = { 0.5f, 0.5f, 0.5f, 1.0f };
    float speculaer_light[ 4 ] = { 0.2f, 0.2f, 0.2f, 1.0f };

    //Scene scene;
    scene = new Scene();
    scene -> resize( WindowWidth, WindowHeight );
    scene -> SetLigthOption( ambient_light, diffuse_light, speculaer_light );
    scene -> startTimer( 10 );
    scene -> show();
}

void MainWindow :: CreateStatusBar()
{

}

void MainWindow :: NewGame()
{

}

void MainWindow :: Exit()
{

}

void MainWindow :: closeEvent( QCloseEvent* )
{

}

void MainWindow :: keyPressEvent( QKeyEvent* key )
{
    QApplication::sendEvent( scene, key );
}

void MainWindow :: mousePressEvent( QMouseEvent* mouse )
{
    QApplication::sendEvent( scene, mouse );
}

void MainWindow :: mouseMoveEvent( QMouseEvent* mouse )
{
     QApplication::sendEvent( scene, mouse );
}

void MainWindow :: resizeEvent( int new_width, int new_height )
{
     scene -> resize( new_width, new_height );
}

