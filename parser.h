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

#include <ctime>
#include <math.h>
#include <vector>

#include <QtCore>
#include <QDate>
#include <QDebug>
#include <QDateTime>
#include <QFile>
#include <QFileDialog>
#include <QList>
#include <QMessageBox>
#include <QPolygonF>
#include <QString>
#include <QTextStream>
#include <QVector>



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

    DataSet::Test CreateTest( QStringList sl);

    QPolygonF ToQPFADC(std::vector<DataSet::Test>::iterator itr_test);
    QStringList ToQSLADC( std::vector<DataSet::Test>::iterator itr_test );
    QString ToQStrAmpGain(std::vector<DataSet::Test>::iterator itr_test);
    QString ToQStrCalc(std::vector<DataSet::Test>::iterator itr_test);
    QDateTime ToQDateTime(std::vector<DataSet::Test>::iterator itr_test);
    QString ToQStrDensity(std::vector<DataSet::Test>::iterator itr_test);
    QString ToQStrEMethod(std::vector<DataSet::Test>::iterator itr_test);
    QString ToQStrRate(std::vector<DataSet::Test>::iterator itr_test);
    QString ToQStrTransitTime(std::vector<DataSet::Test>::iterator itr_test);
    QString ToQStrWave(std::vector<DataSet::Test>::iterator itr_test);

    DataSet* Data;

private:
    static qint64 DataSetSize(void){return(16);}

    std::vector<long> CreateADC( QStringList sl );
    DataSet::Prop CreateProp(QStringList sl);
    qint64 HexQByteArraytoInt(QByteArray &in);
    QByteArray RemoveAscii(QByteArray &in);

    DataSet::AmpGain QStringtoAmpGain(QString in);
    DataSet::Calc QStringtoCalc(QString in);
    tm QStringtoDateTime(QString in);
    unsigned QStringtoDensity(QString in);
    DataSet::EMethod QStringtoEMethod(QString in);
    DataSet::Rate QStringtoRate(QString in);
//    DataSet::Units QStringtoUnits(QString in);
    DataSet::Wave QStringtoWave(QString in);
};

#endif // PARSER_H
