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
    QByteArray copy = in;
    Data = new DataSet;

    copy = copy.remove(0,1);  //remove number of sets
    for(qint64 i=0; copy.size() > 0 ; i+=DataSetSize()){
        QByteArray d = copy.left(DataSetSize());
        copy = copy.remove(0,(int)DataSetSize());
        DataSet::Test newtest = CreateTest(d);
        Data->AddTest(newtest);
    }
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

DataSet::Test Parser::CreateTest(QByteArray array)
{
    QByteArray clean = array;
    QByteArray qbatime, qbapwr, qbadensity, qbaweight,
            qbamoh, qbaunits, qbaaggsize;
    tm test_time;

    DataSet::Test return_test;



    return(return_test);
}

QByteArray Parser::RemoveAscii(QByteArray &in){

    QByteArray out;

    for(qint16 i=0; i<in.size();++i){
        out[i]=in[i]-48;//removes ascii offset
    }
    return(out);
}

DataSet::AmpGain Parser::QBAtoAmpGain(QByteArray &in)
{

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

tm Parser::QBAtoDateTime(QByteArray &in)
{
    qint64 month, day, year, hour, minute;
    tm return_tm;

    return(return_tm);
}

DataSet::Wave Parser::QBAtoWave(QByteArray &in)
{

}

DataSet::Rate Parser::QBAtoRate(QByteArray &in)
{

}

DataSet::Pulse Parser::QBAtoPulse(QByteArray &in)
{

}

DataSet::Units Parser::QBAtoUnits(QByteArray &in)
{

}

DataSet::Calc Parser::QBAtoCalc(QByteArray &in)
{

}

DataSet::EMethod Parser::QBAtoEMethod(QByteArray &in)
{

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

}

QString Parser::ToQStrWave(std::vector<DataSet::Test>::iterator itr_test)
{

}

QString Parser::ToQStrRate(std::vector<DataSet::Test>::iterator itr_test)
{

}

QString Parser::ToQStrPulse(std::vector<DataSet::Test>::iterator itr_test)
{

}

QString Parser::ToQStrUnits(std::vector<DataSet::Test>::iterator itr_test)
{

}

QString Parser::ToQStrCalc(std::vector<DataSet::Test>::iterator itr_test)
{

}

QString Parser::ToQStrEMethod(std::vector<DataSet::Test>::iterator itr_test)
{

}
