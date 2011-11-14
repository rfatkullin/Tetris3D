#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QAction>
#include <QMenu>
#include "Scene.h"
#include "SelectFiguresDialog.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public :
    MainWindow();
protected :
    void closeEvent( QCloseEvent *event );
private slots :
    void NewGame();
    void Exit();
private:
    enum WindowMinSize{ MIN_WIDTH = 800, MIN_HEIGHT = 600 };
    void CreateScene();
    void CreateActions();
    void CreateMenus();
    void CreateStatusBar();

    void keyPressEvent( QKeyEvent* key );
    void mousePressEvent( QMouseEvent* mouse );
    void mouseMoveEvent( QMouseEvent* mouse );
    void timerEvent( QTimerEvent * );
    void resizeEvent( int new_width, int new_height );

    Scene*                  mpScene;
    Game*                   mpGame;

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
public slots :
    void SelectFigures();
};


#endif // MAINWINDOW_H
