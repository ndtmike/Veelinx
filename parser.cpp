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


std::vector<long> Parser::CreateADC(QStringList sl)
{
    std::vector<long> return_vector;

    for(auto i = sl.begin(); i != sl.end(); ++i){
        bool ok = true;
        QString line = *i;
        return_vector.push_back( line.toLong( &ok ));
    }

    return( return_vector);
}

DataSet::Prop Parser::CreateProp(QStringList sl)
{
    DataSet::Prop return_prop;
    int n;

    n = sl.indexOf(QRegExp(".*Gain:.*")); // .* wildcard for QRegExp
    return_prop.PropAmpGain = QStringtoAmpGain( sl.at(n));

    n=sl.indexOf(QRegExp(".*Measured.*"));
    return_prop.PropCalc = QStringtoCalc( sl.at(n));

    n=sl.indexOf(QRegExp(".*Young's modulus:.*"));
    return_prop.PropEMethod = QStringtoEMethod(sl.at(n));

    n=sl.indexOf(QRegExp(".*feet/second.*"));
    return_prop.PropEMethod = QStringtoEMethod(sl.at(n));

    n=sl.indexOf(QRegExp(".*WAVE TYPE:.*"));
    return_prop.PropWave = QStringtoWave(sl.at(n));

    return(return_prop);
}


DataSet::Test Parser::CreateTest(QStringList sl)
{
    DataSet::Test return_test;
    int n;//index

    QStringList header = sl.mid(0,8);
    QStringList adc_data = sl.mid(9,sl.size()-9);

    return_test.TestProp = CreateProp( header);
    return_test.ADC = CreateADC(adc_data);

    n = sl.indexOf(QRegExp(".*Date/Time:.*"));
    return_test.TestTime = QStringtoDateTime( sl.at(n) );

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

    if( in.contains(" 1 ")){
        return_value = DataSet::Gain_1;
    }else if( in.contains(" 5 ")){
        return_value = DataSet::GAIN_5;
    }else if( in.contains(" 10 ")){
        return_value = DataSet::GAIN_10;
    }else if( in.contains(" 25 ")){
        return_value = DataSet::GAIN_10;
    }else if( in.contains(" 50 ")){
        return_value = DataSet::GAIN_10;
    }else if( in.contains(" 100 ")){
        return_value = DataSet::GAIN_10;
    }else if( in.contains(" 250 ")){
        return_value = DataSet::GAIN_10;
    }else if( in.contains(" 500 ")){
        return_value = DataSet::GAIN_10;
    }

    return(return_value);
}

DataSet::Calc Parser::QStringtoCalc(QString in)
{
    DataSet::Calc return_value;

    if( in.contains("-Distance ")){
        return_value = DataSet::Dist;
    }else if( in.contains("-Velocity ")){
        return_value = DataSet::Vel;
    }
    return(return_value);
}


tm Parser::QStringtoDateTime(QString in)
{
    int working_int;
    tm return_tm;

    int current_pos = in.indexOf( "Time");
    bool ok = true;
    QString working_qstring = "";

    current_pos += 9;//position of month

    working_qstring = in.mid( current_pos, 2 );//month
    working_int = working_qstring.toInt( &ok );
    return_tm.tm_mon = working_int;

    current_pos += 3;
    working_qstring = in.mid( current_pos, 2 );//month
    working_int = working_qstring.toInt( &ok );
    return_tm.tm_mday = working_int;

    current_pos += 3;
    working_qstring = in.mid( current_pos, 2 );//month
    working_int = working_qstring.toInt( &ok );
    return_tm.tm_year = working_int;

    current_pos += 3;
    working_qstring = in.mid( current_pos, 2 );//month
    working_int = working_qstring.toInt( &ok );
    return_tm.tm_hour = working_int;

    current_pos += 3;
    working_qstring = in.mid( current_pos, 2 );//month
    working_int = working_qstring.toInt( &ok );
    return_tm.tm_min = working_int;

    current_pos += 3;
    working_qstring = in.mid( current_pos, 2 );//month
    working_int = working_qstring.toInt( &ok );
    return_tm.tm_sec = working_int;

    return(return_tm);
}

DataSet::EMethod Parser::QStringtoEMethod(QString in)
{
    DataSet::EMethod return_value;

    if( in.contains(" SIMPLE ")){
        return_value = DataSet::SimpleE;
    }else if( in.contains(" ARBITRARY ")){
        return_value = DataSet::ArbMu;
    }else if( in.contains(" DERIVED ")){
        return_value = DataSet::DerivedMu;
    }

    return(return_value);
}

/*
DataSet::Pulse Parser::QStringtoPulse(QString in)
{
    DataSet::Pulse return_value;

    return(return_value);
}
*/

DataSet::Rate Parser::QStringtoRate(QString in)
{
    DataSet::Rate return_value;

    if( in.contains(" 250 kHz ")){
        return_value = DataSet::RATE_250KHZ;
    }else if( in.contains(" 500 kHz ")){
        return_value = DataSet::RATE_500KHZ;
    }else if( in.contains(" 1.0 mHz ")){
        return_value = DataSet::RATE_1000KHZ;
    }else if( in.contains(" 2.0 mHz ")){
        return_value = DataSet::RATE_2000KHZ;
    }

    return(return_value);
}

DataSet::Wave Parser::QStringtoWave(QString in)
{
    DataSet::Wave return_value;

    if( in.contains(" 'P' ")){
        return_value = DataSet::PWave;
    }else if( in.contains(" 'S' ")){
        return_value = DataSet::SWave;
    }

    return(return_value);
}

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
