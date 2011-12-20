#include "ControlDialog.h"


 ControlDialog :: ControlDialog( QWidget* apParent ) : QDialog( apParent )
 {
    QVBoxLayout*    layout = new QVBoxLayout();
    QTextBrowser*   help = new QTextBrowser();

    setFixedSize( DIALOG_WIDTH, DIALOG_HEIGHT );
    help -> setSource( QString( "Help.html" ) );
    help -> setAlignment( Qt :: AlignCenter );

    layout -> addWidget( help );
    layout -> setAlignment( Qt :: AlignCenter );

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
