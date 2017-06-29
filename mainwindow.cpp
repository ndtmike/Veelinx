/****************************************************************************
**
** Copyright (C) 2012 Denis Shienkov <denis.shienkov@gmail.com>
** Copyright (C) 2012 Laszlo Papp <lpapp@kde.org>
** Contact: http://www.qt.io/licensing/
**
** This file is part of the QtSerialPort module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL21$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see http://www.qt.io/terms-conditions. For further
** information use the contact form at http://www.qt.io/contact-us.
**
** GNU Lesser General Public License Usage
** Alternatively, this file may be used under the terms of the GNU Lesser
** General Public License version 2.1 or version 3 as published by the Free
** Software Foundation and appearing in the file LICENSE.LGPLv21 and
** LICENSE.LGPLv3 included in the packaging of this file. Please review the
** following information to ensure the GNU Lesser General Public License
** requirements will be met: https://www.gnu.org/licenses/lgpl.html and
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** As a special exception, The Qt Company gives you certain additional
** rights. These rights are described in The Qt Company LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** $QT_END_LICENSE$
**
****************************************************************************/
/* ---------------------------------------------------------------------------
** This software is furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** File Name: mainwindow.cpp
**
** mainwindow.cpp - Veelinx software
**
** Author: Michael W. Hoag
** Copyright Michael W. Hoag 2017
** Email: mike@ndtjames.com
** -------------------------------------------------------------------------*/
#include "mainwindow.h"
#include "ui_mainwindow.h"


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    console = new Console;
    setCentralWidget(console);

    serial = new QSerialPort(this);
    serialTimeOut = new QTimer(this);

    GraphData = new DataPlot(this);

    CurrentLocale = QLocale::system(); //standardized number strings
    QString systemlocale = CurrentLocale.name();
    CurrentLocale = QLocale(systemlocale);
    QLocale::setDefault(CurrentLocale);
    Translator = new QTranslator(this);

    initActionsConnections();
    saveFileName = "";

    connect(serial, SIGNAL(error(QSerialPort::SerialPortError)), this,
            SLOT(handleError(QSerialPort::SerialPortError)));
    connect(serial, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(serialTimeOut,SIGNAL(timeout()), this,SLOT(endUpload()));

    DataUpload = false;

#ifndef QT_DEBUG
    QTimer* init_timer = new QTimer(this);
    init_timer->singleShot(100, this, SLOT(showSplash()));
#else
#ifdef TEST_REG
    QTimer* init_timer = new QTimer(this);
    init_timer->singleShot(100, this, SLOT(showSplash()));
#else
    QTimer* init_timer = new QTimer(this);
    init_timer->singleShot(20, this, SLOT(loadExampleFile()));
#endif
#endif
    connectTimer = new QTimer(this);
}

MainWindow::~MainWindow()
{
    delete GraphData;
    closeSerialPort();
    delete connectTimer;
    delete ui;
    delete console;
    delete serial;
}

void MainWindow::about()
{
    QString s;
    QTextStream toabout(&s);

    toabout << tr("The <b>Veelinx Software</b> is used with the <br>") <<
               tr("<b><i>James Instruments Inc.</i></b> V-Meter MK IV.<br><br>")<<
               tr("USA: +1773.4636565<br>")<<
               tr("Europe: +31.548.659032<br>")<<
               tr("Email: <a href=\"mailto:info@ndtjames.com?Subject=Windsorlinx\" target=\"_top\">info@ndtjames.com</a><br>")<<
               tr("Web: <a href=\"http://www.ndtjames.com\">http://www.ndtjames.com</a><br>")<<
               tr("Copyright 2017<br>")<<
               tr("Veelinx is based in part on the work of the <a href=\"http://qwt.sf.net\"> Qwt project (http://qwt.sf.net)");

    QMessageBox::information(this, tr("About Windsorlinx"), s);
}

/*
 * changes text of Menu System
*/

void MainWindow::changeEvent(QEvent *e)
{
    if(e->type() == QEvent::LanguageChange){
        ui->retranslateUi(this);
    }else{
        QMainWindow::changeEvent(e);
    }
}

void MainWindow::closeEvent (QCloseEvent* /*event*/)
{
    GraphData->close();
}


bool MainWindow::checkSerialPort()
{
    QString description;
    QString manufacturer;
    QString portname;

    CurrentLocale = QLocale::system(); //standardized number strings
    QString systemlocale = CurrentLocale.name();
    CurrentLocale = QLocale(systemlocale);
    QLocale::setDefault(CurrentLocale);
    Translator = new QTranslator(this);

    const QString portmanufacturer = "FTDI";
    const QString noport = tr("No Available Ports") + '\n'
                              + tr("Check instrument is plugged in") + '\n'
                              + tr("or serial port installed properly") + '\n'
                              + tr("then restart Windsorlinx");
    const QString messageTitle = tr("Check Serial Port");
    const QString connected = tr("Connected to ");
    QList <QSerialPortInfo> availablePorts;
    bool r = false;
    availablePorts = QSerialPortInfo::availablePorts();

    if(!availablePorts.isEmpty()){
        foreach (const QSerialPortInfo &info, QSerialPortInfo::availablePorts())
        {
            portname = info.portName();
            description = info.description();
            manufacturer = info.manufacturer();
            if(manufacturer == portmanufacturer){
                serial->setPortName(portname);
                r = true;
            }
        if( r == true )break;
        }
    }
    if(r == false){
        QMessageBox::information(this,messageTitle,noport);
    }else{
        QMessageBox::information(this ,messageTitle , connected + portname );
    }
    return(r);
}



void MainWindow::cleanData()//main function that takes raw data and transforms to usable
{
    QString buffer = "";
    QTextStream display( &buffer );
    QStringList datalist;

    Parser p( this, Data );

    qint64 i = 0;

    for( std::vector<DataSet::Test>::iterator itr = p.Data->GetBeginItr();
         itr != p.Data->GetEndItr();++i,++itr ){

        QLocale::setDefault(CurrentLocale);
        LNGLoadTranslator();

        display << "James Instruments Inc. V-Meter MK IV Veelinx 1.0"<< '\n'
                << tr("Test Number: ")<< i+1 <<'\t'
                << tr("Test Date/Time: ")
                << p.ToQDateTime(itr).toString(CurrentLocale.timeFormat(QLocale::ShortFormat))<<' '
                << p.ToQDateTime(itr).toString(CurrentLocale.dateFormat(QLocale::ShortFormat))/*("MM/dd/yyyy hh:mm")*/ <<'\n'
                << tr("Transit Time: ")<< p.ToQStrTransitTime(itr) << ' ' << QChar(0xB5) << 's' << '\n'
                << tr("Capture Rate: ")<< p.ToQStrRate(itr) << '\t'
                << tr("Amplifier Gain ") << p.ToQStrAmpGain(itr) << '\t'
                << tr("Density: ") << p.ToQStrDensity(itr) << '\n'
                << tr("Wave Type: ")<< p.ToQStrWave(itr) << '\n'
                << tr("Calculate: ") << p.ToQStrCalc(itr) << '\t'
                << tr("E Method: ")<< p.ToQStrEMethod(itr) << '\t'
                << '\n'<<'\n'
                << p.ToQSLADC(itr).join('\n');

        GraphData->SetData(p.ToQPFADC(itr));
    }
    console->setPlainText( buffer );
    DataUpload = true;
}

void MainWindow::copy()
{
    console->selectAll();
    console->copy();
}

void MainWindow::closeSerialPort()
{
    if (serial->isOpen())
        serial->close();
    console->setEnabled(false);
    ui->statusBar->showMessage(tr("Disconnected"));
}

void MainWindow::endUpload()
{
    serialTimeOut->stop();

    if(DataUpload == false){
        QMessageBox::information(this, "endUpload", tr("Upload Complete"));
#ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
        cleanData();
#ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
#endif
    }else{
        QMessageBox::information(this, "endUpload", tr("Restart Program Before Uploading More Data"));
    }

    ui->actionSaveAs->setEnabled(true);
    ui->action_Open->setEnabled(false);
    ui->actionPlot->setEnabled(true);
}

void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        closeSerialPort();
    }
}

void MainWindow::help()
{
    QProcess* help = new QProcess(this);
    help->start("hh.exe Veelinx.chm");
}

void MainWindow::initActionsConnections()
{
    ui->actionQuit->setEnabled(true);
    ui->actionPlot->setEnabled(false);
    ui->actionSaveAs->setEnabled(false);
    ui->action_Open->setEnabled(true);

    connect(ui->actionQuit, SIGNAL(triggered()), this, SLOT(close()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));
    connect(ui->actionSaveAs, SIGNAL(triggered()), this, SLOT(saveAs()));
    connect(ui->actionCopy, SIGNAL(triggered()), this, SLOT(copy()));
    connect(ui->actionHelp, SIGNAL(triggered()), this, SLOT(help()));
    connect(ui->action_Open, SIGNAL(triggered()), this, SLOT(openFile()));
    connect(ui->actionPlot, SIGNAL(triggered()), this, SLOT(cleanData()));
    connect(ui->actionDeutche, SIGNAL(triggered()), this, SLOT(lngDeutche()));
    connect(ui->actionEnglish, SIGNAL(triggered()), this, SLOT(lngEnglish()));
    connect(ui->actionEspanol, SIGNAL(triggered()), this, SLOT(lngEspanol()));
    connect(ui->actionControl, SIGNAL(triggered()), this, SLOT(showControl()));
}

/*
 * sets translator object to German
 */
void MainWindow::lngDeutche()
{
    CurrentLocale = QLocale(QLocale::German);
    QLocale::setDefault(CurrentLocale);
    LNGLoadTranslator();
}
/*
 * sets translator object to English
 */
void MainWindow::lngEnglish()
{
    CurrentLocale = QLocale(QLocale::English);
    QLocale::setDefault(CurrentLocale);
    LNGLoadTranslator();
}
/*
 * sets translator object to Spanish
 */
void MainWindow::lngEspanol()
{
    CurrentLocale = QLocale(QLocale::Spanish);
    QLocale::setDefault(CurrentLocale);
    LNGLoadTranslator();
}
/*
 *loads translator  object
*/
void MainWindow::LNGLoadTranslator()
{
    if(Translator->isEmpty()){
            Translator->load(CurrentLocale.language(), "Internationalization","_");
            qApp->installTranslator(Translator);
    }else{
        qApp->removeTranslator(Translator);
        Translator->load(CurrentLocale.language(), "Internationalization","_");
        qApp->installTranslator(Translator);
    }
}
void MainWindow::loadExampleFile()
{
    saveFileName = "C:/Users/Mike/Documents/Projects/Veelinx/ExampleFile/ReviewDataSet-2013_04_02 15_43_03.txt";
    QFile file(saveFileName);
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream load(&file);
    QString buffer = load.readAll();
    Data.append( buffer );
    cleanData(); // loads data
    file.close();

#ifdef TEST_GRAPH
    GraphData->show();
#endif
#ifdef TEST_CD
    showControl();
#endif
}

void MainWindow::openFile()
{
    QString fileName = "";
    fileName = QFileDialog::getOpenFileName(this,
        tr("Open Text File"), "", tr("Text FIles (*.txt)"));

    if(fileName !=""){ //check if cancel pressed
        saveFileName = fileName;
        QFile file(saveFileName);
        file.open(QFile::ReadOnly | QFile::Text);
        QTextStream load(&file);
#ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
        console->setPlainText(load.readAll());
#ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
#endif
        file.close();
        ui->actionSaveAs->setEnabled(true);
        ui->action_Open->setEnabled(false);
        ui->actionPlot->setEnabled(true);
    }
}

void MainWindow::openSerialPort()
{
    serial->setBaudRate(9600);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    if (serial->open(QIODevice::ReadWrite
                     )) {
            console->setEnabled(true);
            console->setLocalEchoEnabled( true);
            ui->statusBar->showMessage(tr("Connected"));
    } else {
        QMessageBox::critical(this, tr("Error"), serial->errorString());

        ui->statusBar->showMessage(tr("Open error"));
    }
}

void MainWindow::processSerialPort()
{

    foundSerialPort = checkSerialPort();
    if(foundSerialPort){
        openSerialPort();
        connectTimer->start(500);
    }
}

void MainWindow::readData()
{
    connectTimer->stop();
    serialTimeOut->start(500);
    Data += serial->readAll();
    console->putData(serial->readAll());
}

QString MainWindow::resultsFormat( Parser &r, std::vector<DataSet::Test>::iterator itr ){
    QString buffer;
    QTextStream display( &buffer );

    if(itr->TestProp.PropUnits == DataSet::Metric){

    }else{

    }
    return (buffer);
}

/*void MainWindow::save()
{
    if(saveFileName != ""){
        saveFile(saveFileName);
        QMessageBox::information(this,"Windsorlinx",tr("File: ")+
                                 '\n' + saveFileName +
                                 '\n' + tr("Saved Successfully"));
    }else{
        saveAs();
    }
}
*/
bool MainWindow::saveAs()
{
    QFileDialog dialog(this);
    dialog.setWindowModality(Qt::WindowModal);
    dialog.setAcceptMode(QFileDialog::AcceptSave);
    dialog.setNameFilter(tr("Text (*.txt)"));
    QStringList files;
    if (dialog.exec())
        files = dialog.selectedFiles();
    else
        return false;
    saveFileName = files.at(0);
    return saveFile(files.at(0));
}

bool MainWindow::saveFile(const QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::WriteOnly | QFile::Text)) {
        QMessageBox::warning(this, "MainWindow",
                             tr("Cannot write file %1:\n%2.")
                             .arg(fileName)
                             .arg(file.errorString()));
        return false;
    }
    QTextStream out(&file);
#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    out << console->toPlainText();
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
    return true;
}

void MainWindow::showControl()
{
    QString AmpGainTest = "";
    CD = new Control_Dialog(this);
    CD->setModal( true );
    if(CD->exec() == QDialog::Accepted){
        QMessageBox::information(this,"showControl", "Accepted",QMessageBox::Ok);
        CD->Return_Control_Dialog();
    }
}

void MainWindow::showSplash()
{
    const int five_sec = 5000;

    SplashDialog* splash = new SplashDialog();
    splash->setModal( true );
    splash->show();

    QTimer* splash_timer = new QTimer(this);
    splash_timer->singleShot(five_sec, this, SLOT(processSerialPort()));
}
