#include <QLayout>
#include <QLabel>
#include <QScrollArea>
#include <QPushButton>
#include <stdio.h>
#include "TopViewDialog.h"


TopViewDialog :: TopViewDialog( int aX, int aY, const PlayerInfoList& aTopList, int aMaxListLength,  QWidget* apParent ) : QDialog( apParent )
{
    QWidget*        tmp_widget             = new QWidget();
    QScrollArea*    scroll_area            = new QScrollArea();
    QHBoxLayout*    hor_layout             = NULL;
    QVBoxLayout*    ver_layout             = new QVBoxLayout();
    QHBoxLayout*    hor_sub_left_layout    = NULL;
    QHBoxLayout*    hor_sub_right_layout   = NULL;
    QVBoxLayout*    gen_layout             = new QVBoxLayout();
    QPushButton*    ok_button              = new QPushButton();
    QString         player_number_str;
    QString         player_score_str;
    int             player_number          = 1;
    int             list_length            = aTopList.size();

    setFixedSize( DIALOG_WIDTH, DIALOG_HEIGHT );
    setWindowTitle( "Tetris3D top players");
    setWindowFlags( Qt::Dialog | Qt::WindowTitleHint );
    move( aX - width() / 2, aY - height() / 2 );


    for ( PlayerInfoList :: const_iterator it = aTopList.begin(); it != aTopList.end(); ++it )
    {
        player_number_str.setNum( player_number );
        player_score_str.setNum( it -> second );

        switch ( player_number_str.length() )
        {
            case 1 :
                player_number_str = "    " + player_number_str;
                break;
            case 2 :
                player_number_str = "  " + player_number_str;
                break;
            default :
                break;
        }

        hor_layout = new QHBoxLayout();
        hor_sub_left_layout = new QHBoxLayout();
        hor_sub_right_layout = new QHBoxLayout();

        hor_sub_left_layout -> addWidget( new QLabel( player_number_str + ") " + it -> first ) );
        hor_sub_right_layout -> addWidget( new QLabel(player_score_str ) );
        hor_layout -> addLayout( hor_sub_left_layout );
        hor_layout -> addStretch();
        hor_layout -> addStretch();
        hor_layout -> addLayout( hor_sub_right_layout );

        ver_layout -> addLayout( hor_layout );
        player_number++;
    }

    player_score_str = " ";
    if ( list_length < aMaxListLength )
        for ( int i = player_number; i <= aMaxListLength; ++i )
        {
            player_number_str.setNum( i );

            switch ( player_number_str.length() )
            {
                case 1 :
                    player_number_str = "    " + player_number_str;
                    break;
                case 2 :
                    player_number_str = "  " + player_number_str;
                    break;
                default :
                    break;
            }

            hor_layout = new QHBoxLayout();
            hor_sub_left_layout = new QHBoxLayout();
            hor_sub_right_layout = new QHBoxLayout();

            hor_sub_left_layout -> addWidget( new QLabel( player_number_str + ") " ) );
            hor_sub_right_layout -> addWidget( new QLabel( player_score_str ) );
            hor_layout -> addLayout( hor_sub_left_layout );
            hor_layout -> addStretch();
            hor_layout -> addLayout( hor_sub_right_layout );

            ver_layout -> addLayout( hor_layout );
        }

    ok_button -> setText( "OK");
    connect( ok_button, SIGNAL( clicked() ), this, SLOT( accept() ) );
    hor_layout = new QHBoxLayout();
    hor_layout -> addStretch();
    hor_layout -> addWidget( ok_button );

    tmp_widget -> setLayout( ver_layout );
    scroll_area -> setWidget( tmp_widget );
    gen_layout -> addWidget( scroll_area );
    gen_layout -> addLayout( hor_layout );
    setLayout( gen_layout );
}
