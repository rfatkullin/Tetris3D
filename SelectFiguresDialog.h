#ifndef SelectFiguresDialog_H
#define SelectFiguresDialog_H

#include <QDialog>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>

class SelectFiguresDialog : public QDialog
{
    Q_OBJECT
public :
                        SelectFiguresDialog( QWidget* apParent = 0 );
    void                SetSelectFigures( bool* apSelectedFigures );
    void                GetSelectFigures( bool* apSelectedFigures );
    void                SetPosition( int aX, int aY );
private :
    static const int    mcFiguresCnt = 7;

    QCheckBox*          mpFigures[ mcFiguresCnt ];
    QPushButton*        mpOkButton;
    QPushButton*        mpCancelButton;
};

#endif // SelectFiguresDialog_H
