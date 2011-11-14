#include <QApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include <stdio.h>
#include "MainWindow.h"

MainWindow :: MainWindow() : QMainWindow()
{
    mpGame = new Game();
    mpSelectFiguresDialog = new SelectFiguresDialog();

    CreateScene();
    CreateActions();
    CreateMenus();
    CreateStatusBar();

    setMinimumSize( MIN_WIDTH, MIN_HEIGHT );
    setCentralWidget( mpScene );
    startTimer( 10 );
}

void MainWindow :: CreateActions()
{
    mpNewGameAction = new QAction( "&New game", this );
    connect( mpNewGameAction, SIGNAL( triggered() ), mpGame, SLOT( Start() ) );

    mpExitGameAction = new QAction( "&Exit", this );
    connect( mpExitGameAction, SIGNAL( triggered() ), this, SLOT( close() ) );

    mpTrun3dAction = new QAction( "3D effect", this );
    mpTrun3dAction -> setCheckable( true );
    mpTrun3dAction -> setChecked( false );
    connect( mpTrun3dAction, SIGNAL( toggled( bool ) ), mpScene, SLOT( set3D( bool ) ) );

    mpSelectFiguresAction = new QAction( "Select figures", this );
    connect( mpSelectFiguresAction, SIGNAL( triggered() ), this, SLOT( SelectFigures() ) );

    mpChangePresentMusicAction = new QAction( "Music", this );
    mpChangePresentMusicAction -> setCheckable( true );
    mpChangePresentMusicAction -> setChecked( true );

    mpChangePresentSoundsAction = new QAction( "Sounds", this );
    mpChangePresentSoundsAction -> setCheckable( true );
    mpChangePresentSoundsAction -> setChecked( true );

}

void MainWindow :: CreateMenus()
{
    mpMainMenu = menuBar() -> addMenu( "&Game" );
    mpMainMenu -> addAction( mpNewGameAction );
    mpMainMenu -> addAction( mpExitGameAction );

    mpSettingsMenu = menuBar() -> addMenu( "&Settings" );
    mpSettingsMenu -> addAction( mpTrun3dAction );
    mpSettingsMenu -> addAction( mpSelectFiguresAction );
    mpSettingsMenu -> addAction( mpChangePresentMusicAction );
    mpSettingsMenu -> addAction( mpChangePresentSoundsAction );
}

void MainWindow :: CreateScene()
{
    float ambient_light[ 4 ] = { 1.0f, 1.0f, 1.0f, 1.0f };
    float diffuse_light[ 4 ] = { 0.5f, 0.5f, 0.5f, 1.0f };
    float speculaer_light[ 4 ] = { 0.2f, 0.2f, 0.2f, 1.0f };

    mpScene = new Scene( mpGame);
    mpScene -> resize( WindowWidth, WindowHeight );
    mpScene -> SetLigthOption( ambient_light, diffuse_light, speculaer_light );
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
   switch ( key -> key() )
    {
        case Qt :: Key_P :
	    mpGame-> ChangePause();
            return;
            break;
        case Qt :: Key_Escape :
            exit( 0 );
            break;
        default:
            break;
    }

    if ( mpGame-> IsPause() )
        return;

    switch ( key -> key() )
    {
        case Qt :: Key_Escape :

        case Qt :: Key_Space :
	    mpGame-> DropDownFigure();
            break;
       case Qt :: Key_Right :
            mpGame-> SetShift( Game :: XAxis, Game :: ShiftDirectly );
            break;
        case Qt :: Key_Left :
            mpGame-> SetShift( Game :: XAxis, Game :: ShiftBack );
            break;
        case Qt :: Key_Up :
	    mpGame-> SetShift( Game :: ZAxis, Game :: ShiftBack );
            break;
        case Qt :: Key_Down :
            mpGame-> SetShift( Game :: ZAxis, Game :: ShiftDirectly );
            break;
        case Qt :: Key_D :
	    mpGame-> Rotate( Game :: PlaneXY, Game :: RotateByClockWise );
            break;
        case Qt :: Key_A :
	    mpGame-> Rotate( Game :: PlaneXY, Game :: RotateByAntiClockWise );
            break;
        case Qt :: Key_W :
	    mpGame-> Rotate( Game :: PlaneZY, Game :: RotateByClockWise );
            break;
        case Qt :: Key_S :
	    mpGame-> Rotate( Game :: PlaneZY, Game :: RotateByAntiClockWise );
            break;
        case Qt :: Key_Q :
	    mpGame-> Rotate( Game :: PlaneZX, Game :: RotateByClockWise );
            break;
        case Qt :: Key_E :
	    mpGame-> Rotate( Game :: PlaneZX, Game :: RotateByAntiClockWise );
            break;
        default:
            break;
    }
}

void MainWindow :: mousePressEvent( QMouseEvent* mouse )
{
    if ( mouse -> button() == Qt :: LeftButton )
	mpGame-> SetLastMousePosition( mouse -> globalX(), mouse -> globalY() );
}

void MainWindow :: mouseMoveEvent( QMouseEvent* mouse )
{
    Point3Df last_mouse_position = mpGame-> GetLastMousePosition();
    mpGame-> ChangeCameraPosition( mouse -> globalX() - last_mouse_position.x, mouse -> globalY() - last_mouse_position.y );
    mpGame-> SetLastMousePosition( mouse -> globalX(), mouse -> globalY() );
}

void MainWindow :: resizeEvent( int new_width, int new_height )
{
     mpScene -> resize( new_width, new_height );
}

void MainWindow :: timerEvent( QTimerEvent * )
{
    mpGame-> NextStep();
    mpScene -> paintGL();
}

void MainWindow :: SelectFigures()
{
    bool* select_figures = new bool[ Game :: FiguresMaxCnt ];

    mpGame -> GetSelectFigures( select_figures );
    mpSelectFiguresDialog -> SetSelectFigures( select_figures );

    mpGame -> ChangePause();
    if ( mpSelectFiguresDialog -> exec() == QDialog :: Accepted )
    {
        mpSelectFiguresDialog -> GetSelectFigures( select_figures );
        mpGame -> SetSelectFigures( select_figures );
    }
    mpGame -> ChangePause();

    delete [] select_figures;
}
