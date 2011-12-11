#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QPushButton>
#include <QDialog>
#include <QLabel>
#include <QLayout>

class AboutDialog : public QDialog
{
public:
            AboutDialog( QWidget* parent );
    void    Show();
};

#endif // ABOUTDIALOG_H
