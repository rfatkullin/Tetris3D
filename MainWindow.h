#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QAction>
#include <QTime>
#include <QMenu>
#include <QFile>
#include <QIcon>
#include <QTextStream>
#include "Scene.h"
#include "SelectFiguresDialog.h"
#include "GameOverDialog.h"
#include "AboutDialog.h"
#include "ControlDialog.h"

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
public slots :
    void		    SelectFigures();

private slots :
    void                        NewGame();
    void                        Exit();
    void                        Save();
    void                        Load();
    void                        ViewTop();
    void                        ViewControl();
    void                        ViewAbout();

protected :
    void                        closeEvent( QCloseEvent *event );

public :
                                MainWindow();
                                ~MainWindow();

private:
    static const int            VIEW_CNT = 4;
    static const int            BUTTONS_CNT = 4;
    static const int            PLAYER_NAME_MAX_LENGTH = 16;
    static const int            TOP_LIST_LENGTH = 100;
    static const char* const    TOP_LIST_FILE;  
    static FigureShift          msFigureControl[ VIEW_CNT ][ BUTTONS_CNT ];

    enum                        WindowMinSize{ MIN_WIDTH  = 800,
                                               MIN_HEIGHT = 625 };

    enum                        ControlButtons{ S_BUTTON = 0,
                                            W_BUTTON = 1,
                                            A_BUTTON = 2,
                                            D_BUTTON = 3 };

    Scene*                      mpScene;
    Game*                       mpGame;
    bool                        mIsGame;
    bool                        mIsPause;

    QMenu*                      mpMainMenu;
    QMenu*                      mpSettingsMenu;
    QMenu*                      mpFiguresMenu;
    QMenu*                      mpHelpMenu;

    QAction*                    mpNewGameAction;
    QAction*                    mpExitGameAction;
    QAction*                    mpTrun3dAction;
    QAction*                    mpSelectFiguresAction;
    QAction*                    mpChangePresentMusicAction;
    QAction*                    mpChangePresentSoundsAction;
    QAction*                    mpViewTopAction;
    QAction*                    mpControlAction;
    QAction*                    mpAboutAction;
    QAction*                    mpSaveAction;
    QAction*                    mpLoadAction;

    SelectFiguresDialog*        mpSelectFiguresDialog;
    GameOverDialog*             mpGameOverDialog;
    AboutDialog*                mpAboutDialog;
    ControlDialog*              mpControlDialog;

    Qt :: MouseButton	        mLastMouseButton;
    QPoint		        mLastMousePos;
    bool		        mIsRightButtonPressed;

    QFile*                      mpTopFile;
    QTextStream                 mTopStream;
    PlayerInfoList              mTopList;

    bool                        mIsFullScreen;

    static void                 SetFigurShiftConstants();
    void                        CreateScene();
    void                        CreateActions();
    void                        CreateMenus();
    void                        SelectRotate( int aX, int aY );
    void                        ReadTop();
    void                        ChangeTop();

    void		        keyPressEvent( QKeyEvent* apKey );
    void		        mousePressEvent( QMouseEvent* apMouse );
    void		        mouseReleaseEvent( QMouseEvent* apMouse );
    void		        mouseMoveEvent( QMouseEvent* apMouse );
    void                        wheelEvent ( QWheelEvent * apEvent );
    void		        timerEvent( QTimerEvent * apEvent );
    void                        resizeEvent ( QResizeEvent * apEvent );

};


#endif // MAINWINDOW_H
