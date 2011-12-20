#include "AboutDialog.h"

AboutDialog :: AboutDialog( QWidget* apParent ) : QDialog( apParent )
{
    QPushButton* ok_button          = new QPushButton( "OK" );
    QVBoxLayout* ver_layout         = new QVBoxLayout();
    QHBoxLayout* first_hor_layout   = new QHBoxLayout();
    QHBoxLayout* second_hor_layout  = new QHBoxLayout();
    QHBoxLayout* third_hor_layout   = new QHBoxLayout();
    QLabel*      game_name_label    = new QLabel( "Tetris3D v1.0" );
    QLabel*      my_name_label      = new QLabel( "(C) 2011 Fatkullin Rustam" );

    connect( ok_button, SIGNAL( clicked() ), this, SLOT( accept() ) );

    first_hor_layout -> addStretch();
    first_hor_layout -> addWidget( game_name_label );
    first_hor_layout -> addStretch();

    second_hor_layout -> addStretch();
    second_hor_layout -> addWidget( my_name_label );
    second_hor_layout -> addStretch();

    third_hor_layout -> addStretch();
    third_hor_layout -> addWidget( ok_button );


    ver_layout -> addLayout( first_hor_layout );
    ver_layout -> addLayout( second_hor_layout );
    ver_layout -> addLayout( third_hor_layout );

    setLayout( ver_layout );
    setFixedSize( sizeHint() );

    setWindowFlags( Qt::Dialog | Qt::WindowTitleHint );

}

void AboutDialog :: Show()
{
    QPoint parent_widget_pos  = parentWidget() -> pos();
    QSize  parent_widget_size = parentWidget() -> size();

    move( parent_widget_pos.x() + parent_widget_size.width() / 2 - width() / 2,
          parent_widget_pos.y() + parent_widget_size.height() / 2 - height() / 2 );
    exec();
}
