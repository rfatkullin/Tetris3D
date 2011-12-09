#ifndef GAMEOVERDIALOG_H
#define GAMEOVERDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QCheckBox>
#include <QPushButton>
#include <QLineEdit>

class GameOverDialog : public QDialog
{
    Q_OBJECT
public :
                        GameOverDialog( int aX, int aY, int aMaxPlayerNameLength, int aScore, QWidget* parent = 0 );
    QString             GetPlayerName();
private :
    QLabel*             mpGameOverLabel;
    QLabel*             mpScoresTextLabel;
    QLabel*             mpScoresScoreLabel;
    QLineEdit*          mpPlayerName;

    QPushButton*        mpOkButton;
};

#endif // GAMEOVERDIALOG_H
