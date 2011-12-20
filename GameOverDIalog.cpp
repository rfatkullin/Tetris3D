#include <QLayout>
#include <QRegExpValidator>
#include "GameOverDialog.h"

GameOverDialog :: GameOverDialog( int aX, int aY, int aMaxPlayerNameLength, int aScore, QWidget* apParent ) : QDialog( apParent )
{
    QRegExp       player_name_valid( "[A-Za-z]+[_A-Za-z0-9]*" );
    QString       score;
    score.setNum( aScore );

    mpGameOverLabel     = new QLabel( "Enter your name", this );
    mpScoresTextLabel   = new QLabel( "Score", this );
    mpScoresScoreLabel  = new QLabel( score, this );
    mpPlayerName        = new QLineEdit( "PlayerName", this);
    mpOkButton          = new QPushButton( "Ok", this );

    connect( mpOkButton, SIGNAL( clicked() ), this, SLOT( accept() ) );
    mpPlayerName -> setValidator( new QRegExpValidator( player_name_valid, this ) );
    mpPlayerName -> setMaxLength( aMaxPlayerNameLength );

    QVBoxLayout* v_layout        = new QVBoxLayout();
    QHBoxLayout* first_h_layout  = new QHBoxLayout();
    QHBoxLayout* second_h_layout = new QHBoxLayout();
    QHBoxLayout* third_h_layout  = new QHBoxLayout();

    first_h_layout -> addWidget( mpGameOverLabel );
    first_h_layout -> addWidget( mpPlayerName );

    second_h_layout -> addWidget( mpScoresTextLabel );
    second_h_layout -> addStretch();
    second_h_layout -> addWidget( mpScoresScoreLabel );

    third_h_layout -> addStretch();
    third_h_layout -> addWidget( mpOkButton );

    v_layout -> addLayout( first_h_layout );
    v_layout -> addLayout( second_h_layout );
    v_layout -> addLayout( third_h_layout );

    setLayout( v_layout );
    setWindowTitle( "Game over!" );
    move( aX, aY );
    setFixedSize( sizeHint() );

    mpPlayerName -> setFocus();
}

QString GameOverDialog :: GetPlayerName()
{
    return mpPlayerName -> text();
}
