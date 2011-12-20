#ifndef TOPVIEWDIALOG_H
#define TOPVIEWDIALOG_H

#include <QDialog>

class TopViewDialog : public QDialog
{
    Q_OBJECT

private :
    enum DialogSize{ DIALOG_WIDTH  = 300,
                     DIALOG_HEIGHT = 200
                   };
public :
    typedef std :: pair< QString, int > PlayerInfo;
    typedef std :: list< PlayerInfo >   PlayerInfoList;

    TopViewDialog( int aX, int aY, const PlayerInfoList& aTopList, int aMaxListLength,  QWidget* apParent );

};

#endif // TOPVIEWDIALOG_H
