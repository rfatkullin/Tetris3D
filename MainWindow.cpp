#include <QApplication>
#include <QMouseEvent>
#include <QKeyEvent>
#include <stdio.h>
#include "MainWindow.h"

FigureShift MainWindow :: msFigureControl[ MainWindow :: ViewCnt ][ MainWindow :: ButtonsCnt ];

void MainWindow :: SetFigurShiftConstants()
{
    //Buttons indexes :
    //                 S - 0, W - 1, A - 2, D - 3

    msFigureControl[ 0 ][ 0 ] = FigureShift( Game :: Z_AXIS, Game :: SHIFT_DIRECTLY );
    msFigureControl[ 0 ][ 1 ] = FigureShift( Game :: Z_AXIS, Game :: SHIFT_BACK );
    msFigureControl[ 0 ][ 2 ] = FigureShift( Game :: X_AXIS, Game :: SHIFT_BACK );
    msFigureControl[ 0 ][ 3 ] = FigureShift( Game :: X_AXIS, Game :: SHIFT_DIRECTLY );

    msFigureControl[ 1 ][ 0 ] = FigureShift( Game :: X_AXIS, Game :: SHIFT_DIRECTLY );
    msFigureControl[ 1 ][ 1 ] = FigureShift( Game :: X_AXIS, Game :: SHIFT_BACK );
    msFigureControl[ 1 ][ 2 ] = FigureShift( Game :: Z_AXIS, Game :: SHIFT_DIRECTLY );
    msFigureControl[ 1 ][ 3 ] = FigureShift( Game :: Z_AXIS, Game :: SHIFT_BACK );

    msFigureControl[ 2 ][ 0 ] = FigureShift( Game :: Z_AXIS, Game :: SHIFT_BACK );
    msFigureControl[ 2 ][ 1 ] = FigureShift( Game :: Z_AXIS, Game :: SHIFT_DIRECTLY );
    msFigureControl[ 2 ][ 2 ] = FigureShift( Game :: X_AXIS, Game :: SHIFT_DIRECTLY );
    msFigureControl[ 2 ][ 3 ] = FigureShift( Game :: X_AXIS, Game :: SHIFT_BACK );

    msFigureControl[ 3 ][ 0 ] = FigureShift( Game :: X_AXIS, Game :: SHIFT_BACK );
    msFigureControl[ 3 ][ 1 ] = FigureShift( Game :: X_AXIS, Game :: SHIFT_DIRECTLY );
    msFigureControl[ 3 ][ 2 ] = FigureShift( Game :: Z_AXIS, Game :: SHIFT_BACK );
    msFigureControl[ 3 ][ 3 ] = FigureShift( Game :: Z_AXIS, Game :: SHIFT_DIRECTLY );
}

MainWindow :: MainWindow() : QMainWindow()
{
    mpGame		    = new Game( this );
    mpSelectFiguresDialog   = new SelectFiguresDialog( this );
    mpGameOverDialog        = NULL;
    mIsRightButtonPressed   = false;
    mIsGame                 = false;
    mIsFullScreen           = false;

    CreateScene();
    CreateActions();
    CreateMenus();   

    SetFigurShiftConstants();

    setMinimumSize( MIN_WIDTH, MIN_HEIGHT );
    setCentralWidget( mpScene );
    startTimer( 10 );
}

void MainWindow :: CreateActions()
{
    mpNewGameAction = new QAction( "New game", this );
    connect( mpNewGameAction, SIGNAL( triggered() ), SLOT( NewGame() ) );

    mpExitGameAction = new QAction( "Exit", this );
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
    connect( mpChangePresentMusicAction, SIGNAL( toggled( bool ) ), mpGame, SLOT( AmbientMusicStateChange( bool ) ) );

    mpChangePresentSoundsAction = new QAction( "Sounds", this );
    mpChangePresentSoundsAction -> setCheckable( true );
    mpChangePresentSoundsAction -> setChecked( true );
    connect( mpChangePresentSoundsAction, SIGNAL( toggled( bool ) ), mpGame, SLOT( SoundsStateChange( bool ) ) );

}

void MainWindow :: CreateMenus()
{
    mpMainMenu = menuBar() -> addMenu( "Game" );
    mpMainMenu -> addAction( mpNewGameAction );
    mpMainMenu -> addAction( mpExitGameAction );

    mpSettingsMenu = menuBar() -> addMenu( "Settings" );
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

    mpScene = new Scene( mpGame );
    mpScene -> SetLigthOption( ambient_light, diffuse_light, speculaer_light );
}

void MainWindow :: NewGame()
{
    mpGame -> End();
    mpGame -> Start();
    mIsGame = true;
}

void MainWindow :: Exit()
{
    QApplication :: exit( 0 );
}

void MainWindow :: closeEvent( QCloseEvent* )
{
    Exit();
}

void MainWindow :: keyPressEvent( QKeyEvent* key )
{
    int side = mpScene -> GetViewSide();

    switch ( key -> key() )
    {
	case Qt :: Key_P :
            mIsGame = !mIsGame;
            return;
            break;
        case Qt :: Key_Escape :
	    Exit();
            break;
        case Qt :: Key_F1 :
            mpScene -> ChangeShowHelp();
            break;
        case Qt :: Key_F2 :
            NewGame();
            break;
//        case Qt :: Key_F5 :
//            game_over = new GameOverDialog( 100 );
//            game_over -> exec();
//            break;
        default:
            break;
    }

    if ( !mIsGame )
        return;

    switch ( key -> key() )
    {

//	case Qt :: Key_I :
//	    mpScene -> mFrustumFocalLength++;
//	    printf( "%f\n", mpScene -> mFrustumFocalLength );
//	    mpScene -> mFrustumEyeSep = mpScene -> mFrustumFocalLength / 30.0f;
//	    break;
//	case Qt :: Key_K :
//	    mpScene -> mFrustumFocalLength--;
//	    printf( "%f\n", mpScene -> mFrustumFocalLength );
//	    mpScene -> mFrustumEyeSep = mpScene -> mFrustumFocalLength / 30.0f;
//	    break;
        case Qt :: Key_F4 :
            mpGame -> AmbientMusicStateChange( !mpGame -> AmbientMusicState() );
            break;
        case Qt :: Key_F3 :
            mIsFullScreen = !mIsFullScreen;
            this -> menuBar() -> setVisible( !mIsFullScreen );

            if ( mIsFullScreen )
            {
                this -> showFullScreen();
                this -> setCursor( Qt :: BlankCursor );
            }
            else
            {
                this -> showNormal();
                this -> setCursor( Qt :: ArrowCursor );
            }

            break;
        case Qt :: Key_Space :
	    mpGame-> DropDownFigure();
            break;
        case Qt :: Key_S :
            mpGame -> SetShift( msFigureControl[ side ][ S_BUTTON ].mAxis,
                                msFigureControl[ side ][ S_BUTTON ].mDirection );
            break;
        case Qt :: Key_W :
            mpGame -> SetShift( msFigureControl[ side ][ W_BUTTON ].mAxis,
                                msFigureControl[ side ][ W_BUTTON ].mDirection );
            break;
        case Qt :: Key_A :
            mpGame -> SetShift( msFigureControl[ side ][ A_BUTTON ].mAxis,
                                msFigureControl[ side ][ A_BUTTON ].mDirection );
            break;
        case Qt :: Key_D :
            mpGame -> SetShift( msFigureControl[ side ][ D_BUTTON ].mAxis,
                                msFigureControl[ side ][ D_BUTTON ].mDirection );
            break;
	default:
            break;
    }
}

void MainWindow :: mousePressEvent( QMouseEvent* mouse )
{
    if ( !mIsGame )
        return;

    Qt :: MouseButton button = mouse -> button();

    //if ( button == Qt :: RightButton )
        //mpScene -> mIsOneSide = !mpScene -> mIsOneSide;

    if ( ( button == Qt :: LeftButton ) || ( button == Qt :: RightButton )  )
	mLastMousePos = mouse -> globalPos();

    if ( button == Qt :: RightButton )
	mIsRightButtonPressed = true;

    mLastMouseButton = button;
}
void MainWindow :: mouseReleaseEvent( QMouseEvent* mouse )
{
    if ( !mIsGame )
        return;

    if (  mLastMouseButton == Qt :: RightButton )
        SelectRotate( mouse -> globalX(), mouse -> globalY() );
}

void MainWindow :: wheelEvent ( QWheelEvent * aEvent )
{
    if ( !mIsGame )
        return;

    if ( aEvent -> delta() > 0 )
        mpGame-> Rotate( Game :: PLANE_ZX, Game :: ROTATE_BY_CLOCK_WISE );
    else
        mpGame-> Rotate( Game :: PLANE_ZX, Game :: ROTATE_BY_ANTI_CLOCKWISE );
}

void MainWindow :: mouseMoveEvent( QMouseEvent* mouse )
{
    if ( !mIsGame )
        return;

    if ( mLastMouseButton != Qt :: LeftButton )
	return;

    mpScene -> ChangeCameraPosition( mouse -> globalX() - mLastMousePos.x(),
                          mouse -> globalY() - mLastMousePos.y() );
    mLastMousePos = mouse -> globalPos();
}

void MainWindow :: resizeEvent( int aNewWidth, int aNewHeight )
{
     mpScene -> Resize( aNewWidth, aNewHeight );
}

void MainWindow :: timerEvent( QTimerEvent * )
{
    if ( mpGame -> IsGameOver() )
    {
        mpGameOverDialog = new GameOverDialog( width() / 2, height() / 2, mpGame -> GetScore() );
        mpGameOverDialog -> exec();
        delete mpGameOverDialog;
        mIsGame = false;
    }

   if ( mIsGame )
        mpGame-> NextStep();

    mpScene -> paintGL();

    if ( mIsGame )
        mpGame -> ClearMessagesList();
}

void MainWindow :: SelectFigures()
{
    bool* select_figures = new bool[ Game :: FIGURES_MAX_CNT ];

    mpGame -> GetSelectFigures( select_figures );
    mpSelectFiguresDialog -> SetSelectFigures( select_figures );

    mIsGame = false;
    if ( mpSelectFiguresDialog -> exec() == QDialog :: Accepted )
    {
        mpSelectFiguresDialog -> GetSelectFigures( select_figures );
        mpGame -> SetSelectFigures( select_figures );
    }
    mIsGame = true;

    delete [] select_figures;
}

void MainWindow :: SelectRotate( int aX, int aY )
{
    Game :: RotatePlane rot_plane;
    int                 inverse     = 1;
    int                 side        = mpScene -> GetViewSide();
    int                 move_by_x   = aX - mLastMousePos.x();
    int                 move_by_y   = aY - mLastMousePos.y();

    if ( abs( move_by_x ) > abs( move_by_y ) )
    {
        if ( side % 2 == 0 )
            rot_plane = Game :: PLANE_XY;
        else
            rot_plane = Game :: PLANE_ZY;

        if ( ( side == 2 ) || ( side == 3 ) )
            inverse = -1;

        if ( move_by_x * inverse > 0 )
            mpGame-> Rotate( rot_plane, Game :: ROTATE_BY_CLOCK_WISE );
        else
            mpGame-> Rotate( rot_plane, Game :: ROTATE_BY_ANTI_CLOCKWISE );
    }
    else
    {
        if ( side % 2 == 0 )
            rot_plane = Game :: PLANE_ZY;
        else
            rot_plane = Game :: PLANE_XY;

        if ( ( side == 0 ) || ( side == 3 ) )
            inverse = -1;

        if ( move_by_y * inverse > 0)
            mpGame-> Rotate( rot_plane, Game :: ROTATE_BY_CLOCK_WISE );
	else
            mpGame-> Rotate( rot_plane, Game :: ROTATE_BY_ANTI_CLOCKWISE );
    }
}

