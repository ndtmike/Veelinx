/* ---------------------------------------------------------------------------
** This software is furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** File Name: parser.cpp
**
** parser.cpp - Veelinx software
** Divides the input stream up into data packets
**
** Author: Michael W. Hoag
** Copyright Michael W. Hoag 2017
** Email: mike@ndtjames.com
** -------------------------------------------------------------------------*/

#include "parser.h"
//#include "datapacket.h"

Parser::Parser( QWidget*, const QByteArray &in ) //no parameter for QWidget Pointer it is not used
{
//    QByteArray copy = in;
    QString strcopy = QString::QString( in );
    QStringList strlistcopy = strcopy.split(QRegExp("[\r\n]"),QString::SkipEmptyParts);

    Data = new DataSet;

    DataSet::Test newtest = CreateTest(strlistcopy);

    Data->AddTest(newtest);
}

Parser::~Parser()
{
}

std::vector<DataSet::Test>::iterator Parser::GetBeginItr()
{
    return(Data->GetBeginItr());
}

std::vector<DataSet::Test>::iterator Parser::GetEndItr()
{
    return(Data->GetEndItr());
}

DataSet::Test Parser::CreateTest(QStringList sl)
{
    DataSet::Test return_test;
    int n;//index

    n = sl.indexOf(QRegExp("Gain:"));
    return_test.TestProp.PropAmpGain = QStringtoAmpGain( sl.at(n));

    n=sl.indexOf(QRegExp("Measured"));
    return_test.TestProp.PropCalc = QStringtoCalc( sl.at(n));

    n = sl.indexOf(QRegExp("Date/Time:"));
    return_test.TestTime = QStringtoDateTime( sl.at(n) );

    n=sl.indexOf(QRegExp("Young's modulus:"));
    return_test.TestProp.PropEMethod = QStringtoEMethod(sl.at(n));

    n=sl.indexOf(QRegExp("feet/second"));
    return_test.TestProp.PropEMethod = QStringtoEMethod(sl.at(n));

    n=sl.indexOf(QRegExp("WAVE TYPE:"));
    return_test.TestProp.PropWave = QStringtoWave(sl.at(n));

    return(return_test);
}

QByteArray Parser::RemoveAscii(QByteArray &in){

    QByteArray out;

    for(qint16 i=0; i<in.size();++i){
        out[i]=in[i]-48;//removes ascii offset
    }
    return(out);
}

qint64 Parser::HexQByteArraytoInt(QByteArray &in)
{
    QByteArray data = in;
    bool ok;
    qint64 out;

    data = data.toHex(); //convert to hex string
    out = data.toInt( &ok, 16 );
    return(out);
}

DataSet::AmpGain Parser::QStringtoAmpGain(QString in)
{
    DataSet::AmpGain return_value;

    return(return_value);
}

DataSet::Calc Parser::QStringtoCalc(QString in)
{
    DataSet::Calc return_value;

    return(return_value);
}


tm Parser::QStringtoDateTime(QString in)
{
    qint64 month, day, year, hour, minute;
    tm return_tm;

    return(return_tm);
}

DataSet::EMethod Parser::QStringtoEMethod(QString in)
{
    DataSet::EMethod return_value;

    return(return_value);
}


DataSet::Wave Parser::QStringtoWave(QString in)
{
    DataSet::Wave return_value;

    return(return_value);
}

/*
DataSet::Rate Parser::QStringtoRate(QString in)
{
    DataSet::Rate return_value;

    return(return_value);
}

DataSet::Pulse Parser::QStringtoPulse(QString in)
{
    DataSet::Pulse return_value;

    return(return_value);
}
*/

DataSet::Units Parser::QStringtoUnits(QString in)
{
    DataSet::Units return_value;

    return(return_value);
}



QDateTime Parser::ToQDateTime(std::vector<DataSet::Test>::iterator itr_test)
{
    tm test_time;
    QDateTime return_time;

    DataSet::Test current_test = Data->GetTest( itr_test );
    test_time = current_test.TestTime;
    return_time.setDate( QDate( test_time.tm_year + 1900, test_time.tm_mon , test_time.tm_mday ) );
    return_time.setTime( QTime( test_time.tm_hour, test_time.tm_min ) );

    return(return_time);
}

QString Parser::ToQStrAmpGain(std::vector<DataSet::Test>::iterator itr_test)
{
    QString return_string;

    return(return_string);
}

QString Parser::ToQStrWave(std::vector<DataSet::Test>::iterator itr_test)
{
    QString return_string;

    return(return_string);
}

QString Parser::ToQStrRate(std::vector<DataSet::Test>::iterator itr_test)
{
    QString return_string;

    return(return_string);
}

QString Parser::ToQStrPulse(std::vector<DataSet::Test>::iterator itr_test)
{
    QString return_string;

    return(return_string);
}

QString Parser::ToQStrUnits(std::vector<DataSet::Test>::iterator itr_test)
{
    QString return_string;

    return(return_string);
}

QString Parser::ToQStrCalc(std::vector<DataSet::Test>::iterator itr_test)
{
    QString return_string;

    return(return_string);
}

QString Parser::ToQStrEMethod(std::vector<DataSet::Test>::iterator itr_test)
{
    QString return_string;

    return(return_string);
}
