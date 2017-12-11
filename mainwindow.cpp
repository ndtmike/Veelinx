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
    CD = new Control_Dialog(this);
    Splash = new SplashDialog();

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
//    ControlDialogData = false; //is V-Meter on?

#ifndef QT_DEBUG
    QTimer* init_timer = new QTimer(this);
    init_timer->singleShot(1, this, SLOT(showSplash())); //delay before splash screen
#endif
#ifdef TEST_REG
    QTimer* init_timer = new QTimer(this);
    init_timer->singleShot(1, this, SLOT(showSplash()));  //delay before splash screen
#endif
#ifdef TEST_EX
    QTimer* init_timer = new QTimer(this);
    init_timer->singleShot(1, this, SLOT(loadExampleFile()));

#endif
}

MainWindow::~MainWindow()
{
    delete GraphData;
    closeSerialPort();
    delete ui;
    delete console;
    delete serial;
}

/******************************************************************************

  Function: about()

  Description:
  ============
  'About' software window

******************************************************************************/
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

/******************************************************************************

  Function: changeEvent(QEvent *e)

  Description:
  ============
  Changes text of menu system when a new language is selected

******************************************************************************/

void MainWindow::changeEvent(QEvent *e)
{
    if(e->type() == QEvent::LanguageChange){
        ui->retranslateUi(this);
    }else{
        QMainWindow::changeEvent(e);
    }
}

/******************************************************************************

  Function: closeEvent(QCloseEvent*)

  Description:
  ============
  Actions that should occur when the window closes

******************************************************************************/
void MainWindow::closeEvent (QCloseEvent* /*event*/)
{
    GraphData->close();
}

/******************************************************************************

  Function: checkSerialPort()

  Description:
  ============
  Checks that an instrument is connected to the PC

******************************************************************************/
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
                              + tr("then restart Veelinx");
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

/******************************************************************************

  Function: cleanData()

  Description:
  ============
  cleans recieved data parses it and prints it to the screen

******************************************************************************/

void MainWindow::displayData()//main function that takes raw data and transforms to usable
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

    GraphData->show();

    console->setPlainText( buffer );
    DataUpload = true;
}

/******************************************************************************

  Function: copy()

  Description:
  ============
  Copies data in ASCII form to the clipboard
******************************************************************************/
void MainWindow::copy()
{
    console->selectAll();
    console->copy();
}

/******************************************************************************

  Function: closeSerialPort()

  Description:
  ============
  Closes serial port
******************************************************************************/
void MainWindow::closeSerialPort()
{
    if (serial->isOpen())
        serial->close();
    console->setEnabled(false);
    ui->statusBar->showMessage(tr("Disconnected"));
}

/******************************************************************************

  Function: endUpload

  Description:
  ============
  function called when all data has been uploaded to the PC.  I.E. When the
    upload timer times out

******************************************************************************/
void MainWindow::endUpload()
{
   QByteArray header;
    QByteArray init;

    init.append('Z'); //set to check init bytes
    init.append('@');
    header = Data.left(2);
    serialTimeOut->stop();
    if( DataUpload == false ){
#ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
        if( header == init ){
            if(!CD->Set_Control_Dialog( Data )){
                QMessageBox::warning(this, "endUpload", tr("Check V-Meter Connected to PC and Turned on!"));
                close();
            }
            Data = ""; //empty array
            DataUpload = false;
        }else if( header[0] == init[0] ){ //confirming change
        }else if(Data != "" ){        //uploading data
            QMessageBox::information(this, "endUpload", tr("Upload Complete"));
            displayData();
            ui->actionSaveAs->setEnabled(true);
            ui->action_Open->setEnabled(false);
            ui->actionPlot->setEnabled(true);
        }
#ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
#endif
    }else if( DataUpload == true ){
        QMessageBox::information(this, "endUpload", tr("Restart Program Before Uploading More Data"));
        close();
    }
}

/******************************************************************************

  Function: GetCurrentSettings()

  Description:
  ============
  Return QByteArray of CurrentSettings from V-Meter Serial Port
******************************************************************************/
bool MainWindow::GetCurrentSettings()
{
    bool returnbool = false;
    QByteArray initmsg;
    initmsg.resize( REMOTE_CTRL_MSG_SIZE );
    initmsg[0] = REMOTE_CTRL_HEADER;
    initmsg[1] = MSG_CODE_INITIAL_SETTINGS;
    initmsg[2] = MSG_CODE_FILL;
    initmsg[3] = MSG_CODE_FILL;
    initmsg[4] = REMOTE_CTRL_FOOTER;

    qint64 bytesWritten = serial->write(initmsg);

    if (bytesWritten == -1) {
        QMessageBox::information(this,"GetCurrentSettings", tr("Failed to write the data to port %1, error: %2")
                             .arg(serial->portName(), serial->errorString()));
    } else if (bytesWritten != initmsg.size()) {
        QMessageBox::information(this,"GetCurrentSettings", tr("Failed to write all the data to port %1, error: %2")
                             .arg(serial->portName(), serial->errorString()));
    } else if (!serial->waitForBytesWritten(5000)) {
        QMessageBox::information(this,"GetCurrentSettings", tr("Operation timed out or an error occurred for port %1, error: %2")
                             .arg(serial->portName(), serial->errorString()));
    } else{
#ifdef QT_DEBUG
//        QMessageBox::information(this,"GetCurrentSettings", tr("Data successfully sent to port %1").arg(serial->portName()));
#endif
        returnbool = true;
    }

    return( returnbool );
}

/******************************************************************************

  Function: handleError(QSerialPort::SerialPortError error)

  Description:
  ============
  Handles unexpect serial port errors

******************************************************************************/
void MainWindow::handleError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QMessageBox::critical(this, tr("Critical Error"), serial->errorString());
        closeSerialPort();
    }
}

/******************************************************************************

  Function: help()

  Description:
  ============
  Launches appropriate help file

******************************************************************************/
void MainWindow::help()
{
    QProcess* help = new QProcess(this);
    help->start("hh.exe Veelinx.chm");
}


/******************************************************************************

  Function: initActionsConnections()

  Description:
  ============
  This routine initializes the cycle time between pulse sequences

 Init all the connection for the main window page

******************************************************************************/
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
    connect(ui->actionPlot, SIGNAL(triggered()), this, SLOT(displayData()));

    connect(ui->actionDeutche, SIGNAL(triggered()), this, SLOT(lngDeutche()));
    connect(ui->actionEnglish, SIGNAL(triggered()), this, SLOT(lngEnglish()));
    connect(ui->actionEspanol, SIGNAL(triggered()), this, SLOT(lngEspanol()));
    connect(ui->actionControl, SIGNAL(triggered()), this, SLOT(showControl()));
}

/******************************************************************************

  Function: lngDeutche()

  Description:
  ============
  Sets Translator object to German

******************************************************************************/
void MainWindow::lngDeutche()
{
    CurrentLocale = QLocale(QLocale::German);
    QLocale::setDefault(CurrentLocale);
    LNGLoadTranslator();
}

/******************************************************************************

  Function: lngEnglish()

  Description:
  ============
  Sets Translator object to English

******************************************************************************/
void MainWindow::lngEnglish()
{
    CurrentLocale = QLocale(QLocale::English);
    QLocale::setDefault(CurrentLocale);
    LNGLoadTranslator();
}

/******************************************************************************

  Function: lngSpanish()

  Description:
  ============
  Sets Translator object to Spanish

******************************************************************************/
void MainWindow::lngEspanol()
{
    CurrentLocale = QLocale(QLocale::Spanish);
    QLocale::setDefault(CurrentLocale);
    LNGLoadTranslator();
}

/******************************************************************************

  Function: LNGLoadTranslator()

  Description:
  ============
  Load Translator Object

******************************************************************************/
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

/******************************************************************************

  Function: loadExampleFile()

  Description:
  ============
  For testing purposes loads an example file.

******************************************************************************/
void MainWindow::loadExampleFile()
{
    saveFileName = "C:/Users/Mike/Documents/Projects/Veelinx/ExampleFile/ReviewDataSet-2013_04_02 15_43_03.txt";
    QFile file(saveFileName);
    file.open(QFile::ReadOnly | QFile::Text);
    QTextStream load(&file);
    QString buffer = load.readAll();
    Data.append( buffer );
    displayData(); // loads data
    file.close();

#ifdef TEST_GRAPH
    GraphData->show();
#endif
#ifdef TEST_CD
    showControl();
#endif
}

/******************************************************************************

  Function: openFile()

  Description:
  ============
  Opens a file fo ASCII Data

******************************************************************************/
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

/******************************************************************************

  Function: openSerialPort()

  Description:
  ============
  Opens Serial Port for Communication

******************************************************************************/
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

/******************************************************************************

  Function: processSerialPort()

  Description:
  ============
  Opens Serial Port for Communication and starts upload timer

******************************************************************************/
void MainWindow::processSerialPort()
{
    foundSerialPort = checkSerialPort();
    if(foundSerialPort){
        openSerialPort();       
    }
}

/******************************************************************************

  Function:ProgramAmpGain()

  Description:
  ============

******************************************************************************/
void MainWindow::ProgramAmpGain(){
    const QByteArray no_change = "no change";
    QByteArray msg = no_change;

    if(CD->CheckAmpGain == true ){
#ifdef QT_DEBUG
        QMessageBox::information(this, "showControl", "AmpGain Changed", QMessageBox::Ok);
#endif
        msg = CD->BufferAmpGain;
    }
    if(msg != no_change){
        if( sendVmeterMsg( msg )){
#ifdef QT_DEBUG
//                  QMessageBox::information(this,"showControl", "Accepted AmpGain Changed",QMessageBox::Ok);
#endif
        }else{
            QMessageBox::information(this,"showControl", "Problem Programming AmpGain",QMessageBox::Ok);
        }
    }
    CD->CheckAmpGain = false;
}

/******************************************************************************

  Function:ProgramCalc()

  Description:
  ============

******************************************************************************/
void MainWindow::ProgramCalc(){
    const QByteArray no_change = "no change";
    QByteArray msg = no_change;

    if( CD->CheckCalc == true ){
#ifdef QT_DEBUG
        QMessageBox::information(this, "showControl", "Calc Changed", QMessageBox::Ok);
#endif
        msg = CD->BufferCalc;
    }
    if( msg != no_change ){
        if( sendVmeterMsg( msg )){
#ifdef QT_DEBUG
//                  QMessageBox::information(this,"showControl", "Accepted Calc Changed",QMessageBox::Ok);
#endif
        }else{
            QMessageBox::information(this,"showControl", "Problem Programming Calc",QMessageBox::Ok);
        }
    }
    CD->CheckCalc = false;
}

/******************************************************************************

  Function:ProgramCaptureRate()

  Description:
  ============

******************************************************************************/
void MainWindow::ProgramCaptureRate(){
    const QByteArray no_change = "no change";
    QByteArray msg = no_change;

    if( CD->CheckCaptureRate == true ){
#ifdef QT_DEBUG
        QMessageBox::information(this, "showControl", "CaptureRate Changed", QMessageBox::Ok);
#endif
        msg = CD->BufferCaptureRate;
    }
    if( msg != no_change ){
        if( sendVmeterMsg( msg )){
#ifdef QT_DEBUG
            QMessageBox::information(this,"showControl", "Accepted CaptureRate Changed",QMessageBox::Ok);
#endif
        }else{
            QMessageBox::information(this,"showControl", "Problem Programming CaptureRate",QMessageBox::Ok);
        }
    }
    CD->CheckCaptureRate = false;
}

/******************************************************************************

  Function:ProgramDataSave()

  Description:
  ============

******************************************************************************/
void MainWindow::ProgramDataSave(){
    const QByteArray no_change = "no change";
    QByteArray msg = no_change;

    if( CD->CheckDataSave == true ){
#ifdef QT_DEBUG
        QMessageBox::information(this, "showControl", "DataSave Changed", QMessageBox::Ok);
#endif
        msg = CD->BufferDataSave;
    }
    if( msg != no_change ){
        if( sendVmeterMsg( msg )){
#ifdef QT_DEBUG
            QMessageBox::information(this,"showControl", "Accepted DataSave Changed",QMessageBox::Ok);
#endif
        }else{
            QMessageBox::information(this,"showControl", "Problem Programming DataSave",QMessageBox::Ok);
        }
    }
    CD->CheckDataSave = false;
}

/******************************************************************************

  Function:ProgramDensity()

  Description:
  ============

******************************************************************************/
void MainWindow::ProgramDensity(){
    const QByteArray no_change = "no change";
    QByteArray msg = no_change;

    if( CD->CheckDensity == true ){
#ifdef QT_DEBUG
        QMessageBox::information(this, "showControl", "Density Changed", QMessageBox::Ok);
#endif
        msg = CD->BufferDensity;
    }
    if( msg != no_change ){
        if( sendVmeterMsg( msg )){
#ifdef QT_DEBUG
            QMessageBox::information(this,"showControl", "Accepted Density Changed",QMessageBox::Ok);
#endif
        }else{
            QMessageBox::information(this,"showControl", "Problem Programming Density",QMessageBox::Ok);
        }
    }
    CD->CheckDensity = false;
}

/******************************************************************************

  Function:ProgramDistance()

  Description:
  ============

******************************************************************************/
void MainWindow::ProgramDistance(){
    const QByteArray no_change = "no change";
    QByteArray msg = no_change;

    if( CD->CheckDistance == true ){
#ifdef QT_DEBUG
        QMessageBox::information(this, "showControl", "Distance Changed", QMessageBox::Ok);
#endif
        msg = CD->BufferDistance;
    }
    if( msg != no_change ){
        if( sendVmeterMsg( msg )){
#ifdef QT_DEBUG
            QMessageBox::information(this,"showControl", "Accepted Distance Changed",QMessageBox::Ok);
#endif
        }else{
            QMessageBox::information(this,"showControl", "Problem Programming Distance",QMessageBox::Ok);
        }
    }
    CD->CheckDistance = false;
}

/******************************************************************************

  Function:ProgramEMethod()

  Description:
  ============


******************************************************************************/
void MainWindow::ProgramEMethod(){
    const QByteArray no_change = "no change";
    QByteArray msg = no_change;

    if( CD->CheckEMethod == true ){
#ifdef QT_DEBUG
        QMessageBox::information(this, "showControl", "EMethod Changed", QMessageBox::Ok);
#endif
        msg = CD->BufferEMethod;
    }
    if( msg != no_change ){
        if( sendVmeterMsg( msg )){
#ifdef QT_DEBUG
            QMessageBox::information(this,"showControl", "Accepted EMethod Changed",QMessageBox::Ok);
#endif
        }else{
            QMessageBox::information(this,"showControl", "Problem Programming EMethod",QMessageBox::Ok);
        }
    }
    CD->CheckEMethod = false;
}

/******************************************************************************

  Function:ProgramPicSave()

  Description:
  ============

******************************************************************************/
void MainWindow::ProgramPicSave(){
    const QByteArray no_change = "no change";
    QByteArray msg = no_change;

    if( CD->CheckPicSave == true ){
#ifdef QT_DEBUG
        QMessageBox::information( this, "showControl", "PicSave Changed", QMessageBox::Ok );
#endif
        msg = CD->BufferPicSave;
    }
    if( msg != no_change ){
        if( sendVmeterMsg( msg )){
#ifdef QT_DEBUG
            QMessageBox::information(this,"showControl", "Accepted PicSave Changed",QMessageBox::Ok);
#endif
        }else{
            QMessageBox::information(this,"showControl", "Problem Programming PicSave",QMessageBox::Ok);
        }
    }
    CD->CheckPicSave = false;
}

/******************************************************************************

  Function:ProgramPulseRate()

  Description:
  ============

******************************************************************************/
void MainWindow::ProgramPulseRate(){
    const QByteArray no_change = "no change";
    QByteArray msg = no_change;

    if( CD->CheckPulseRate == true ){
#ifdef QT_DEBUG
        QMessageBox::information( this, "showControl", "PulseRate Changed", QMessageBox::Ok );
#endif
        msg = CD->BufferPulseRate;
    }
    if( msg != no_change ){
        if( sendVmeterMsg( msg )){
#ifdef QT_DEBUG
            QMessageBox::information(this,"showControl", "Accepted PulseRate Changed",QMessageBox::Ok);
#endif
        }else{
            QMessageBox::information(this,"showControl", "Problem Programming PulseRate",QMessageBox::Ok);
        }
    }
    CD->CheckPulseRate = false;
}

/******************************************************************************

  Function:ProgramRun()

  Description:
  ============

******************************************************************************/
void MainWindow::ProgramRun(){
    const QByteArray no_change = "no change";
    QByteArray msg = no_change;

    if( CD->CheckRun == true ){
#ifdef QT_DEBUG
        QMessageBox::information( this, "showControl", "Run Changed", QMessageBox::Ok );
#endif
        msg = CD->BufferRun;
    }
    if( msg != no_change ){
        if( sendVmeterMsg( msg )){
#ifdef QT_DEBUG
            QMessageBox::information(this,"showControl", "Accepted Run Changed",QMessageBox::Ok);
#endif
        }else{
//            QMessageBox::information(this,"showControl", "Problem Programming Run",QMessageBox::Ok);
        }
    }
    CD->CheckRun = false;
}

/******************************************************************************

  Function:ProgramVelocity()

  Description:
  ============

******************************************************************************/
void MainWindow::ProgramVelocity(){
    const QByteArray no_change = "no change";
    QByteArray msg = no_change;

    if( CD->CheckVelocity == true ){
#ifdef QT_DEBUG
        QMessageBox::information(this, "showControl", "Velocity Changed", QMessageBox::Ok);
#endif
        msg = CD->BufferVelocity;
    }
    if( msg != no_change ){
        if( sendVmeterMsg( msg )){
#ifdef QT_DEBUG
            QMessageBox::information(this,"showControl", "Accepted Velocity Changed",QMessageBox::Ok);
#endif
        }else{
            QMessageBox::information(this,"showControl", "Problem Programming Velocity"
                                                         "",QMessageBox::Ok);
        }
    }
    CD->CheckVelocity = false;
}

/******************************************************************************

  Function:ProgramVolt()

  Description:
  ============

******************************************************************************/
void MainWindow::ProgramVolt(){
    const QByteArray no_change = "no change";
    QByteArray msg = no_change;

    if( CD->CheckVolt == true ){
#ifdef QT_DEBUG
        QMessageBox::information( this, "showControl", "Volt Changed", QMessageBox::Ok );
#endif
        msg = CD->BufferVolt;
    }
    if( msg != no_change ){
        if( sendVmeterMsg( msg )){
#ifdef QT_DEBUG
            QMessageBox::information(this,"showControl", "Accepted Volt Changed",QMessageBox::Ok);
#endif
        }else{
            QMessageBox::information(this,"showControl", "Problem Programming Volt",QMessageBox::Ok);
        }
    }
    CD->CheckVolt = false;
}

/******************************************************************************

  Function:ProgramWaveType()

  Description:
  ============
    Can load P wave from S but not S from p :-(

******************************************************************************/
void MainWindow::ProgramWaveType(){
    const QByteArray no_change = "no change";
    QByteArray msg = no_change;

    QByteArray bedermu;
    bedermu.resize( REMOTE_CTRL_MSG_SIZE );
    bedermu[0] = REMOTE_CTRL_HEADER; bedermu[1] =  MSG_CODE_E_MU_CALC_METHOD;
    bedermu[2] = CALC_METHOD_DERIVED_MU;
    bedermu[3] = MSG_CODE_FILL; bedermu[4] = REMOTE_CTRL_FOOTER;

    if( CD->CheckWaveType == true ){
#ifdef QT_DEBUG
        QMessageBox::information( this, "ProgramWaveType", "WaveType Changed", QMessageBox::Ok );
#endif
        msg = CD->BufferWaveType;
    }

    if( msg != no_change ){

    if( sendVmeterMsg( msg )){
    #ifdef QT_DEBUG
            QMessageBox::information(this,"ProgramWaveType", "Accepted  Changed",QMessageBox::Ok);
    #endif
        }else{
            QMessageBox::information(this,"ProgramWaveType", "Problem Programming WaveType",QMessageBox::Ok);
        }
/*               for( int i = 1; i <= 170; ++i){
                    msg[2] = i;
                    if(sendVmeterMsg( msg ) == true){
                        QMessageBox::information(this,"ProgramWaveType",QString("Success: %1").arg(i,0,16), QMessageBox::Ok);
                        break;
                    }
                    if( i == 170 ){
                        QMessageBox::information(this,"ProgramWaveType",QString("Fail: %1").arg(i), QMessageBox::Ok);
                    }
                }*/
        if( sendVmeterMsg( bedermu )){
#ifdef QT_DEBUG
            QMessageBox::information(this,"ProgramWaveType", "Accepted EType Changed",QMessageBox::Ok);
#endif
        }else{
            QMessageBox::information(this,"ProgramWaveType", "Problem Programming EType",QMessageBox::Ok);
        }
    }
    CD->CheckWaveType = false;
}

/******************************************************************************

  Function: readData()

  Description:
  ============
  Takes data from serial port and adds to console buffer
******************************************************************************/

void MainWindow::readData()
{
    serialTimeOut->start(500);
    Data += serial->readAll();
}


/******************************************************************************

  Function: saveAs()

  Description:
  ============
  Save data to a file, get file name with dialog
******************************************************************************/
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

/******************************************************************************

  Function: saveFile()

  Description:
  ============
  Save as given filename
******************************************************************************/

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

/******************************************************************************

  Function: sendVmeterMsg( QByteArray msg )

  Description:
  ============
  Return QByteArray of CurrentSettings from V-Meter Serial Port

******************************************************************************/
bool MainWindow::sendVmeterMsg( QByteArray msg )
{
    bool return_result = false;
    Data = "";
    if( serial->clear()){
        for(int i = 0; i< msg.size(); ++i){
            serial->putChar(msg[i]);
        }
    }

    QTime dieTime = QTime::currentTime().addSecs(2); // wait one second
    while (QTime::currentTime() < dieTime)
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);

    QByteArray confirm_msg = Data;

    if( confirm_msg[0] == 'Z'  ) return_result = true;

    return(return_result);
}

/******************************************************************************

  Function: showControl()

  Description:
  ============
  Show V-Meter control dialog

******************************************************************************/
void MainWindow::showControl()
{
    bool current_settings;
    current_settings = GetCurrentSettings();

    serialTimeOut->stop();

    if( current_settings == true ){        DataSet::Prop proptest;
        CD->setModal( true );
        if( CD->exec() == QDialog::Accepted ){
            if( CD->CheckFormChange == true ){
                proptest = CD->Return_Control_Dialog();
                ProgramAmpGain();
                ProgramCalc();
                ProgramCaptureRate();
                ProgramDataSave();
                ProgramDensity();
                ProgramDistance();
                ProgramEMethod();
                ProgramPicSave();
                ProgramPulseRate();
                ProgramRun();
                ProgramVelocity();
                ProgramVolt();
                ProgramWaveType();
                CD->CheckFormChange = false;
            }
        }
    }else{
        QMessageBox::information( this, "showControl","Check V-Meter Connected and Turned On!");
    }

    serial->clear();
    serialTimeOut->start(500);
    CD->CheckFormChange = false;
}

/******************************************************************************

  Function: showSplash

  Description:
  ============
  Show splash screen

******************************************************************************/
void MainWindow::showSplash()
{
    const int five_sec = 5000;//need to change dialog here and in splash

//    SplashDialog* splash = new SplashDialog();
    Splash->setModal( true );
    Splash->show();
#ifdef QT_DEBUG
    QTimer* init_timer = new QTimer(this);
    init_timer->singleShot( 500, this, SLOT(processSerialPort( )));
#else
    QTimer* init_timer = new QTimer(this);
    init_timer->singleShot(five_sec, this, SLOT(processSerialPort()));
#endif
}
