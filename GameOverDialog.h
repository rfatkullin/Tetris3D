#ifndef GAMEOVERDIALOG_H
#define GAMEOVERDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>

class GameOverDialog : public QDialog
{
    Q_OBJECT
public :
                        GameOverDialog( QWidget* parent = 0 );
private :
    QLabel*             mpGameOverLabel;
    QLabel*             mpScoresLabel;

    QPushButton*        mpOkButton;
};

#endif // GAMEOVERDIALOG_H
