#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QGLWidget>

class MainWindow : public QGLWidget
{
public:
    MainWindow( QWidget* pwgt = 0 );
protected:
    void initializeGL();
    void resizeGL( int new_width, int new_height );
    void paintGL();
};

#endif // MAINWINDOW_H
