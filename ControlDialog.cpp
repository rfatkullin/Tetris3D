#include "ControlDialog.h"


 ControlDialog :: ControlDialog( QWidget* parent ) : QDialog( parent )
 {
    QVBoxLayout*    layout = new QVBoxLayout();
    QLabel*         help = new QLabel();
    QScrollArea*    scroll_area = new QScrollArea();

    setFixedSize( DIALOG_WIDTH, DIALOG_HEIGHT );

    help -> setText( "<Qt>"
                     "<HTML>"
                     "<BODY>"
                     "<H3 align = 'center' > TETRIS 3D </H3>"
                     "<P> Hello!. This is help by game Tetris3D </P>"
                     "<P> OLOLOLOSHENKI!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11 </P>"
                     "<P> OLOLOLOSHENKI!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11 </P>"
                     "<P> OLOLOLOSHENKI!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11 </P>"
                     "<P> OLOLOLOSHENKI!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11 </P>"
                     "<P> OLOLOLOSHENKI!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11 </P>"
                     "<P> OLOLOLOSHENKI!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11 </P>"
                     "<P> OLOLOLOSHENKI!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11 </P>"
                     "<P> OLOLOLOSHENKI!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11 </P>"
                     "</BODY>"
                     "</HTML>"
                     "</Qt>"
                    );

    scroll_area -> setWidget( help );

    layout -> addWidget( scroll_area );

    setLayout( layout );
}

 void ControlDialog :: Show()
 {
    QPoint parent_widget_pos     = parentWidget() -> pos();
    QSize  parent_widget_size    = parentWidget() -> size();

    move( parent_widget_pos.x() + parent_widget_size.width() / 2 - width() / 2,
          parent_widget_pos.y() + parent_widget_size.height() / 2 - height() / 2 );

    exec();
 }
