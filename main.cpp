#include <QApplication>
#include <QMainWindow>
#include <QtGui>
#include "MainWindow.h"

int main( int argc, char **argv )
{
    QApplication app( argc, argv );

    MainWindow* p_main_window = new MainWindow();

    p_main_window -> show();

    return app.exec();
}
