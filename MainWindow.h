#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QAction>
#include <QMenu>
#include "Scene.h"

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
    void resizeEvent( int new_width, int new_height );

    QMenu*   main_menu;
    QMenu*   settings_menu;
    QMenu*   figures_menu;
    Scene*   scene;
    QAction* new_game_action;
    QAction* exit_game_action;
    QAction* turn_3d_action;
    QAction* with_i_action;
    QAction* with_j_action;
    QAction* with_l_action;
    QAction* with_o_action;
    QAction* with_s_action;
    QAction* with_t_action;
    QAction* with_z_action;
};


#endif // MAINWINDOW_H
