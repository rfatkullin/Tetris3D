#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QAction>
#include <QTime>
#include <QMenu>
#include <QFile>
#include <QTextStream>
#include "Scene.h"
#include "SelectFiguresDialog.h"
#include "GameOverDialog.h"

struct FigureShift
{
public :
                            FigureShift(){}
                            FigureShift( Game :: Axises aAxis, Game :: ShiftDirection aDirection ) :
                                                    mAxis( aAxis ), mDirection( aDirection ) {}
    Game :: Axises          mAxis;
    Game :: ShiftDirection  mDirection;

};

class MainWindow : public QMainWindow
{
    typedef std :: pair< QString, int > PlayerInfo;
    typedef std :: list< PlayerInfo >   PlayerInfoList;

    Q_OBJECT
public :
                                MainWindow();
                                ~MainWindow();
protected :
    void                        closeEvent( QCloseEvent *event );
private slots :
    void                        NewGame();
    void                        Exit();
    void                        ViewTop();
    void                        ViewControl();
    void                        ViewAbout();
private:
    static const int            VIEW_CNT = 4;
    static const int            BUTTONS_CNT = 4;
    static const int            PLAYER_NAME_MAX_LENGTH = 16;
    static const int            TOP_LIST_LENGTH = 100;
    static const char* const    TOP_LIST_FILE;
    static const char* const    SAVE_FILE;
    static FigureShift          msFigureControl[ VIEW_CNT ][ BUTTONS_CNT ];
    enum                        WindowMinSize{ MIN_WIDTH  = 800,
                                           MIN_HEIGHT = 625 };

    enum                        ControlButtons{ S_BUTTON = 0,
                                            W_BUTTON = 1,
                                            A_BUTTON = 2,
                                            D_BUTTON = 3 };

    static void             SetFigurShiftConstants();
    void		    CreateScene();
    void		    CreateActions();
    void		    CreateMenus();    
    void		    SelectRotate( int aX, int aY );
    void                    ReadTop();
    void                    ChangeTop();

    void		    keyPressEvent( QKeyEvent* key );
    void		    mousePressEvent( QMouseEvent* mouse );
    void		    mouseReleaseEvent( QMouseEvent* mouse );
    void		    mouseMoveEvent( QMouseEvent* mouse );
    void                    wheelEvent ( QWheelEvent * aEvent );
    void		    timerEvent( QTimerEvent * );
    void		    resizeEvent( int new_width, int new_height );

    Scene*                  mpScene;
    Game*                   mpGame;
    bool                    mIsGame;

    QMenu*                  mpMainMenu;
    QMenu*                  mpSettingsMenu;
    QMenu*                  mpFiguresMenu;
    QMenu*                  mpHelpMenu;

    QAction*                mpNewGameAction;
    QAction*                mpExitGameAction;
    QAction*                mpTrun3dAction;
    QAction*                mpSelectFiguresAction;
    QAction*                mpChangePresentMusicAction;
    QAction*                mpChangePresentSoundsAction;
    QAction*                mpViewTopAction;
    QAction*                mpControlAction;
    QAction*                mpAboutAction;

    SelectFiguresDialog*    mpSelectFiguresDialog;
    GameOverDialog*         mpGameOverDialog;
    Qt :: MouseButton	    mLastMouseButton;
    QPoint		    mLastMousePos;
    bool		    mIsRightButtonPressed;

    QFile*                  mpTopFile;
    QTextStream             mTopStream;
    PlayerInfoList          mTopList;

    bool                    mIsFullScreen;
public slots :
    void		    SelectFigures();
};


#endif // MAINWINDOW_H
