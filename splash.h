/* ---------------------------------------------------------------------------
** This software is furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** File Name: settingsdialog.cpp
**
** splash.cpp - Veelinx software
**
** Author: Michael W. Hoag
** Copyright Michael W. Hoag 2017
** Email: mike@ndtjames.com
** -------------------------------------------------------------------------*/

#ifndef SPLASH_H
#define SPLASH_H

#include <QDialog>
#include <QTimer>

namespace Ui {
class SplashDialog;
}

class SplashDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SplashDialog(QWidget *parent = 0);
    ~SplashDialog();

private:
    Ui::SplashDialog *ui;

private slots:
    void closeSplash();
};

#endif // SPLASH_H
