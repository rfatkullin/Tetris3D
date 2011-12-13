#ifndef CONTROLDIALOG_H
#define CONTROLDIALOG_H

#include <QDialog>
#include <QLayout>
#include <QLabel>
#include <QScrollArea>
#include <QTextBrowser>

class ControlDialog : public QDialog
{
private :
    enum DialogSize{ DIALOG_WIDTH  = 500,
                     DIALOG_HEIGHT = 500
                   };
public :
    ControlDialog( QWidget* parent );
    void Show();
};

#endif // CONTROLDIALOG_H
