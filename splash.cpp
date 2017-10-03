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

#include "splash.h"
#include "ui_splash.h"

SplashDialog::SplashDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SplashDialog)
{
#ifdef QT_DEBUG
    const int five_sec = 500;
#else
    const int five_sec = 5000;  //need to change both 5 sec here and in "mainwindow"
#endif

    ui->setupUi(this);
    QTimer* init_timer = new QTimer(this);
    connect(init_timer, SIGNAL(timeout()), this, SLOT(update()));
    init_timer->singleShot(five_sec, this, SLOT(closeSplash()));
}

SplashDialog::~SplashDialog()
{
    delete ui;
}

void SplashDialog::closeSplash()
{
    close();
}
