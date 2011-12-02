#include <QLayout>
#include "GameOverDialog.h"

GameOverDialog :: GameOverDialog( int aX, int aY, int aScore, QWidget* parent ) : QDialog( parent )
{
    QString         score;
    score.setNum( aScore );
    mpGameOverLabel = new QLabel( "Game over !!! ", this );
    mpScoresLabel   = new QLabel(  QString( "Score                                 ") + score, this );
    mpOkButton      = new QPushButton( "Ok", this );
    connect( mpOkButton, SIGNAL( clicked() ), this, SLOT( accept() ) );

    QVBoxLayout* v_layout        = new QVBoxLayout();
    QHBoxLayout* first_h_layout  = new QHBoxLayout();
    QHBoxLayout* second_h_layout = new QHBoxLayout();
    QHBoxLayout* third_h_layout  = new QHBoxLayout();

    first_h_layout -> addStretch();
    first_h_layout -> addWidget( mpGameOverLabel );
    first_h_layout -> addStretch();

    second_h_layout -> addWidget( mpScoresLabel );

    third_h_layout -> addStretch();
    third_h_layout -> addWidget( mpOkButton );

    v_layout -> addLayout( first_h_layout );
    v_layout -> addLayout( second_h_layout );
    v_layout -> addLayout( third_h_layout );

    setLayout( v_layout );
    setWindowTitle( "Bye)" );
    move( aX, aY );
    setFixedSize( sizeHint() );
}
