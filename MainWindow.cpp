#include <QApplication>
#include <QDesktopWidget>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QStyle>
#include <Qt>
#include <QTime>
#include <QWindowsStyle>
#include <stdio.h>
#include <time.h>
#include "MainWindow.h"
#include "TopViewDialog.h"
#include <stdio.h>

FigureShift MainWindow :: msFigureControl[ MainWindow :: VIEW_CNT ][ MainWindow :: BUTTONS_CNT ];
const char* const       MainWindow :: TOP_LIST_FILE = "top.dat";

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
    mpAboutDialog           = new AboutDialog( this );
    mpControlDialog         = new ControlDialog( this );
    mpGameOverDialog        = NULL;
    mIsRightButtonPressed   = false;
    mIsGame                 = false;
    mIsFullScreen           = false;
    mIsPause                = false;

    CreateScene();
    CreateActions();
    CreateMenus();

    SetFigurShiftConstants();

    move( 0, 0 );
    setMinimumSize( MIN_WIDTH, MIN_HEIGHT );
    setCentralWidget( mpScene );
    setWindowIcon( QIcon( "Img/Icon.jpg" ) );

    mpTopFile = new QFile( TOP_LIST_FILE, this );
    mTopStream.setDevice( mpTopFile );

    ReadTop();

    startTimer( 10 );
}

MainWindow :: ~MainWindow()
{

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

    mpViewTopAction = new QAction( "Top", this );
    connect( mpViewTopAction,  SIGNAL( triggered() ), SLOT( ViewTop() ) );

    mpControlAction = new QAction( "Control", this );
    connect( mpControlAction,  SIGNAL( triggered() ), SLOT( ViewControl() ) );

    mpAboutAction = new QAction( "About Tetris3D", this );
    connect( mpAboutAction,  SIGNAL( triggered() ), SLOT( ViewAbout() ) );

    mpSaveAction = new QAction( "Save game", this );
    connect( mpSaveAction, SIGNAL( triggered() ), this, SLOT( Save() ) );

    mpLoadAction = new QAction( "Load game", this );
    connect( mpLoadAction, SIGNAL( triggered() ), this, SLOT( Load() ) );
}

void MainWindow :: CreateMenus()
{
    mpMainMenu = menuBar() -> addMenu( "Game" );
    mpMainMenu -> addAction( mpNewGameAction );
    mpMainMenu -> addAction( mpSaveAction );
    mpMainMenu -> addAction( mpLoadAction );
    mpMainMenu -> addAction( mpExitGameAction );

    mpSettingsMenu = menuBar() -> addMenu( "Settings" );  
    mpSettingsMenu -> addAction( mpSelectFiguresAction );
    mpSettingsMenu -> addAction( mpTrun3dAction );
    mpSettingsMenu -> addAction( mpChangePresentMusicAction );
    mpSettingsMenu -> addAction( mpChangePresentSoundsAction );

    mpHelpMenu = menuBar() -> addMenu( "Help" );
    mpHelpMenu -> addAction( mpViewTopAction );
    mpHelpMenu -> addAction( mpControlAction );
    mpHelpMenu -> addAction( mpAboutAction );

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
    mpGame -> End();
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
        case Qt :: Key_Escape :
            Exit();
            break;

        case Qt :: Key_F1 :
            mpScene -> ChangeShowHelp();
            break;

        case Qt :: Key_F2 :
            NewGame();
            break;

         case Qt :: Key_F3 :
            mIsFullScreen = !mIsFullScreen;
            this -> menuBar() -> setVisible( !mIsFullScreen );

            if ( mIsFullScreen )
            {
               this -> setCursor( Qt :: BlankCursor );
               this -> showFullScreen();
            }
            else
            {
                this -> showNormal();
                this -> setCursor( Qt :: ArrowCursor );
            }
            break;

        case Qt :: Key_F6 :
            Load();
            break;

        default:
            break;
    }

    if ( !mIsGame )
        return;

    if ( key -> key() == Qt :: Key_P )
        mIsPause = !mIsPause;

    if ( mIsPause )
        return;

    switch ( key -> key() )
    {
        case Qt :: Key_F4 :
            mpGame -> AmbientMusicStateChange( !mpGame -> AmbientMusicState() );
            break;

        case Qt :: Key_F5 :
            Save();
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
    //if ( !mIsGame )
      //  return;

    Qt :: MouseButton button = mouse -> button();

    if ( ( button == Qt :: LeftButton ) || ( button == Qt :: RightButton )  )
	mLastMousePos = mouse -> globalPos();

    if ( button == Qt :: RightButton )
	mIsRightButtonPressed = true;

    mLastMouseButton = button;
}

void MainWindow :: mouseReleaseEvent( QMouseEvent* mouse )
{
    if ( !mIsGame || mIsPause  )
        return;

    if ( ( !mIsPause ) && ( mLastMouseButton == Qt :: RightButton ) )
        SelectRotate( mouse -> globalX(), mouse -> globalY() );
}

void MainWindow :: wheelEvent ( QWheelEvent * aEvent )
{
    if ( !mIsGame || mIsPause )
        return;

    if ( aEvent -> delta() > 0 )
        mpGame-> Rotate( Game :: PLANE_ZX, Game :: ROTATE_BY_CLOCK_WISE );
    else
        mpGame-> Rotate( Game :: PLANE_ZX, Game :: ROTATE_BY_ANTI_CLOCKWISE );
}

void MainWindow :: mouseMoveEvent( QMouseEvent* mouse )
{
    if ( mLastMouseButton != Qt :: LeftButton )
	return;

    mpScene -> ChangeCameraPosition( mouse -> globalX() - mLastMousePos.x(),
                          mouse -> globalY() - mLastMousePos.y() );
    mLastMousePos = mouse -> globalPos();
}

void MainWindow :: resizeEvent ( QResizeEvent * aEvent )
{
    QSize win_size = aEvent -> size();

    if ( ( !mIsFullScreen ) && ( ( win_size.width() != MIN_WIDTH ) || ( win_size.height() != MIN_HEIGHT ) ) )
        resize( MIN_WIDTH, MIN_HEIGHT );

    mpScene -> Resize( win_size.width(), win_size.height() );
}

void MainWindow :: timerEvent( QTimerEvent * )
{
    if ( mpGame -> IsGameOver() )
    {
        mpGameOverDialog = new GameOverDialog( pos().x() + width() / 2, pos().y() + height() / 2, PLAYER_NAME_MAX_LENGTH, mpGame -> GetScore() );
        mpGameOverDialog -> exec();
        ChangeTop();
        delete mpGameOverDialog;
        mIsGame = false;
    }

   if ( mIsGame && ( !mIsPause ) )
        mpGame-> NextStep();

    mpScene -> paintGL();

if ( mIsGame && ( !mIsPause ) )
        mpGame -> ClearMessagesList();
}

void MainWindow :: SelectFigures()
{
    bool  game_state = false;
    bool* select_figures = new bool[ Game :: FIGURES_MAX_CNT ];

    mpGame -> GetSelectFigures( select_figures );
    mpSelectFiguresDialog -> SetSelectFigures( select_figures );
    mpSelectFiguresDialog -> SetPosition( pos().x() + width() / 2, pos().y() + height() / 2 );

    game_state = mIsGame;
    mIsGame    = false;

    if ( mpSelectFiguresDialog -> exec() == QDialog :: Accepted )
    {
        mpSelectFiguresDialog -> GetSelectFigures( select_figures );
        mpGame -> SetSelectFigures( select_figures );
    }

    mIsGame = game_state;

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

void MainWindow :: ReadTop()
{
    QString player_name;
    QString player_score_str;
    bool    to_int_ok           = false;
    int     player_score;
    int     top_list_length     = 0;

    if ( !mpTopFile -> open( QIODevice :: ReadOnly ) )
        return;

    while ( ( !mTopStream.atEnd() ) && ( top_list_length <= TOP_LIST_LENGTH ) )
    {
        mTopStream >> player_name >> player_score_str;

        player_score = player_score_str.toInt( &to_int_ok );

        if ( ( player_name.length() > PLAYER_NAME_MAX_LENGTH ) || ( !to_int_ok ) )
            continue;

        mTopList.push_back( PlayerInfo( player_name, player_score ) );
        top_list_length++;
    }

    mpTopFile -> close();
}

void MainWindow :: ChangeTop()
{
    PlayerInfo player_info( mpGameOverDialog -> GetPlayerName(), mpGame -> GetScore() );
    bool            is_insert = false;

    for ( PlayerInfoList :: iterator it = mTopList.begin(); it != mTopList.end(); ++it )
    {
        if ( it -> second < player_info.second )
        {
            mTopList.insert( it, player_info );
            is_insert = true;
            break;
        }
    }

    if ( !is_insert )
         mTopList.push_back( player_info );

    if ( mTopList.size() > TOP_LIST_LENGTH )
        mTopList.pop_back();

    mpTopFile -> open( QIODevice :: WriteOnly );

    for ( PlayerInfoList :: iterator it = mTopList.begin(); it != mTopList.end(); ++it )
        mTopStream << ( it -> first.toStdString().c_str() ) << '\t' << it -> second << '\n';

    mpTopFile -> close();
}

void MainWindow :: ViewTop()
{
    bool game_state = false;

    TopViewDialog* top_view_dialog;

    game_state = mIsGame;
    mIsGame = false;

    top_view_dialog = new TopViewDialog( pos().x() + width() / 2, pos().y() + height() / 2, mTopList, TOP_LIST_LENGTH, this );

    top_view_dialog -> exec();

    delete top_view_dialog;
    mIsGame = game_state;
}

void MainWindow :: ViewControl()
{
    bool game_state = false;

    game_state = mIsGame;
    mIsGame = false;

    mpControlDialog -> Show();

    mIsGame = game_state;
}

void MainWindow :: ViewAbout()
{
    bool game_state = false;

    game_state = mIsGame;
    mIsGame = false;

    mpAboutDialog -> Show();

    mIsGame = game_state;
}

void MainWindow :: Save()
{
    if ( ( mIsGame ) && ( !mIsPause ) )
        mpGame -> Save();
}

void MainWindow :: Load()
{
    mpGame -> Load();
    mIsGame = true;
}
