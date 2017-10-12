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
** File Name: mainwindow.h
**
** Header file for mainwindow.h - Veelinx software
**
** Author: Michael W. Hoag
** Copyright Michael W. Hoag 2017
** Email: mike@ndtjames.com
** -------------------------------------------------------------------------*/
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore/QtGlobal>
#include <QIODevice>
#include <QMainWindow>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include <QLocale>
#include <QtSerialPort/QSerialPort>
#include <QSerialPortInfo>
#include <QTranslator>
#include <QTimer>
#include <QList>
#include <QProcess>

#include <vector>

#include "console.h"
#include "control_dialog.h"
#include "dataplot.h"
#include "inst_data.h"
#include "parser.h"
#include "splash.h"

//test Regular Operation
#define TEST_REG

//test Graph
//#define TEST_GRAPH

//test Control Dialog one Dialog at a time
#ifndef TEST_GRAPH
//test Example File
//#define TEST_EX
#endif

//message codes from uC five bytes
//header msg data1 data2 footer

// Remote control protocol msg. codes
#define MSG_CODE_PULSE_FREQ             0x00
#define MSG_CODE_PULSE_CYCLE_TIME       0x01
#define MSG_CODE_SAVE_TEST              0x02
#define MSG_CODE_DISPLAY_SIG            0x03
#define MSG_CODE_MSMT_MODE              0x04
#define MSG_CODE_DISTANCE               0x00
#define MSG_CODE_VELOCITY               0x01
#define MSG_CODE_RUN_A_TEST             0x07
#define MSG_CODE_AMP_GAIN               0x10
#define MSG_CODE_PICTURE_RATE           0x11
#define MSG_CODE_PULSER_VOLTAGE         0x12
#define MSG_CODE_WAVE_TYPE              0x20
#define MSG_CODE_DENSITY                0x21
#define MSG_CODE_E_MU_CALC_METHOD       0x22
#define MSG_CODE_REVIEW_TEST_NUM        0x30
#define MSG_CODE_INITIAL_SETTINGS       0x40
#define MSG_CODE_WATCHDOG_RESET         0x50
#define MSG_CODE_FILL                   0xFF

// Remote control protocol miscellaneous definitions
#define REMOTE_CTRL_HEADER              0x5A
#define REMOTE_CTRL_FOOTER              0xA3
#define REMOTE_CTRL_MSG_SETS            2
#define REMOTE_CTRL_MSG_SIZE            5
#define SERIAL_PORT_ERROR               0xAA
#define SERIAL_PORT_OK                  0x55

QT_BEGIN_NAMESPACE

namespace Ui {
class MainWindow;
}

QT_END_NAMESPACE

class Console;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QLocale CurrentLocale;
    QString saveFileName;

protected:

   void closeEvent(QCloseEvent *) Q_DECL_OVERRIDE;

private slots:
    void about();
    void changeEvent(QEvent *e);
    void displayData();
    void closeSerialPort();
    void copy();
    void endUpload();
    void handleError(QSerialPort::SerialPortError error);
    void help();
    void lngDeutche();
    void lngEnglish();
    void lngEspanol();
    void LNGLoadTranslator();
    void loadExampleFile();
    void openFile();
    void openSerialPort();
    void processSerialPort();
    void readData();
    bool saveAs();
    void showSplash();
    void showControl();

private:

    Control_Dialog* CD;
    QTimer* connectTimer;
    Console *console;
    QByteArray Data;
    bool DataUpload;
//    bool ControlDialogData;
    bool foundSerialPort;
    DataPlot *GraphData;
    QSerialPort *serial;
    SplashDialog* Splash;
    QTimer *serialTimeOut;
    QTranslator* Translator;
    Ui::MainWindow *ui;

    bool checkSerialPort();
    bool GetCurrentSettings();
    void ProgramAmpGain();
    void ProgramCalc();
    void ProgramCaptureRate();
    void ProgramDataSave();
    void ProgramEMethod();
    void ProgramPicSave();
    void ProgramPulseRate();
    void ProgramVolt();
    void ProgramWaveType();

    bool sendVmeterMsg( QByteArray msg);
    void initActionsConnections();
    QString resultsFormat( Parser &r, std::vector<DataSet::Test>::iterator itr);
    bool saveFile(const QString &fileName);

    static const QString helpString(void){ return("hh.exe aggralinx.chm");};
};
#endif // MAINWINDOW_H
