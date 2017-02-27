/* ---------------------------------------------------------------------------
** This software is furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** File Name: parser.h
**
** Header file for parser.cpp - Veelinx software
**
** Author: Michael W. Hoag
** Copyright Michael W. Hoag 2017
** Email: mike@ndtjames.com
** -------------------------------------------------------------------------*/

#ifndef PARSER_H
#define PARSER_H

#include <math.h>
#include <ctime>
#include <QString>
#include <QFile>
#include <QList>
#include <QTextStream>
#include <QMessageBox>
#include <QFileDialog>
#include <QDateTime>
#include <QVector>
#include <QtCore>
#include <QDebug>
#include <QDate>

#include <vector>

#include "Inst_Data.h"

/*
 *
 * Handles the interface between the DataSet class and Qt
 *
 */
class Parser:QWidget
{
    Q_OBJECT
public:
    Parser( QWidget* parent = 0 , const QByteArray &in = "");
    ~Parser();

    std::vector<DataSet::Test>::iterator GetBeginItr();
    std::vector<DataSet::Test>::iterator GetEndItr();

    DataSet::Test CreateTest( QByteArray array);

    QString ToQStrAmpGain(std::vector<DataSet::Test>::iterator itr_test);
    QString ToQStrCalc(std::vector<DataSet::Test>::iterator itr_test);
    QDateTime ToQDateTime(std::vector<DataSet::Test>::iterator itr_test);
    QString ToQStrEMethod(std::vector<DataSet::Test>::iterator itr_test);
    QString ToQStrPulse(std::vector<DataSet::Test>::iterator itr_test);
    QString ToQStrRate(std::vector<DataSet::Test>::iterator itr_test);
    QString ToQStrUnits(std::vector<DataSet::Test>::iterator itr_test);
    QString ToQStrWave(std::vector<DataSet::Test>::iterator itr_test);

    DataSet* Data;

private:
    static qint64 DataSetSize(void){return(16);}

    qint64 HexQByteArraytoInt(QByteArray &in);
    QByteArray RemoveAscii(QByteArray &in);

    DataSet::AmpGain QBAtoAmpGain(QByteArray &in);
    DataSet::Calc QBAtoCalc(QByteArray &in);
    tm QBAtoDateTime(QByteArray &in);
    DataSet::EMethod QBAtoEMethod(QByteArray &in);
    DataSet::Pulse QBAtoPulse(QByteArray &in);
    DataSet::Rate QBAtoRate(QByteArray &in);
    DataSet::Units QBAtoUnits(QByteArray &in);
    DataSet::Wave QBAtoWave(QByteArray &in);

    //for Constructor
    static int ADCZeroLength(void){return(1);}
    static int ADCZeroPos(void){return(11);}
    static int ADCFullScaleLength(void){return(1);}
    static int ADCFullScalePos(void){return(12);}
    static int ADCData0Length(void){return(1);}
    static int ADCData0Pos(void){return(13);}
    static int ADCData1Length(void){return(1);}
    static int ADCData1Pos(void){return(14);}
    static int ADCData2Length(void){return(1);}
    static int ADCData2Pos(void){return(15);}
    static int AggSizeLength(void){return(1);}
    static int AggSizePos(void){return(10);}
    static int DensityLength(void){return(1);}
    static int DensityPos(void){return(6);}
    static int HexToDec(void){return(16);}
    static int MohLength(void){return(1);}
    static int MohPos(void){return(8);}
    static int PWRLength(void){return(1);}
    static int PWRPos(void){return(5);}
    static int TimeLength(void){return(5);}
    static int WeightLength(void){return(1);}
    static int WeightPos(void){return(7);}
    static int UnitsLength(void){return(1);}
    static int UnitsPos(void){return(9);}
};

#endif // PARSER_H
