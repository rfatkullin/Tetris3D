#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QAction>
#include <QTime>
#include <QMenu>
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
    Q_OBJECT
public :
			    MainWindow();
protected :
    void		    closeEvent( QCloseEvent *event );
private slots :
    void		    NewGame();
    void		    Exit();
private:
    static const int        ViewCnt = 4;
    static const int        ButtonsCnt = 4;
    static const int	    NativeKeyEsc = 27;
    static FigureShift      msFigureControl[ ViewCnt ][ ButtonsCnt ];
    enum		    WindowMinSize{ MIN_WIDTH  = 800,
                                           MIN_HEIGHT = 625 };

    enum                    ControlButtons{ S_BUTTON = 0,
                                            W_BUTTON = 1,
                                            A_BUTTON = 2,
                                            D_BUTTON = 3 };

    static void             SetFigurShiftConstants();
    void		    CreateScene();
    void		    CreateActions();
    void		    CreateMenus();    
    void		    SelectRotate( int aX, int aY );

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

    QAction*                mpNewGameAction;
    QAction*                mpExitGameAction;
    QAction*                mpTrun3dAction;
    QAction*                mpSelectFiguresAction;

    QAction*                mpChangePresentMusicAction;
    QAction*                mpChangePresentSoundsAction;

    SelectFiguresDialog*    mpSelectFiguresDialog;
    GameOverDialog*         mpGameOverDialog;
    Qt :: MouseButton	    mLastMouseButton;
    QPoint		    mLastMousePos;
    bool		    mIsRightButtonPressed;

    bool                    mIsFullScreen;
public slots :
    void		    SelectFigures();
};


#endif // MAINWINDOW_H
