#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGLWidget>
#include "Game.h"

const int    WindowWidth = 600;
const int   WindowHeight = 600;

class MainWindow : public QGLWidget
{
public:
    MainWindow( QWidget* pwgt = 0 );
    ~MainWindow();
protected:
    void initializeGL();
    void resizeGL( int new_width, int new_height );
    void paintGL();
    void keyPressEvent( QKeyEvent* key );
    void mousePressEvent( QMouseEvent* mouse );
    void mouseMoveEvent( QMouseEvent* mouse );
private:
    Game* game;
};

#endif // MAINWINDOW_H
