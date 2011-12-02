#include <QLayout>
#include "SelectFiguresDialog.h"

SelectFiguresDialog :: SelectFiguresDialog( QWidget* parent ) : QDialog( parent )
{
    QVBoxLayout* p_figures_layout   = new QVBoxLayout();
    QVBoxLayout* p_ok_cancel_layout = new QVBoxLayout();
    QHBoxLayout* p_common_layout    = new QHBoxLayout();

    mpFigures[ 0 ] = new QCheckBox( "I figure" );
    mpFigures[ 1 ] = new QCheckBox( "J figure" );
    mpFigures[ 2 ] = new QCheckBox( "L figure" );
    mpFigures[ 3 ] = new QCheckBox( "O figure" );
    mpFigures[ 4 ] = new QCheckBox( "S figure" );
    mpFigures[ 5 ] = new QCheckBox( "T figure" );
    mpFigures[ 6 ] = new QCheckBox( "Z figure" );

    for ( int i = 0; i < mcFiguresCnt; i++ )
    {
        mpFigures[ i ] -> setChecked( true );
        p_figures_layout -> addWidget( mpFigures[ i ] );
    }

    mpOkButton = new QPushButton( "OK", this );
    connect( mpOkButton, SIGNAL( clicked() ), this, SLOT( accept() ) );

    mpCancelButton = new QPushButton( "Cancel", this );
    connect( mpCancelButton, SIGNAL( clicked() ), this, SLOT( reject() ) );

    p_ok_cancel_layout -> addWidget( mpOkButton );
    p_ok_cancel_layout -> addWidget( mpCancelButton );
    p_ok_cancel_layout -> addStretch();

    p_common_layout -> addLayout( p_figures_layout );
    p_common_layout -> addLayout( p_ok_cancel_layout );

    setLayout( p_common_layout );
    setWindowTitle( "Figures" );
    setFixedSize( sizeHint() );
}

void SelectFiguresDialog :: SetSelectFigures( bool* selected_figures )
{
    for ( int i = 0; i < mcFiguresCnt; i++ )
        if ( selected_figures[ i ] )
            mpFigures[ i ] -> setChecked( true );
        else
            mpFigures[ i ] -> setChecked( false );
}

void SelectFiguresDialog :: GetSelectFigures( bool* select_figures )
{
    for ( int i = 0; i < mcFiguresCnt; i++ )
        if ( mpFigures[ i ] -> isChecked() )
            select_figures[ i ] = true;
        else
            select_figures[ i ] = false;
}
